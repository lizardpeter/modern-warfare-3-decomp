"""GDK isolation, local engine startup and vendor transport (docs/online.md).

The runtime bootstrap is local-only. Every interface query fails, including Store and
user interfaces. No authorization is fabricated. HTTP provider behavior remains offline.
"""
import csv
import struct
from contextlib import contextmanager
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


def query(ctx, first, second, state=None):
    """QueryApiImpl(first, second, &out): (hr, interface).

    The differential harness rolls back image writes after every invocation.
    Replay only this test's captured DLL writes to model a single process,
    while retaining the default isolated behavior for unrelated checks.
    """
    def setup(w):
        for addr in sorted(state)[::8]:
            w.write(addr, b"".join(state[addr + i] for i in range(8)))

    hr, bufs, writes, _ = ctx.m.call(
        ctx.enhanced("QueryApiImpl"), [first, second, b"\xff" * 8], "int",
        setup=setup if state is not None else None)
    assert isinstance(hr, int), f"QueryApiImpl did not return: {hr}"
    if state is not None:
        assert all(ctx.recon.base <= addr < ctx.recon.base + ctx.recon.size
                   for addr in writes), "unexpected non-DLL state mutation"
        state.update(writes)
    return hr, struct.unpack("<Q", bufs[-1])[0]


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
    trace = []
    def debug_string(m, mu):
        trace.append(m.read_cstr(mu.reg_read(ARGS[0])))
    ctx.imports({"kernel32.dll!OutputDebugStringA": (debug_string, 0)})
    pairs = original_interface_pairs(ctx.build)
    # A busy async interface must not exhaust the budget for later Store/user queries.
    _, first_iid, first_cls = next(p for p in pairs if p[0] == "XStoreCreateContext")
    state = {}
    for i in range(300):
        hr, out = query(ctx, first_iid, first_cls, state)
        assert (hr & 0xffffffff) == 0x80004002 and out == 0, (i, hr, out)
        assert len(trace) == 1, f"duplicate trace at query {i}: {len(trace)}"
    seen = {(first_iid, first_cls)}
    for name, first, second in pairs + [("unknown", bytes(16), bytes(16))]:
        before = len(trace)
        hr, out = query(ctx, first, second, state)
        if name.startswith(("XUser", "XAsync", "XTaskQueue", "XThread")):
            assert hr == 0 and out != 0 and len(trace) == before, name
            continue
        assert (hr & 0xffffffff) == 0x80004002 and out == 0, f"{name}: {hr:#x}, {out:#x}"
        expected = f"iw5_gdk_rejected iid_le={first.hex()} class_le={second.hex()}\n".encode()
        if (first, second) in seen:
            assert len(trace) == before, name
        else:
            assert len(trace) == before + 1 and trace[-1] == expected, name
            seen.add((first, second))
    before = len(trace)
    hr, _, _, _ = ctx.m.call(ctx.enhanced("QueryApiImpl"), [0, 0, 0], "uint")
    assert hr == 0x80004003 and len(trace) == before, f"null output: {hr:#x}"
    hr, out = query(ctx, 0, 0, state)
    assert (hr & 0xffffffff) == 0x80004002 and out == 0
    assert trace[-1] == b"iw5_gdk_rejected iid_le=" + b"-" * 32 + b" class_le=" + b"-" * 32 + b"\n"


@contextmanager
def installed_redirect(ctx, entry):
    table = ctx.recon.exports["iw5_redirects"]
    count, = struct.unpack("<Q", ctx.m.mu.mem_read(table, 8))
    rva = entry - 0x140000000
    for i in range(count):
        record = table + 8 + i * 32
        address, target, head = struct.unpack("<QQ16s", ctx.m.mu.mem_read(record, 32))
        if address == rva:
            break
    else:
        raise AssertionError(f"entry {entry:#x} is missing from the redirect table")
    assert bytes(ctx.m.mu.mem_read(entry, 16)) == head
    try:
        ctx.m.patch_code(entry, bytes.fromhex("ff2500000000") + struct.pack("<Q", target))
        yield
    finally:
        ctx.m.patch_code(entry, head)


