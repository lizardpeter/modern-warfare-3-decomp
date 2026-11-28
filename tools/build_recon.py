#!/usr/bin/env python3
"""Compile the reconstruction into one Windows DLL, hybrid-linked against the Store exe.

    python3 tools/build_recon.py [--build iw5mp_ship]              # the faithful build: src/
    python3 tools/build_recon.py [--build iw5mp_ship] --enhanced   # the enhanced build: enhanced/ over src/
    -> build/<build>/[enhanced/]recon.dll, recon.symbols.tsv, hybrid.tsv

Ported from black-ops-2-decomp (its x64 Store path; the slot table comes from black-ops-3-decomp, TLS from
ghosts-decomp). MW3's references are Xbox 360 builds, which aren't run, so only x64 retail builds are linked.

- Every .cpp under src/ is compiled with clang for x64 Windows (MSVC calling convention and name mangling, VS2019
  16.11 compatibility). tools/recon/include holds the build-environment headers.
- Every defined function the Store build identifies is exported under its decorated name, so tests find it.
- Hybrid link: anything the reconstruction uses but does not define yet resolves to the Store exe through
  identifications (tools/symbols.py: records/names.tsv and the matcher's anchored pairs, by scoped plain name). So
  a function can be reconstructed and tested before anything it calls is. hybrid.tsv lists every such dependency.
- Every exe address lives in one slot table in the DLL, rebased at load by the DLL's entry point (iw5_hybrid_entry)
  to wherever the exe is. Functions become a thunk through their slot, imports go through the exe's import table,
  and units compiled with -mcmodel=large have each `movabsq $sym` of exe data rewritten into a load from its slot.
  Anything not identified in the Store build yet links to a trap (ud2, or data at address 0). No code may hold an
  exe address directly; the link checks it.
- Thread-local variables are placed in the DLL's own .tls section at the offset they have in the original's TLS
  template, so the offsets the compiler emits are the original's.

The enhanced build (records/patches.tsv) is the same link with enhanced/ first: a function enhanced/ defines replaces
src/'s. It is also the DLL loaded into the game (tools/runtime_link.py, runtime/loader): there, a function src/
defines that the build identifies in the exe is a thunk into the exe's own copy ("exe first"), so the DLL runs only
the replacements; the redirect table lists each replacement for the loader; and Win32 functions the exe doesn't
import come from tools/recon/lib/*.def as real imports of the DLL.

This is the `compiles` gate. It is not byte matching: that needs the original compiler.
"""
import csv
import os
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
import symbols  # noqa: E402

# -fno-math-errno: VC100 and VS2019 expand sqrt and friends as intrinsics that never set errno
COMMON = ["-O0", "-ffp-contract=off", "-fno-math-errno", "-fno-exceptions", "-fms-extensions", "-fno-stack-protector", "-ffreestanding",
          "-fno-builtin", "-Wall", "-Werror", "-Wno-unused-function", "-Wno-unused-variable",
          f"-I{ROOT / 'include'}", f"-isystem{ROOT / 'tools' / 'recon' / 'include'}"]
ARCH = {
    # VS2019 16.11 (cl 19.29). -mcmodel=large: every global is reached as `movabsq $sym, %reg`, the one form
    # exe_relative_asm rewrites into a slot load. -fno-jump-tables: large-model COFF jump tables store 32-bit
    # cross-section differences, a negative one comes back 4 GB off.
    "x64": {"triple": "--target=x86_64-pc-windows-msvc",
            "cflags": ["-fms-compatibility-version=19.29", "-mcmodel=large", "-fno-jump-tables"]},
}
SLOT_PREFIX = "iw5x@"  # x64: the slot holding the exe address of a symbol the reconstruction uses as data
csv.field_size_limit(1 << 30)


def out_dir(build, enhanced=False):
    return ROOT / "build" / build / ("enhanced" if enhanced else "")


