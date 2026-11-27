#!/usr/bin/env python3
"""Download one build's originals (exe, and pdb/map when the build has them) from R2 and verify their sha256.

    python3 tools/fetch_originals.py <build-id> <dest-dir> [--only exe|pdb|map]

Writes <dest>/<build-id>.exe (.pdb, .map). A build with `unpack = "xex"` (the Xbox 360 references) is stored in R2
as an XEX2: it is fetched as <build-id>.xex, verified, unpacked with tools/unpack/xex.py to the PE the linker wrote,
and the PE is checked against `unpacked_sha256` before it is written as <build-id>.exe. Nothing derived is stored.
"""
import hashlib
import sys
import tomllib
import urllib.parse
import urllib.request
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools" / "unpack"))


def fetch(cfg, spec, out):
    url = cfg["r2_public"] + urllib.parse.quote(spec["key"])
    h = hashlib.sha256()
    req = urllib.request.Request(url, headers={"User-Agent": "modern-warfare-3-decomp/fetch"})
    with urllib.request.urlopen(req) as r, open(out, "wb") as f:
        while chunk := r.read(1 << 20):
            h.update(chunk)
            f.write(chunk)
    if h.hexdigest() != spec["sha256"]:
        sys.exit(f"sha256 mismatch for {spec['key']}: {h.hexdigest()}")


def main():
    build_id, dest = sys.argv[1], Path(sys.argv[2])
    cfg = tomllib.loads((ROOT / "target.toml").read_text())
    build = next(b for b in cfg["build"] if b["id"] == build_id)
    dest.mkdir(parents=True, exist_ok=True)
    only = sys.argv[sys.argv.index("--only") + 1] if "--only" in sys.argv else None
    for role in ("exe", "pdb", "map"):
        if (only and role != only) or role not in build:  # retail builds have no PDB/MAP
            continue
        spec = build[role]
        out = dest / f"{build_id}.{role}"
        if role == "exe" and build.get("unpack") == "xex":
            from xex import Xex
            xex = dest / f"{build_id}.xex"
            fetch(cfg, spec, xex)
            image, key = Xex(xex.read_bytes()).image()
            digest = hashlib.sha256(image).hexdigest()
            if digest != build["unpacked_sha256"]:
                sys.exit(f"{build_id}: unpacked PE sha256 {digest} != unpacked_sha256 in target.toml")
            out.write_bytes(image)
            print(f"{role}: {out} ok (unpacked from {xex.name}, {key} key)")
            continue
        fetch(cfg, spec, out)
        print(f"{role}: {out} ok")


if __name__ == "__main__":
    main()
