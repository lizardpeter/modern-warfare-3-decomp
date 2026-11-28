#!/usr/bin/env python3
"""Build the enhanced Store build as it is installed: two DLLs for the game folder. From black-ops-2-decomp's.

    python3 tools/build_runtime.py [--build iw5mp_ship]
    -> build/<build>/runtime/X3DAudio1_7.dll, iw5_enhanced.dll

- iw5_enhanced.dll: the enhanced build with the runtime link's exe-first thunks, redirect table and checks
  (tools/build_recon.py --enhanced, tools/runtime_link.py).
- X3DAudio1_7.dll: the loader (runtime/loader): forwards X3DAudio to the system DLL, and for the supported multiplayer
  exe loads iw5_enhanced.dll and applies its redirects.

Then the package is checked the way the loader will use it: the loader exports X3DAudioInitialize and
X3DAudioCalculate, and the enhanced DLL's redirect table holds, for each replaced function, an RVA inside the exe,
the exe's own first bytes there, and the address of an export. Installing is in docs/enhanced-build.md.
"""
import argparse
import shutil
import struct
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
sys.path.insert(0, str(ROOT / "tests" / "differential"))
TARGETS = {"iw5mp_ship": "iw5_enhanced.dll"}
LOADER = ROOT / "runtime" / "loader"
LOADER_DLL = "X3DAudio1_7.dll"
TRIPLE = "--target=x86_64-pc-windows-msvc"


def run(*args):
    subprocess.run([str(a) for a in args], check=True)


def build_loader(work, out):
    run("clang", TRIPLE, "-O2", "-ffreestanding", "-fno-builtin", "-fno-stack-protector", "-Wall", "-Werror",
        "-c", LOADER / "x3daudio_proxy.c", "-o", work / "x3daudio_proxy.obj")
    run("clang", TRIPLE, "-c", LOADER / "x3daudio_stubs.s", "-o", work / "x3daudio_stubs.obj")
    run("llvm-dlltool", "-m", "i386:x86-64", "-d", LOADER / "kernel32.def", "-l", work / "kernel32.lib")
    run("lld-link", "/dll", "/Brepro", "/nodefaultlib", "/entry:ProxyMain", f"/def:{LOADER / 'x3daudio.def'}",
        f"/out:{out / LOADER_DLL}", work / "x3daudio_proxy.obj", work / "x3daudio_stubs.obj", work / "kernel32.lib")


def check(exe, out, dll_name):
    import pefile
    from symbols import scoped_name
    loader = pefile.PE(str(out / LOADER_DLL))
    names = {(e.name or b"").decode() for e in loader.DIRECTORY_ENTRY_EXPORT.symbols}
    for name in ("X3DAudioInitialize", "X3DAudioCalculate"):
        assert name in names, f"loader: no export {name}"

    base, size = exe.OPTIONAL_HEADER.ImageBase, exe.OPTIONAL_HEADER.SizeOfImage
    dll = pefile.PE(str(out / dll_name))
    from gdk.audit_isolation import check_pe
    for label, image in ((LOADER_DLL, loader), (dll_name, dll)):
        imports = check_pe(image, label)
        print(f"{label}: {len(imports)} import modules; no known GDK runtime imports")
    dbase = dll.OPTIONAL_HEADER.ImageBase
    exports = {(e.name or b"").decode(): dbase + e.address for e in dll.DIRECTORY_ENTRY_EXPORT.symbols}
    by_address = {}
    for name, va in exports.items():
        by_address.setdefault(va, []).append(name)
    table = exports["iw5_redirects"]
    count = struct.unpack("<Q", dll.get_data(table - dbase, 8))[0]
    assert count, f"{dll_name}: empty redirect table"
    identity = exports["iw5_redirects_identity"]
    assert identity == table + 8 + 32 * count, f"{dll_name}: invalid redirect table length"
    assert struct.unpack("<II", dll.get_data(identity - dbase, 8)) == (exe.FILE_HEADER.TimeDateStamp, size), (
        f"{dll_name}: redirect table targets another executable")
    lines = []
    for i in range(count):
        rva, target = struct.unpack("<QQ", dll.get_data(table - dbase + 8 + 32 * i, 16))
        head = dll.get_data(table - dbase + 8 + 32 * i + 16, 16)
        assert rva + len(head) <= size, f"redirect {i}: RVA {rva:#x} outside the exe"
        assert exe.get_data(rva, 16) == head, f"redirect {i}: prologue differs from the exe at {base + rva:X}"
        names_ = by_address.get(target)
        assert names_, f"redirect {i}: target {target:#x} is not an export"
        lines.append(f"{base + rva:X} -> {scoped_name(names_[0])}")
    return lines


def main():
    import pefile
    from harness import original_exe
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--build", choices=TARGETS, default="iw5mp_ship")
    args = parser.parse_args()
    build = args.build
    exe = pefile.PE(str(original_exe(build)), fast_load=True)
    out = ROOT / "build" / build / "runtime"
    dll_name = TARGETS[build]
    run(sys.executable, ROOT / "tools" / "build_recon.py", "--build", build, "--enhanced")
    out.mkdir(parents=True, exist_ok=True)
    work = out / "obj"
    work.mkdir(exist_ok=True)
    shutil.copyfile(ROOT / "build" / build / "enhanced" / "recon.dll", out / dll_name)
    build_loader(work, out)
    for line in check(exe, out, dll_name):
        print("redirect", line)
    print(f"{out}: {LOADER_DLL}, {dll_name}")


if __name__ == "__main__":
    main()
