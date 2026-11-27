#!/usr/bin/env python3
"""Match functions of a target build (retail, no symbols) to a reference build (with PDB/MAP).

    python3 tools/match/match.py <reference-build> <target-build> [--originals DIR] [--same-compiler]

--same-compiler tightens the size check for two builds compiled the same way (e.g. the two server
builds, which is how the matcher is validated against ground truth: tools/match/validate.py).

Reads analysis/<build>/functions.tsv and ghidra/callgraph.tsv for both builds, and the two original exes
(from DIR, default originals/, fetched with tools/fetch_originals.py; an Arxan-protected build must be its
unpacked image, an Xbox 360 build the PE unpacked from its XEX) and disassembles every function (x86, x64 or Xenon
PowerPC, from the PE header) to find its references to strings and its distinctive immediates.

Writes analysis/<target>/match/functions.tsv (generated; never hand-edit):
  target_va  ref_va  ref_name  source  method  confidence  evidence

Methods, in the order they run:
  string     the pair are the only functions in their builds referencing a given string
             (several such strings strengthen the match). When one target function holds the strings of
             several reference functions, the one that calls the others is the match: the target inlined them
  constant   the same with distinctive numeric immediates (at least two shared, each unique on both sides)
  named      a string registered or tabled together with exactly one function on each side: a string and a
             function address loaded for the same call (Cmd_AddCommand("name", Func)), or a {name, ..., function}
             entry in a data table (script built-ins, callbacks)
  global     a global stored from a registration call under the same name string in both builds (a dvar
             pointer) referenced by exactly one function on each side
  callee     propagation: a matched pair's unmatched callees pair up when each side has exactly one
             candidate with the same matched-callee signature, or exactly one unmatched callee
  caller     the same, upwards through callers
  calls      the only unmatched function on each side calling the same set of (at least two) matched functions
  votes      the same pairing from many callers' gaps of equal length (hubs called from everywhere)
  order      within a matched pair, the one unmatched callee on each side between the same two matched calls
             (link-time code generation inlines calls but keeps the order of the rest)
Propagated pairs must also pass structural checks (shared strings when both reference any, size
ratio, callee count). Every match records its evidence. Nothing is matched on position: the retail build is linked with
whole-program optimization and reordered (docs/retail-builds.md).
"""
import bisect
import collections
import csv
import re
import struct
import sys
from pathlib import Path

import pefile

ROOT = Path(__file__).resolve().parents[2]
csv.field_size_limit(1 << 30)


XENON_HELPER = re.compile(r"__(save|rest)(gprlr|gpr|fpr|vmx)(_\d+)?$")


def read_tsv(path):
    with open(path, newline="") as f:
        return list(csv.DictReader(f, delimiter="\t", quoting=csv.QUOTE_NONE))