def run(*cmd, quiet_duplicates=False):
    if not quiet_duplicates:
        subprocess.run([str(c) for c in cmd], check=True)
        return
    # the enhanced link: its intended duplicates (enhanced/ replacing src/) are not news
    r = subprocess.run([str(c) for c in cmd], capture_output=True, text=True)
    noise = ("duplicate symbol", ">>> defined at")
    out = [l for l in (r.stdout + r.stderr).splitlines() if l.strip() and not any(n in l for n in noise)]
    if out:
        print("\n".join(out))
    if r.returncode:
        raise subprocess.CalledProcessError(r.returncode, cmd)


def nm(obj, *flags):
    out = subprocess.run(["llvm-nm", *flags, str(obj)], check=True, capture_output=True, text=True).stdout
    return [line.split() for line in out.splitlines() if line.strip()]


def original_exe(build):
    path = Path(os.environ.get("IW5_ORIGINALS", ROOT / "originals")) / f"{build}.exe"
    if not path.exists():
        sys.exit(f"{path}: the hybrid link needs the original exe (tools/fetch_originals.py {build} originals)")
    return path


def tls_template(build):
    """[start, end) of the TLS template in a build's exe (its TLS directory): a TLS variable's offset in the block is
    its address (PDB statics, MAP, identifications) minus the start."""
    import pefile
    pe = pefile.PE(str(original_exe(build)), fast_load=True)
    pe.parse_data_directories(directories=[pefile.DIRECTORY_ENTRY["IMAGE_DIRECTORY_ENTRY_TLS"]])
    if not hasattr(pe, "DIRECTORY_ENTRY_TLS"):
        return 0, 0
    t = pe.DIRECTORY_ENTRY_TLS.struct
    return t.StartAddressOfRawData, t.EndAddressOfRawData + t.SizeOfZeroFill


def tls_asm(tls, lo, hi):
    """The DLL's .tls section: each original TLS variable at its offset in the original's template."""
    if not tls:
        return []
    asm = ['\t.section .tls$, "dw"', "iw5_tls_start:"]
    for off, q in sorted(tls):
        asm += [f"\t.skip {off} - (. - iw5_tls_start)", f"\t.globl {q}", f"{q}:"]
    size = max([hi - lo] + [off + 8 for off, _ in tls])
    return asm + [f"\t.skip {size} - (. - iw5_tls_start)"]


def compile_unit(src, obj, arch, extra=()):
    """src -> obj through assembly: obj.s is kept, since exe_relative_asm may rewrite it and reassemble."""
    asm = obj.with_suffix(".s")
    run("clang++", "-std=c++17", ARCH[arch]["triple"], *ARCH[arch]["cflags"], *COMMON, *extra, "-S", src, "-o", asm)
    for line in asm.read_text().splitlines():
        if re.match(r"\s*\.(?:quad|long)\s+[.\w$@?\"]+\s*-\s*[.\w$@?\"]+\s*$", line):
            sys.exit(f"{asm}: a label-difference table ({line.strip()}): truncated to 32 bits across COFF sections")
    run("clang", ARCH[arch]["triple"], "-c", asm, "-o", obj)


# a symbol in a line of assembly: quoted (C++ names), or bare (C names), which can't start with `$` (an immediate's
# prefix: `movabsq $__imp_X` names __imp_X)
_TOKEN = re.compile(r'"([^"]+)"|([A-Za-z_.?@][\w.?$@]*)')
_MOVABS = re.compile(r'^(\s*)movabsq\s+\$("[^"]+"|[A-Za-z_.?$@][\w.?$@]*)([+-](?:0x[0-9a-fA-F]+|\d+))?\s*,\s*(%r\w+)\s*(#.*)?$')


