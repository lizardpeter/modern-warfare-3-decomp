// Pre-analysis script for builds with symbol tables: create a function at every address the PDB, the MAP and the
// exception directory know, and give it the original name, before auto-analysis runs. Architecture-neutral, so it
// names the Xbox 360 (PowerPC) references even where Ghidra's own PDB analyzer can't use their PDBs (an Xbox 360 exe
// has no CodeView record to match), and it seeds disassembly at every known entry.
// Args: <analysis/<build>> (reads pdb/functions.tsv, map/publics.tsv and pdata.tsv, whichever exist)
//@category IW5
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.Map;

import ghidra.app.cmd.disassemble.DisassembleCommand;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.symbol.SourceType;

public class ApplySymbols extends GhidraScript {

	/** Columns of a TSV with a header row, as maps; empty when the file doesn't exist. */
	private Iterable<Map<String, String>> rows(File f) throws Exception {
		java.util.List<Map<String, String>> out = new java.util.ArrayList<>();
		if (!f.isFile()) {
			return out;
		}
		try (BufferedReader r = new BufferedReader(new FileReader(f))) {
			String[] head = r.readLine().split("\t", -1);
			String line;
			while ((line = r.readLine()) != null) {
				String[] v = line.split("\t", -1);
				Map<String, String> m = new HashMap<>();
				for (int i = 0; i < head.length && i < v.length; i++) {
					m.put(head[i], v[i]);
				}
				out.add(m);
			}
		}
		return out;
	}

	private static String clean(String name) {
		return name.replaceAll("\\s+", "_");
	}

	@Override
	public void run() throws Exception {
		File dir = new File(getScriptArgs()[0]);
		// address -> name; PDB names (undecorated) win over MAP names, which win over nothing
		Map<Long, String> names = new LinkedHashMap<>();
		Map<Long, String> fallback = new HashMap<>();
		for (Map<String, String> r : rows(new File(dir, "pdata.tsv"))) {
			names.putIfAbsent(Long.parseLong(r.get("va"), 16), null);
		}
		for (Map<String, String> r : rows(new File(dir, "map/publics.tsv"))) {
			if (r.getOrDefault("flags", "").contains("f")) {
				long va = Long.parseLong(r.get("va"), 16);
				names.putIfAbsent(va, null);
				fallback.putIfAbsent(va, r.get("name"));
			}
		}
		for (Map<String, String> r : rows(new File(dir, "pdb/functions.tsv"))) {
			names.put(Long.parseLong(r.get("va"), 16), r.get("name"));
		}
		int created = 0, named = 0, failed = 0;
		for (Map.Entry<Long, String> e : names.entrySet()) {
			if (monitor.isCancelled()) {
				break;
			}
			Address a = toAddr(e.getKey());
			MemoryBlock block = currentProgram.getMemory().getBlock(a);
			if (block == null || !block.isExecute()) {
				continue;
			}
			String name = e.getValue() != null ? e.getValue() : fallback.get(e.getKey());
			try {
				Function f = getFunctionAt(a);
				if (f == null) {
					if (getInstructionAt(a) == null) {
						new DisassembleCommand(a, null, true).applyTo(currentProgram, monitor);
					}
					f = createFunction(a, null);
					if (f != null) {
						created++;
					}
				}
				if (f != null && name != null && !name.isEmpty()) {
					f.setName(clean(name), SourceType.IMPORTED);
					named++;
				}
			}
			catch (Exception ex) {
				failed++;
				if (failed <= 20) {
					printerr(String.format("%08X %s: %s", e.getKey(), name, ex.getMessage()));
				}
			}
		}
		println(String.format("ApplySymbols: %d addresses, %d functions created, %d named, %d failed", names.size(),
			created, named, failed));
	}
}
