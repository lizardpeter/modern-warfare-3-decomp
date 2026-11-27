"""P0001 (B0001, B0002, B0003): raw mouse input, no acceleration, no smoothing; P0002: MW2019 sensitivity.

- CL_MouseMove is the original Store function at equivalent settings (cl_mouseAccel 0, m_filter off, the FOV scale
  1, m_yaw 0.022, m_pitch +-0.022 and sensitivity m_sens * 0.3): the same command, calls and writes, the view
  angles within rounding (the enhanced build turns in double).
- One count turns the view by exactly m_sens * 0.0066 degrees (in double, rounded once to float), whatever the FOV
  scale, the sensitivity dvar and m_yaw; m_pitch's sign inverts the pitch and 0 stops it; the yaw ends in
  [-180, 180).
- The same movement over two frames ends where one frame does (the angles are kept in double between calls).
- cl_mouseAccel and m_filter change nothing.
- m_sens is registered once, on first use: 1.5, 0.01 to 100, archived.
- CL_GetMouseMovementWithSensitivity (the remote-control view) is the original without cl_mouseAccel or m_filter.
- IN_Frame: where the raw counts equal the cursor's movement it is the original Store function exactly; where they
  differ, CL_MouseEvent gets the counts (menus still get the cursor's position); a failed raw input registration
  stops the game with an error instead of falling back to the cursor.
- The raw counters add relative reports only, ignore absolute ones and reports while the game isn't in front, take
  and reset, and clamp instead of wrapping.
- In the game: the loader's 14-byte jumps at the exe's three functions reach the replacements (the redirect table
  applied to the emulated exe as runtime/loader does it).
"""
import math
import random
import re
import struct
from pathlib import Path

from unicorn import x86_const as X

import test_cl_input as tc
import test_win_input as tw
from harness import SCRATCH

PATCH = "P0001"  # and P0002 (records/patches.tsv)
BUILDS = ["iw5mp_ship"]
ROOT = Path(__file__).resolve().parents[2]

PITCH, YAW = tc.CLIENTS + 0x34F4, tc.CLIENTS + 0x34F8
ANGLE_BYTES = set(range(PITCH, PITCH + 8))
SENS_DVAR = SCRATCH + 0xA0000  # the m_sens dvar_t the registration fake returns
SENS = {"value": 1.5}
F32 = lambda v: struct.unpack("<f", struct.pack("<f", v))[0]
ARGS = (X.UC_X86_REG_RCX, X.UC_X86_REG_RDX, X.UC_X86_REG_R8, X.UC_X86_REG_R9)


def dll_static(name):
    """Address of a static of the enhanced DLL, from its link map."""
    for line in (ROOT / "build" / "iw5mp_ship" / "enhanced" / "recon.map").read_text().splitlines():
        parts = line.split()
        if len(parts) >= 3 and parts[1] == name:
            return int(parts[2], 16)
    raise KeyError(name)


def register_sens(mu):
    """Dvar_RegisterFloat's fake: the m_sens dvar (Store layout: current value at +0x10), holding SENS['value']."""
    mu.mem_write(SENS_DVAR, bytes(0x10) + struct.pack("<f", SENS["value"]) + bytes(12))
    return SENS_DVAR


def aim_identity(mu):
    """AimAssist_UpdateGamePadInput's fake: the view angles through unchanged, no melee charge."""
    inp = bytes(mu.mem_read(mu.reg_read(ARGS[0]), 0x30))
    pitch, yaw = struct.unpack_from("<f", inp, 8)[0], struct.unpack_from("<f", inp, 0x14)[0]
    mu.mem_write(mu.reg_read(ARGS[1]), struct.pack("<fffHBx", pitch, yaw, 0.0, 0, 0))


FAKES = {"CL_Demo_IsPlaying": lambda mu: tc.STATE["demo"], "AimAssist_UpdateGamePadInput": aim_identity,
         "CG_GetAirburstMarkDistance": tc.airburst, "Dvar_RegisterFloat": register_sens}
