#!/usr/bin/env python3
"""Read-only Store callsite and branch evidence. Static paths are not runtime hits."""
import argparse
import hashlib
import tomllib
from pathlib import Path
import capstone
import pefile
from task_evidence import read_rows, collect_edges

ROOT = Path(__file__).resolve().parents[2]
BUILD = "iw5mp_ship"
FOCUS = {
    0x1403390D0: "connection submission wrapper",
    0x1401A4DC0: "original user submission",
    0x1401145C0: "dwLogOnFrame",
    0x140114C40: "dwLogOnStart",
    0x1401148E0: "dwLogOnUpdate",
    0x140405490: "original token-related HTTP path",
    0x1403E9FE0: "BdBeginWinHttpRequest",
    0x1403F3980: "bdLobbyService::connect",
    0x1401A5A10: "original Store setup",
    0x1401A4400: "original licence query",
}

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("exe", type=Path)
    args = parser.parse_args()
    raw = args.exe.read_bytes()
    spec = next(b for b in tomllib.loads((ROOT/"target.toml").read_text())["build"] if b["id"] == BUILD)
    digest = hashlib.sha256(raw).hexdigest()
    if digest != spec["exe"]["sha256"]:
        raise SystemExit("unexpected executable SHA-256")
    pe = pefile.PE(data=raw, fast_load=True)
    base = pe.OPTIONAL_HEADER.ImageBase
    pe.parse_data_directories(directories=[pefile.DIRECTORY_ENTRY["IMAGE_DIRECTORY_ENTRY_EXCEPTION"]])
    spans = {base+e.struct.BeginAddress:e.struct.EndAddress-e.struct.BeginAddress
             for e in pe.DIRECTORY_ENTRY_EXCEPTION}
    directory = ROOT/"analysis"/BUILD
    api = {int(r["va"],16):r["name"] for r in read_rows(directory/"gdk/thunks.tsv")
           if r["name"].startswith(("XUser", "XStore"))}
    names = dict(FOCUS, **{})
    names.update(api)
    with (directory/"ghidra/callgraph.tsv").open() as f:
        edges = collect_edges(f,set(names))
    print("Pinned SHA-256:",digest)
    print("Static direct callers; not a dynamic trace")
    for va,name in sorted(names.items()):
        print(f"{va:09X} {name}: " + " ".join(f"{x:09X}" for x in sorted(edges[va])))
    md = capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_64)
    md.detail = True
    for va,name in sorted(FOCUS.items()):
        size=spans.get(va)
        print(f"\n{name} {va:09X}: {size or 0} bytes")
        if not size:
            print("No exact primary unwind region; not inferred.")
            continue
        for ins in md.disasm(pe.get_data(va-base,size),va):
            suffix=""
            if ins.mnemonic == "call" and ins.operands and ins.operands[0].type == capstone.x86.X86_OP_IMM:
                target=ins.operands[0].imm
                suffix=" ; "+names.get(target,"") if target in names else ""
            print(f"{ins.address:09X} {ins.mnemonic:8} {ins.op_str}{suffix}")
    for marker in (b"cl_activeAction",b"CL_InitCGame:",b"Server Initialization"):
        print(f"Original literal {marker.decode()!r}: {'present' if marker in raw else 'not found'}")

if __name__=="__main__":
    main()