def check_engine_startup_without_xbox_live(ctx):
    """P0005: reach the local startup branch even when there is no Xbox Live queue."""
    entry = ctx.original("Gdk_InitEngineTasks")
    task_init = ctx.original("TaskGroup_Init")
    calls = []

    def init_group(mu):
        group = mu.reg_read(ARGS[0])
        name = ctx.m.read_cstr(mu.reg_read(ARGS[1]))
        calls.append((group, name))

    def xbox_live_unavailable(mu):
        calls.append("Xbox Live")
        return 0x800701AB

    def init_local_runtime(mu):
        calls.append("local runtime")
        return local_result

    local_result = 0
    fakes = {
        task_init: init_group,
        0x140369F20: xbox_live_unavailable,
        ctx.enhanced("InitializeApiImpl"): init_local_runtime,
    }
    native_groups = ("Gdk_InitUserTasks", "Gdk_InitPartyTasks", "Gdk_InitStoreTasks")
    for name in native_groups:
        fakes[ctx.original(name)] = lambda mu, group=name: calls.append(group)

    general_tasks = (0x143D5A8D0, b"GDK tasks.")
    save_tasks = (0x143D58CB0, b"GDK gamesave tasks.")
    status, _, _, _ = ctx.m.call(entry, [], "bool", fakes=fakes)
    assert status == 0 and calls == [general_tasks, "Xbox Live"], (status, calls)

    with installed_redirect(ctx, entry):
        calls.clear()
        status, _, _, _ = ctx.m.call(entry, [], "bool", fakes=fakes)
        assert status == 1 and calls == ["local runtime", general_tasks, save_tasks, *native_groups], (
            status, calls)

        local_result = 0x89240101
        calls.clear()
        status, _, _, _ = ctx.m.call(entry, [], "bool", fakes=fakes)
        assert status == 0 and calls == ["local runtime"], (status, calls)


def check_demonware_dns_fails_locally(ctx):
    """STUN and every other DemonWare name find nothing; the auth and lobby names resolve to the local DemonWare
    (127.0.0.1), starting it."""
    entry = ctx.original("BdGetHostByName")
    output = b"\xA5" * 0x210
    started = []
    fakes = ctx.hooks({"Start@LocalDemonWare": lambda mu: started.append(1) or 1,
                       "Trace@LocalDemonWare": lambda mu: None})
    with installed_redirect(ctx, entry):
        for host in (b"mw3-stun.us.demonware.net", b"stun.eu.demonware.net", b"auth3.prod.demonware.net.evil",
                     b"lsg.2310.prod.demonware.net.example", b"demonware.net"):
            count, buffers, writes, _ = ctx.m.call(entry, [host + b"\0", output, 4], "uint", fakes=fakes)
            assert count == 0 and buffers[1] == output and not writes, (host, count, writes)
            assert not ctx.m.imports_called and not started, (host, ctx.m.imports_called)
        for host in (b"auth3.prod.demonware.net", b"lsg.2310.prod.demonware.net", b"LSG.2310.PROD.DEMONWARE.NET"):
            count, buffers, _, _ = ctx.m.call(entry, [host + b"\0", output, 4], "uint", fakes=fakes)
            assert count == 1 and buffers[1][:4] == b"\x7f\0\0\x01" and buffers[1][4:] == output[4:], (host, count)
            assert not ctx.m.imports_called, (host, ctx.m.imports_called)
        assert len(started) == 3


def check_vendor_socket_calls_fail_locally(ctx):
    entry = ctx.original("BdSendTo")
    with installed_redirect(ctx, entry):
        for socket, family, expected in ((-1, 2, -10), (9, 2, -5), (9, 23, -5), (9, 0, -1)):
            address = bytes(0x80) + struct.pack("<H", family)
            count, _, writes, _ = ctx.m.call(entry, [socket, address, 3074, b"data", 4], "int")
            assert count == expected and not writes and not ctx.m.imports_called, (socket, family, count)

    entry = ctx.original("BdConnectStream")
    with installed_redirect(ctx, entry):
        # only 127.0.0.1:3074, once the local DemonWare listens (not here), connects anywhere
        for ip, port in ((b"\x7f\0\0\x01", 3074), (b"\xc0\0\x02\x01", 3074), (b"\x7f\0\0\x01", 443)):
            status, _, writes, _ = ctx.m.call(entry, [9, ip, port], "int")
            assert status == -5 and not writes and not ctx.m.imports_called, (ip, port, status)


