#!/usr/bin/env python3
"""Name the Store exe's GDK API thunks from the GDK's own thunk library.

    python3 tools/gdk/name_thunks.py <exe> [--build iw5mp_ship] [--cache DIR]

Every GDK API the game calls is a thunk linked from xgameruntime.lib: it asks QueryApiImpl (through the exe's
loader at the build's query wrapper) for an interface by two GUIDs, calls one vtable slot, and releases it
(slot 0x10). The library ships with symbols, so (interface GUID, GUID, slot) names each of the exe's copies.
Both sides are read from the bytes: the library's COFF relocations give its GUID symbols; the exe's
`lea rcx/rdx, [rip+x]` give its GUIDs, and the first `mov rax, [rax+disp]` after the query that isn't Release is
the slot. Writes analysis/<build>/gdk/thunks.tsv. The package is downloaded from NuGet (target.toml [gdk]) and
verified by sha256; nothing from it is committed.
"""
import argparse
import hashlib
import io
import re
import struct
import sys
import tomllib
import urllib.request
import zipfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools"))


def fetch_package(cfg, cache):
    spec = cfg["gdk"]["nupkg"]
    path = cache / Path(spec["url"]).name
    if not path.exists():
        cache.mkdir(parents=True, exist_ok=True)
        req = urllib.request.Request(spec["url"], headers={"User-Agent": "modern-warfare-3-decomp/gdk"})
        with urllib.request.urlopen(req) as r:
            path.write_bytes(r.read())
    data = path.read_bytes()
    if hashlib.sha256(data).hexdigest() != spec["sha256"]:
        sys.exit(f"sha256 mismatch for {path}")
    with zipfile.ZipFile(io.BytesIO(data)) as z:
        return z.read(cfg["gdk"]["thunk_lib"])


def archive_members(lib):
    """Yield the object members of an ar archive (skipping the linker members)."""
    assert lib[:8] == b"!<arch>\n"
    pos = 8
    while pos + 60 <= len(lib):
        name = lib[pos:pos + 16].rstrip()
        size = int(lib[pos + 48:pos + 58])
        body = lib[pos + 60:pos + 60 + size]
        if name not in (b"/", b"//"):
            yield body
        pos += 60 + size + (size & 1)


def fmt_guid(b):
    a, b1, c = struct.unpack("<IHH", b[:8])
    return f"{a:08x}-{b1:04x}-{c:04x}-{b[8:10].hex()}-{b[10:16].hex()}"


def slot_after(code, start, end):
    """The first vtable slot loaded into rax from [rax+disp] after `start`, ignoring IUnknown::Release (0x10)."""
    for i in range(start, end - 3):
        if code[i:i + 3] == b"\x48\x8b\x40" and code[i + 3] != 0x10:
            return code[i + 3]
        if code[i:i + 3] == b"\x48\x8b\x80" and i + 7 <= end:
            return struct.unpack_from("<I", code, i + 3)[0]
    return None


def library_thunks(obj):
    """{(rcx guid, rdx guid, slot): name} from the library's COFF object."""
    machine, nsec, _, symptr, nsym, optsz, _ = struct.unpack_from("<HHIIIHH", obj, 0)
    strtab = symptr + nsym * 18
    def sym_name(raw):
        if raw[:4] == b"\0\0\0\0":
            off = struct.unpack_from("<I", raw, 4)[0]
            return obj[strtab + off:obj.index(b"\0", strtab + off)].decode()
        return raw.rstrip(b"\0").decode()
    syms, i = {}, 0
    while i < nsym:
        raw = obj[symptr + i * 18:symptr + i * 18 + 18]
        value, sec, typ, scl, naux = struct.unpack_from("<IhHBB", raw, 8)
        syms[i] = (sym_name(raw[:8]), value, sec, typ, scl)
        i += 1 + naux
    # GUID values: the _GUID_ symbols' bytes.
    sections = [struct.unpack_from("<8sIIIIIIHHI", obj, 20 + optsz + 40 * k) for k in range(nsec)]
    def sec_data(k):
        s = sections[k - 1]
        return obj[s[4]:s[4] + s[3]]
    guid_of = {}
    for idx, (name, value, sec, typ, scl) in syms.items():
        if name.startswith("_GUID_") and sec > 0:
            guid_of[idx] = fmt_guid(sec_data(sec)[value:value + 16])
    funcs = {}
    for idx, (name, value, sec, typ, scl) in syms.items():
        if typ == 0x20 and scl == 2 and sec > 0 and value == 0:
            funcs.setdefault(sec, name)
    out = {}
    for sec, name in funcs.items():
        s = sections[sec - 1]
        code = sec_data(sec)
        relocs = [struct.unpack_from("<IIH", obj, s[5] + 10 * r) for r in range(s[7])]
        regs, rcx_at = {}, 0
        for off, symidx, typ in sorted(relocs):
            target = syms[symidx][0]
            if symidx in guid_of:
                op = code[off - 3:off]
                if op == b"\x48\x8d\x0d":
                    regs["rcx"] = guid_of[symidx]
                    rcx_at = off
                elif op == b"\x48\x8d\x15":
                    regs["rdx"] = guid_of[symidx]
            elif target == "QueryApiImpl":
                if "rcx" in regs and b"\x48\x8b\xd1" in code[rcx_at:off]:  # mov rdx, rcx: one GUID for both
                    regs["rdx"] = regs["rcx"]
                slot = slot_after(code, off + 4, len(code))
                if slot is not None and "rcx" in regs and "rdx" in regs:
                    out[(regs["rcx"], regs["rdx"], slot)] = name
    return out