OBSERVE = {"CL_Demo_IsPlaying": "i", "CG_GetAirburstMarkDistance": "iqix"}


def hooks(ctx):
    for name, (fn, n) in {"api-ms-win-crt-math-l1-1-0.dll!fmod": (_fmod, 0)}.items():
        ctx.m.on_import(name, (lambda f: lambda mu: f(ctx.m, mu))(fn), n)
    return ctx.hooks(FAKES), ctx.hooks(OBSERVE)


def _fmod(m, mu):
    x, y = (struct.unpack("<d", struct.pack("<Q", mu.reg_read(r) & ((1 << 64) - 1)))[0]
            for r in (X.UC_X86_REG_XMM0, X.UC_X86_REG_XMM1))
    mu.reg_write(X.UC_X86_REG_XMM0, struct.unpack("<Q", struct.pack("<d", math.fmod(x, y)))[0])


def dvars(sens=5.0, yaw=0.022, pitch=0.022, accel=0.0, filt=False, forward=0.25, side=0.25, freelook=True):
    return {"m_filter": filt, "cl_mouseAccel": accel, "sensitivity": sens, "m_pitch": pitch, "m_yaw": yaw,
            "m_forward": forward, "m_side": side, "cl_freelook": freelook}


def setup(values, dx=0, dy=0, strafe=0, mlook=1, fov=1.0, caps=(0.0, 0.0), angles=(0.0, 0.0, 0.0), frame=16,
          mark=None, extra=None):
    base = tc.mouse_move_setup(frame, values, strafe, mlook, 0, 0, mark, (dx, 0), (dy, 0), fov, caps, angles, 16)

    def run(w):
        base(w)
        if extra:
            extra(w)
    return run


def angles_after(writes, before):
    out = []
    for va, b in ((PITCH, before[0]), (YAW, before[1])):
        raw = b"".join(writes.get(va + i, struct.pack("<f", b)[i:i + 1]) for i in range(4))
        out.append(struct.unpack("<f", raw)[0])
    return out


def exe_writes(writes):
    """Writes into the exe's image, the view angles aside (the DLL's own statics differ by design)."""
    return {a: v for a, v in writes.items() if a < 0x180000000 and a not in ANGLE_BYTES}


def wrapped(yaw):
    if yaw >= 180.0 or yaw < -180.0:
        yaw = math.fmod(yaw, 360.0)
        if yaw >= 180.0:
            yaw -= 360.0
        elif yaw < -180.0:
            yaw += 360.0
    return yaw


def check_mouse_move_is_the_original_at_equivalent_settings(ctx):
    fk, ob = hooks(ctx)
    rng = random.Random(77)
    for i in range(400):
        sens = F32(rng.choice([1.5, 0.75, 4.0, rng.uniform(0.05, 10)]))
        SENS["value"] = sens
        pitch_sign = rng.choice((1.0, -1.0, 0.0))
        dx, dy = rng.choice((0, 1, -1, 7, -40, 300, -2500)), rng.choice((0, 1, -3, 25, -900))
        caps = (rng.choice((0.0, 0.0, 90.0, 400.0)), rng.choice((0.0, 0.0, 120.0, 360.0)))
        angles = (rng.uniform(-85, 85), rng.uniform(-179, 179), 0.0)
        common = dict(dx=dx, dy=dy, strafe=rng.randrange(2), mlook=rng.randrange(2), caps=caps, angles=angles,
                      frame=rng.choice((1, 7, 16, 33)), mark=rng.choice((None, 0, 777)))
        side, forward, freelook = rng.choice((0.25, 1.0)), rng.choice((0.25, 1.0)), rng.random() < 0.8
        want = ctx.m.call(ctx.original("CL_MouseMove"), [0, bytes(0x2C), ("f", 0.016)], "void",
                          setup(dvars(F32(sens * F32(0.0066 / 0.022)), 0.022, 0.022 * pitch_sign, 0.0, False, forward,
                                      side, freelook), fov=1.0, **common), fk, ob)
        got = ctx.m.call(ctx.enhanced("CL_MouseMove"), [0, bytes(0x2C), ("f", 0.016)], "void",
                         setup(dvars(rng.choice((5.0, 0.3)), rng.choice((0.022, 0.5)), 0.022 * pitch_sign * 3,
                                     rng.choice((0.0, 2.0)), rng.random() < 0.5, forward, side, freelook),
                               fov=rng.choice((1.0, 0.65)), **common), fk, ob)
        assert not isinstance(got[0], str), f"input {i}: {got[0]}"
        assert got[1] == want[1], f"input {i}: the command differs\n  original {want[1]}\n  enhanced {got[1]}"
        assert got[3] == want[3], f"input {i}: the calls differ\n  original {want[3]}\n  enhanced {got[3]}"
        assert exe_writes(got[2]) == exe_writes(want[2]), f"input {i}: the writes differ"
        for a, b in zip(angles_after(want[2], angles), angles_after(got[2], angles)):
            d = (a - b + 180.0) % 360.0 - 180.0
            assert abs(d) <= 1e-3 + 1e-5 * abs(a), f"input {i}: angle {b} where the original gives {a}"