class Build:
    def __init__(self, build_id, exe):
        self.id = build_id
        a = ROOT / "analysis" / build_id
        rows = read_tsv(a / "functions.tsv")
        self.fn = {}
        for r in rows:
            if r.get("thunk_of"):
                continue
            # the Xenon register save/restore helpers are compiler plumbing called from nearly every prologue and
            # epilogue, not calls of the source: in the call graph they make unrelated functions look alike, and the
            # x64 target has nothing like them. Named in every Xenon analysis, blind ones included (ApplySymbols)
            if XENON_HELPER.match(r.get("ghidra_name") or "") or XENON_HELPER.match(r.get("pdb_name") or ""):
                continue
            va = int(r["va"], 16)
            self.fn[va] = {"va": va, "size": int(r["size"] or 0),
                           "name": r.get("pdb_name") or r.get("ghidra_name", ""), "source": r.get("source", "")}
        # code ranges to scan per function: on x64, .pdata gives each function's true extent and its separated
        # (chained) parts, independent of how far Ghidra's flow got; elsewhere the function's own extent
        self.ranges = {va: [(va, f["size"])] for va, f in self.fn.items()}
        pdata = a / "pdata.tsv"
        if pdata.exists():
            for r in read_tsv(pdata):
                va, n, parent = int(r["va"], 16), int(r["size"]), r.get("part_of")
                if parent and int(parent, 16) in self.fn:
                    self.ranges[int(parent, 16)].append((va, n))
                elif not parent and va in self.fn and n > self.fn[va]["size"]:
                    self.ranges[va][0] = (va, n)
        self.starts = sorted(self.fn)
        self.callees = collections.defaultdict(set)
        self.callers = collections.defaultdict(set)
        self.calls = collections.defaultdict(list)  # caller -> [(site, callee)], in address order
        for e in read_tsv(a / "ghidra" / "callgraph.tsv"):
            c, d = int(e["caller"], 16), int(e["callee"], 16)
            if c in self.fn and d in self.fn and c != d:
                self.callees[c].add(d)
                self.callers[d].add(c)
                self.calls[c].append((int(e["site"], 16), d))
        for c in self.calls:
            self.calls[c].sort()
        self._scan(exe)

    def owner(self, addr):
        i = bisect.bisect_right(self.starts, addr) - 1
        if i < 0:
            return None
        f = self.fn[self.starts[i]]
        return f["va"] if addr < f["va"] + max(f["size"], 1) else None

    def _scan(self, exe):
        """One disassembly pass over every function: the C strings it references and its distinctive immediates.
        x64 code reaches data RIP-relatively (lea rcx, [rip+disp]); x86 code by absolute address, as an immediate
        (push offset aName) or a base-less memory operand. Both resolve to the same string and function
        identities, so an x86 reference matches an x64 target (docs/matching.md)."""
        pe = pefile.PE(str(exe), fast_load=True)
        base = pe.OPTIONAL_HEADER.ImageBase
        size = pe.OPTIONAL_HEADER.SizeOfImage
        machine = pe.FILE_HEADER.Machine
        x64 = machine == 0x8664
        ppc = machine == 0x1F2
        ptr, ptrfmt = (8, "<Q") if x64 else (4, ">I") if ppc else (4, "<I")
        secs = {s.Name.rstrip(b"\0"): s for s in pe.sections}

        def sec_data(s):
            # through the section header as written: pefile's get_data rounds addresses to SectionAlignment first,
            # which Xbox 360 sections don't follow (docs/xbox360.md)
            return pe.__data__[s.PointerToRawData:s.PointerToRawData + min(s.SizeOfRawData, s.Misc_VirtualSize)]
        strs = {}
        for name in (b".rdata", b".data"):
            s = secs.get(name)
            if not s:
                continue
            d, va = sec_data(s), base + s.VirtualAddress
            for m in re.finditer(rb"[\x20-\x7e\t\n\r]{5,}\x00", d):
                strs[va + m.start()] = m.group()[:-1]
        self.str_refs = collections.defaultdict(set)    # string -> {function}
        self.fn_strs = collections.defaultdict(set)     # function -> {string}
        self.const_refs = collections.defaultdict(set)  # immediate -> {function}
        self.fn_consts = collections.defaultdict(set)
        self.named = collections.defaultdict(set)      # string -> {function it names}: registrations, tables
        self.fn_globals = collections.defaultdict(set)  # function -> {data address it reads or writes}
        self.global_refs = collections.defaultdict(set)
        self.named_data = collections.defaultdict(set)  # string -> {global}: g = Register("name", ...)
        if ppc:
            self._scan_ppc(pe, base, size, strs)
        else:
            self._scan_x86(pe, base, size, x64, secs, strs)
        self._scan_tables(secs, sec_data, ptr, ptrfmt, strs)

    def _note_constant(self, va, v, mask):
        """A distinctive immediate: not small, not a power of two or low-bit mask, not a small negative."""
        if v < 0x10000 or v > mask - 0xFFFF or 0xFFFF0000 <= v <= 0xFFFFFFFF:
            return
        if v & (v - 1) == 0 or (v + 1) & v == 0:
            return  # powers of two and low-bit masks are everywhere
        self.const_refs[v].add(va)
        self.fn_consts[va].add(v)

    def _note_address(self, va, target, base, size, code_lo, code_hi, strs, recent, loads_function):
        """A data or code address the function va builds or accesses. Returns the updated recent list."""
        if base <= target < base + size and not code_lo <= target < code_hi and target not in strs:
            self.fn_globals[va].add(target)
            self.global_refs[target].add(va)
        s = strs.get(target)
        if s is not None:
            self.str_refs[s].add(va)
            self.fn_strs[va].add(s)
            recent.append(("s", s))
        elif target in self.fn and loads_function:
            recent.append(("f", target))
        kinds = {k for k, _ in recent[-4:]}
        if kinds == {"s", "f"}:
            names = [x for k, x in recent[-4:] if k == "s"]
            funcs = [x for k, x in recent[-4:] if k == "f"]
            if len(names) == 1 and len(funcs) == 1:
                self.named[names[0]].add(funcs[0])
                return []
        return recent

    def _registered_global(self, call_strings, g, strs):
        """g = Register("name", ...): the call's only string, or its only identifier-like one
        (Dvar_Register*("name", ..., "description"))."""
        names = call_strings if len(call_strings) == 1 else \
            [x for x in call_strings if re.fullmatch(rb"[A-Za-z_][A-Za-z0-9_]{2,}", x)]
        if len(names) == 1 and g not in strs:
            self.named_data[names[0]].add(g)

    def _scan_ppc(self, pe, base, size, strs):
        """Xbox 360 (PowerPC, big-endian, 32-bit addresses). A data address is built in two instructions:
        `lis rX, hi` then `addi rY, rX, lo` / `ori rY, rX, lo`, or used as `lwz/stw/lfs/... rY, lo(rX)` (lo signed,
        so hi is pre-adjusted for addi and D-form displacements). The scan tracks the pending high half per register
        within straight-line code: an instruction writing a register drops it, a call drops the volatile registers
        (r0, r3-r12), any other branch drops all of them. 32-bit constants are built the same way (lis + ori/addi)
        and are distinctive when they aren't addresses in the image. A registration's result is stored by
        `stw r3, lo(rX)` after the call (r3 holds the return value). Instructions are decoded by hand: only the
        integer forms that write general registers matter, and VMX128 (which capstone doesn't decode) writes none."""
        mask = 0xFFFFFFFF
        code = [(base + s.VirtualAddress, base + s.VirtualAddress + s.Misc_VirtualSize, s) for s in pe.sections
                if s.Characteristics & 0x20000000]
        code_lo = min(lo for lo, _, _ in code)
        code_hi = max(hi for _, hi, _ in code)
        sext = lambda x: x - 0x10000 if x & 0x8000 else x
        D_LOADS = {32, 34, 40, 42, 46, 48, 50}            # lwz lbz lhz lha lmw lfs lfd (+ update forms below)
        D_UPDATE = {33, 35, 37, 39, 41, 43, 45, 49, 51, 53, 55}
        D_MEM = set(range(32, 56)) | {58, 62}             # every D/DS-form load or store: base register rA
        WRITES_RD = {7, 8, 12, 13, 14, 15} | {32, 33, 34, 35, 40, 41, 42, 43, 46, 58}
        WRITES_RA = {20, 21, 23, 24, 25, 26, 27, 28, 29, 30}
        VOLATILE = {0} | set(range(3, 13))
        for va, f in self.fn.items():
            if f["size"] <= 0:
                continue
            sec = next((s for lo, hi, s in code if lo <= va < hi), None)
            if sec is None:
                continue
            off = sec.PointerToRawData + va - base - sec.VirtualAddress
            words = struct.unpack_from(f">{f['size'] // 4}I", pe.__data__, off)
            hi = {}           # register -> pending high half (already shifted)
            recent = []
            call_strings = None
            for w in words:
                op = w >> 26
                rd, ra = (w >> 21) & 31, (w >> 16) & 31
                imm = w & 0xFFFF
                if op == 18 or op == 16 or (op == 19 and ((w >> 1) & 0x3FF) in (16, 528)):
                    if w & 1:  # bl / bcl / bclrl / bcctrl: a call
                        call_strings = [x for k, x in recent if k == "s"]
                        for r in VOLATILE:
                            hi.pop(r, None)
                    else:
                        hi.clear()
                    recent = []  # a registration's arguments are loaded between two calls
                    continue
                if op == 15 and ra == 0:  # lis rD, imm
                    hi[rd] = (imm << 16) & mask
                    continue
                target = None
                if op in (14, 24) and ra in hi:  # addi / ori rX, rA(lis), lo
                    v = (hi[ra] + sext(imm)) & mask if op == 14 else hi[ra] | imm
                    if base <= v < base + size:
                        target = v
                    else:
                        self._note_constant(va, v, mask)
                elif op in D_MEM and ra in hi:
                    d = sext(imm & ~3 if op in (58, 62) else imm)
                    target = (hi[ra] + d) & mask
                    # the result of the last call stored to a global: g = Register("name", ...)
                    if op == 36 and rd == 3 and call_strings is not None:
                        self._registered_global(call_strings, target, strs)
                        call_strings = None
                if target is not None:
                    recent = self._note_address(va, target, base, size, code_lo, code_hi, strs, recent,
                                                loads_function=op == 14)
                # registers this instruction writes lose their pending high half
                if op in WRITES_RD or (op == 58 and w & 3 == 1):
                    hi.pop(rd, None)
                if op in WRITES_RA or op in D_UPDATE or (op in (58, 62) and w & 3 == 1):
                    hi.pop(ra, None)
                if op == 31:  # X-form: destination is rD or rA depending on the instruction
                    hi.pop(rd, None)
                    hi.pop(ra, None)

    def _scan_x86(self, pe, base, size, x64, secs, strs):
        import capstone
        text = secs[b".text"]
        tva, tdata = base + text.VirtualAddress, text.get_data()
        md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_64 if x64 else capstone.CS_MODE_32)
        md.detail = True
        X86 = capstone.x86
        rip = X86.X86_REG_RIP
        mask = 0xFFFFFFFFFFFFFFFF if x64 else 0xFFFFFFFF
        ret_regs = {X86.X86_REG_EAX, X86.X86_REG_RAX}
        code_lo, code_hi = tva, tva + len(tdata)
        def code(va):
            for start, n in self.ranges[va]:
                off = start - tva
                if 0 <= off < len(tdata) and n > 0:
                    yield from md.disasm(tdata[off:off + n], start)
        for va, f in self.fn.items():
            recent = []  # last few address operands: ("s", string) or ("f", function)
            call_strings = None  # the strings loaded for the last call, until its result is stored
            for ins in code(va):
                if ins.mnemonic.startswith("call"):
                    call_strings = [x for k, x in recent if k == "s"]
                if ins.mnemonic.startswith("call") or ins.mnemonic.startswith("j"):
                    recent = []  # a registration's arguments are loaded between two calls
                if ins.mnemonic == "mov" and len(ins.operands) == 2 and call_strings is not None:
                    dst, src = ins.operands
                    if src.type == X86.X86_OP_REG and src.reg in ret_regs and dst.type == X86.X86_OP_MEM \
                            and dst.mem.index == 0 and ((x64 and dst.mem.base == rip) or (not x64 and dst.mem.base == 0)):
                        g = (ins.address + ins.size + dst.mem.disp) if x64 else dst.mem.disp & mask
                        self._registered_global(call_strings, g, strs)
                        call_strings = None
                for op in ins.operands:
                    target = None
                    if op.type == X86.X86_OP_MEM and op.mem.index == 0:
                        if x64 and op.mem.base == rip:
                            target = ins.address + ins.size + op.mem.disp
                        elif not x64 and op.mem.base == 0:
                            target = op.mem.disp & mask
                    elif op.type == X86.X86_OP_IMM:
                        v = op.imm & mask
                        if base <= v < base + size:
                            if not x64:
                                target = v  # x86: push offset / mov reg, offset
                        else:
                            self._note_constant(va, v, mask)
                            continue
                    if target is None:
                        continue
                    recent = self._note_address(va, target, base, size, code_lo, code_hi, strs, recent,
                                                loads_function=ins.mnemonic in ("lea", "push", "mov"))

    def _scan_tables(self, secs, sec_data, ptr, ptrfmt, strs):
        """Data tables of {const char *name; ...; function pointer} (script built-ins, command and callback
        tables)."""
        for name in (b".rdata", b".data"):
            sec = secs.get(name)
            if not sec:
                continue
            d = sec_data(sec)
            for i in range(0, len(d) - 4 * ptr, ptr):
                sv = struct.unpack_from(ptrfmt, d, i)[0]
                st = strs.get(sv)
                if st is None:
                    continue
                for k in (ptr, 2 * ptr, 3 * ptr):
                    fv = struct.unpack_from(ptrfmt, d, i + k)[0]
                    if fv in self.fn:
                        self.named[st].add(fv)
                        break
                    if strs.get(fv) is not None:
                        break  # next entry's name: no function in this entry