def exe_thunks(exe_bytes, build, query):
    """[(va, rcx guid, rdx guid, slot)] for every caller of the exe's query wrapper."""
    import pefile
    pe = pefile.PE(data=exe_bytes, fast_load=True)
    base = pe.OPTIONAL_HEADER.ImageBase
    def read(va, n):
        off = pe.get_offset_from_rva(va - base)
        return exe_bytes[off:off + n]
    funcs = {}
    for line in (ROOT / "analysis" / build / "functions.tsv").read_text().splitlines()[1:]:
        p = line.split("\t")
        funcs[int(p[0], 16)] = int(p[1])
    callers = set()
    for line in (ROOT / "analysis" / build / "ghidra" / "callgraph.tsv").read_text().splitlines()[1:]:
        p = line.split("\t")
        if int(p[1], 16) == query:
            callers.add(int(p[0], 16))
    out = []
    for va in sorted(callers):
        code = read(va, funcs[va])
        regs, found = {}, None
        for i in range(len(code) - 7):
            if code[i:i + 3] in (b"\x48\x8d\x0d", b"\x48\x8d\x15"):
                target = va + i + 7 + struct.unpack_from("<i", code, i + 3)[0]
                regs["rcx" if code[i + 2] == 0x0d else "rdx"] = fmt_guid(read(target, 16))
            elif code[i:i + 3] == b"\x48\x8b\xd1":
                regs["rdx"] = regs.get("rcx")
            elif code[i] == 0xE8 and va + i + 5 + struct.unpack_from("<i", code, i + 1)[0] == query:
                slot = slot_after(code, i + 5, len(code))
                if slot is not None:
                    found = (regs.get("rcx"), regs.get("rdx"), slot)  # the last query is the API's own
        out.append((va,) + found)
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("exe")
    ap.add_argument("--build", default="iw5mp_ship")
    ap.add_argument("--query", default="140360220", help="the exe's QueryApiImpl wrapper")
    ap.add_argument("--cache", default=str(ROOT / "build" / "cache"))
    a = ap.parse_args()
    cfg = tomllib.loads((ROOT / "target.toml").read_text())
    lib = fetch_package(cfg, Path(a.cache))
    names = {}
    for obj in archive_members(lib):
        names.update(library_thunks(obj))
    rows, missing = [], 0
    for va, rcx, rdx, slot in exe_thunks(Path(a.exe).read_bytes(), a.build, int(a.query, 16)):
        name = names.get((rcx, rdx, slot), "")
        missing += not name
        rows.append(f"{va:X}\t{name}\t{rcx}\t{rdx}\t0x{slot:X}")
    out = ROOT / "analysis" / a.build / "gdk" / "thunks.tsv"
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text("# GENERATED by tools/gdk/name_thunks.py. Do not edit.\n"
                   "va\tname\tiid\tclass\tslot\n" + "\n".join(rows) + "\n")
    print(f"{out.relative_to(ROOT)}: {len(rows)} thunks, {missing} unnamed")
    return 1 if missing else 0


if __name__ == "__main__":
    sys.exit(main())