def exe_relative_asm(asm_path, exe_data):
    """x64: rewrite one unit's references to exe data into loads from the DLL's slot table. With -mcmodel=large
    clang reaches every global as `movabsq $sym[+off], %reg`; for an exe symbol that becomes
    `movq "iw5x@sym"(%rip), %reg` (and `leaq off(%reg), %reg`, which leaves the flags alone). Any other reference to
    an exe symbol stops the build. Returns the number of references rewritten."""
    out, n = [], 0
    for line in asm_path.read_text().splitlines():
        names = {a or b for a, b in _TOKEN.findall(line.split("#", 1)[0])} & exe_data
        if not names:
            out.append(line)
            continue
        if line.strip().startswith(".addrsig_sym"):
            continue  # an address-significance hint for a symbol that no longer exists
        m = _MOVABS.match(line)
        if not m or m.group(2).strip('"') not in exe_data:
            sys.exit(f"{asm_path}: unsupported reference to exe data {sorted(names)}: {line.strip()}")
        indent, sym, off, reg = m.group(1), m.group(2).strip('"'), m.group(3), m.group(4)
        out.append(f'{indent}movq\t"{SLOT_PREFIX}{sym}"(%rip), {reg}')
        if off:
            out.append(f"{indent}leaq\t{int(off, 0)}({reg}), {reg}")
        n += 1
    asm_path.write_text("\n".join(out) + "\n")
    return n


def import_slots(pe):
    """{import name: IAT slot va} of the exe: the Store build links the CRT dynamically."""
    import pefile
    pe.parse_data_directories(directories=[pefile.DIRECTORY_ENTRY["IMAGE_DIRECTORY_ENTRY_IMPORT"]])
    out = {}
    for d in getattr(pe, "DIRECTORY_ENTRY_IMPORT", []):
        for imp in d.imports:
            name = imp.name.decode() if imp.name else BY_ORDINAL.get((d.dll.decode().lower(), imp.ordinal))
            if name:
                out[name] = imp.address
    return out


# imports by ordinal, named (the exes import XInput by ordinal only)
BY_ORDINAL = {(dll, ordinal): name for dll in ("xinput1_3.dll", "xinput1_4.dll")
              for ordinal, name in ((2, "XInputGetState"), (3, "XInputSetState"), (4, "XInputGetCapabilities"))}


def hybrid_entry(exe_base):
    """x64: the DLL's entry point. On DLL_PROCESS_ATTACH it reads the exe's actual base from the PEB and adds the
    difference from its preferred base to every slot between iw5_exe_slots_start and iw5_exe_slots_end. Trap slots
    lie after the end and stay 0. Returns TRUE; does nothing for any other reason or at the preferred base."""
    return ["\t.text", "\t.globl iw5_hybrid_entry", "iw5_hybrid_entry:",
            "\tcmpl $1, %edx", "\tjne 2f",
            "\tmovq %gs:0x60, %rax", "\tmovq 0x10(%rax), %rax",
            f"\tmovabsq ${exe_base:#x}, %rcx", "\tsubq %rcx, %rax", "\tje 2f",
            "\tleaq iw5_exe_slots_start(%rip), %rcx", "\tleaq iw5_exe_slots_end(%rip), %rdx",
            "1:", "\tcmpq %rdx, %rcx", "\tjae 2f", "\taddq %rax, (%rcx)", "\taddq $8, %rcx", "\tjmp 1b",
            "2:", "\tmovl $1, %eax", "\tretq"]


def check_no_exe_addresses(dll, exe_base, exe_size):
    """x64: the linked DLL's code must hold no address inside the exe's image, as an immediate or as a RIP-relative
    target. Only the slot table (data) holds exe addresses, and iw5_hybrid_entry rebases it."""
    out = subprocess.run(["llvm-objdump", "-d", "--no-show-raw-insn", "--section=.text", str(dll)],
                         check=True, capture_output=True, text=True).stdout
    bad = [line.strip() for line in out.splitlines() if line.startswith(" ") and ":" in line
           and any(exe_base <= int(h, 16) < exe_base + exe_size
                   for h in re.findall(r"0x([0-9a-fA-F]+)", line.split(":", 1)[1]))]
    if bad:
        sys.exit(f"{dll}: code refers to the exe's image directly ({len(bad)}):\n  " + "\n  ".join(bad[:20]))


