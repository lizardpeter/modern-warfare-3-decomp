# runtime/loader/x3daudio_stubs.s: X3DAudio1_7.dll's exports, each a jump through its slot to the system DLL's function
# (filled by LoadSystemX3DAudio), so every argument passes through untouched whatever the signature.
	.text
	.globl X3DAudioInitialize
X3DAudioInitialize: jmpq *x3daudio_slots+0(%rip)
	.globl X3DAudioCalculate
X3DAudioCalculate: jmpq *x3daudio_slots+8(%rip)
