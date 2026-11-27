// After Ghidra's PDB analyzer has applied a PDB: write every composite and enum it created, for
// tools/ghidra/xenon/pdb_layouts.py check-applied. Composites list their components; nested composites the
// reconstruction made (in the parent's own category, as DefaultCompositeMember places them) are expanded.
// Args: <out.json>
//@category IW5
import java.io.FileWriter;
import java.util.Iterator;

import com.google.gson.*;

import ghidra.app.script.GhidraScript;
import ghidra.program.model.data.*;

public class DumpPdbTypes extends GhidraScript {

	private static boolean isInternal(Composite parent, Composite nested) {
		CategoryPath own = new CategoryPath(parent.getCategoryPath(), parent.getName());
		return nested.getCategoryPath().isAncestorOrSelf(own);
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
			else if (d instanceof Composite nested && isInternal(c, nested) && depth < 8) {
				o.add("components", components(nested, depth + 1));
			}
			out.add(o);
		}
		return out;
	}

	@Override
	public void run() throws Exception {
		DataTypeManager dtm = currentProgram.getDataTypeManager();
		JsonArray composites = new JsonArray();
		JsonArray enums = new JsonArray();
		Iterator<DataType> it = dtm.getAllDataTypes();
		while (it.hasNext()) {
			DataType d = it.next();
			if (d instanceof Composite c) {
				JsonObject o = new JsonObject();
				o.addProperty("path", c.getPathName());
				o.addProperty("union", c instanceof Union);
				o.addProperty("size", c.isZeroLength() ? 0 : c.getLength());
				o.add("components", components(c, 0));
				composites.add(o);
			}
			else if (d instanceof ghidra.program.model.data.Enum e) {
				JsonObject o = new JsonObject();
				o.addProperty("path", e.getPathName());
				o.addProperty("size", e.getLength());
				enums.add(o);
			}
		}
		JsonObject out = new JsonObject();
		out.addProperty("bigEndian", dtm.getDataOrganization().isBigEndian());
		out.add("composites", composites);
		out.add("enums", enums);
		try (FileWriter w = new FileWriter(getScriptArgs()[0])) {
			new Gson().toJson(out, w);
		}
		println("wrote " + composites.size() + " composites, " + enums.size() + " enums");
	}
}
