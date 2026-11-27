# The Xbox 360 references

## From XEX to PE

`tools/unpack/xex.py` reads an XEX2 and writes the PE the linker wrote before `imagexex` wrapped it:

1. Optional headers: file format info (`0x3FF`) gives the encryption and compression; the security info gives the
   image size, load address (0x82000000 for all of these) and the encrypted session key.
2. The session key is the XEX's AES key decrypted (AES-128-ECB) with the retail master key, then the development
   key (all zero); the one that decrypts the data to `MZ` is used. **All TU1 files use the development key**: these
   are devkit-signed builds, not retail-signed discs.
3. The data is decrypted (AES-128-CBC, zero IV) and expanded: basic compression is a list of (data, zeros) blocks.
   Normal (LZX) compression is refused with an error; none of these files use it.
4. The result is the loader's mapped image. An XEX's PE has 512-byte file alignment but 64 KB section alignment, so
   each section's raw data is copied back from its virtual address to its `PointerToRawData`. The output is a
   normal PE file of about the XEX's size, deterministic, pinned as `unpacked_sha256`.

`python tools/unpack/xex.py --info <file>` prints the headers. `tools/fetch_originals.py` runs the unpacker for
builds with `unpack = "xex"` and checks the hash; `check.yml` re-checks every Xbox 360 build on each push.

## Identity

Xbox 360 exes carry no CodeView record. The PDB's signature is the exe's PE timestamp (`pdb_signature` in
`target.toml`, checked by `tools/inventory.py`); the PDB's GUID and age are pinned from the PDB itself.

## Function bounds

