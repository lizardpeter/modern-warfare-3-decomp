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
`tools/inventory.py` writes them to `pdata.tsv`: 20,424 functions in `iw5mp_xenon` (the PDB knows 28,867
procedures, including leaves without unwind data and SDK code).

pefile's `get_data` returns only part of this table on these images (119,808 of 176,728 bytes), so the inventory
reads the file bytes directly and checks the length.

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

Ghidra's PDB Universal analyzer reads these PDBs without CodeView (the PDB is given to it explicitly), but on
`iw5mp_xenon` it failed at once with an `ArrayIndexOutOfBoundsException` in `DefaultCompositeMember.isGoodAlignment`,
which aborted the whole PDB step: no PDB types, prototypes or locals reached the project. The bug is Ghidra's
(still on master as of 2026-11-25): checking whether packing moved a struct's members, it walks the packed test
copy's components while indexing the original's defined components, without comparing their counts. The Xbox 360
GPU register unions (big-endian bit-fields, which Ghidra already warns it can't align) give the copy a different
count. A different count means the layout moved, so the check now returns false
(`tools/ghidra/patches/pdb-isGoodAlignment-bounds.patch`, applied by `tools/ghidra/patch_ghidra.sh` to the installed
release in CI). With it, the PDB step runs to completion and applies the PDB's types and prototypes.

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

## Layouts

Every struct in the PDBs is the 32-bit big-endian layout. Pointer-holding structs are larger on the x64 Store
build; padding and alignment otherwise follow the same rules (MSVC, natural alignment). Predict with
`tools/pdb/layout64.py`, then confirm against the Store exe's own field accesses before relying on an offset.