def check_one_count_turns_m_sens_times_0_0066_degrees(ctx):
    fk, ob = hooks(ctx)
    rng = random.Random(3200)
    for i in range(300):
        sens = F32(rng.choice((1.5, 1.0, 0.01, 100.0, rng.uniform(0.01, 100.0))))
        SENS["value"] = sens
        dx, dy = rng.choice((1, -1, 2, 160, -8000, 123457)), rng.choice((0, 1, -1, 3, 999))
        pitch = rng.choice((0.022, -0.022, 0.0, 5.0, -1.0))
        angles = (rng.uniform(-80, 80), rng.choice((0.0, 179.999, -180.0, rng.uniform(-180, 180))), 0.0)
        got = ctx.m.call(ctx.enhanced("CL_MouseMove"), [0, bytes(0x2C), ("f", 0.016)], "void",
                         setup(dvars(rng.choice((5.0, 0.4)), rng.choice((0.022, 0.1)), pitch), dx=dx, dy=dy,
                               fov=rng.choice((1.0, 0.65, 0.3)), angles=angles), fk, ob)
        per = float(sens) * 0.0066
        want_yaw = F32(wrapped(F32(angles[1]) - float(dx) * per))
        sign = 1.0 if pitch > 0 else -1.0 if pitch < 0 else 0.0
        want_pitch = F32(F32(angles[0]) + float(dy) * per * sign)
        got_pitch, got_yaw = angles_after(got[2], angles)
        assert got_yaw == want_yaw, f"input {i}: yaw {got_yaw!r}, want {want_yaw!r} ({dx} counts at m_sens {sens})"
        assert got_pitch == want_pitch, f"input {i}: pitch {got_pitch!r}, want {want_pitch!r}"
        assert -180.0 <= got_yaw < 180.0, f"input {i}: yaw {got_yaw} outside [-180, 180)"