Xenon `.pdata` entries are 8 big-endian bytes: the function's absolute start address, then a word packing the prolog
length (bits 0-7), the function length **in instructions** (bits 8-29), a 32-bit flag and an exception flag.
`tools/inventory.py` writes them to `pdata.tsv`: 22,091 functions in `iw5mp_xenon` (the PDB knows 28,867
procedures, including leaves without unwind data and SDK code; 21,210 `.pdata` starts are PDB functions and 20,880 of
those have the PDB's size) and 16,029 in `iw5mp_xenon_ship`. Every entry must lie inside a code section and each start
may appear once; the inventory fails otherwise.

**Don't map Xenon addresses with pefile** (`get_offset_from_rva`, `get_data`): it first rounds each section's address
down to `SectionAlignment` (64 KB), which Xenon sections don't follow, and so reads the wrong bytes: `.pdata` at RVA
0x21E000 in `iw5mp_xenon` was read from 0xE000 further on. An earlier inventory did, and its `pdata.tsv` lacked the
table's first 7,168 entries and ended in junk, 29 rows of which seeded false function starts inside real functions.
`xenon_read` in `tools/inventory.py` maps through the section headers as written.

### Register save/restore helpers, found by their code

`tools/inventory.py` also writes `xenon_helpers.tsv`: every entry point of the compiler's register save/restore
helpers, found by scanning the code sections for their instruction chains, encoded from the instruction formats
(`xenon_helper_chains`). It finds the same 236 entries as the MAP in both `iw5mp_xenon` and `iw5mp_xenon_ship`.
`ApplySymbols.java` reads it in every analysis, including blind ones (`ghidra_pdb = false`, which pass `blind` and use
only `pdata.tsv` and `xenon_helpers.tsv`), so a build without symbols gets the same helper handling (below).

## Code and compiler

VS2010 for Xenon (linker 10.00), PowerPC 64-bit in 32-bit-address mode, big-endian. Sections include Bink's own
(`BINK`, `BINKDATA`, `BINKBSS`), `.XBMOVIE`, `.XEXID`, `.XBLD`. Libraries visible in the PDB: the XDK of February
2011 (`E:\Xenon\XDK-Main-FEB11`, D3D, XAudio, XAPI, XRTL), DemonWare (`demonware\bdcore`), libtomcrypt, jpeg-6,
the WebM encoder (`WebM_atvi_opt`).

### VMX128: the Xenon Ghidra language

Stock Ghidra has no VMX128: the Xenon's 128-register vector extension (opcodes 4, 5 and 6, with register numbers
split across the word), nor the Cell loads and stores `lvlx`/`lvrx`/`stvlx`/`stvrx` (and their LRU forms) or
`dcbz128`. Its closest language, `PowerPC:BE:64:A2ALT-32addr` (Power ISA 3.0 with AltiVec), also has later
instructions on encodings VMX128 uses, so adding VMX128 to it would decode Xbox 360 code wrongly rather than not
at all: POWER9's `maddhd`/`maddhdu`/`maddld` and SPE's `brinc` on opcode 4, 135 AltiVec instructions from after
ISA 2.03 (`vabsdub` sits on `lvlx128`'s encoding), Book E's `stvepx`/`stvepxl` on `lvrxl`/`lvlxl`'s, and the 970's
`dcbzl` on `dcbz128`'s. On `iw5mp_xenon`'s `.text` (1,807,437 non-zero words) that language can't decode 11,724
words: 3,060 on opcode 4, 4,311 on 5, 3,749 on 6, 48 on 31.

`PowerPC:BE:64:Xenon-32addr` fixes that (`tools/ghidra/xenon/`). `build_language.py` generates it from the installed
Ghidra's own PowerPC files, so nothing of Ghidra's is copied into this repo and an upgrade regenerates it:

- `ppc_common.sinc` with the VMX128 fields (`vmx128_fields.sinc`) in its instruction token, and its embedded
  include without `dcbzl`;
- `ppc_isa.sinc` and `altivec.sinc` without the instructions above: every opcode-4 constructor not in Xenon's
  AltiVec set (Xenia's list, 156 instructions) and every opcode-31 one on the eight Cell load/store XOPs;
- `vmx128.sinc`: registers v32..v127 (v0..v31 are AltiVec's), all 76 VMX128 instructions and the 9 Cell/Xenon
  additions. Loads, stores, moves and logic are real p-code; arithmetic is named p-code operations, as in Ghidra's
  own AltiVec. Encodings, operand order and semantics follow Xenia (BSD-3-Clause), including VMX128's accumulator
  forms (`vmaddfp128`: vD = vA·vB + vD; `vsel128` selects with vD).

With only Xenon's instructions present, SLEIGH's most-specific-pattern rule decodes opcode 4 in Xenon's order
(VMX128 loads and stores, then AltiVec, then `vsldoi128`). `xenon-language.yml` checks it: 5,784 words generated from
Xenia's table at a pinned commit (241 instructions: every VMX128, AltiVec and Cell vector instruction, canonical
plus 23 random fillings of the operand fields) all decode to Xenia's mnemonic, with every 7-bit register right. On
`iw5mp_xenon`'s `.text` no word on opcodes 4, 5, 6 or 31 is left undecodable; the 556 that remain are opcode-0
words, which no PowerPC defines (data in `.text`).

### Ghidra's PDB analyzer

Ghidra's PDB Universal analyzer reads these PDBs without CodeView (the PDB is given to it explicitly). Three Ghidra
bugs kept it from applying them; all are fixed by patches to the installed release
(`tools/ghidra/patches/`, applied by `tools/ghidra/patch_ghidra.sh` in CI), still present on Ghidra master as of
2026-11-25.

**The crash** (`pdb-isGoodAlignment-bounds.patch`). On `iw5mp_xenon` the analyzer failed at once with an
`ArrayIndexOutOfBoundsException` in `DefaultCompositeMember.isGoodAlignment`, aborting the whole PDB step: no PDB
types, prototypes or locals reached the project. Checking whether packing moved a struct's members, it walks the
packed copy's components while indexing the original's defined components, without comparing their counts. A
different count means the layout moved, so the check now returns false.

**Big-endian bit-fields** (`pdb-big-endian-bitfields.patch`). With the crash fixed, the analyzer ran for 3 h 40 m
and applied the PDB, but 5,749 functions (19%) then failed to decompile with "Field ... does not fit in structure":
`entityState_s`, `usercmd_s`, `cg_s`, the GPU packet and register types, everything holding a bit-field. Ghidra's
PDB code is little-endian only (it says so: "TODO: Add support for big-endian"):

- A PDB records a bit-field's position from the least significant bit of its base type, on every target. Ghidra
  turned that into a byte as `position / 8`; on big-endian bit 0 is in the base type's last byte, so bit-fields
  landed in the wrong bytes.
- It groups bit-fields that share a storage unit by expecting each to start where the previous one ended,
  counting from the least significant bit. Xenon's MSVC allocates from the most significant bit: `Weapon` lists
  `padding` (bit 24), `scopeVariation` (21), ... `weaponIdx` (0), positions decreasing in declaration order (IW
  declared the fields in reverse on Xbox so `weaponIdx` stays in the low byte, which the code reads with
  `clrlwi r, r, 24`). Every such field looked like an overlap, and the fallback built nested unions and structs
  too large for their parent.
- Some Xbox 360 system headers declare `#pragma bitfield_order(lsb_to_msb)` (the GPU packet types, the schema
  and audio descriptors): there positions increase. The order is a property of the declaration, not the
  compiler.

The patch keeps the PDB's position for placement (the byte and shift follow from it and the endianness alone, for
either order), works out each run of bit-fields' allocation order from the member order (decreasing positions:
from the most significant bit; increasing: from the least; a single bit-field takes its composite's other runs'
order, or the target's default), gives padding fillers their run's order, and lets a union hold a bit-field it
can't place directly (bits 0-2 of a big-endian byte, say) inside a structure, as it already did for zero-length
arrays.

**Enum sizes** (`pdb-enum-underlying-type.patch`). With the bit-fields fixed, the analyzer stopped two seconds in
("Negative values not permitted when defining bitfield"): `D3D::MEMORYREGION`, an `int` enum used as a bit-field
base at bit 28, had been created one byte long. `EnumTypeApplier` takes an enum's length from its underlying type
with `getDataType()`, which returns null for a primitive no applier has produced yet, and then falls back to one
byte; whether that happens depends on the order types are processed. Every enum processed early came out one byte
long, which also mislays every structure holding one. The applier now asks with `getDataTypeOrSchedule()` and
retries once the underlying type exists, as the other appliers do.

With all three patches the PDB step applies the whole PDB in 15 minutes (on an 8 GB machine; the 3 h 40 m before was
the analyzer fighting the broken bit-field layouts). End to end (`pdb_layouts.py applied`: iw5mp_xenon imported with
only the PDB analyzer, everything it created compared with the PDB), 7,181 of the 7,183 named composites defined
once and all 1,131 such enums match; the two differences, a libjpeg name defined twice and a compiler RTTI type, are
in `tools/ghidra/xenon/pdb_known.tsv` with their reasons, and any other difference fails CI.

`tools/ghidra/xenon/pdb_layouts.py` tests the reconstruction in a minute: every composite with bit-fields in
`iw5mp_xenon`'s PDB and every type they use (2,651, C++ classes included: their data members, as
`createMembersOnlyClassLayout` applies them) is rebuilt with Ghidra's own reconstruction (`TestPdbLayouts.java`)
and compared with the PDB, member by member and bit by bit:

