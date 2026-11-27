// Pre-analysis script for builds with symbol tables: create a function at every address the PDB, the MAP and the
// exception directory know, and give it the original name, before auto-analysis runs. Architecture-neutral, so it
// names the Xbox 360 (PowerPC) references even where Ghidra's own PDB analyzer can't use their PDBs (an Xbox 360 exe
// has no CodeView record to match), and it seeds disassembly at every known entry.
// Args: <analysis/<build>> [blind] (reads pdb/functions.tsv, map/publics.tsv, pdata.tsv and xenon_helpers.tsv,
// whichever exist). "blind" uses only what a build without symbols has: pdata.tsv and xenon_helpers.tsv, which
// tools/inventory.py derives from the exe itself (a ground-truth build for the matcher is analysed that way).
//
// PowerPC (Xbox 360) only: the compiler's register save/restore helpers (__savegprlr_14 .. _31, __restgprlr_*,
// __savefpr_*, __restfpr_*, __savevmx_*, __restvmx_*) are chains of 4-byte entry points that fall through into one
// shared return. As separate functions none of them has a return of its own, so Ghidra's "Non-Returning Functions -
// Discovered" analyzer marks them non-returning and every caller ends at its prologue's `bl __savegprlr_NN` (8,396
// functions of iw5mp_xenon cut to 8 bytes in the first run). They are marked returning here, and that analyzer is
// turned off (for every build: it misfires on the x64 Store build too). Known non-returning functions still apply.
// With the Xenon language they also get its "xenon_save_restore" call fixup, so the decompiler doesn't show them as
// calls returning a value (tools/ghidra/xenon/build_language.py). The helpers come from xenon_helpers.tsv (found by
// their code, so blind analyses get them too) as well as the PDB and MAP; every entry is labelled with its name, so
// an entry inside another entry's chain, which can't be a function of its own until a call creates one, still gets
// the fixup by name (Ghidra's Call-Fixup Installer).
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
import java.util.regex.Pattern;

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

	private static final String SAVE_RESTORE_FIXUP = "xenon_save_restore";

	private static final Pattern XENON_HELPER =
		Pattern.compile("^__(save|rest)(gprlr|gpr|fpr|vmx)(_\\d+)?$");

	private static String clean(String name) {
		return name.replaceAll("\\s+", "_");
	}

	@Override
	public void run() throws Exception {
		File dir = new File(getScriptArgs()[0]);
		boolean blind = getScriptArgs().length > 1 && getScriptArgs()[1].equals("blind");
		// address -> name; PDB names (undecorated) win over MAP names, which win over nothing
		Map<Long, String> names = new LinkedHashMap<>();
		Map<Long, String> fallback = new HashMap<>();
		for (Map<String, String> r : rows(new File(dir, "pdata.tsv"))) {
			// a chained entry (x64 part_of) is a separated part of its parent function, not a function: Ghidra joins
			// it to the parent by flow
			if (r.getOrDefault("part_of", "").isEmpty()) {
				names.putIfAbsent(Long.parseLong(r.get("va"), 16), null);
			}
		}
		for (Map<String, String> r : blind ? rows(new File("-")) : rows(new File(dir, "map/publics.tsv"))) {
			if (r.getOrDefault("flags", "").contains("f")) {
				long va = Long.parseLong(r.get("va"), 16);
				names.putIfAbsent(va, null);
				fallback.putIfAbsent(va, r.get("name"));
			}
		}
		for (Map<String, String> r : blind ? rows(new File("-")) : rows(new File(dir, "pdb/functions.tsv"))) {
			names.put(Long.parseLong(r.get("va"), 16), r.get("name"));
		}
		for (Map<String, String> r : rows(new File(dir, "xenon_helpers.tsv"))) {
			names.put(Long.parseLong(r.get("va"), 16), r.get("name"));
		}
		if (blind) {
			println("ApplySymbols: blind (pdata.tsv and xenon_helpers.tsv only)");
		}
		boolean ppc = currentProgram.getLanguage().getProcessor().toString().equals("PowerPC");
		// the Xenon language's compiler spec models the helpers as a no-op call fixup (build_language.py)
		boolean fixup = ppc && java.util.Arrays.asList(
			currentProgram.getCompilerSpec().getPcodeInjectLibrary().getCallFixupNames())
				.contains(SAVE_RESTORE_FIXUP);
		// Every build: the analyzer's guesses cascade. On PowerPC it took the save/restore helpers for non-returning;
		// on the x64 Store build it marked memcpy, memset, free, sqrtf, __chkstk and hundreds of game functions
		// non-returning (one function with 1,353 callers that ends in a plain ret), cutting every caller short.
		// Ghidra's list of known non-returning functions (exit, longjmp, _CxxThrowException, ...) still applies.
		setAnalysisOption(currentProgram, "Non-Returning Functions - Discovered", "false");
		println("Non-Returning Functions - Discovered turned off" +
			(fixup ? "; PowerPC save/restore call fixup " + SAVE_RESTORE_FIXUP : ""));
		int created = 0, named = 0, failed = 0, helpers = 0;
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
			boolean helper = ppc && name != null && XENON_HELPER.matcher(name).matches();
			try {
				if (helper) {
					createLabel(a, clean(name), true, SourceType.IMPORTED);
				}
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
				if (helper) {
					helpers++;
					if (f != null) {
						f.setNoReturn(false);
						if (fixup) {
							f.setCallFixup(SAVE_RESTORE_FIXUP);
						}
					}
				}
			}
			catch (Exception ex) {
				failed++;
				if (failed <= 20) {
					printerr(String.format("%08X %s: %s", e.getKey(), name, ex.getMessage()));
				}
			}
		}
		println(String.format("ApplySymbols: %d addresses, %d functions created, %d named, %d failed, %d save/restore helpers",
			names.size(), created, named, failed, helpers));
	}
}
