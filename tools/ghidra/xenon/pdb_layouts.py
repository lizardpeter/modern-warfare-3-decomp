#!/usr/bin/env python3
"""Test Ghidra's PDB composite reconstruction on a big-endian (Xbox 360) PDB, in minutes instead of a full analysis.

    python3 tools/ghidra/xenon/pdb_layouts.py cases <pdb> <out.json>     composites with bit-fields, and every type
                                                                       they use, from the PDB's type records
    (analyzeHeadless ... -postScript TestPdbLayouts.java <cases.json> <ghidra.json>)
    python3 tools/ghidra/xenon/pdb_layouts.py check <cases.json> <ghidra.json>
    python3 tools/ghidra/xenon/pdb_layouts.py applied <pdb> <dump.json>   after Ghidra's PDB analyzer applied the
                                                                       PDB (DumpPdbTypes.java wrote dump.json):
                                                                       every named composite and enum

`applied` is the end-to-end check: what the analyzer itself created (sizes, members, bit-fields, enum lengths)
against the PDB, for every named type defined once (types with several definitions and anonymous ones aside).
`cases` reads the PDB with llvm-pdbutil. TestPdbLayouts.java rebuilds each composite in a program of the Xenon
language with Ghidra's own reconstruction (DefaultCompositeMember.applyDataTypeMembers, the call
CompositeTypeApplier makes) and writes the result. `check` compares every member with the PDB: the size of each
composite, each member's absolute byte offset, and for a bit-field its bit position counted from the least
significant bit of its base type, which is how a PDB records it (docs/xbox360.md, Ghidra's PDB analyzer).
"""
import collections
import json
import re
import subprocess
import sys

REC = re.compile(r"^\s+(0x[0-9A-F]+) \| (LF_\w+) \[size = \d+\](?: `(.*)`)?$")
PRIM = {  # CodeView primitive types this PDB uses in composites -> (Ghidra builtin, size)
    "char": ("char", 1), "signed char": ("schar", 1), "unsigned char": ("uchar", 1), "bool": ("bool", 1),
    "short": ("short", 2), "unsigned short": ("ushort", 2), "wchar_t": ("wchar16", 2),
    "long": ("int", 4), "unsigned long": ("uint", 4), "int": ("int", 4), "unsigned": ("uint", 4),
    "__int64": ("longlong", 8), "unsigned __int64": ("ulonglong", 8), "float": ("float", 4),
    "double": ("double", 8), "HRESULT": ("int", 4), "char16_t": ("wchar16", 2),
}


def parse(pdb):
    out = subprocess.run(["llvm-pdbutil", "dump", "--types", pdb], capture_output=True, text=True, check=True).stdout
    recs, cur = {}, None
    for line in out.splitlines():
        m = REC.match(line)
        if m:
            cur = {"kind": m.group(2), "name": m.group(3), "lines": []}
            recs[m.group(1)] = cur
        elif cur is not None:
            cur["lines"].append(line.strip())
    return recs


def attr(rec, key):
    for l in rec["lines"]:
        m = re.search(rf"(?:^|[\s,:]){re.escape(key)} = ([^,]+)", l)
        if m:
            return m.group(1).strip()
    return None


def typeref(rec, key):
    """A type reference written 'key: 0x1234 (name)' or 'key = 0x1234 (name)'."""
    m = re.search(rf"(?:^|[\s,]){re.escape(key)}\s*[:=]\s*(0x[0-9A-F]+(?: \([^)]*\))?)", " ".join(rec["lines"]))
    return m.group(1) if m else None


def field(rec, key):
    text = " ".join(rec["lines"])
    m = re.search(rf"{key}: (0x[0-9A-F]+)", text)
    return m.group(1) if m else None


