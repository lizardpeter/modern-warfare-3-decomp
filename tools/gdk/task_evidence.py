#!/usr/bin/env python3
"""Read-only evidence for ordinary engine task bookkeeping on the pinned Store PE.

The existing Ghidra call graph is an index, not proof of dynamic execution.
Disassemble TaskGroup_Init from the original x64 unwind extent, not from the
Xbox reference. No executable, import table, runtime or authorization is changed.
"""
import argparse
import csv
import hashlib
import tomllib
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
BUILD = "iw5mp_ship"
TASK_GROUP = 0x14027E8C0


def read_rows(path):
    with path.open(encoding="utf-8") as stream:
        yield from csv.DictReader((line for line in stream if not line.startswith("#")), delimiter="\t")


def scheduling_catalogue(rows):
    return {int(row["va"], 16): row["name"] for row in rows
            if row["name"].startswith(("XAsync", "XTaskQueue", "XThread"))}


def collect_edges(lines, targets):
    """The existing callgraph index uses caller VA and callee VA in its first two columns."""
    edges = {target: set() for target in targets}
    for line in lines:
        fields = line.rstrip("\n").split("\t")
        if len(fields) < 2:
            continue
        try:
            caller, callee = (int(value, 16) for value in fields[:2])
        except ValueError:
            continue
        if callee in edges:
            edges[callee].add(caller)
    return edges


def main():
    import capstone
    import pefile

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
    spans = {base + e.struct.BeginAddress: e.struct.EndAddress - e.struct.BeginAddress
             for e in pe.DIRECTORY_ENTRY_EXCEPTION}
    task = scheduling_catalogue(read_rows(ROOT / "analysis" / BUILD / "gdk" / "thunks.tsv"))
    if len(task) != 19:
        raise SystemExit(f"unexpected scheduling thunk count: {len(task)}")
    with (ROOT / "analysis" / BUILD / "ghidra" / "callgraph.tsv").open(encoding="utf-8") as graph:
        edges = collect_edges(graph, set(task) | {TASK_GROUP})
    if not any(edges.values()):
        raise SystemExit("call graph yielded no task edges; inspect input format")

    print(f"Original SHA-256: {digest}")
    print("Static Ghidra graph: distinct caller counts (not runtime call frequencies)")
    for addr, name in sorted(task.items()):
        print(f"{name:36} {len(edges[addr]):5}")
    print(f"TaskGroup_Init indexed callers: {len(edges[TASK_GROUP])}")
    size = spans.get(TASK_GROUP)
    if not size:
        raise SystemExit("TaskGroup_Init has no exact primary unwind extent")
    body = pe.get_data(TASK_GROUP - base, size)
    if len(body) != size:
        raise SystemExit("truncated TaskGroup_Init")
    print(f"\nTaskGroup_Init: primary unwind extent ({size} bytes)")
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_64)
    md.skipdata = True
    for ins in md.disasm(body, TASK_GROUP):
        print(f"{ins.address:09X}  {ins.bytes.hex():<32} {ins.mnemonic:<10} {ins.op_str}")
    print("\nChained unwind regions and indirect calls require separate inspection.")
    print("No function is classified as matched by this report.")


if __name__ == "__main__":
    main()