def check_two_frames_end_where_one_does(ctx):
    """The second frame starts from the double the first left (DLL statics set as the first frame wrote them)."""
    fk, ob = hooks(ctx)
    rng = random.Random(8000)
    view, written = dll_static("s_viewAngles"), dll_static("s_viewAnglesWritten")
    for i in range(200):
        SENS["value"] = sens = F32(rng.uniform(0.2, 5.0))
        a, b = rng.randrange(-50, 50), rng.randrange(-50, 50)
        y0 = F32(rng.uniform(-170, 170))
        one = ctx.m.call(ctx.enhanced("CL_MouseMove"), [0, bytes(0x2C), ("f", 0.016)], "void",
                         setup(dvars(), dx=a + b, angles=(0.0, y0, 0.0)), fk, ob)
        mid = float(y0) - a * (float(sens) * 0.0066)

        def after_first(w, mid=mid):
            w.write(view, struct.pack("<dd", 0.0, mid))
            w.write(written, struct.pack("<ff", 0.0, F32(mid)))
        two = ctx.m.call(ctx.enhanced("CL_MouseMove"), [0, bytes(0x2C), ("f", 0.016)], "void",
                         setup(dvars(), dx=b, angles=(0.0, F32(mid), 0.0), extra=after_first), fk, ob)
        y1 = angles_after(one[2], (0.0, y0))[1]
        y2 = angles_after(two[2], (0.0, F32(mid)))[1]
        assert y1 == y2, f"input {i}: {a}+{b} counts end at {y2!r} over two frames, {y1!r} in one"


def check_accel_and_filter_change_nothing(ctx):
    fk, ob = hooks(ctx)
    rng = random.Random(5)
    for i in range(150):
        SENS["value"] = F32(rng.uniform(0.1, 8))
        common = dict(dx=rng.randrange(-500, 500), dy=rng.randrange(-500, 500), strafe=rng.randrange(2),
                      mlook=rng.randrange(2), angles=(rng.uniform(-80, 80), rng.uniform(-179, 179), 0.0))
        plain = ctx.m.call(ctx.enhanced("CL_MouseMove"), [0, bytes(0x2C), ("f", 0.016)], "void",
                           setup(dvars(accel=0.0, filt=False), **common), fk, ob)
        set_ = ctx.m.call(ctx.enhanced("CL_MouseMove"), [0, bytes(0x2C), ("f", 0.016)], "void",
                          setup(dvars(accel=rng.uniform(0.5, 5), filt=True), **common), fk, ob)
        assert plain[1:] == set_[1:], f"input {i}: cl_mouseAccel or m_filter changed the result"


def check_m_sens_is_registered_once(ctx):
    fk, _ = hooks(ctx)
    ob = ctx.hooks({"Dvar_RegisterFloat": "sfffis"})
    m_sens = dll_static("?m_sens@@3PEBUdvar_t@@EB")
    got = ctx.m.call(ctx.enhanced("CL_MouseMove"), [0, bytes(0x2C), ("f", 0.016)], "void",
                     setup(dvars(), dx=5), fk, ob)
    regs = [c[1:] for c in got[3]]
    assert len(regs) == 1, f"registered {len(regs)} times"
    name, value, lo, hi, flags, desc = regs[0]
    name = name.decode() if isinstance(name, bytes) else name
    as_f = lambda bits: struct.unpack("<f", struct.pack("<I", bits & 0xFFFFFFFF))[0]
    assert (name, as_f(value), as_f(lo), as_f(hi), flags & 0xFFFF) == ("m_sens", 1.5, F32(0.01), 100.0, 1), regs[0]
    # registered already: not again
    again = ctx.m.call(ctx.enhanced("CL_MouseMove"), [0, bytes(0x2C), ("f", 0.016)], "void",
                       setup(dvars(), dx=5, extra=lambda w: w.ptr(m_sens, register_sens(ctx.m.mu))), fk, ob)
    assert not again[3], f"registered again: {again[3]}"


def check_remote_control_movement_has_no_accel_or_filter(ctx):
    rng = random.Random(9)
    for i in range(200):
        cl = tc.client((rng.randrange(-900, 900), rng.randrange(-900, 900)),
                       (rng.randrange(-900, 900), rng.randrange(-900, 900)), rng.randrange(2),
                       rng.choice((1.0, 0.65)))
        frame, sens = rng.choice((1, 7, 16)), rng.choice((1.0, 5.0, 0.4))
        want = ctx.m.call(ctx.original("CL_GetMouseMovementWithSensitivity"), [cl, b"\xcc" * 4, b"\xcc" * 4], "void",
                          tc.setup(frame, False, 0.0, sens))
        got = ctx.m.call(ctx.enhanced("CL_GetMouseMovementWithSensitivity"), [cl, b"\xcc" * 4, b"\xcc" * 4], "void",
                         tc.setup(frame, True, rng.uniform(0.5, 4), sens))
        assert got[1:3] == want[1:3], f"input {i}: differs from the original without cl_mouseAccel and m_filter"


