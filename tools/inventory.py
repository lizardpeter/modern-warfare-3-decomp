#!/usr/bin/env python3
"""Write a PE inventory for one hash-pinned original without loading it.

    python3 tools/inventory.py <build> <exe>

<exe> is what tools/fetch_originals.py writes: for an Xbox 360 build, the PE unpacked from its XEX (checked against
`unpacked_sha256`).

Writes analysis/<build>/pe.json, and for x64 builds analysis/<build>/pdata.tsv: one row per function from the
exception directory (.pdata), the compiler's own record of every non-leaf function's bounds. Xbox 360 builds
(architecture "ppc") get the same table from their own .pdata format: 8 big-endian bytes per function, the
absolute start address and a packed word (prolog length, function length in instructions, 32-bit and exception
flags); Xenon code lists nearly every function there, leaves included. Entries whose unwind info
is chained (UNW_FLAG_CHAININFO) are separated-out parts of another function and are listed with `part_of`, not counted
as functions. Leaf functions that never touch the stack have no entry; Ghidra's inventory supersedes this table.
Xbox 360 builds also get xenon_helpers.tsv (the compiler's register save/restore helpers, found by their code) and
code_refs.tsv (code addresses referred to outside every .pdata function: leaf function candidates).
"""
import argparse
import bisect
import hashlib
import json
import struct
import uuid
from pathlib import Path

import pefile

from model import ROOT, config


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("build")
    parser.add_argument("exe", type=Path)
    args = parser.parse_args()
    build = next(item for item in config()["build"] if item["id"] == args.build)
    data = args.exe.read_bytes()
    digest = hashlib.sha256(data).hexdigest()
    if digest != build.get("unpacked_sha256", build["exe"]["sha256"]):
        raise ValueError(f"{args.build}: executable SHA-256 mismatch")
    pe = pefile.PE(data=data)
    machine = {"x86": 0x14c, "x64": 0x8664, "ppc": 0x1F2}[build.get("architecture", "x86")]
    if pe.FILE_HEADER.Machine != machine or pe.OPTIONAL_HEADER.ImageBase != build["image_base"]:
        raise ValueError(f"{args.build}: expected {build.get('architecture', 'x86')} PE at the pinned image base")

    if "pdb_signature" in build:
        # Xbox 360: no CodeView record; the PDB's signature is the exe's link timestamp (its GUID and age are pinned
        # from the PDB itself and checked by tools/pdb/extract.py)
        if pe.FILE_HEADER.TimeDateStamp != build["pdb_signature"]:
            raise ValueError(f"{args.build}: PE timestamp is not the pinned PDB signature")
        pdb_guid, pdb_age = build["pdb_guid"], build["pdb_age"]
    else:
        codeview = next(entry.entry for entry in pe.DIRECTORY_ENTRY_DEBUG
                        if getattr(getattr(entry, "entry", None), "CvSignature", None) == b"RSDS")
        pdb_guid, pdb_age = str(uuid.UUID(bytes_le=codeview.__pack__()[4:20])).upper(), codeview.Age
        if (pdb_guid, pdb_age) != (build["pdb_guid"], build["pdb_age"]):
            raise ValueError(f"{args.build}: CodeView identity mismatch")

    metadata = {
        "build": args.build,
        "sha256": digest,
        "size": len(data),
        "image_base": f"{pe.OPTIONAL_HEADER.ImageBase:08X}",
        "entry_point": f"{pe.OPTIONAL_HEADER.ImageBase + pe.OPTIONAL_HEADER.AddressOfEntryPoint:08X}",
        "linker_version": f"{pe.OPTIONAL_HEADER.MajorLinkerVersion}.{pe.OPTIONAL_HEADER.MinorLinkerVersion:02d}",
        "pdb": {"guid": pdb_guid, "age": pdb_age},
        "sections": [section.Name.rstrip(b"\0").decode("ascii") for section in pe.sections],
        "imports": [entry.dll.decode("ascii") for entry in getattr(pe, "DIRECTORY_ENTRY_IMPORT", [])],
        "delay_imports": [entry.dll.decode("ascii") for entry in getattr(pe, "DIRECTORY_ENTRY_DELAY_IMPORT", [])],
    }
    out = ROOT / "analysis" / args.build / "pe.json"
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text(json.dumps(metadata, indent=2) + "\n")
    if machine == 0x8664:
        write_pdata(pe, out.parent / "pdata.tsv")
    elif machine == 0x1F2:
        rows = write_pdata_xenon(pe, out.parent / "pdata.tsv")
        write_xenon_helpers(pe, out.parent / "xenon_helpers.tsv")
        write_xenon_code_refs(pe, rows, out.parent / "code_refs.tsv")


