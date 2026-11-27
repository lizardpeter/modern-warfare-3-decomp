// Disassemble every 4-byte word of the current program (a raw import of tools/ghidra/xenon/test_words.py's
// words.bin) one at a time and write "<offset>\t<instruction text>" per word, or "<offset>\tBAD" where the
// language has no constructor. test_words.py check compares the result with Xenia's table.
// Args: <out.tsv>
//@category IW5
import java.io.BufferedWriter;
import java.io.FileWriter;

import ghidra.app.cmd.disassemble.DisassembleCommand;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressSet;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.mem.MemoryBlock;

public class TestXenon extends GhidraScript {
	@Override
	public void run() throws Exception {
		MemoryBlock block = currentProgram.getMemory().getBlocks()[0];
		Address start = block.getStart();
		long n = block.getSize() / 4;
		println("language " + currentProgram.getLanguageID() + ", " + n + " words");
		try (BufferedWriter w = new BufferedWriter(new FileWriter(getScriptArgs()[0]))) {
			for (long i = 0; i < n; i++) {
				Address a = start.add(i * 4);
				DisassembleCommand cmd = new DisassembleCommand(new AddressSet(a, a.add(3)), null, false);
				cmd.applyTo(currentProgram, monitor);
				Instruction ins = getInstructionAt(a);
				w.write((i * 4) + "\t" + (ins == null ? "BAD" : ins.toString()) + "\n");
			}
		}
	}
}