class Reader:
    def __init__(self, recs):
        self.recs = recs
        # composite key -> defining record; forward references resolve here. The key is the type's unique
        # (decorated) name, except where several definitions share it (all 110 anonymous structures here are
        # U<unnamed-tag>@@): those are referenced by record, never forward, so the record number keys them
        self.ids = {id(r): k for k, r in recs.items()}
        unique = collections.Counter(self.unique(r) for r in recs.values()
                                     if r["kind"] in ("LF_STRUCTURE", "LF_CLASS", "LF_UNION") and not self.forward(r))
        self.shared = {u for u, n in unique.items() if n > 1}
        self.defs = {}
        self.first = {}  # shared unique name -> key of its first definition, for forward references
        for k, r in recs.items():
            if r["kind"] in ("LF_STRUCTURE", "LF_CLASS", "LF_UNION") and not self.forward(r):
                self.defs.setdefault(self.key(r), k)
                self.first.setdefault(self.unique(r), self.key(r))

    @staticmethod
    def unique(r):
        m = re.search(r"unique name: `(.*?)`", " ".join(r["lines"]))
        return m.group(1) if m else r["name"]

    def key(self, r):
        u = self.unique(r)
        if u not in self.shared:
            return u
        # a forward reference to a shared name means the type of that name: its first definition
        return self.first.get(u, u) if self.forward(r) else f"{u}#{self.ids[id(r)]}"

    @staticmethod
    def forward(r):
        return any("forward ref" in l for l in r["lines"])

    def type_of(self, ref):
        """A member type as {kind, ...}; refs below 0x1000 are CodeView primitives."""
        m = re.match(r"(0x[0-9A-F]+)(?: \((.*)\))?", ref)
        key, pname = m.group(1), m.group(2)
        if int(key, 16) < 0x1000:
            if pname is None or pname.endswith("*"):
                return {"kind": "ptr"}
            if pname not in PRIM:
                raise KeyError(f"primitive {pname}")
            return {"kind": "prim", "name": PRIM[pname][0], "size": PRIM[pname][1]}
        r = self.recs[key]
        k = r["kind"]
        if k == "LF_MODIFIER":
            return self.type_of(typeref(r, "referent"))
        if k == "LF_POINTER":
            return {"kind": "ptr"}
        if k == "LF_ENUM":
            under = self.type_of(typeref(r, "underlying type"))
            return {"kind": "prim", "name": under["name"], "size": under["size"]}
        if k == "LF_ARRAY":
            size = int(re.search(r"size: (\d+)", " ".join(r["lines"])).group(1))
            return {"kind": "array", "elem": self.type_of(typeref(r, "element type")), "size": size}
        if k == "LF_BITFIELD":
            return {"kind": "bitfield", "base": self.type_of(typeref(r, "type")), "pos": int(attr(r, "bit offset")),
                    "bits": int(attr(r, "# bits"))}
        if k in ("LF_STRUCTURE", "LF_CLASS", "LF_UNION"):
            if self.key(r) not in self.defs:
                raise KeyError(f"no definition of {r['name']}")
            return {"kind": "named", "name": self.key(r)}
        raise KeyError(f"type {k}")

    def composite(self, name):
        r = self.recs[self.defs[name]]
        size = int(re.search(r"sizeof (\d+)", " ".join(r["lines"])).group(1))
        fl = self.recs[field(r, "field list")]
        members = []
        for l in fl["lines"]:
            if not l.startswith("- LF_"):
                continue
            m = re.match(r"- LF_MEMBER \[name = `(.*?)`, Type = (0x[0-9A-F]+(?: \(.*?\))?), offset = (\d+)", l)
            if not m:
                # static members, nested types and methods have no storage; base classes and vtable pointers are
                # laid out apart from the data members (CppCompositeType.createMembersOnlyClassLayout applies only
                # these, at their offsets in the class), so the case is the data members alone
                if l.startswith(("- LF_STMEMBER", "- LF_NESTTYPE", "- LF_ONEMETHOD", "- LF_METHOD", "- LF_BCLASS",
                                 "- LF_VFUNCTAB", "- LF_VBCLASS", "- LF_IVBCLASS")):
                    continue
                raise KeyError(f"member record {l[:30]}")
            members.append({"name": m.group(1), "offset": int(m.group(3)), "type": self.type_of(m.group(2))})
        return {"name": name, "display": r["name"], "union": r["kind"] == "LF_UNION", "size": size,
                "members": members}


