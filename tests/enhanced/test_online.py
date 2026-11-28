"""P0003: fail-closed GDK isolation; P0004: no Xbox Live HTTP (docs/online.md).

The runtime bootstrap is local-only. Every interface query fails, including Store and
user interfaces. No authorization is fabricated. HTTP provider behavior remains offline.
"""
import csv
import struct
from pathlib import Path

from unicorn import x86_const as X

from harness import SCRATCH

PATCH = "P0003"  # and P0004 (records/patches.tsv)
BUILDS = ["iw5mp_ship"]
ARGS = (X.UC_X86_REG_RCX, X.UC_X86_REG_RDX, X.UC_X86_REG_R8, X.UC_X86_REG_R9)

def guid(text):
    a, b, c, d, e = text.split("-")
    return struct.pack("<IHH", int(a, 16), int(b, 16), int(c, 16)) + bytes.fromhex(d + e)


ROOT = Path(__file__).resolve().parents[2]


def original_interface_pairs(build):
    path = ROOT / "analysis" / build / "gdk" / "thunks.tsv"
    with path.open(encoding="utf-8") as f:
        rows = list(csv.DictReader((line for line in f if not line.startswith("#")), delimiter="\t"))
    assert rows and all(row["name"] for row in rows), "incomplete GDK thunk catalogue"
    assert len({row["va"] for row in rows}) == len(rows), "duplicate thunk addresses"
    return [(row["name"], guid(row["iid"]), guid(row["class"])) for row in rows]


def query(ctx, first, second):
    """QueryApiImpl(first, second, &out): (hr, interface)."""
    hr, bufs, _, _ = ctx.m.call(ctx.enhanced("QueryApiImpl"), [first, second, b"\xff" * 8], "int")
    return hr, struct.unpack("<Q", bufs[2])[0]


def check_local_bootstrap(ctx):
    calls = []
    module = 0x180000000
    path = b"C:\\IW5\\iw5_enhanced.dll\0"

    def self_module(m, mu):
        calls.append("self")
        assert mu.reg_read(ARGS[0]) == 6
        mu.mem_write(mu.reg_read(ARGS[2]), struct.pack("<Q", module))
        mu.reg_write(X.UC_X86_REG_RAX, 1)

    def module_path(m, mu):
        calls.append("path")
        assert mu.reg_read(ARGS[0]) == module
        assert mu.reg_read(ARGS[2]) >= len(path)
        mu.mem_write(mu.reg_read(ARGS[1]), path)
        mu.reg_write(X.UC_X86_REG_RAX, len(path) - 1)

    ctx.imports({"kernel32.dll!GetModuleHandleExA": (self_module, 0),
                 "kernel32.dll!GetModuleFileNameA": (module_path, 0)})
    for name, args, expected in [
        ("XGameRuntimeLoadModule", [], ["self"]),
        ("InitializeApiImpl", [25398, 0], ["self", "path"]),
        ("InitializeApiImplEx", [25398, 0, 0], ["self", "path"]),
    ]:
        calls.clear()
        hr, _, _, _ = ctx.m.call(ctx.enhanced(name), args, "uint")
        assert hr == 0 and calls == expected, f"{name}: {hr}, {calls}"

    def missing_module(m, mu):
        mu.reg_write(X.UC_X86_REG_RAX, 0)

    ctx.imports({"kernel32.dll!GetModuleHandleExA": (missing_module, 0)})
    for name, args in [("XGameRuntimeLoadModule", []), ("InitializeApiImpl", [25398, 0]),
                       ("InitializeApiImplEx", [25398, 0, 0])]:
        hr, _, _, _ = ctx.m.call(ctx.enhanced(name), args, "uint")
        assert hr == 0x89240101, f"{name}: {hr}"


def check_gdk_isolation(ctx):
    # Test every exact IID/class pair recovered from the original, not hand-copied GUIDs.
    for name, first, second in original_interface_pairs(ctx.build) + [("unknown", bytes(16), bytes(16))]:
        hr, out = query(ctx, first, second)
        assert (hr & 0xffffffff) == 0x80004002 and out == 0, f"{name}: {hr:#x}, {out:#x}"
    hr, _, _, _ = ctx.m.call(ctx.enhanced("QueryApiImpl"), [0, 0, 0], "uint")
    assert hr == 0x80004003, f"null output: {hr:#x}"


ALLOC = SCRATCH + 0x8000


def allocator():
    state = {"next": ALLOC}

    def alloc(mu):
        size = mu.reg_read(ARGS[0])
        at = state["next"]
        state["next"] += (size + 15) & ~15
        mu.reg_write(X.UC_X86_REG_RAX, at)
    return alloc


def check_curl_provider_without_xcurl(ctx):
    calls = []
    ctx.imports({"XCurl.dll!curl_global_init": (lambda m, mu: (calls.append("init"), mu.reg_write(X.UC_X86_REG_RAX, 0)), 0),
                 "MSVCP140.dll!_Mtx_init_in_situ": (lambda m, mu: calls.append("mutex"), 0)})
    fakes = ctx.hooks({"http_alloc@httpclient@xbox": allocator()})
    calls.clear()
    want = ctx.m.call(ctx.original("Initialize@CurlProvider@httpclient@xbox"), [bytes(0x40)], "uint64", fakes=fakes)
    original_calls = list(calls)
    fakes = ctx.hooks({"http_alloc@httpclient@xbox": allocator()})
    calls.clear()
    got = ctx.m.call(ctx.enhanced("Initialize@CurlProvider@httpclient@xbox"), [bytes(0x40)], "uint64", fakes=fakes)
    assert original_calls == ["init", "mutex"], f"the original: {original_calls}"
    assert calls == ["mutex"], f"the enhanced build starts XCurl: {calls}"
    assert want[1][0] == got[1][0], f"Result: {want[1][0].hex()} / {got[1][0].hex()}"
    provider = struct.unpack_from("<Q", got[1][0], 0x30)[0]
    assert provider == ALLOC, "the provider isn't the first allocation"


def check_perform_sends_nothing(ctx):
    hr, _, writes, _ = ctx.m.call(ctx.enhanced("PerformAsync@CurlProvider@httpclient@xbox"),
                                  [SCRATCH, SCRATCH + 0x100, SCRATCH + 0x200], "uint")
    assert hr == 0x89235006, f"PerformAsync: {hr:#x}"
    assert not writes, "PerformAsync wrote memory"


CHECKS = [check_local_bootstrap, check_gdk_isolation, check_curl_provider_without_xcurl,
          check_perform_sends_nothing]
