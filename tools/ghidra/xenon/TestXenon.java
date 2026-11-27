// Decode every 4-byte word of the current program (a raw import of tools/ghidra/xenon/test_words.py's words.bin)
// on its own and write "<offset>\t<instruction text>" per word, or "<offset>\tBAD" where the language has no
// constructor. Each word goes through the pseudo-disassembler, which doesn't touch the listing, so a word that
// fails can't affect the next one. test_words.py check compares the result with Xenia's table.
// Args: <out.tsv>
//@category IW5
import java.io.BufferedWriter;
import java.io.FileWriter;

import ghidra.app.script.GhidraScript;
import ghidra.app.util.PseudoDisassembler;
import ghidra.app.util.PseudoInstruction;
import ghidra.program.model.address.Address;
import ghidra.program.model.mem.MemoryBlock;

public class TestXenon extends GhidraScript {
	@Override
	public void run() throws Exception {
		MemoryBlock block = currentProgram.getMemory().getBlocks()[0];
		Address start = block.getStart();
		long n = block.getSize() / 4;
		println("language " + currentProgram.getLanguageID() + ", " + n + " words");
		PseudoDisassembler dis = new PseudoDisassembler(currentProgram);
		try (BufferedWriter w = new BufferedWriter(new FileWriter(getScriptArgs()[0]))) {
			for (long i = 0; i < n; i++) {
				Address a = start.add(i * 4);
				String text;
				try {
					PseudoInstruction ins = dis.disassemble(a);
					text = ins == null ? "BAD" : ins.toString();
				}
				catch (Exception e) {
					text = "BAD";
				}
				w.write((i * 4) + "\t" + text + "\n");
			}
		}
	}
}