def main():
    ref_id, tgt_id = sys.argv[1], sys.argv[2]
    originals = Path(sys.argv[sys.argv.index("--originals") + 1]) if "--originals" in sys.argv \
        else ROOT / "originals"
    ref = Build(ref_id, originals / f"{ref_id}.exe")
    tgt = Build(tgt_id, originals / f"{tgt_id}.exe")

    match = {}       # tgt va -> (ref va, method, confidence, evidence)
    matched_ref = {}  # ref va -> tgt va

    size_lo, size_hi = (0.5, 2.0) if "--same-compiler" in sys.argv else (0.15, 6.0)
    same_compiler = "--same-compiler" in sys.argv
    order_lo, order_hi = (0.8, 1.25) if "--same-compiler" in sys.argv else (0.6, 2.5)

    def compatible(t, r):
        """Cheap structural checks a propagated pair must pass."""
        ts, rs = tgt.fn_strs.get(t, set()), ref.fn_strs.get(r, set())
        if ts and not (ts & rs):
            # the target's strings may all come from callees it inlined (link-time code generation)
            reach = set(rs)
            frontier = {r}
            for _ in range(2):
                frontier = {c for x in frontier for c in ref.callees[x]}
                reach |= {st for c in frontier for st in ref.fn_strs.get(c, ())}
            if not ts <= reach:
                return False
        tz, rz = tgt.fn[t]["size"], ref.fn[r]["size"]
        if tz and rz and not (size_lo <= tz / rz <= size_hi):
            return False
        tc, rc = len(tgt.callees[t]), len(ref.callees[r])
        if max(tc, rc) >= 4 and min(tc, rc) * 4 < max(tc, rc):
            return False
        return True

    def accept(t, r, method, conf, ev):
        if t in match or r in matched_ref:
            return False
        if not method.startswith("string") and not compatible(t, r):
            return False
        match[t] = (r, method, conf, ev)
        matched_ref[r] = t
        return True

    def inlined_into_caller(t, r):
        """Whether the target function t, holding r's anchoring content, is r or r's only caller with r inlined.

        Link-time code generation inlines a function with one caller into it, so the content that anchors r (its
        strings, constants, globals) can end up in the caller's target function, and a caller with no anchors of its
        own then looks like r. Sizes tell the two apart only sometimes, because r itself grows by inlining its own
        callees. Returns ("r", None) when t can only be r, ("caller", rc) when t can only be rc with r inside (too
        large for r even with its callees inlined, the right size for rc and r), ("either", rc) when both fit."""
        callers = ref.callers[r]
        if len(callers) != 1:
            return "r", None
        rc = next(iter(callers))
        if rc in matched_ref or rc == r:
            return "r", None
        tz, rz, cz = tgt.fn[t]["size"], ref.fn[r]["size"], ref.fn[rc]["size"]
        if not (tz and rz and cz):
            return "r", None
        grown = rz + sum(ref.fn[c]["size"] or 0 for c in ref.callees[r] if c != r)
        as_r = order_lo <= tz / rz <= order_hi or (grown > rz and order_lo <= tz / grown <= order_hi) \
            or rz < tz < grown
        as_caller = order_lo <= tz / (rz + cz) <= order_hi
        if as_caller and not as_r:
            return "caller", rc
        if as_caller:
            return "either", rc
        return "r", None

    inline_stats = collections.Counter()

    def accept_anchored(t, r, method, conf, ev):
        """accept() for a pair resting on r's own content: the target may be r's caller with r inlined."""
        kind, rc = inlined_into_caller(t, r)
        inline_stats[kind] += 1
        # Either way the pair rests on content the target only might own: its method says so, and the hybrid link
        # leaves it out until it is reviewed into records/names.tsv (tools/symbols.py ANCHORED)
        if kind == "caller":
            return accept(t, rc, f"{method}-via-callee", max(0.70, conf - 0.10),
                          f"{ev}, which belong(s) to {ref.fn[r]['name']}, inlined into its only caller: "
                          f"{tgt.fn[t]['size']} bytes against {ref.fn[r]['size']} alone and "
                          f"{ref.fn[r]['size'] + ref.fn[rc]['size']} with it")
        if kind == "either":
            return accept(t, r, f"{method}-or-caller", conf,
                          f"{ev}; or its only caller {ref.fn[rc]['name']} with it inlined")
        return accept(t, r, method, conf, ev)

    # 1. unique strings
    votes = collections.defaultdict(lambda: collections.defaultdict(list))  # tgt -> ref -> [strings]
    for s, rfs in ref.str_refs.items():
        tfs = tgt.str_refs.get(s)
        if len(rfs) == 1 and tfs and len(tfs) == 1:
            votes[next(iter(tfs))][next(iter(rfs))].append(s)
    by_ref = collections.defaultdict(set)
    for t, cands in votes.items():
        for r in cands:
            by_ref[r].add(t)
    def reaches(r, others, depth=3):
        """r calls every function in others within depth steps (it could have inlined them)."""
        frontier, seen = {r}, set()
        for _ in range(depth):
            frontier = {c for x in frontier for c in ref.callees[x]} - seen
            seen |= frontier
        return others <= seen

    inlined = 0
    for t, cands in votes.items():
        if len(cands) != 1:
            # several reference functions' strings in one target function: link-time code generation inlined
            # all but one of them into it, so the one that calls the others is the match
            roots = [r for r in cands if reaches(r, set(cands) - {r})]
            if len(roots) != 1:
                continue
            r = roots[0]
            if len(by_ref[r]) != 1:
                continue
            ss = [x for v in cands.values() for x in v]
            if accept(t, r, "string", min(0.95, 0.75 + 0.05 * len(cands[r])),
                      f"{len(cands[r])} unique string(s) of its own and {len(ss) - len(cands[r])} of "
                      f"{len(cands) - 1} callee(s) it inlined: " + repr(cands[r][0][:60].decode("latin1"))):
                inlined += 1
            continue
        r, ss = next(iter(cands.items()))
        if len(by_ref[r]) != 1:
            continue  # reference function's strings spread over several target functions
        conf = min(0.99, 0.80 + 0.05 * len(ss))
        accept_anchored(t, r, "string", conf, f"{len(ss)} unique string(s): " + repr(ss[0][:60].decode("latin1")))
    n_string = len(match)

    # 1b. unique numeric constants: same rules as strings, but a pair needs two constants (one is too weak)
    cvotes = collections.defaultdict(lambda: collections.defaultdict(list))
    for v, rfs in ref.const_refs.items():
        tfs = tgt.const_refs.get(v)
        if len(rfs) == 1 and tfs and len(tfs) == 1:
            cvotes[next(iter(tfs))][next(iter(rfs))].append(v)
    cby_ref = collections.defaultdict(set)
    for t, cands in cvotes.items():
        for r in cands:
            cby_ref[r].add(t)
    for t, cands in cvotes.items():
        if len(cands) != 1:
            continue
        r, vs = next(iter(cands.items()))
        if len(cby_ref[r]) != 1 or len(vs) < 2:
            continue
        accept_anchored(t, r, "constant", min(0.95, 0.75 + 0.05 * len(vs)),
               f"{len(vs)} unique constant(s): " + ", ".join(f"{v:#x}" for v in vs[:4]))

    # 1c. names attached to functions by registrations (Cmd_AddCommand("name", Func)) and data tables
    nvotes = collections.defaultdict(lambda: collections.defaultdict(list))
    for st, rfs in ref.named.items():
        tfs = tgt.named.get(st)
        if len(rfs) == 1 and tfs and len(tfs) == 1:
            nvotes[next(iter(tfs))][next(iter(rfs))].append(st)
    nby_ref = collections.defaultdict(set)
    for t, cands in nvotes.items():
        for r in cands:
            nby_ref[r].add(t)
    for t, cands in nvotes.items():
        if len(cands) != 1:
            continue
        r, ss = next(iter(cands.items()))
        if len(nby_ref[r]) != 1:
            continue
        accept(t, r, "named", min(0.97, 0.85 + 0.04 * len(ss)),
               f"registered/tabled under {len(ss)} name(s): " + repr(ss[0][:60].decode("latin1")))

    # 1d. globals: a global stored from a registration call with the same name string in both builds
    # (dvar_t pointers: g = Dvar_Register*("name", ...)) is the same variable. A function that is the only one
    # referencing such a global on each side is the same function (as with strings); several shared globals make it
    # stronger. Inlining is handled as for strings.
    gmap = {}  # target global -> reference global
    for st, tgs in tgt.named_data.items():
        rgs = ref.named_data.get(st)
        if len(tgs) == 1 and rgs and len(rgs) == 1:
            gmap[next(iter(tgs))] = next(iter(rgs))
    gvotes = collections.defaultdict(lambda: collections.defaultdict(list))
    for tg, rg in gmap.items():
        tfs, rfs = tgt.global_refs.get(tg, set()), ref.global_refs.get(rg, set())
        if len(tfs) == 1 and len(rfs) == 1:
            gvotes[next(iter(tfs))][next(iter(rfs))].append(rg)
    gby_ref = collections.defaultdict(set)
    for t, cands in gvotes.items():
        for r in cands:
            gby_ref[r].add(t)
    n_global = 0
    for t, cands in gvotes.items():
        if len(cands) != 1:
            roots = [r for r in cands if reaches(r, set(cands) - {r})]
            if len(roots) != 1:
                continue
            r = roots[0]
        else:
            r = next(iter(cands))
        if len(gby_ref[r]) != 1:
            continue
        if accept_anchored(t, r, "global", min(0.95, 0.80 + 0.05 * len(cands[r])),
                  f"only function referencing {len(cands[r])} global(s) registered under the same name"):
            n_global += 1

    # 1e. the same set of named globals: functions whose references to globals paired in 1d are the same set (at
    # least two), unique on both sides
    rg_set = set(gmap.values())
    tkeys = collections.defaultdict(list)
    for t, gs in tgt.fn_globals.items():
        k = frozenset(gmap[g] for g in gs if g in gmap)
        if len(k) >= 2:
            tkeys[k].append(t)
    rkeys = collections.defaultdict(list)
    for r, gs in ref.fn_globals.items():
        k = frozenset(g for g in gs if g in rg_set)
        if len(k) >= 2:
            rkeys[k].append(r)
    for k, ts in tkeys.items():
        rs = rkeys.get(k, [])
        if len(ts) == 1 and len(rs) == 1:
            if accept_anchored(ts[0], rs[0], "global", min(0.95, 0.75 + 0.05 * len(k)),
                      f"the only function on each side referencing the same {len(k)} named globals"):
                n_global += 1

    # 2./3. call-graph propagation until nothing changes
    def signature(build, va, mapping):
        """Matched neighbours of a function, expressed in reference addresses."""
        return (frozenset(mapping(x) for x in build.callees[va] if mapping(x) is not None),
                frozenset(mapping(x) for x in build.callers[va] if mapping(x) is not None))

    rounds = 0
    while True:
        rounds += 1
        added = 0
        for direction in ("callee", "caller"):
            for t, (r, *_rest) in list(match.items()):
                tn = [x for x in (tgt.callees[t] if direction == "callee" else tgt.callers[t]) if x not in match]
                rn = [x for x in (ref.callees[r] if direction == "callee" else ref.callers[r]) if x not in matched_ref]
                if not tn or not rn:
                    continue
                if len(tn) == 1 and len(rn) == 1:
                    if accept(tn[0], rn[0], direction, 0.75, f"only unmatched {direction} of {ref.fn[r]['name']}"):
                        added += 1
                    continue
                # pair by matched-neighbour signature when it is unique on both sides
                tsig = collections.defaultdict(list)
                for x in tn:
                    tsig[signature(tgt, x, lambda v: match[v][0] if v in match else None)].append(x)
                rsig = collections.defaultdict(list)
                for x in rn:
                    rsig[signature(ref, x, lambda v: v if v in matched_ref else None)].append(x)
                for sig, txs in tsig.items():
                    if len(txs) == 1 and len(rsig.get(sig, [])) == 1 and (sig[0] or len(sig[1]) > 1):
                        if accept(txs[0], rsig[sig][0], direction, 0.85,
                                  f"{direction} of {ref.fn[r]['name']} with the same {len(sig[0])} matched callees "
                                  f"and {len(sig[1])} matched callers"):
                            added += 1
        # the same set of matched callees anywhere in the graph: an unmatched function calling at least two matched
        # functions, and the only unmatched function on each side calling exactly that set, is the same function
        # (reaches islands no matched caller or callee touches yet)
        tsets = collections.defaultdict(list)
        for x in tgt.fn:
            if x not in match:
                k = frozenset(match[c][0] for c in tgt.callees[x] if c in match)
                if len(k) >= 2:
                    tsets[k].append(x)
        rsets = collections.defaultdict(list)
        for y in ref.fn:
            if y not in matched_ref:
                k = frozenset(c for c in ref.callees[y] if c in matched_ref)
                if len(k) >= 2:
                    rsets[k].append(y)
        for k, xs in tsets.items():
            ys = rsets.get(k, [])
            if len(xs) == 1 and len(ys) == 1:
                if accept(xs[0], ys[0], "calls", min(0.90, 0.70 + 0.04 * len(k)),
                          f"the only unmatched function on each side calling the same {len(k)} matched functions"):
                    added += 1
        # call order: within a matched pair, the calls between two calls to already matched functions are the same
        # calls in the same order, except those the target inlined. One unmatched callee on each side between the
        # same two anchors pairs up.
        votes_order = collections.defaultdict(collections.Counter)  # target callee -> Counter(reference callee)
        for t, (r, *_rest) in list(match.items()):
            tc = [d for _, d in tgt.calls.get(t, [])]
            rc = [d for _, d in ref.calls.get(r, [])]
            if not tc or not rc:
                continue
            ta = [(i, match[d][0]) for i, d in enumerate(tc) if d in match]
            ra = {}
            for i, d in enumerate(rc):
                if d in matched_ref:
                    ra.setdefault(d, []).append(i)
            # anchors: matched callees called exactly once on each side, kept where both orders agree
            once_t = collections.Counter(x for _, x in ta)
            anchors = [(i, ra[x][0]) for i, x in ta if once_t[x] == 1 and len(ra.get(x, [])) == 1]
            good, last = [], -1
            for i, j in anchors:
                if j > last:
                    good.append((i, j))
                    last = j
            bounds = [(-1, -1)] + good + [(len(tc), len(rc))]
            for (i0, j0), (i1, j1) in zip(bounds, bounds[1:]):
                tslice, rslice = tc[i0 + 1:i1], rc[j0 + 1:j1]
                tgap = {d for d in tslice if d not in match}
                rgap = {d for d in rslice if d not in matched_ref}
                # votes: gaps of the same length pair position by position (hubs such as Com_Printf are called
                # from everywhere and never alone in a gap, but are paired consistently across many callers)
                tun = [d for d in tslice if d not in match]
                run = [d for d in rslice if d not in matched_ref]
                if tun and len(tun) == len(run) <= 4 and (len(tslice) == len(rslice) or not same_compiler):
                    for x, y in zip(tun, run):
                        votes_order[x][y] += 1
                # a short gap, and the same number of calls to the candidate on each side
                if len(tgap) == 1 and len(rgap) == 1 and len(rslice) <= 3 and len(tslice) <= 3 \
                        and tslice.count(next(iter(tgap))) == rslice.count(next(iter(rgap))):
                    (x,), (y,) = tgap, rgap
                    # an inlined reference callee leaves its own callee at its place: the pair must look alike
                    tz, rz = tgt.fn[x]["size"], ref.fn[y]["size"]
                    if not (tz and rz and order_lo <= tz / rz <= order_hi):
                        continue
                    tn, rn = len(tgt.callees[x]), len(ref.callees[y])
                    if abs(tn - rn) > max(1, rn // 2):
                        continue
                    if accept(x, y, "order", 0.80, f"called by {ref.fn[r]['name']} between the same matched calls"):
                        added += 1
        # hubs by votes: at least three gaps pairing them, and most of each side's votes agreeing
        back = collections.defaultdict(collections.Counter)
        for x, c in votes_order.items():
            for y, n in c.items():
                back[y][x] += n
        for x, c in votes_order.items():
            if x in match:
                continue
            y, n = c.most_common(1)[0]
            if y in matched_ref or n < 3 or n < 0.8 * sum(c.values()):
                continue
            if back[y].most_common(1)[0][0] != x or back[y][x] < 0.8 * sum(back[y].values()):
                continue
            if accept(x, y, "votes", min(0.95, 0.70 + 0.03 * n),
                      f"paired with it at the same place among matched calls in {n} callers"):
                added += 1
        if not added:
            break

    out = ROOT / "analysis" / tgt_id / "match"
    out.mkdir(parents=True, exist_ok=True)
    with open(out / "functions.tsv", "w", newline="\n") as f:
        f.write("target_va\tref_build\tref_va\tref_name\tsource\tmethod\tconfidence\tevidence\n")
        for t in sorted(match):
            r, method, conf, ev = match[t]
            rf = ref.fn[r]
            f.write(f"{t:08X}\t{ref_id}\t{r:08X}\t{rf['name']}\t{rf['source']}\t{method}\t{conf:.2f}\t"
                    f"{ev.replace(chr(9), ' ').replace(chr(10), ' ')}\n")
    methods = collections.Counter(m[1] for m in match.values())
    print(f"anchored pairs whose function has one caller: {dict(inline_stats)}")
    print(f"{tgt_id} <- {ref_id}: {len(match):,} of {len(tgt.fn):,} target functions matched "
          f"({100 * len(match) / len(tgt.fn):.1f}%); by method {dict(methods)}; propagation rounds {rounds}")


if __name__ == "__main__":
    main()