def write_pdata(pe, path):
    """Function bounds from .pdata. A chained entry's unwind info ends with the parent RUNTIME_FUNCTION."""
    base = pe.OPTIONAL_HEADER.ImageBase
    d = pe.OPTIONAL_HEADER.DATA_DIRECTORY[3]
    raw = pe.get_data(d.VirtualAddress, d.Size)
    rows, parts = [], 0
    for i in range(0, len(raw) - 11, 12):
        begin, end, unwind = struct.unpack_from("<III", raw, i)
        if not begin:
            continue
        head = pe.get_data(unwind & ~1, 4)
        flags, codes = head[0] >> 3, head[2]
        part_of = ""
        if flags & 4:  # UNW_FLAG_CHAININFO: the parent's RUNTIME_FUNCTION follows the (even-padded) unwind codes
            parent = pe.get_data((unwind & ~1) + 4 + 2 * (codes + (codes & 1)), 12)
            part_of = f"{base + struct.unpack_from('<I', parent, 0)[0]:08X}"
            parts += 1
        rows.append((base + begin, end - begin, part_of))
    rows.sort()
    with open(path, "w", newline="\n") as f:
        f.write("va\tsize\tpart_of\n")
        for va, size, part_of in rows:
            f.write(f"{va:08X}\t{size}\t{part_of}\n")
    print(f"{path}: {len(rows) - parts:,} functions, {parts:,} chained parts")


def xenon_read(pe, rva, size):
    """Bytes at an RVA of an Xbox 360 image, mapped through the section headers as written. pefile's own mapping
    (get_offset_from_rva, get_data) first rounds each section's address down to SectionAlignment, which Xenon
    sections don't follow (.pdata at RVA 0x7B600 with 64 KB alignment), and so reads from the wrong offset."""
    for s in pe.sections:
        if s.VirtualAddress <= rva and rva + size <= s.VirtualAddress + max(s.Misc_VirtualSize, s.SizeOfRawData):
            off = s.PointerToRawData + rva - s.VirtualAddress
            raw = pe.__data__[off:off + size]
            if len(raw) != size:
                raise ValueError(f"RVA {rva:X}: {len(raw)} of {size} bytes in the file")
            return raw
    raise ValueError(f"RVA {rva:X} (+{size:X}) is not inside one section")


def write_pdata_xenon(pe, path):
    """Function bounds from an Xbox 360 .pdata: (start VA, packed) big-endian; length is bits 8..29, in instructions."""
    d = pe.OPTIONAL_HEADER.DATA_DIRECTORY[3]
    raw = xenon_read(pe, d.VirtualAddress, d.Size)
    base = pe.OPTIONAL_HEADER.ImageBase
    code = [(base + s.VirtualAddress, base + s.VirtualAddress + s.Misc_VirtualSize) for s in pe.sections
            if s.Characteristics & 0x20000000]
    rows = set()
    for i in range(0, len(raw) - 7, 8):
        begin, packed = struct.unpack_from(">II", raw, i)
        length = (packed >> 8) & 0x3FFFFF
        if not (begin and length):
            continue
        if not any(lo <= begin and begin + 4 * length <= hi for lo, hi in code):
            raise ValueError(f"{path}: entry {i // 8} ({begin:08X}, {4 * length} bytes) is outside the code sections")
        rows.add((begin, 4 * length))
    starts = [va for va, _ in rows]
    if len(set(starts)) != len(starts):
        raise ValueError(f"{path}: a function start is listed with two lengths")
    with open(path, "w", newline="\n") as f:
        f.write("va\tsize\tpart_of\n")
        for va, size in sorted(rows):
            f.write(f"{va:08X}\t{size}\t\n")
    print(f"{path}: {len(rows):,} functions")
    return sorted(rows)


