"""P0003: fail-closed GDK isolation; P0004: no Xbox Live HTTP (docs/online.md).

The runtime bootstrap is local-only. Every interface query fails, including Store and
user interfaces. No authorization is fabricated. HTTP provider behavior remains offline.
"""
import struct

from unicorn import x86_const as X

from harness import SCRATCH

PATCH = "P0003"  # and P0004 (records/patches.tsv)
BUILDS = ["iw5mp_ship"]
ARGS = (X.UC_X86_REG_RCX, X.UC_X86_REG_RDX, X.UC_X86_REG_R8, X.UC_X86_REG_R9)

def guid(text):
    a, b, c, d, e = text.split("-")
    return struct.pack("<IHH", int(a, 16), int(b, 16), int(c, 16)) + bytes.fromhex(d + e)


USER = guid("01acd177-91f9-4763-a38e-ccbb55ce32e0")
USER_IMPL = guid("079415e3-6727-437f-8e9d-8f8f9b2439f7")
GAMERTAG = guid("cef4fac0-7676-4a94-a119-4c43f9eb5b74")
STORE = guid("0dd112ac-7c24-448c-b92b-3960fb5bd30c")
STORE_IMPL = guid("5c48dedf-0b67-4492-a4b5-6829b8e796e1")


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
    for first, second in [
        (USER, USER_IMPL), (USER, GAMERTAG), (STORE, STORE_IMPL),
        *[(guid(value), guid(value)) for value in (
            "073b7dcb-1fcf-4030-94be-e3c9eb623428",
            "704c3f58-e629-4cc2-b197-30119f1b1be2",
            "37e56907-2f10-41e8-b1b2-f2fb72c8acaa",
            "af406016-e850-4aa8-a82d-2f3dcb0d7f7e",
            "8836fe87-edb9-4fe3-8dad-05f0d2cd5b40",
            "e349bd1a-fc20-4e40-b99c-416e40642a9f",
            "973a344e-24bf-4d0f-8457-562b062c2b29",
            "0651aae2-4012-4077-bf84-8b9009700e2c",
            "bbfbdcc7-bfe7-409b-a5ca-edf054960b4d",
            "dfcd4649-4ff8-4043-ba07-35d607df98b0",
            "00000000-0000-0000-0000-000000000000",
        )],
    ]:
        hr, out = query(ctx, first, second)
        assert (hr & 0xffffffff) == 0x80004002 and out == 0, f"QueryApiImpl: {hr:#x}, {out:#x}"


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
