"""P0003: the local GDK runtime; P0004: no Xbox Live HTTP (docs/online.md).

- QueryApiImpl answers XUser and its gamertag interface locally: the local player is "Unknown Soldier", signed in,
  with the fixed local XUID, every privilege; an interface it doesn't know is E_NOINTERFACE when there's no system
  runtime to ask.
- CurlProvider::Initialize builds the same provider object and Result as the original, without starting XCurl
  (no curl_global_init); PerformAsync sends nothing and returns E_HC_NO_NETWORK.
"""
import struct

from unicorn import x86_const as X

from harness import SCRATCH
from test_mouse import dll_static

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
    hr, bufs, _, _ = ctx.m.call(ctx.enhanced("QueryApiImpl"), [first, second, bytes(8)], "int")
    return hr, struct.unpack("<Q", bufs[2])[0]


def method(ctx, interface, slot):
    vtable = struct.unpack("<Q", bytes(ctx.m.mu.mem_read(interface, 8)))[0]
    return struct.unpack("<Q", bytes(ctx.m.mu.mem_read(vtable + slot, 8)))[0]


def check_local_player(ctx):
    hr, user = query(ctx, USER, USER_IMPL)
    assert hr == 0 and user, f"XUser interface: {hr:#x}"
    hr, tag = query(ctx, USER, GAMERTAG)
    assert hr == 0 and tag, f"XUser gamertag interface: {hr:#x}"
    player = dll_static("?g_gdkLocalUser@@3UXUser@@A")
    hr, bufs, _, _ = ctx.m.call(method(ctx, tag, 0x18), [tag, player, 0, 16, bytes(16), bytes(8)], "int")
    assert hr == 0 and bufs[0].rstrip(b"\0") == b"Unknown Soldier", f"gamertag {hr:#x} {bufs[0]!r}"
    assert struct.unpack("<Q", bufs[1])[0] == 16
    hr, bufs, _, _ = ctx.m.call(method(ctx, user, 0x58), [user, player, bytes(8)], "int")
    assert hr == 0 and struct.unpack("<Q", bufs[0])[0] == 0x0009000000000001, "XUserGetId"
    hr, bufs, _, _ = ctx.m.call(method(ctx, user, 0x70), [user, player, bytes(4)], "int")
    assert hr == 0 and bufs[0] == bytes(4), "XUserGetState: signed in"
    hr, bufs, _, _ = ctx.m.call(method(ctx, user, 0xA0), [user, player, 0, 254, bytes(1), bytes(4)], "int")
    assert hr == 0 and bufs[0] == b"\1" and bufs[1] == bytes(4), "XUserCheckPrivilege(Multiplayer)"


def check_store_without_system_runtime(ctx):
    hr, out = query(ctx, STORE, STORE_IMPL)
    assert hr == 0x80004002 - (1 << 32) or hr == 0x80004002, f"XStore without a system runtime: {hr:#x}"
    assert out == 0


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


CHECKS = [check_local_player, check_store_without_system_runtime, check_curl_provider_without_xcurl,
          check_perform_sends_nothing]