def write_xenon_code_refs(pe, pdata, path):
    """Code addresses the image refers to outside every .pdata function: the starts of leaf functions (no unwind
    data, so no .pdata entry) that Ghidra's PowerPC analysis doesn't reach because nothing calls them directly. Found
    from the exe alone, three ways:
      call     a `bl` target
      address  built by `lis rX, hi` + `addi rY, rX, lo` (a callback passed by address: Cmd_AddCommand(..., Func))
      pointer  a 4-byte word in a data section (function tables)
    SeedCodeRefs.java creates functions from them after Ghidra's own analysis (call targets anywhere, the others only
    where no function covers them; docs/xbox360.md)."""
    base = pe.OPTIONAL_HEADER.ImageBase
    code = [(base + s.VirtualAddress, base + s.VirtualAddress + s.Misc_VirtualSize, s) for s in pe.sections
            if s.Characteristics & 0x20000000]
    starts = [va for va, _ in pdata]
    ends = {va: va + size for va, size in pdata}

    def outside_pdata(v):
        if v % 4 or not any(lo <= v < hi for lo, hi, _ in code):
            return False
        i = bisect.bisect_right(starts, v) - 1
        return i < 0 or v >= ends[starts[i]]

    sext = lambda x: x - 0x10000 if x & 0x8000 else x
    how = {}
    for lo, hi, s in code:
        words = struct.unpack_from(f">{(hi - lo) // 4}I", xenon_read(pe, lo - base, hi - lo))
        pending = {}  # register -> high half from lis, in straight-line code
        for i, w in enumerate(words):
            op, rd, ra = w >> 26, (w >> 21) & 31, (w >> 16) & 31
            if op == 18:
                if w & 1:
                    disp = (w & 0x03FFFFFC) - (0x04000000 if w & 0x02000000 else 0)
                    how.setdefault((disp if w & 2 else lo + 4 * i + disp) & 0xFFFFFFFF, "call")
                pending.clear()
                continue
            if op in (16, 19):
                pending.clear()
                continue
            if op == 15 and ra == 0:
                pending[rd] = (w & 0xFFFF) << 16
                continue
            if op == 14 and ra in pending:
                how.setdefault((pending[ra] + sext(w & 0xFFFF)) & 0xFFFFFFFF, "address")
            if op in (7, 8, 12, 13, 14, 15) or 32 <= op <= 46 or op == 58:
                pending.pop(rd, None)
            if op in (20, 21, 23, 24, 25, 26, 27, 28, 29, 30, 31):
                pending.pop(ra, None)
                pending.pop(rd, None)
    for s in pe.sections:
        if s.Characteristics & 0x20000000 or not s.SizeOfRawData:
            continue
        n = min(s.SizeOfRawData, s.Misc_VirtualSize) // 4
        for w in struct.unpack_from(f">{n}I", pe.__data__, s.PointerToRawData):
            how.setdefault(w, "pointer")
    rows = sorted((v, h) for v, h in how.items() if outside_pdata(v))
    with open(path, "w", newline="\n") as f:
        f.write("va\thow\n")
        for v, h in rows:
            f.write(f"{v:08X}\t{h}\n")
    print(f"{path}: {len(rows):,} code addresses outside .pdata functions")



BLR = 0x4E800020


