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

### The PowerPC scan

`Build._scan_ppc` reads the Xbox 360 builds (machine 0x1F2) with its own decoder; capstone isn't needed and doesn't
know VMX128. It gives the same tables as the x86/x64 scan, so everything downstream is shared:

- **data references**: `lis rX, hi` followed by `addi`/`ori rY, rX, lo` or a D/DS-form load or store `lo(rX)`.
  The pending high half is tracked per register in straight-line code; a write to the register drops it, a call
  (`bl`, `bcl`, `bctrl`, `bclrl`) drops the volatile ones (r0, r3-r12), other branches drop all.
- **registrations**: strings and a function address loaded between two calls (`Cmd_AddCommand("name", Func)`), and
  `stw r3, lo(rX)` after a call for `g = Dvar_Register*("name", ...)`.
- **constants**: 32-bit values built by `lis` + `ori`/`addi` that aren't image addresses (16-bit immediates are
  below the distinctiveness threshold anyway). Float constants are loaded from memory on both architectures and are
  not compared by value yet.
- **tables**: 4-byte big-endian pointers.
- Sections are read through their headers as written, never with pefile's `get_data` (docs/xbox360.md).

Checked on `iw5mp_xenon` against its PDB: assert and format strings land in the functions that own them, command
registrations pair the right names (`"viewpos"` → `CG_Viewpos_f`), and 1,641 of the 1,706 globals stored from
dvar registrations are the PDB variable of that dvar's name (the rest are keyed by the description string, which
loses a match but doesn't make a wrong one). The x86/x64 scan was refactored around it and gives identical tables
on `iw5mp_ship`.

### Validation

As in BO2: `iw5mp_xenon_ship` (the ship configuration of the same tree) is analysed blind (`ghidra_pdb = false`),
matched against `iw5mp_xenon`, and scored with its own PDB/MAP:

    python tools/match/match.py iw5mp_xenon iw5mp_xenon_ship --same-compiler
    python tools/match/validate.py iw5mp_xenon iw5mp_xenon_ship

That scores the PowerPC scan and the ship configuration's inlining. Current result: 8,428 of 21,138 ship functions
matched (39.9%), 8,336 right and 51 wrong by the ship PDB, **99.4% precision**. The ceiling is about 96% (ship
functions whose name exists in the dev build); coverage is limited by inlining: the ship configuration has 21k
functions to the dev build's 30k, so call-graph neighbourhoods differ, and it has no assert strings, the dev build's
densest anchors. What changed the score:

- the register save/restore helpers out of the call graph (`XENON_HELPER`): they are plumbing called from nearly
  every prologue, made unrelated functions look alike (67 wrong pairs before, 47 after), and the x64 target has
  nothing like them;
- `calls`: the only unmatched function on each side calling the same set of at least two matched functions (+200
  pairs at 98%);
- correct analyses of both builds (docs/xbox360.md: `.pdata` read through the section headers, helpers found by
  their code, leaf functions seeded from their references).

Tried and dropped: pairing function-pointer tables by index once two entries agree (37 pairs, 28 right).

### Store build (cross-architecture)

    python tools/match/match.py iw5mp_xenon iw5mp_ship

2,947 of 14,220 Store functions matched (20.7%): 631 by unique strings, 139 by registrations, 104 by registered
globals, the rest by call-graph propagation (callee 834, caller 823, order 196, calls 108, votes 82). There is no
ground truth. Consistency check: where both functions of a pair reference strings (870 pairs), 842 share at least one
(97%); the 28 that don't are mostly inlining differences on reading (`ClientThink_real` holds the inactivity-drop
strings of a function the Store build inlines into it, `PartyHost_RemovePlayer` the party commands it sends). The
Store build is 13 years and a platform away from the reference: 4,735 of its 6,779 referenced strings exist in the
Xbox build, and its PC, GDK and Demonware code has no counterpart.

Two Store-analysis faults cost most of the earlier result (6.9%): Ghidra's non-returning discovery marked `memcpy`,
`memset`, `free`, `sqrtf`, `__chkstk` and hundreds of returning functions non-returning, truncating their callers,
and the 7,688 chained `.pdata` parts were created as functions of their own (docs/store-build.md). The matcher reads
each x64 function over its `.pdata` extent and chained parts.

## Other sources of names

- The PC Steam exes (`iw5mp_steam`, x86) are the same platform layer as the Store build on the 2011 code, and
  community IW5 clients name many of their addresses. Steam → Store is x86 → x64, the case match.py already
  handles; names found there carry a second line of evidence.
- The Store build's own strings: RTTI names (`.?AV...`), assert-free but many `Com_Printf` and dvar strings.
