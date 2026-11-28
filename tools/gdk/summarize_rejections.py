#!/usr/bin/env python3
"""Summarize debugger telemetry without exposing identity or network data."""
import argparse
import collections
import csv
import re
import uuid
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
MARKER = re.compile(r"iw5_gdk_rejected iid_le=([0-9a-f]{32}|-{32}) class_le=([0-9a-f]{32}|-{32})")


def decode(raw):
    return "null" if raw.startswith("-") else str(uuid.UUID(bytes_le=bytes.fromhex(raw)))


def summarize(lines, catalogue):
    counts = collections.Counter()
    first_seen = []
    for line in lines:
        for match in MARKER.finditer(line):
            pair = tuple(map(decode, match.groups()))
            if pair not in counts:
                first_seen.append(pair)
            counts[pair] += 1
    result = [f"Raw rejection marker occurrences: {sum(counts.values())} (not unique calls; emitter bounded to first 256 per process)",
              f"Distinct interface/class pairs: {len(counts)}"]
    for pair in first_seen:
        names = ", ".join(sorted(catalogue.get(pair, ()))) or "unknown"
        result.append(f"{counts[pair]:4}  {pair[0]}  {pair[1]}  {names}")
    if not counts:
        result.append("No rejection markers captured; absence of markers does not prove successful initialization.")
    result.append("Interface pairs identify candidate APIs, not the exact thunk or failing caller.")
    return "\n".join(result)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("log", type=Path)
    args = parser.parse_args()
    catalogue = collections.defaultdict(set)
    path = ROOT / "analysis/iw5mp_ship/gdk/thunks.tsv"
    with path.open(encoding="utf-8") as f:
        for row in csv.DictReader((line for line in f if not line.startswith("#")), delimiter="\t"):
            catalogue[(row["iid"].lower(), row["class"].lower())].add(row["name"])
    with args.log.open(encoding="utf-8", errors="replace") as f:
        print(summarize(f, catalogue))


if __name__ == "__main__":
    main()