# -- IN_Frame ---------------------------------------------------------------------------------------------------

RAW = {}


def take_raw(mu):
    mu.mem_write(mu.reg_read(ARGS[0]), struct.pack("<i", RAW["dx"]))
    mu.mem_write(mu.reg_read(ARGS[1]), struct.pack("<i", RAW["dy"]))


def frame_hooks(ctx, raw_ok=True):
    ctx.imports(tw.DESKTOP.models())
    fakes = {"Dvar_GetBool": lambda mu: tw.STATE["click"], "CL_MouseEvent": lambda mu: tw.STATE["recenter"],
             "sub_1400C7DA0": lambda mu: None, "IN_StartRawInput": lambda mu: int(raw_ok),
             "IN_TakeRawMouse": take_raw, "Com_Error": lambda mu: ctx.m.end_call()}
    observe = {"Dvar_GetBool": "s", "CL_MouseEvent": "iiii", "sub_1400C7DA0": "i"}
    return ctx.hooks(fakes), ctx.hooks(observe)


def by_name(ctx, calls, spec):
    """Observed calls with the function as a name: the enhanced DLL reaches an exe function through its exe-first
    thunk, where the hook fires, not at the exe's address."""
    names = {}
    for name in spec:
        for va in ctx.hooks({name: None}):
            names[va] = name
    return [(names.get(c[0], c[0]),) + tuple(c[1:]) for c in calls]


OBSERVED_IN_FRAME = ("Dvar_GetBool", "CL_MouseEvent", "sub_1400C7DA0")


def check_in_frame_is_the_original_where_raw_equals_cursor(ctx):
    fk, ob = frame_hooks(ctx)
    for i, inp in enumerate(tw.frame_inputs()):
        # the counts the original would pass: the cursor's movement (when it's read at all)
        setup_fn = inp["setup"]
        cell = {}

        def recording(w, f=setup_fn):
            f(w)
            cell["cursor"], cell["old"] = list(tw.DESKTOP.cursor), struct.unpack(
                "<ii", bytes(ctx.m.mu.mem_read(tw.S_WMV + 4, 8)))
        want = ctx.m.call(ctx.original("IN_Frame"), [], "void", recording, fk, ob)
        RAW["dx"] = (cell["cursor"][0] - cell["old"][0] + 2**31) % 2**32 - 2**31
        RAW["dy"] = (cell["cursor"][1] - cell["old"][1] + 2**31) % 2**32 - 2**31
        got = ctx.m.call(ctx.enhanced("IN_Frame"), [], "void", setup_fn, fk, ob)
        assert not isinstance(got[0], str), f"input {i}: {got[0]}"
        assert got[2] == want[2], f"input {i}: the writes differ"
        w, g = by_name(ctx, want[3], OBSERVED_IN_FRAME), by_name(ctx, got[3], OBSERVED_IN_FRAME)
        assert g == w, f"input {i}: the calls differ\n  original {w}\n  enhanced {g}"


def check_in_frame_turns_by_the_raw_counts(ctx):
    fk, ob = frame_hooks(ctx)
    rng = random.Random(4)
    seen = 0
    for i, inp in enumerate(tw.frame_inputs()[:200]):
        RAW["dx"], RAW["dy"] = rng.randrange(-5000, 5000), rng.randrange(-5000, 5000)
        got = ctx.m.call(ctx.enhanced("IN_Frame"), [], "void", inp["setup"], fk, ob)
        events = [c for c in got[3] if len(c) == 5 and not isinstance(c[0], str)]
        for c in events:
            assert (c[3], c[4]) == (RAW["dx"] & 0xFFFFFFFF, RAW["dy"] & 0xFFFFFFFF), f"input {i}: CL_MouseEvent {c}"
            seen += 1
    assert seen > 50, f"only {seen} mouse events"


