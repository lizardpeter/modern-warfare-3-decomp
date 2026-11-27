// Pre-script for the PDB tests: turn off every analyzer except PDB Universal, so an import applies the PDB and
// nothing else (minutes instead of a full analysis). Used by xenon-language.yml's pdb-applied job.
//@category IW5
import java.util.Map;

import ghidra.app.script.GhidraScript;

public class OnlyPdb extends GhidraScript {
	@Override
	public void run() throws Exception {
		Map<String, String> options = getCurrentAnalysisOptionsAndValues(currentProgram);
		int off = 0;
		for (Map.Entry<String, String> e : options.entrySet()) {
			String name = e.getKey();
			boolean isAnalyzerSwitch = !name.contains(".") &&
				(e.getValue().equals("true") || e.getValue().equals("false"));
			if (isAnalyzerSwitch && !name.equals("PDB Universal")) {
				setAnalysisOption(currentProgram, name, "false");
				off++;
			}
		}
		println("analyzers off except PDB Universal: " + off);
	}
}
