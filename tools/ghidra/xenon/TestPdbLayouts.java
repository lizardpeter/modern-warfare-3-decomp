// Rebuild PDB composites with Ghidra's own PDB reconstruction and write the resulting layouts, for
// tools/ghidra/xenon/pdb_layouts.py check. Run on any program of the target language (the data organization, and so
// the endianness, comes from it); each case is built exactly as CompositeTypeApplier builds it: members from the PDB,
// bit-fields as BitfieldTypeApplier makes them, DefaultCompositeMember.applyDataTypeMembers.
// Args: <cases.json> <out.json>
//@category IW5
import java.io.FileReader;
import java.io.FileWriter;
import java.util.*;

import com.google.gson.*;

import ghidra.app.script.GhidraScript;
import ghidra.app.util.bin.format.pdb.*;
import ghidra.program.model.data.*;

public class TestPdbLayouts extends GhidraScript {

	private DataTypeManager dtm;
	private final Map<String, DataType> built = new HashMap<>();

	private static class Member extends PdbMember {
		private final DataType dt;
		private final boolean zeroLengthArray;

		Member(String name, DataType dt, boolean zeroLengthArray, int offset) {
			super(name, dt.getName(), offset, null);
			this.dt = dt;
			this.zeroLengthArray = zeroLengthArray;
		}

		@Override
		protected WrappedDataType getDataType() {
			return new WrappedDataType(dt, zeroLengthArray, false);
		}
	}

	private DataType prim(String name) {
		DataType d = switch (name) {
			case "char" -> CharDataType.dataType;
			case "schar" -> SignedCharDataType.dataType;
			case "uchar" -> UnsignedCharDataType.dataType;
			case "bool" -> BooleanDataType.dataType;
			case "short" -> ShortDataType.dataType;
			case "ushort" -> UnsignedShortDataType.dataType;
			case "wchar16" -> WideChar16DataType.dataType;
			case "int" -> IntegerDataType.dataType;
			case "uint" -> UnsignedIntegerDataType.dataType;
			case "longlong" -> LongLongDataType.dataType;
			case "ulonglong" -> UnsignedLongLongDataType.dataType;
			case "float" -> FloatDataType.dataType;
			case "double" -> DoubleDataType.dataType;
			default -> throw new IllegalArgumentException("primitive " + name);
		};
		return d.clone(dtm);
	}

	private DataType type(JsonObject t) throws Exception {
		switch (t.get("kind").getAsString()) {
			case "prim":
				return prim(t.get("name").getAsString());
			case "ptr":
				return new PointerDataType(VoidDataType.dataType, dtm);
			case "array": {
				DataType e = type(t.getAsJsonObject("elem"));
				int size = t.get("size").getAsInt();
				int n = e.getLength() > 0 ? size / e.getLength() : 0;
				// a zero-length array (T x[0]) is a 0-element array, flagged as such (Member), as the PDB
				// applier passes it
				return new ArrayDataType(e, n, e.getLength(), dtm);
			}
			case "named": {
				DataType d = built.get(t.get("name").getAsString());
				if (d == null) {
					throw new IllegalStateException("not built: " + t.get("name").getAsString());
				}
				return d;
			}
			case "bitfield": {
				DataType base = type(t.getAsJsonObject("base"));
				int bits = t.get("bits").getAsInt();
				int pos = t.get("pos").getAsInt();
				// as BitfieldTypeApplier: the position as the PDB records it
				return new PdbBitField(base, bits, pos) {};
			}
		}
		throw new IllegalArgumentException("kind " + t);
	}

	private JsonArray components(Composite c, int depth) {
		JsonArray out = new JsonArray();
		for (DataTypeComponent dtc : c.getDefinedComponents()) {
			JsonObject o = new JsonObject();
			o.addProperty("name", dtc.getFieldName() == null ? "" : dtc.getFieldName());
			o.addProperty("offset", dtc.getOffset());
			o.addProperty("length", dtc.getLength());
			DataType d = dtc.getDataType();
			if (d instanceof BitFieldDataType bf) {
				o.addProperty("bitfield", true);
				o.addProperty("storage", bf.getStorageSize());
				o.addProperty("bitOffset", bf.getBitOffset());
				o.addProperty("bits", bf.getBitSize());
			}
			// composites the reconstruction made (not one of the PDB's own types): descend
			else if (d instanceof Composite nested && !built.containsKey(nested.getName()) && depth < 8) {
				o.add("components", components(nested, depth + 1));
			}
			out.add(o);
		}
		return out;
	}

	@Override
	public void run() throws Exception {
		String[] args = getScriptArgs();
		dtm = currentProgram.getDataTypeManager();
		println("data organization big-endian: " + dtm.getDataOrganization().isBigEndian());
		JsonArray cases = JsonParser.parseReader(new FileReader(args[0])).getAsJsonObject()
				.getAsJsonArray("composites");
		CategoryPath cat = new CategoryPath("/pdblayouts");
		JsonArray results = new JsonArray();
		int tx = currentProgram.startTransaction("pdb layouts");
		try {
			for (JsonElement e : cases) {
				JsonObject c = e.getAsJsonObject();
				String name = c.get("name").getAsString();
				JsonObject r = new JsonObject();
				r.addProperty("name", name);
				try {
					Composite comp = c.get("union").getAsBoolean() ? new UnionDataType(cat, name, dtm)
							: new StructureDataType(cat, name, 0, dtm);
					List<Member> members = new ArrayList<>();
					for (JsonElement me : c.getAsJsonArray("members")) {
						JsonObject m = me.getAsJsonObject();
						JsonObject t = m.getAsJsonObject("type");
						boolean zeroLength = t.get("kind").getAsString().equals("array") &&
							t.get("size").getAsInt() == 0;
						members.add(new Member(m.get("name").getAsString(), type(t), zeroLength,
							m.get("offset").getAsInt()));
					}
					List<String> errors = new ArrayList<>();
					boolean ok = DefaultCompositeMember.applyDataTypeMembers(comp, false, false,
						c.get("size").getAsInt(), members, errors::add, monitor);
					Composite resolved = (Composite) dtm.resolve(comp, DataTypeConflictHandler.REPLACE_HANDLER);
					built.put(name, resolved);
					r.addProperty("size", resolved.getLength());
					r.addProperty("applied", ok);
					if (!errors.isEmpty()) {
						r.addProperty("messages", String.join(" | ", errors));
					}
					r.add("components", components(resolved, 0));
				}
				catch (Exception ex) {
					r.addProperty("error", ex.toString());
				}
				results.add(r);
			}
		}
		finally {
			currentProgram.endTransaction(tx, false);
		}
		JsonObject out = new JsonObject();
		out.addProperty("bigEndian", dtm.getDataOrganization().isBigEndian());
		out.add("composites", results);
		try (FileWriter w = new FileWriter(args[1])) {
			new Gson().toJson(out, w);
		}
		println("wrote " + results.size() + " layouts");
	}
}
