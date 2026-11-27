#!/usr/bin/env python3
"""Test the Xenon language against Xenia's instruction table.

    python3 tools/ghidra/xenon/test_words.py make  <ppc-instructions.xml> <out-dir>
    python3 tools/ghidra/xenon/test_words.py check <out-dir>/expected.tsv <ghidra-output.tsv>

`make` writes words.bin (big-endian instruction words) and expected.tsv: for every Xenon vector instruction in
Xenia's table (opcodes 4, 5, 6: VMX128 and AltiVec; the AltiVec and Cell/Xenon vector loads and stores on opcode 31;
dcbz128), its canonical encoding plus random fillings of every operand field. Xenia's decoder has run the Xbox 360
library for years, so its table is the oracle. `check` compares TestXenon.java's disassembly with it: the mnemonic
(with '.' when Rc is set), and for VMX128 every 7-bit register operand, in order.
"""
import random
import struct
import sys
import xml.etree.ElementTree as ET
from pathlib import Path

# operand fields per form, LSB-0 (lo, hi)
FIELDS = {
    "VX128":   ["VB128h", "VD128h", "VA128h", "VA128H", "VB128l", "VA128l", "VD128l"],
    "VX128_1": ["VD128h", "RB", "RA", "VD128l"],
    "VX128_2": ["VB128h", "VD128h", "VA128h", "VC3", "VA128H", "VB128l", "VA128l", "VD128l"],
    "VX128_3": ["VB128h", "VD128h", "VB128l", "IMM5", "VD128l"],
    "VX128_4": ["VB128h", "VD128h", "Z", "VB128l", "IMM5", "VD128l"],
    "VX128_5": ["VB128h", "VD128h", "VA128h", "SH", "VA128H", "VB128l", "VA128l", "VD128l"],
    "VX128_R": ["VB128h", "VD128h", "VA128h", "RC6", "VA128H", "VB128l", "VA128l", "VD128l"],
    "VX128_P": ["VB128h", "VD128h", "PERMh", "VB128l", "PERMl", "VD128l"],
    "VX": ["D5", "A5", "B5"],
    "VA": ["D5", "A5", "B5", "C5"],
    "VC": ["D5", "A5", "B5", "RC10"],
    "X": ["D5", "A5", "B5"],
    "DCBZ": ["A5", "B5"],
}
BITS = {
    "VB128h": (0, 1), "VD128h": (2, 3), "VA128h": (5, 5), "VA128H": (10, 10), "VB128l": (11, 15),
    "VA128l": (16, 20), "VD128l": (21, 25), "RB": (11, 15), "RA": (16, 20), "VC3": (6, 8), "IMM5": (16, 20),
    "Z": (6, 7), "SH": (6, 9), "RC6": (6, 6), "PERMh": (6, 8), "PERMl": (16, 20),
    "D5": (21, 25), "A5": (16, 20), "B5": (11, 15), "C5": (6, 10), "RC10": (10, 10),
    "U4": (16, 19), "U3": (16, 18), "U2": (16, 17),
}
# Instructions whose operand fields aren't all free: reserved fields stay as encoded (0). One-source AltiVec
# instructions have no vA; the ISA reserves that field and Ghidra requires it to be zero.
ONLY = {"mfvscr": ["D5"], "mtvscr": ["B5"]}
for _m in ("vexptefp vlogefp vrefp vrsqrtefp vrfim vrfin vrfip vrfiz vupkhsb vupkhsh vupklsb vupklsh vupkhpx "
           "vupklpx").split():
    ONLY[_m] = ["D5", "B5"]
ONLY["vsldoi"] = ["D5", "A5", "B5", "SH"]  # 4-bit shift in bits 6-9; bit 10 is reserved
# splats: element index of 4/3/2 bits in the vA field, the rest reserved; splat-immediates have no vB
ONLY.update({"vspltb": ["D5", "U4", "B5"], "vsplth": ["D5", "U3", "B5"], "vspltw": ["D5", "U2", "B5"],
             "vspltisb": ["D5", "A5"], "vspltish": ["D5", "A5"], "vspltisw": ["D5", "A5"]})
