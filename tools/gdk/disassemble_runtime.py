#!/usr/bin/env python3
"""Emit read-only instruction evidence for the Store GDK runtime and its named thunks.

Usage: python tools/gdk/disassemble_runtime.py /tmp/originals/iw5mp_ship.exe
The input is SHA-256 pinned. The report is generated from the PE, not edited by hand.
"""
import argparse
import csv
import hashlib
import tomllib
from pathlib import Path

import capstone
import pefile

ROOT = Path(__file__).resolve().parents[2]
BUILD = "iw5mp_ship"
CORE = {
    0x14035FFC4: "XGameRuntimeEnsureInitialized",
    0x1403601E0: "XGameRuntimeLoadModule",
    0x140360220: "XGameRuntimeQueryApi",
}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("exe", type=Path)
    args = parser.parse_args()
    config = tomllib.loads((ROOT / "target.toml").read_text())
    spec = next(b for b in config["build"] if b["id"] == BUILD)
    raw = args.exe.read_bytes()
    digest = hashlib.sha256(raw).hexdigest()
    if digest != spec["exe"]["sha256"]:
        raise SystemExit(f"unexpected original SHA-256: {digest}")
    pe = pefile.PE(data=raw, fast_load=True)
    base = pe.OPTIONAL_HEADER.ImageBase
    pe.parse_data_directories(directories=[pefile.DIRECTORY_ENTRY["IMAGE_DIRECTORY_ENTRY_EXCEPTION"]])
    sizes = {base + e.struct.BeginAddress: e.struct.EndAddress - e.struct.BeginAddress
             for e in pe.DIRECTORY_ENTRY_EXCEPTION}
    functions = dict(CORE)
    catalogue = ROOT / "analysis" / BUILD / "gdk" / "thunks.tsv"
    with catalogue.open(encoding="utf-8") as f:
        for row in csv.DictReader((line for line in f if not line.startswith("#")), delimiter="\t"):
            va = int(row["va"], 16)
            if va in functions or not row["name"]:
                raise SystemExit(f"duplicate or unnamed thunk: {row}")
            functions[va] = row["name"]
    print(f"Original SHA-256: {digest}")
    print(f"Functions: {len(functions)}")
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_64)
    md.skipdata = True
    for va, name in sorted(functions.items()):
        size = sizes.get(va)
        if not size:
            raise SystemExit(f"no unwind extent for {name} at {va:X}")
        code = pe.get_data(va - base, size)
        if len(code) != size:
            raise SystemExit(f"truncated function {name}")
        print(f"\n{name} {va:X}, {size} bytes")
        for insn in md.disasm(code, va):
            print(f"{insn.address:09X}  {insn.bytes.hex():<32} {insn.mnemonic:<10} {insn.op_str}")


if __name__ == "__main__":
    main()