| | stock Ghidra 12.1.4 | patched |
|---|---|---|
| big-endian (Xenon language) | 617 of 2,530 | 2,651 of 2,651 |
| little-endian (x86) | 1,744 of 2,530 | 2,651 of 2,651 |

(The stock runs predate the C++ classes being in the case set.) On little-endian no composite stock Ghidra rebuilt
correctly is changed, so PC PDBs can't regress. `xenon-language.yml` runs the test on both, and `pdb_layouts.py
applied` checks what the analyzer itself created end to end (below).

### Our own PDB extraction

`pdb/types.h` (resymc, `tools/pdb/extract.py`) had the same blind spot: resym groups bit-fields into unnamed structs
assuming positions increase in declaration order and each run starts at bit 0. It dropped six of `Weapon`'s seven
fields and printed some types empty (`GPUFLOW_RETURN`). `tools/pdb/resym-big-endian-bitfields.patch` gives each run
its allocation order the same way, starts an unnamed struct at a run that begins above bit 0, and adds that run's
leading padding. Of the 346 named types with bit-fields, 323 had every member before; all 346 do now (one,
`RefString`, has a second definition in the PDB without bit-fields, which `types.h` shows). `types.h` keeps the
Xbox 360 declaration order: a run allocated from the most significant bit (`Weapon`) must be declared in reverse,
ascending `BitPos`, for the x64 Store build, whose MSVC allocates from the least significant bit.

`ApplySymbols.java` still runs first: it creates and names every function the PDB, MAP and `.pdata` know, from
`analysis/<build>/pdb` and `map` (tools/pdb/extract.py, llvm-pdbutil and resymc), independently of Ghidra's PDB
support, and handles the register save/restore helpers below.

### Register save/restore helpers

The compiler saves and restores registers through `__savegprlr_14`..`_31`, `__restgprlr_*`, `__savefpr_*`,
`__restfpr_*`, `__savevmx_*`, `__restvmx_*`: chains of 4-byte entries falling through into one shared `blr`, called
from nearly every prologue (`mflr r12; bl __savegprlr_NN`) and epilogue. As separate functions none has a return
of its own, so Ghidra's "Non-Returning Functions - Discovered" analyzer marks them non-returning and every caller
stops after its prologue: the first run cut 8,396 `iw5mp_xenon` functions to 8 bytes and dropped 3,493 more.
`ApplySymbols.java` marks the helpers returning and turns that analyzer off for PowerPC builds (known non-returning
functions are still applied). With the fix, Ghidra has all 29,938 functions of `iw5mp_xenon` and agrees with the PDB's size on 27,468 of 28,517
sized ones; 1,047 end early (after a call Ghidra knows doesn't return or a tail branch, at an undecoded VMX128
instruction, or at a `bctr` jump table). Check after each Xbox 360 analysis that Ghidra's sizes agree with the PDB's.

Marked returning, the helpers were still ordinary calls to the decompiler, which took the registers they leave
behind for return values: `iVar1 = __savegprlr_28(...)`, with asserts and arithmetic then reading `iVar1` instead
of the real parameter. The Xenon language's compiler spec, `ppc_xenon.cspec` (generated by `build_language.py`
from Ghidra's `ppc_64_32.cspec`), adds a call fixup `xenon_save_restore` targeting all 242 helper names, with a
no-op body, and `ApplySymbols.java` attaches it to each helper it finds (74 entry points in `iw5mp_xenon`; the
fixup is only attached when the program's compiler spec defines it). The decompiler then drops the calls, notes
`replaced with injection: xenon_save_restore` at the top of the function, and the data flow is the source's: e.g.
`AimAssist_LerpConstDvars` asserts on `param_1`/`param_3` and returns
`(to->current.value - from->current.value) * frac + from->current.value`. The helpers only touch callee-saved
registers and the caller's own save area, so nothing the function computes is lost. In the committed analysis all 29,808
functions decompile with 0 failures, and the helpers appear only in their own definitions (`_lib/libcmt/crtvmx.c`).
The decompile logs' 321 "Unable to resolve constructor" warnings are all at zero words or ASCII text (paths
in `.rdata`-like data that flow reaches), not unknown instructions.

## Layouts

Every struct in the PDBs is the 32-bit big-endian layout. Pointer-holding structs are larger on the x64 Store
build; padding and alignment otherwise follow the same rules (MSVC, natural alignment). Predict with
`tools/pdb/layout64.py`, then confirm against the Store exe's own field accesses before relying on an offset.