# VMX128 register operands in the order vmx128.sinc displays them
REGS = {
    "VX128": ["D", "A", "B"], "VX128_1": ["D"], "VX128_2": ["D", "A", "B", "C"], "VX128_3": ["D", "B"],
    "VX128_4": ["D", "B"], "VX128_5": ["D", "A", "B"], "VX128_R": ["D", "A", "B"], "VX128_P": ["D", "B"],
}
REGS_BY_MNEM = {"vupkhsb128": ["D", "B"], "vupklsb128": ["D", "B"], "vspltisw128": ["D"]}
SAMPLES = 24


def get(w, name):
    lo, hi = BITS[name]
    return (w >> lo) & ((1 << (hi - lo + 1)) - 1)


def regs(w, form, mnem):
    v = {"D": get(w, "VD128l") | get(w, "VD128h") << 5,
         "B": get(w, "VB128l") | get(w, "VB128h") << 5}
    if form in ("VX128", "VX128_2", "VX128_5", "VX128_R"):
        v["A"] = get(w, "VA128l") | get(w, "VA128h") << 5 | get(w, "VA128H") << 6
    if form == "VX128_2":
        v["C"] = get(w, "VC3")
    if form == "VX128_1":
        v["D"] = get(w, "VD128l") | get(w, "VD128h") << 5
    return [f"v{v[k]}" for k in REGS_BY_MNEM.get(mnem, REGS[form])]


def wanted(insn):
    op = int(insn.get("opcode"), 16)
    m, form = insn.get("mnem"), insn.get("form")
    if form not in FIELDS:
        return False
    if op >> 26 in (4, 5, 6):
        return True
    return form == "DCBZ" or (form == "X" and (m.startswith("lv") or m.startswith("stv")))


def make(xml, out):
    rng = random.Random(20111011)
    words, rows = [], []
    for insn in ET.parse(xml).getroot().iter("insn"):
        if not wanted(insn):
            continue
        m, form, base = insn.get("mnem"), insn.get("form"), int(insn.get("opcode"), 16)
        fields = ONLY.get(m, FIELDS[form])
        for k in range(SAMPLES):
            w = base
            if k:
                for f in fields:
                    lo, hi = BITS[f]
                    w |= rng.getrandbits(hi - lo + 1) << lo
            rc = (form == "VX128_R" and get(w, "RC6")) or (form == "VC" and get(w, "RC10"))
            expect = m + ("." if rc else "")
            r = " ".join(regs(w, form, m)) if form.startswith("VX128") else ""
            rows.append((len(words) * 4, w, expect, form, r))
            words.append(w)
    out.mkdir(parents=True, exist_ok=True)
    (out / "words.bin").write_bytes(b"".join(struct.pack(">I", w) for w in words))
    with open(out / "expected.tsv", "w") as f:
        f.write("offset\tword\tmnemonic\tform\tregisters\n")
        for o, w, e, form, r in rows:
            f.write(f"{o}\t{w:08X}\t{e}\t{form}\t{r}\n")
    print(f"{len(words)} words, {len({r[2].rstrip('.') for r in rows})} instructions")


def check(expected, got):
    exp = [l.split("\t") for l in Path(expected).read_text().splitlines()[1:]]
    dis = {}
    for line in Path(got).read_text().splitlines():
        off, text = line.split("\t", 1)
        dis[int(off)] = text
    bad = []
    for off, word, mnem, form, rs in exp:
        text = dis.get(int(off), "<missing>")
        parts = text.replace(",", " ").split()
        ok = bool(parts) and parts[0] == mnem
        if ok and rs:
            ok = [p for p in parts[1:] if p.startswith("v") and p[1:].isdigit()] == rs.split()
        if not ok:
            bad.append(f"{word} {form:8} want {mnem} {rs}  got {text}")
    print(f"{len(exp) - len(bad)} of {len(exp)} decoded as Xenia decodes them")
    for b in bad[:200]:
        print("  " + b)
    return 1 if bad else 0


if __name__ == "__main__":
    if sys.argv[1] == "make":
        make(sys.argv[2], Path(sys.argv[3]))
    else:
        sys.exit(check(sys.argv[2], sys.argv[3]))
