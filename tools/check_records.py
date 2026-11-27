#!/usr/bin/env python3
"""Fail if records/ claims more than the evidence shows.

    python3 tools/check_records.py

- every records/functions.tsv row names a known build, an address in that build's inventory and a ladder status;
- `decompiled` and above name an authored file under src/ or include/ that exists;
- `matches` and above need a differential test: until tests/differential exists for that build's architecture,
  no row may claim them (docs/verification.md);
- records/names.tsv rows carry a method, evidence and confidence (AGENTS.md, Identifications);
- records/bugs.tsv ids are unique and use the ledger's kinds and priorities.
"""
import sys

from model import LADDER, ROOT, config, load, read_tsv

KINDS = {"security", "crash", "hang", "logic", "ub", "robustness", "limit", "precision", "performance", "quality"}


def main():
    builds = {b["id"]: b for b in config()["build"]}
    errors = []
    inventories = {}
    harness = (ROOT / "tests" / "differential" / "run.py").is_file()
    for i, r in enumerate(read_tsv(ROOT / "records" / "functions.tsv"), 2):
        where = f"records/functions.tsv:{i}"
        if r["build"] not in builds:
            errors.append(f"{where}: unknown build {r['build']}")
            continue
        if r["status"] not in LADDER:
            errors.append(f"{where}: unknown status {r['status']}")
            continue
        inv = inventories.setdefault(r["build"], load(r["build"]))
        if inv and r["va"] not in inv:
            errors.append(f"{where}: {r['va']} is not a known function of {r['build']}")
        level = LADDER.index(r["status"])
        if level >= LADDER.index("decompiled") and not (r["src"] and (ROOT / r["src"]).is_file()):
            errors.append(f"{where}: {r['status']} needs an existing src file, got {r['src']!r}")
        if level >= LADDER.index("matches") and not harness:
            errors.append(f"{where}: {r['status']} claimed but there is no differential harness yet")
    for i, r in enumerate(read_tsv(ROOT / "records" / "names.tsv"), 2):
        if not (r["method"] and r["evidence"] and r["confidence"]):
            errors.append(f"records/names.tsv:{i}: identification without method, evidence and confidence")
    seen = set()
    for i, r in enumerate(read_tsv(ROOT / "records" / "bugs.tsv"), 2):
        if r["id"] in seen:
            errors.append(f"records/bugs.tsv:{i}: duplicate id {r['id']}")
        seen.add(r["id"])
        if r["kind"] not in KINDS or r["priority"] not in ("high", "medium", "low"):
            errors.append(f"records/bugs.tsv:{i}: bad kind or priority")
    for e in errors:
        print(e)
    print(f"records: {len(errors)} problem(s)")
    return 1 if errors else 0


if __name__ == "__main__":
    sys.exit(main())