def addressable(name):
    """Types Ghidra files under their plain name path: not anonymous, templates, function-local (f::__l2::T) or in
    an anonymous namespace"""
    return "<" not in name and "::__l" not in name and "anonymous-namespace" not in name


def deps(t):
    if t["kind"] == "named":
        yield t["name"]
    elif t["kind"] == "array":
        yield from deps(t["elem"])


def cases(pdb, out):
    rd = Reader(parse(pdb))
    with_bitfields = [n for n, k in rd.defs.items() if any(
        "LF_MEMBER" in l and rd.recs.get(re.search(r"Type = (0x[0-9A-F]+)", l).group(1), {}).get("kind") == "LF_BITFIELD"
        for l in rd.recs[field(rd.recs[k], "field list")]["lines"] if "Type = 0x" in l)]
    ordered, done, skipped = [], set(), []

    def visit(name, stack):
        if name in done or name in stack:
            return name in done
        try:
            c = rd.composite(name)
            for m in c["members"]:
                for d in deps(m["type"]):
                    if not visit(d, stack | {name}):
                        raise KeyError(f"dependency {d}")
        except KeyError as e:
            skipped.append((name, str(e)))
            return False
        done.add(name)
        ordered.append(c)
        return True

    for n in sorted(with_bitfields):  # keys (unique names)
        visit(n, frozenset())
    json.dump({"composites": ordered}, open(out, "w"))
    print(f"{len(with_bitfields)} composites with bit-fields; {len(ordered)} cases with dependencies; "
          f"{len(skipped)} skipped")
    for n, why in skipped[:10]:
        print(f"  skipped {n}: {why}")


def leaves(layout, case):
    """Flatten Ghidra's members to (name -> list of placements), descending into composites the reconstruction
    created (a component whose name isn't one of the PDB's member names for that composite)."""
    names = {m["name"] for m in case["members"]}
    out = {}

    def walk(comps, base):
        for c in comps:
            if c["name"] not in names and c.get("components") is not None:
                walk(c["components"], base + c["offset"])
            else:
                out.setdefault(c["name"], []).append({**c, "offset": base + c["offset"]})
    walk(layout["components"], 0)
    return out


def compare(case, g, big_endian):
    """Problems in Ghidra's layout `g` of PDB composite `case` (empty if it matches)."""
    problems = []
    if g["size"] != case["size"]:
        problems.append(f"size {g['size']} != {case['size']}")
    lv = leaves(g, case)
    for m in case["members"]:
        places = lv.get(m["name"], [])
        t = m["type"]
        if t["kind"] == "bitfield":
            base = t["base"]["size"]
            want = (m["offset"], t["pos"], t["bits"])

            def as_pdb(p):  # Ghidra's minimal storage and shift -> PDB container and LSB position
                # storage bytes [o, o+n) inside the base type that starts at the PDB member offset; the shift
                # counts from the storage's least significant byte: its last byte on big-endian, first on little
                o, n, shift = p["offset"], p["storage"], p["bitOffset"]
                if big_endian:
                    last = o + n - 1 - m["offset"]
                    return (m["offset"], (base - 1 - last) * 8 + shift, p["bits"])
                return (m["offset"], (o - m["offset"]) * 8 + shift, p["bits"])
            if not any(p.get("bitfield") and as_pdb(p) == want for p in places):
                problems.append(f"{m['name']}: want byte {want[0]} bit {want[1]}:{want[2]}, got " +
                                ", ".join(f"byte {p['offset']} {p.get('storage')}B shift {p.get('bitOffset')}"
                                          f" bits {p.get('bits')}" for p in places))
        elif not any(p["offset"] == m["offset"] and not p.get("bitfield") for p in places):
            problems.append(f"{m['name']}: want offset {m['offset']}, got {[p['offset'] for p in places]}")
    return problems


