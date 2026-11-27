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

Ghidra imports these with `ghidra_language = "PowerPC:BE:64:A2ALT-32addr"` (PowerPC 64, 32-bit addresses, AltiVec).
The Xenon's **VMX128** extensions (128 vector registers, the `vmx128` opcodes used throughout the math and
rendering code) are not in Ghidra's PowerPC language: those instructions disassemble as bad data, and functions that
use them stop decompiling at the first one. A VMX128 SLEIGH extension is on the roadmap; until then, read those
functions from the disassembly and the PDB's line tables.

Ghidra's own PDB analyzer matches a PDB to a program through CodeView, which these exes lack, so it may not apply
the PDB. `ApplySymbols.java` doesn't depend on it: before analysis it creates and names every function the PDB,
MAP and `.pdata` know, from `analysis/<build>/pdb` and `map` (tools/pdb/extract.py, which reads the PDB with
llvm-pdbutil and resymc and needs no CodeView). Types come from `pdb/types.h` either way.

## Layouts

Every struct in the PDBs is the 32-bit big-endian layout. Pointer-holding structs are larger on the x64 Store
build; padding and alignment otherwise follow the same rules (MSVC, natural alignment). Predict with
`tools/pdb/layout64.py`, then confirm against the Store exe's own field accesses before relying on an offset.