def main():
    build = sys.argv[sys.argv.index("--build") + 1] if "--build" in sys.argv else symbols.retail_builds()[0]
    arch = symbols.architecture(build)
    if arch not in ARCH:
        sys.exit(f"{build}: {arch} builds aren't linked (only the x64 Store builds are run)")
    triple = ARCH[arch]["triple"]
    enhanced = "--enhanced" in sys.argv
    ci_local = "--ci-local-response" in sys.argv
    if ci_local and not enhanced:
        sys.exit("CI response requires enhanced test build")
    BUILD = out_dir(build, enhanced) if not ci_local else ROOT / "build" / build / "enhanced-ci-local"
    BUILD.mkdir(parents=True, exist_ok=True)

    objs, src_objs, enhanced_defs, src_defs = [], [], set(), set()
    units = [("src", p) for p in sorted((ROOT / "src").rglob("*.cpp"))]
    if enhanced:
        units = [("enhanced", p) for p in sorted((ROOT / "enhanced").rglob("*.cpp"))] + units
    for tree, src in units:
        obj = BUILD / "obj" / (tree if tree == "enhanced" else "") / src.relative_to(ROOT / tree).with_suffix(".obj")
        obj.parent.mkdir(parents=True, exist_ok=True)
        flags = []
        if not symbols.build_spec(build).get("asserts"):
            flags.append("-DIW5_NO_ASSERTS")  # retail and ship builds compiled their asserts out
        if enhanced:
            # enhanced/'s own headers; and one section per function, so the src/ copies an exe-first thunk replaces
            # are dropped from the DLL (/opt:ref), not left as unnamed code
            flags += [f"-I{ROOT / 'enhanced' / 'include'}", "-ffunction-sections"]
        if ci_local and tree == "enhanced" and src.name in ("gdk_local_user.cpp", "gdk_runtime.cpp", "win_vendor_network.cpp"):
            flags.append("-DIW5_CI_LOCAL_RESPONSE")
        compile_unit(src, obj, arch, flags)
        objs.append(obj)
        defs = {p[2] for p in nm(obj, "--defined-only", "--extern-only") if len(p) == 3 and p[1] in "Tt"}
        if tree == "enhanced":
            enhanced_defs |= defs
        else:
            src_objs.append(obj)
            src_defs |= defs
    if not objs:
        sys.exit("no sources under src/")

    env_objs = []
    (BUILD / "fltused.c").write_text("int _fltused = 0;\n")
    run("clang", triple, "-c", BUILD / "fltused.c", "-o", BUILD / "fltused.obj")
    env_objs.append(BUILD / "fltused.obj")
    run("clang", triple, "-c", ROOT / "tools" / "recon" / "crt_x64.s", "-o", BUILD / "crt_x64.obj")
    env_objs.append(BUILD / "crt_x64.obj")

    defined, exports, rows = set(), [], {}
    for obj in objs:  # enhanced/ first: an override is listed once, from enhanced/
        for parts in nm(obj, "--defined-only", "--extern-only"):
            if len(parts) == 3:
                defined.add(parts[2])
                if parts[1] in ("T", "t") and parts[2] not in rows:
                    exports.append(parts[2])
                    rows[parts[2]] = obj.relative_to(BUILD / "obj").with_suffix(".cpp").as_posix()
    for obj in env_objs:
        defined |= {p[2] for p in nm(obj, "--defined-only", "--extern-only") if len(p) == 3}
    undefined = sorted({p[-1] for o in objs + env_objs for p in nm(o, "--undefined-only")} - defined)
    # The enhanced build may call Win32 functions the exe doesn't import: real imports of the DLL, from
    # tools/recon/lib/*.def (the harness stubs recon.dll's imports like the original's)
    import_libs = []
    if enhanced:
        dll_imports = set()
        for d in sorted((ROOT / "tools" / "recon" / "lib").glob("*.def")):
            lib = BUILD / f"{d.stem}.lib"
            run("llvm-dlltool", "-m", "i386:x86-64", "-d", d, "-l", lib)
            import_libs.append(lib)
            listed = [l.strip() for l in d.read_text().splitlines()]
            dll_imports |= {n for n in listed[listed.index("EXPORTS") + 1:] if n and not n.startswith(";")}
        undefined = [u for u in undefined if u not in dll_imports and not (u.startswith("__imp_") and u[6:] in dll_imports)]

    def where(sym):
        return symbols.resolve(build, sym)

    # Only functions the Store build identifies are exported (and tested); anything else is a header helper or a
    # function not identified there yet.
    stray = [e for e in exports if not where(e) and e not in enhanced_defs and e.lstrip("_") != "fltused"]
    if stray:
        print(f"note: {len(stray)} function(s) in src/ are not identified in {build} yet (not exported): "
              + ", ".join(stray[:8]) + (" ..." if len(stray) > 8 else ""))
    exports = [e for e in exports if where(e) or e in enhanced_defs]

    hybrid = ["symbol\tkind\tva"]
    asm = ["\t.text"]
    rewritten = 0
    tls_lo, tls_hi = tls_template(build)
    tls = []

    import pefile
    pe = pefile.PE(str(original_exe(build)), fast_load=True)
    exe_base, exe_size = pe.OPTIONAL_HEADER.ImageBase, pe.OPTIONAL_HEADER.SizeOfImage
    iat = import_slots(pe)
    exe_slots, exe_data, traps = [], set(), []

    def slot(va, label=None):
        label = label or f"iw5_exe_slot_{len(exe_slots)}"
        exe_slots.append((label, va))
        return label
    ref_table = symbols.table(symbols.reference_build())
    for u in undefined:
        q = '"' + u + '"'
        if u in iat:  # an import of this build is authoritative over a matcher pair for a CRT name
            asm += [f"\t.globl {q}", f"{q}:", f"\tmovq {slot(iat[u])}(%rip), %rax", "\tjmpq *(%rax)"]
            hybrid.append(f"{u}\timport\t{iat[u]:08X}")
            continue
        if u.startswith("__imp_") and u[6:] in iat:
            # a dllimport call (windows.h): __imp_X is the import address table entry itself, exe data whose
            # address comes from its slot like any other, so the call reads the exe's resolved pointer
            exe_data.add(u)
            slot(iat[u[6:]], f'"{SLOT_PREFIX}{u}"')
            hybrid.append(f"{u}\timport-entry\t{iat[u[6:]]:08X}")
            continue
        hit = where(u)
        if not hit:
            ref = symbols.resolve(symbols.reference_build(), u) or next(
                (v for k, v in ref_table.items() if symbols.scoped_name(k) == symbols.scoped_name(u)), None)
            if (ref and ref[1] == "data") or u.startswith(symbols.STATIC_PREFIX):
                exe_data.add(u)
                traps.append(f'"{SLOT_PREFIX}{u}"')
                hybrid.append(f"{u}\tunresolved-data\t")
            else:
                asm += [f"\t.globl {q}", f"{q}:", "\tud2"]
                hybrid.append(f"{u}\tunresolved-function\t")
            continue
        va, kind = hit
        if kind == "data" and tls_lo <= va < tls_hi:
            tls.append((va - tls_lo, q))
            hybrid.append(f"{u}\ttls\t{va:08X}")
            continue
        if kind == "function":
            asm += [f"\t.globl {q}", f"{q}:", f"\tjmpq *{slot(va)}(%rip)"]
        else:
            exe_data.add(u)
            slot(va, f'"{SLOT_PREFIX}{u}"')
        hybrid.append(f"{u}\t{kind}\t{va:08X}")
    redirects = []
    if enhanced:
        import runtime_link
        for u in sorted(src_defs - enhanced_defs):
            hit = where(u)
            if hit and hit[1] == "function":  # exe first: the exe's own copy, ahead of src/'s
                q = '"' + u + '"'
                asm += [f"\t.globl {q}", f"{q}:", f"\tjmpq *{slot(hit[0])}(%rip)"]
                hybrid.append(f"{u}\texe-first\t{hit[0]:08X}")
        redirects = [(where(u)[0], u) for u in sorted(enhanced_defs) if where(u) and where(u)[1] == "function"]
        runtime_link.check_targets(build, original_exe(build), pe, redirects)
        asm += runtime_link.redirect_asm(pe, redirects)
        asm += ["\t.text"]
    asm += hybrid_entry(exe_base)
    asm += ["\t.data", "\t.p2align 3", "\t.globl iw5_exe_slots_start", "iw5_exe_slots_start:"]
    for label, va in exe_slots:
        asm += [f"\t.globl {label}", f"{label}:", f"\t.quad {va:#x}"]
    asm += ["\t.globl iw5_exe_slots_end", "iw5_exe_slots_end:"]
    for label in traps:
        asm += [f"\t.globl {label}", f"{label}:", "\t.quad 0"]
    # the units' references to exe data become loads from their slots (the symbols themselves are never defined,
    # so anything left over fails the link instead of reaching a stale address)
    for obj in objs:
        n = exe_relative_asm(obj.with_suffix(".s"), exe_data)
        if n:
            run("clang", triple, "-c", obj.with_suffix(".s"), "-o", obj)
            rewritten += n
    asm += tls_asm(tls, tls_lo, tls_hi)
    (BUILD / "hybrid.s").write_text("\n".join(asm) + "\n")
    run("clang", triple, "-c", BUILD / "hybrid.s", "-o", BUILD / "hybrid.obj")
    (BUILD / "hybrid.tsv").write_text("\n".join(hybrid) + "\n")

    dll = BUILD / "recon.dll"
    # enhanced: hybrid.obj's exe-first thunks come before src/'s definitions (/force:multiple keeps the first)
    ordered = ([o for o in objs if o not in src_objs] + [BUILD / "hybrid.obj"] + src_objs if enhanced
               else objs + [BUILD / "hybrid.obj"])
    run("lld-link", "/dll", "/nodefaultlib", "/entry:iw5_hybrid_entry", "/base:0x180000000", "/dynamicbase",
        f"/out:{dll}", *[f"/export:{e}" for e in exports],
        *(["/export:iw5_redirects,DATA", "/export:iw5_redirects_identity,DATA", "/force:multiple"] if enhanced else []),
        f"/map:{BUILD / 'recon.map'}", *ordered, *env_objs, *import_libs, quiet_duplicates=enhanced)
    check_no_exe_addresses(dll, exe_base, exe_size)
    if enhanced:
        import runtime_link
        n = runtime_link.check_reachable(dll, BUILD / "recon.map", hybrid[1:], [u for _, u in redirects])
        print(f"runtime: {len(redirects)} exe functions replaced ({', '.join(u for _, u in redirects)}); "
              f"{n} functions reachable from them, no trap")
    (BUILD / "recon.symbols.tsv").write_text(
        "symbol\tsource\tva\n" + "".join(f"{e}\t{rows[e]}\t{where(e)[0]:08X}\n" if where(e) else f"{e}\t{rows[e]}\t\n"
                                         for e in exports))
    unresolved = sum(1 for h in hybrid if "\tunresolved" in h)
    print(f"{dll}: {len(exports)} functions from {len(objs)} files; {len(undefined) - unresolved} symbols resolved "
          f"to {build}, {rewritten} data references through {len(exe_slots)} rebased slots"
          + (f", {unresolved} not identified there yet (trap)" if unresolved else ""))


if __name__ == "__main__":
    main()
