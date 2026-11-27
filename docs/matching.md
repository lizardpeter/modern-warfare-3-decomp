# Matching builds without symbols to the references

The Store build has no symbols. Its functions get the names, prototypes and source positions of the Xbox 360
reference functions by matching, written to `analysis/<target>/match/functions.tsv` (generated) and, once reviewed,
`records/names.tsv` with method, evidence and confidence.

**Never by address position, and never by instruction shape**: the reference is PowerPC compiled by VS2010 for
Xenon, the target x64 compiled by VS2019 thirteen years later. What carries across is content: strings, distinctive
constants, registration tables, and the call graph.

## tools/match/match.py

Copied from black-ops-2-decomp, where it matched x86 servers to the x64 Store build at 98.3% precision on its
ground-truth pair. Stages, in order:

1. **string**: a string referenced by exactly one function in each build.
2. **constant**: at least two distinctive immediates, each unique to one function on both sides.
3. **named**: a string tied to exactly one function by a registration (`Cmd_AddCommand("name", Func)`) or a
   `{name, ..., function}` table entry.
4. **callee / caller**: call-graph propagation from matched pairs, with structural checks.

### What has to change for PowerPC (before the first run)

The reference scan disassembles with capstone in x86 or x64 mode, chosen from the PE machine type. The Xbox 360
builds need a PowerPC scan (capstone `CS_ARCH_PPC`, big-endian, 32-bit addresses):

- **data references** are built in two instructions: `lis rX, hi` then `addi`/`ori`/a load or store `rY, lo(rX)`
  (`hi` adjusted for the sign of `lo` with `addi`/loads). Track `lis` per register within a basic block.
- **calls** are `bl target`; tail calls are `b target` at the end of a function; indirect calls go through `mtctr`
  / `bctrl` (no target).
- **constants**: integer immediates are at most 16 bits per instruction (`li`, `lis`+`ori` for 32-bit); floats are
  loaded from memory, as on x64, so match float constants by value read from the referenced data.
- **tables**: pointers are 4 bytes, big-endian.

Everything downstream of the scan (uniqueness, propagation, scoring) is architecture-neutral.

### Validation

As in BO2: `iw5mp_xenon_ship` (the ship configuration of the same tree) is analysed blind (`ghidra_pdb = false`),
matched against `iw5mp_xenon`, and scored with its own PDB/MAP:

    python tools/match/match.py iw5mp_xenon iw5mp_xenon_ship --same-compiler
    python tools/match/validate.py iw5mp_xenon iw5mp_xenon_ship

That scores the PowerPC scan and the ship configuration's inlining. The cross-architecture step to the Store build
has no ground truth; to check it, sample matches against reading (prototype and behaviour agree), and check
consistency (a match's callees match the reference's callees). Record precision here once measured.

## Other sources of names

- The PC Steam exes (`iw5mp_steam`, x86) are the same platform layer as the Store build on the 2011 code, and
  community IW5 clients name many of their addresses. Steam → Store is x86 → x64, the case match.py already
  handles; names found there carry a second line of evidence.
- The Store build's own strings: RTTI names (`.?AV...`), assert-free but many `Com_Printf` and dvar strings.