def check_failed_raw_input_stops_the_game(ctx):
    fk, ob = frame_hooks(ctx, raw_ok=False)
    ob.update(ctx.hooks({"Com_Error": "is"}))
    got = ctx.m.call(ctx.enhanced("IN_Frame"), [], "void", tw.frame_inputs()[0]["setup"], fk, ob)
    errors = [c for c in got[3] if len(c) == 3 and isinstance(c[2], (bytes, str))]
    assert errors and errors[0][1] == 0, f"no fatal error: {got[3]}"
    text = errors[0][2].decode() if isinstance(errors[0][2], bytes) else errors[0][2]
    assert "Raw mouse input could not be registered" in text, text


# -- the raw counters --------------------------------------------------------------------------------------------

def rawmouse(flags, x, y):
    return struct.pack("<HHIIiiI", flags, 0, 0, 0, x, y, 0)


def check_raw_counters(ctx):
    rx, ry = dll_static("s_rawDx"), dll_static("s_rawDy")
    add, take = ctx.enhanced("IN_RawInputAdd"), ctx.enhanced("IN_TakeRawMouse")
    for flags, x, y, start, want in ((0, 5, -3, (0, 0), (5, -3)), (1, 5, -3, (7, 7), (7, 7)),
                                     (0, -2**31, 2**31 - 1, (2**40, -2**40), (2**40 - 2**31, -2**40 + 2**31 - 1))):
        got = ctx.m.call(add, [rawmouse(flags, x, y)], "void",
                         lambda w, s=start: (w.write(rx, struct.pack("<q", s[0])), w.write(ry, struct.pack("<q", s[1]))))
        after = tuple(struct.unpack("<q", b"".join(got[2].get(a + i, struct.pack("<q", s)[i:i + 1])
                                                   for i in range(8)))[0] for a, s in ((rx, start[0]), (ry, start[1])))
        assert after == want, f"IN_RawInputAdd({flags}, {x}, {y}) from {start}: {after}, want {want}"
    for start, want in (((12, -7), (12, -7)), ((2**40, -2**40), (0x7FFFFFFF, -0x7FFFFFFF)), ((0, 0), (0, 0))):
        got = ctx.m.call(take, [b"\xcc" * 4, b"\xcc" * 4], "void",
                         lambda w, s=start: (w.write(rx, struct.pack("<q", s[0])), w.write(ry, struct.pack("<q", s[1]))))
        assert tuple(struct.unpack("<i", b)[0] for b in got[1]) == want, f"IN_TakeRawMouse from {start}: {got[1]}"
        # the harness reports changed bytes only: a byte not reported kept its value from the setup
        after = [struct.unpack("<q", b"".join(got[2].get(a + i, struct.pack("<q", s)[i:i + 1]) for i in range(8)))[0]
                 for a, s in ((rx, start[0]), (ry, start[1]))]
        assert after == [0, 0], f"IN_TakeRawMouse from {start} left the counters at {after}"