def check_vendor_http_calls_fail_locally(ctx):
    cannot_connect = 0x80072EFD
    calls = []

    def finalize(mu):
        calls.append((mu.reg_read(ARGS[0]), mu.reg_read(ARGS[1]) & 0xffffffff))

    entry = ctx.original("BdBeginWinHttpRequest")
    with installed_redirect(ctx, entry):
        ctx.m.call(entry, [SCRATCH, SCRATCH + 0x100], "void",
                   fakes={ctx.original("BdCompleteWinHttpRequest"): finalize})
        assert calls == [(SCRATCH + 0x100, cannot_connect)] and not ctx.m.imports_called, calls

    entry = ctx.original("WinHttpInitializeRequest")
    with installed_redirect(ctx, entry):
        status, _, writes, _ = ctx.m.call(entry, [SCRATCH], "uint")
        assert status == cannot_connect and not writes and not ctx.m.imports_called, status


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



def check_failed_connection_submission(ctx):
    """P0009: failed original sign-in submission cannot manufacture a pending state."""
    entry = ctx.original("sub_1403390D0")
    submit = ctx.original("sub_1401A4DC0")
    state = 0x1448DA8F4
    for previous in (0, 1, 2):
        for result in (0, 1):
            for changed in (None, 0, 2):
                for controller in (0, 7):
                    calls = []
                    def setup(w):
                        w.write(state, struct.pack("<I", previous))
                    def original_submission(mu):
                        assert mu.reg_read(ARGS[0]) & 0xffffffff == controller
                        assert struct.unpack("<I", mu.mem_read(state, 4))[0] == 1
                        calls.append(controller)
                        if changed is not None:
                            ctx.m.program_write(state, struct.pack("<I", changed))
                        return result
                    with installed_redirect(ctx, entry):
                        got, _, writes, _ = ctx.m.call(
                            entry, [controller], "uint", setup=setup,
                            fakes={submit: original_submission})
                    assert got == result and calls == [controller], (previous, result, changed, got, calls)
                    expected = (changed if changed is not None else
                                (1 if result else previous))
                    after = struct.unpack("<I", b"".join(
                        writes.get(state+i, struct.pack("<I", previous)[i:i+1])
                        for i in range(4)))[0]
                    assert after == expected, (previous, result, changed, after, expected)