def check(cases_path, ghidra_path):
    cases_ = {c["name"]: c for c in json.load(open(cases_path))["composites"]}
    ghidra = json.load(open(ghidra_path))
    big_endian = ghidra.get("bigEndian", True)
    got = {g["name"]: g for g in ghidra["composites"]}
    bad, checked = [], 0
    for name, case in cases_.items():
        g = got.get(name)
        if g is None or g.get("error"):
            bad.append(f"{name}: {g and g.get('error') or 'missing'}")
            continue
        checked += 1
        problems = compare(case, g, big_endian)
        if problems:
            bad.append(f"{case.get('display', name)}: " + "; ".join(problems[:4]) +
                       (f" (+{len(problems) - 4})" if len(problems) > 4 else ""))
    print(f"{checked - sum(1 for b in bad if 'missing' not in b and 'error' not in b)} of {len(cases_)} composites "
          f"match the PDB")
    for b in bad[:60]:
        print("  " + b)
    return 1 if bad else 0


def applied(pdb, dump_path):
    """Every named PDB composite and enum defined once, against the types the PDB analyzer created."""
    rd = Reader(parse(pdb))
    dump = json.load(open(dump_path))
    big_endian = dump.get("bigEndian", True)
    root = "/" + pdb.replace("\\", "/").split("/")[-1]
    path = lambda display: root + "/" + display.replace("::", "/")
    got = {g["path"]: g for g in dump["composites"]}
    enums = {e["path"]: e["size"] for e in dump["enums"]}
    names = collections.Counter(r["name"] for r in rd.recs.values()
                                if r["kind"] in ("LF_STRUCTURE", "LF_CLASS", "LF_UNION", "LF_ENUM")
                                and not rd.forward(r))
    bad, checked, skipped = [], 0, 0
    for key, k in rd.defs.items():
        r = rd.recs[k]
        if names[r["name"]] != 1 or not addressable(r["name"]):
            skipped += 1  # several definitions of the name, or not addressable by name
            continue
        try:
            case = rd.composite(key)
        except KeyError:
            skipped += 1
            continue
        g = got.get(path(r["name"]))
        if g is None:
            bad.append(f"{r['name']}: not created")
            continue
        checked += 1
        problems = compare(case, g, big_endian)
        if problems:
            bad.append(f"{r['name']}: " + "; ".join(problems[:3]) + (f" (+{len(problems) - 3})" if len(problems) > 3
                                                                      else ""))
    enum_bad, enum_checked = [], 0
    for k, r in rd.recs.items():
        if r["kind"] != "LF_ENUM" or rd.forward(r) or names[r["name"]] != 1 or not addressable(r["name"]):
            continue
        under = rd.type_of(typeref(r, "underlying type"))
        size = enums.get(path(r["name"]))
        enum_checked += 1
        if size != under["size"]:
            enum_bad.append(f"enum {r['name']}: size {size}, PDB {under['size']} ({under['name']})")
    known_path = __import__("pathlib").Path(__file__).with_name("pdb_known.tsv")
    known = dict(l.split("\t", 1) for l in known_path.read_text().splitlines()[1:] if l)
    unexplained = [b for b in enum_bad + bad if b.split(":")[0].removeprefix("enum ") not in known]
    print(f"{checked - len(bad)} of {checked} composites match the PDB ({skipped} not checked by name); "
          f"{enum_checked - len(enum_bad)} of {enum_checked} enums have the PDB's size")
    for b in enum_bad + bad:
        name = b.split(":")[0].removeprefix("enum ")
        print(("  known: " if name in known else "  ") + b[:300])
        if name in known:
            print("         " + known[name].strip())
    print(f"{len(unexplained)} unexplained difference(s)")
    return 1 if unexplained else 0


if __name__ == "__main__":
    if sys.argv[1] == "cases":
        cases(sys.argv[2], sys.argv[3])
    elif sys.argv[1] == "applied":
        sys.exit(applied(sys.argv[2], sys.argv[3]))
    else:
        sys.exit(check(sys.argv[2], sys.argv[3]))