def check_raw_reports_only_while_in_front(ctx):
    rx = dll_static("s_rawDx")
    ctx.imports(tw.DESKTOP.models())
    raw = struct.pack("<IIQQ", 0, 48, 0, 0) + rawmouse(0, 9, 4)  # RAWINPUTHEADER (RIM_TYPEMOUSE 0) + RAWMOUSE

    def get_raw_input_data(m, mu):
        size_ptr = mu.reg_read(X.UC_X86_REG_R9)
        dst = mu.reg_read(X.UC_X86_REG_R8)
        m._emulated_write(dst, raw)
        m._emulated_write(size_ptr, struct.pack("<I", len(raw)))
        mu.reg_write(X.UC_X86_REG_RAX, len(raw))
    ctx.imports({"user32.dll!GetRawInputData": (get_raw_input_data, 0),
                 "user32.dll!DefWindowProcA": (lambda m, mu: mu.reg_write(X.UC_X86_REG_RAX, 0), 0)})
    for foreground, want in ((tw.HWND, 9), (0x99, 0)):
        def s(w, fg=foreground):
            tw.DESKTOP.reset(foreground=fg)
            w.ptr(tw.G_WV + 0x10, tw.HWND)
        got = ctx.m.call(ctx.enhanced("IN_RawInputWndProc"), [0x55, 0x00FF, 0, 0x1234], "int64", s)
        dx = struct.unpack("<q", b"".join(got[2].get(rx + i, b"\0") for i in range(8)))[0]
        assert dx == want, f"foreground {foreground:#x}: s_rawDx {dx}, want {want}"


# -- in the game: the loader's jumps --------------------------------------------------------------------------------

def check_loader_jumps_reach_the_replacements(ctx):
    """Apply the redirect table to the emulated exe as runtime/loader does, then call the exe's own addresses: they
    behave as the replacements."""
    mu, dll = ctx.m.mu, ctx.recon
    table = dll.exports["iw5_redirects"]
    count = struct.unpack("<Q", bytes(mu.mem_read(table, 8)))[0]
    exe_base = 0x140000000
    targets = {}
    for i in range(count):
        rva, target = struct.unpack("<QQ", bytes(mu.mem_read(table + 8 + 32 * i, 16)))
        head = bytes(mu.mem_read(table + 8 + 32 * i + 16, 16))
        assert bytes(mu.mem_read(exe_base + rva, 16)) == head, f"redirect {i}: the exe doesn't start as recorded"
        targets[exe_base + rva] = target
    assert {ctx.original(n) for n in ("IN_Frame", "CL_MouseMove", "CL_GetMouseMovementWithSensitivity")} <= set(targets)
    fk, ob = hooks(ctx)
    SENS["value"] = 1.5
    inp = setup(dvars(), dx=100, angles=(0.0, 10.0, 0.0))
    want = ctx.m.call(ctx.enhanced("CL_MouseMove"), [0, bytes(0x2C), ("f", 0.016)], "void", inp, fk, ob)
    saved = {va: bytes(mu.mem_read(va, 14)) for va in targets}
    try:
        for va, target in targets.items():
            ctx.m.patch_code(va, b"\xff\x25\x00\x00\x00\x00" + struct.pack("<Q", target))
        # the exe's address is hooked for the original's fakes only when a test names it; here it runs the jump
        got = ctx.m.call(ctx.original("CL_MouseMove"), [0, bytes(0x2C), ("f", 0.016)], "void", inp, fk, ob)
    finally:
        for va, b in saved.items():
            ctx.m.patch_code(va, b)
    assert got[0] == want[0] and got[1] == want[1], f"buffers: {want[:2]} / {got[:2]}"
    assert got[3] == want[3], f"calls: {want[3]} / {got[3]}"
    diff = sorted(a for a in set(got[2]) | set(want[2]) if got[2].get(a) != want[2].get(a))
    assert not diff, "writes differ at " + ", ".join(f"{a:X}: {want[2].get(a)} / {got[2].get(a)}" for a in diff[:8])


CHECKS = [check_mouse_move_is_the_original_at_equivalent_settings, check_one_count_turns_m_sens_times_0_0066_degrees,
          check_two_frames_end_where_one_does, check_accel_and_filter_change_nothing, check_m_sens_is_registered_once,
          check_remote_control_movement_has_no_accel_or_filter, check_in_frame_is_the_original_where_raw_equals_cursor,
          check_in_frame_turns_by_the_raw_counts, check_failed_raw_input_stops_the_game, check_raw_counters,
          check_raw_reports_only_while_in_front, check_loader_jumps_reach_the_replacements]