def check_local_user_profile(ctx):
    calls = []
    ctx.imports({"kernel32.dll!OutputDebugStringA": (lambda m, mu: calls.append(m.read_cstr(mu.reg_read(ARGS[0]))), 0)})
    pairs = {name: (first, second) for name, first, second in original_interface_pairs(ctx.build)}
    def iface(name):
        hr, ptr = query(ctx, *pairs[name])
        assert hr == 0 and ptr
        table, = struct.unpack("<Q", ctx.m.mu.mem_read(ptr, 8))
        return ptr, table
    user, uv = iface("XUserAddAsync")
    tag, tv = iface("XUserGetGamertag")
    async_iface, av = iface("XAsyncGetStatus")
    state = {}
    def invoke(obj, table, slot, args, ret="uint", fakes=None):
        address, = struct.unpack("<Q", ctx.m.mu.mem_read(table+slot, 8))
        def setup(w):
            for a in sorted(state)[::8]:
                w.write(a, b"".join(state[a+i] for i in range(8)))
        hr, buffers, writes, _ = ctx.m.call(address, [obj, *args], ret, setup=setup, fakes=fakes)
        assert not isinstance(hr, str), hr
        state.update(writes)
        return hr, buffers
    hr, b = invoke(user, uv, 0x30, [bytes(4)])
    assert hr == 0 and struct.unpack("<I", b[0])[0] == 1
    assert invoke(user, uv, 0x30, [0])[0] == 0x80004003
    for flags in (3, 5, 7, 8, 0xffffffff):
        assert invoke(user, uv, 0x38, [flags, bytes(56)])[0] == 0x80070057
    hr, b = invoke(user, uv, 0x50, [1, b"\xff"*8])
    assert hr == 0x80070490 and b[0] == bytes(8)
    hr, b = invoke(user, uv, 0x38, [1, bytes(56)])
    assert hr == 0
    block = b[0]
    assert invoke(async_iface, av, 0x18, [block, 0])[0] == 0
    hr, b = invoke(user, uv, 0x40, [block, bytes(8)])
    assert hr == 0
    handle, = struct.unpack("<Q", b[1])
    assert handle
    assert invoke(user, uv, 0x40, [b[0], bytes(8)])[0] == 0x80070057
    hr, b = invoke(user, uv, 0x48, [handle, bytes(8)])
    assert hr == 0 and struct.unpack("<Q", b[0])[0] == 1
    hr, b = invoke(user, uv, 0x50, [1, bytes(8)])
    assert hr == 0 and struct.unpack("<Q", b[0])[0] == handle
    invoke(user, uv, 0x20, [handle], "void")
    for ident in (0, 2, 0x0009000000000001):
        hr, b = invoke(user, uv, 0x50, [ident, b"\xff"*8])
        assert hr == 0x80070490 and b[0] == bytes(8)
    hr, b = invoke(user, uv, 0x60, [1, b"\xff"*8])
    assert hr == 0x80070490 and b[0] == bytes(8)
    hr, b = invoke(user, uv, 0x70, [handle, bytes(4)])
    assert hr == 0 and struct.unpack("<I", b[0])[0] == 2
    assert b"iw5_local_api XUserGetState result=0x00000000\n" in calls
    hr, b = invoke(user, uv, 0x58, [handle, b"\xff"*8])
    assert hr == 0x80004002 and b[0] == bytes(8)
    assert b"iw5_local_api XUserGetId result=0x80004002\n" in calls
    hr, _ = invoke(user, uv, 0xb8, [handle, 0, 0, 0, 0, 0, 0, 0, bytes(56)])
    assert hr == 0x80004002
    assert b"iw5_local_api XUserGetTokenAndSignatureAsync result=0x80004002\n" in calls
    hr, b = invoke(tag, tv, 0x18, [handle, 0, 16, bytes(16), bytes(8)])
    assert hr == 0 and b[0].split(b"\0")[0] == b"Unknown Soldier"
    for kind in (1, 3):
        hr, b = invoke(tag, tv, 0x18, [handle, kind, 16, bytes(16), bytes(8)])
        assert hr == 0 and b[0].split(b"\0")[0] == b"Unknown Soldier"
    hr, b = invoke(tag, tv, 0x18, [handle, 2, 1, b"\xff", bytes(8)])
    assert hr == 0 and b[0] == bytes(1) and struct.unpack("<Q", b[1])[0] == 1
    hr, b = invoke(tag, tv, 0x18, [handle, 0, 2, b"\xff"*2, bytes(8)])
    assert hr == 0x8007007a and struct.unpack("<Q", b[1])[0] == 16
    assert invoke(tag, tv, 0x18, [handle, 4, 16, bytes(16), bytes(8)])[0] == 0x80070057
    hr, b = invoke(user, uv, 0x18, [handle, bytes(8)])
    assert hr == 0 and struct.unpack("<Q", b[0])[0] == handle
    invoke(user, uv, 0x20, [handle], "void")
    assert invoke(user, uv, 0x48, [handle, bytes(8)])[0] == 0
    invoke(user, uv, 0x20, [handle], "void")
    assert invoke(user, uv, 0x48, [handle, bytes(8)])[0] == 0x80070057
    hr, b = invoke(user, uv, 0x50, [1, b"\xff"*8])
    assert hr == 0x80070490 and b[0] == bytes(8)
    assert invoke(user, uv, 0x38, [1, struct.pack("<Q", 1)+bytes(48)])[0] == 0x80070057

    # Exercise the actual local callback path using an immediate completion port.
    hr, b = invoke(async_iface, av, 0x60, [0, 3, bytes(8)])
    assert hr == 0
    queue, = struct.unpack("<Q", b[0])
    callbacks = []
    callback = ctx.original("TaskGroup_Init")
    def on_completion(mu):
        ptr = mu.reg_read(ARGS[0])
        callbacks.append(struct.unpack("<Q", mu.mem_read(ptr, 8))[0])
    block = struct.pack("<QQQ", queue, 0, callback) + bytes(32)
    hr, b = invoke(user, uv, 0x38, [1, block], fakes={callback: on_completion})
    assert hr == 0 and callbacks == [queue]
    hr, b = invoke(user, uv, 0x40, [b[0], bytes(8)])
    assert hr == 0
    handle, = struct.unpack("<Q", b[1])
    invoke(user, uv, 0x20, [handle], "void")
    invoke(async_iface, av, 0x88, [queue], "void")



CHECKS = [check_local_user_profile, check_local_bootstrap, check_gdk_isolation, check_curl_provider_without_xcurl,
          check_perform_sends_nothing, check_engine_startup_without_xbox_live,
          check_demonware_dns_fails_locally, check_vendor_socket_calls_fail_locally,
          check_vendor_http_calls_fail_locally, check_failed_connection_submission]