def xenon_helper_chains():
    """The Xenon compiler's register save/restore helpers as instruction-word chains: (family, first register,
    words per entry, words). Each is one chain of entries, one per register, falling through to a shared tail, so
    __savegprlr_NN is the entry that saves rNN..r31 and LR. Encoded from the instruction formats, not copied from
    a build, so the same scan finds them in any Xenon exe, symbols or not:
      __savegprlr_14..31   std rN, -0x98+8*(N-14)(r1) ... ; stw r12, -8(r1) ; blr
      __restgprlr_14..31   ld  rN, ...(r1) ... ; lwz r12, -8(r1) ; mtlr r12 ; blr
      __savefpr_14..31     stfd fN, -0x90+8*(N-14)(r12) ... ; blr        (__restfpr: lfd)
      __savevmx_14..31     li r11, -0x120+16*(N-14) ; stvx vN, r11, r12 ... ; blr     (__restvmx: lvx)
      __savevmx_64..127    li r11, -0x400+16*(N-64) ; stvx128 vN, r11, r12 ... ; blr  (__restvmx: lvx128)"""
    def d(x):
        return x & 0xFFFF
    gpr = lambda op: [(op << 26) | (n << 21) | (1 << 16) | d(-0x98 + 8 * (n - 14)) for n in range(14, 32)]
    fpr = lambda op: [(op << 26) | (n << 21) | (12 << 16) | d(-0x90 + 8 * (n - 14)) for n in range(14, 32)]
    li = lambda off: 0x39600000 | d(off)  # addi r11, r0, off
    vmx = lambda xo: [w for n in range(14, 32) for w in (li(-0x120 + 16 * (n - 14)),
                                                         0x7C000000 | (n << 21) | (11 << 16) | (12 << 11) | (xo << 1))]
    # VMX128 VX128_1 form: register bits 0-4 at 21, bits 5-6 at 2 (tools/ghidra/xenon/vmx128.sinc)
    vmx128 = lambda xo: [w for n in range(64, 128) for w in (li(-0x400 + 16 * (n - 64)),
                                                             0x10000000 | ((n & 31) << 21) | (11 << 16) | (12 << 11)
                                                             | xo | ((n >> 5) << 2))]
    return [
        ("savegprlr", 14, 1, gpr(62) + [0x9181FFF8, BLR]),
        ("restgprlr", 14, 1, gpr(58) + [0x8181FFF8, 0x7D8803A6, BLR]),
        ("savefpr", 14, 1, fpr(54) + [BLR]),
        ("restfpr", 14, 1, fpr(50) + [BLR]),
        ("savevmx", 14, 2, vmx(231) + [BLR]),
        ("restvmx", 14, 2, vmx(103) + [BLR]),
        ("savevmx", 64, 2, vmx128(0x1C3) + [BLR]),
        ("restvmx", 64, 2, vmx128(0x0C3) + [BLR]),
    ]


def write_xenon_helpers(pe, path):
    """The register save/restore helpers, found by their code (xenon_helper_chains), one row per entry point.
    ApplySymbols.java gives them their names, marks them returning and attaches the Xenon language's call fixup,
    in every analysis including blind ones: they are toolchain code, identified from the instructions alone."""
    base = pe.OPTIONAL_HEADER.ImageBase
    rows = []
    for s in pe.sections:
        if not s.Characteristics & 0x20000000:  # IMAGE_SCN_MEM_EXECUTE
            continue
        data = pe.__data__[s.PointerToRawData:s.PointerToRawData + s.SizeOfRawData]
        for family, first, step, words in xenon_helper_chains():
            needle = struct.pack(f">{len(words)}I", *words)
            start = 0
            while (i := data.find(needle, start)) >= 0:
                start = i + 4
                if i % 4:
                    continue
                va = base + s.VirtualAddress + i
                count = (len(words) - 1) // step - (2 if family == "restgprlr" else 1 if family == "savegprlr" else 0)
                for k in range(count):
                    rows.append((va + 4 * step * k, f"__{family}_{first + k}"))
    names = [n for _, n in rows]
    dup = {n for n in names if names.count(n) > 1}
    if dup:
        raise ValueError(f"{path}: helper chain found more than once: {sorted(dup)[:4]}")
    with open(path, "w", newline="\n") as f:
        f.write("va\tname\n")
        for va, name in sorted(rows):
            f.write(f"{va:08X}\t{name}\n")
    print(f"{path}: {len(rows)} save/restore helper entry points")


if __name__ == "__main__":
    main()
