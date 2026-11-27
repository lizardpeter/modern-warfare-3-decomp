// Post-analysis script for Xbox 360 builds: create functions at the code addresses in analysis/<build>/code_refs.tsv
// (bl targets, lis+addi callbacks and data-table pointers outside every .pdata function; tools/inventory.py) that are
// still outside every function once Ghidra's own analysis is done. Leaf functions have no .pdata entry, and Ghidra's
// PowerPC analysis doesn't turn an address built with lis+addi into a function, so a callback that is never called
// directly (a command handler passed to Cmd_AddCommand) or a function reached only through a table stays undefined.
// Running after analysis matters: by then jump-table and local labels belong to their functions. A `bl` target is a
// function entry wherever it lies, so one inside another function's body (a leaf reached by a tail branch, which
// Ghidra folds into the function branching to it) becomes a function and the container's body is recomputed, which
// stops at the new entry. An lis+addi or data-pointer target only counts where no function covers it, since inside a
// body it may be a local label. Repeats, with analysis in between, until nothing new is created.
// Args: <analysis/<build>>
//@category IW5
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.List;

import ghidra.app.cmd.disassemble.DisassembleCommand;
import ghidra.app.cmd.function.CreateFunctionCmd;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.mem.MemoryBlock;

public class SeedCodeRefs extends GhidraScript {

	@Override
	public void run() throws Exception {
		File f = new File(getScriptArgs()[0], "code_refs.tsv");
		if (!f.isFile()) {
			println("SeedCodeRefs: no " + f);
			return;
		}
		List<Address> refs = new ArrayList<>();
		List<Boolean> calls = new ArrayList<>();
		try (BufferedReader r = new BufferedReader(new FileReader(f))) {
			r.readLine();
			String line;
			while ((line = r.readLine()) != null) {
				String[] v = line.split("\t");
				refs.add(toAddr(Long.parseLong(v[0], 16)));
				calls.add(v.length > 1 && v[1].equals("call"));
			}
		}
		int total = 0;
		for (int round = 1; round <= 5; round++) {
			int created = 0, split = 0;
			for (int i = 0; i < refs.size(); i++) {
				Address a = refs.get(i);
				if (monitor.isCancelled()) {
					return;
				}
				MemoryBlock block = currentProgram.getMemory().getBlock(a);
				if (block == null || !block.isExecute() || getFunctionAt(a) != null) {
					continue;
				}
				Function container = getFunctionContaining(a);
				if (container != null && !calls.get(i)) {
					continue;
				}
				if (getInstructionAt(a) == null) {
					new DisassembleCommand(a, null, true).applyTo(currentProgram, monitor);
				}
				Function fn = createFunction(a, null);
				if (fn != null) {
					created++;
					if (container != null) {
						CreateFunctionCmd.fixupFunctionBody(currentProgram, container, monitor);
						split++;
					}
				}
			}
			println(String.format("SeedCodeRefs: round %d, %d functions created from %d code references (%d inside " +
				"another function's body, which was recomputed)", round, created, refs.size(), split));
			total += created;
			if (created == 0) {
				break;
			}
			// let auto-analysis follow the new functions (their calls, references, prototypes) before the next round
			analyzeChanges(currentProgram);
		}
		println("SeedCodeRefs: " + total + " functions created");
	}
}
