"""client_mp/cl_input.cpp: the mouse movement (src/client_mp/cl_input.cpp), against the Store build.

The dvars are built per input: a dvar_t (Store layout, current value at +0x10) in scratch memory, its address written
into the exe's dvar pointer (records/names.tsv), as the game's registration would.
"""
import random
import struct

rng = random.Random(3200)

from unicorn import x86_const as X

FRAME_MSEC = 0x140E1EB70
DVARS = {"m_filter": 0x1411D0B38, "cl_mouseAccel": 0x1411D0B08, "sensitivity": 0x1411D0AF8, "m_pitch": 0x1411D0B18,
         "m_yaw": 0x1411D0B20, "m_forward": 0x1411D0B28, "m_side": 0x1411D0B30, "cl_freelook": 0x1411D0B00}
CLIENTS = 0x140E1ED70      # clientActive_t (Store layout, include/client_mp/client_mp.h)
PLAYERS_KB = 0x140E1A690   # kbutton_t[30]
CLS_FRAMETIME = 0x141128D10 + 0x118
STATE = {}
ARGS = (X.UC_X86_REG_RCX, X.UC_X86_REG_RDX, X.UC_X86_REG_R8, X.UC_X86_REG_R9)
CL_SIZE = 0x3454  # clientActive_t up to cgameFOVSensitivityScale (include/client_mp/client_mp.h)


def dvar_bytes(value):
    current = struct.pack("<?3x", value) if isinstance(value, bool) else struct.pack("<f", value)
    return b"\0" * 8 + struct.pack("<IBB2x", 0, 0, 0) + current.ljust(16, b"\0")


def setup(frame_msec, filt, accel, sens):
    def run(w):
        w.u32(FRAME_MSEC, frame_msec)
        for name, value in (("m_filter", filt), ("cl_mouseAccel", accel), ("sensitivity", sens)):
            w.ptr(DVARS[name], w.alloc(dvar_bytes(value)))
    return run


def client(dx, dy, index, fov_scale):
    cl = bytearray(b"\xcd" * CL_SIZE)
    struct.pack_into("<2i2ii", cl, 0x3420, dx[0], dx[1], dy[0], dy[1], index)
    struct.pack_into("<f", cl, 0x3450, fov_scale)
    return bytes(cl)


def movement_inputs():
    out = []
    counts = [0, 1, -1, 3, -7, 40, -125, 400, 1000, -3200, 25000, -2**31, 2**31 - 1]
    for frame_msec in (1, 4, 7, 16, 33, 100):
        for filt in (False, True):
            for accel, sens in ((0.0, 5.0), (0.0, 1.0), (0.5, 3.0), (2.0, 0.25), (-1.0, 10.0)):
                for _ in range(6):
                    dx = (rng.choice(counts), rng.choice(counts))
                    dy = (rng.choice(counts), rng.choice(counts))
                    cl = client(dx, dy, rng.randrange(2), rng.choice((1.0, 0.65, 0.8000001, 1.75, 0.0)))
                    out.append({"args": [cl, b"\xcc" * 4, b"\xcc" * 4], "setup": setup(frame_msec, filt, accel, sens)})
    # 8000 Hz at 3200 DPI: large per-frame counts at short frames, and the 1-count steps of slow aiming
    for dx0 in range(-3, 4):
        out.append({"args": [client((dx0, 0), (0, -dx0), 0, 1.0), b"\xcc" * 4, b"\xcc" * 4],
                    "setup": setup(1, False, 0.0, 1.5)})
    return out


def aim_assist(mu):
    """AimAssist_UpdateGamePadInput's fake: an output that depends on every AimInput field."""
    inp = bytes(mu.mem_read(mu.reg_read(ARGS[0]), 0x30))
    dt, dts, pitch, pa, pm, yaw, ya, ym, fa, ra, buttons, lc = struct.unpack("<10fii", inp)
    out = struct.pack("<fffHB", pitch * 0.5 + dt + pa + pm, yaw - dts + ya + ym + fa + ra, 0.0,
                      (buttons ^ lc ^ 0x5A5A) & 0xFFFF, (buttons + 7) & 0xFF)
    mu.mem_write(mu.reg_read(ARGS[1]), out.ljust(16, b"\0"))  # the caller's local


def airburst(mu):
    """CG_GetAirburstMarkDistance's fake: reports STATE['mark'] (or none) through its pointer."""
    if STATE.get("mark") is None:
        return 0
    mu.mem_write(mu.reg_read(ARGS[3]), struct.pack("<I", STATE["mark"]))
    return 1


def kbutton(active):
    return struct.pack("<iiIIBBxx", 0, 0, 0, 0, active, 0)


def mouse_move_setup(frame_msec, values, strafe, mlook, pm_flags, demo, mark, dx, dy, fov, caps, angles, frametime):
    def run(w):
        STATE.update(demo=demo, mark=mark)
        w.u32(FRAME_MSEC, frame_msec)
        w.u32(CLS_FRAMETIME, frametime)
        for name, value in values.items():
            w.ptr(DVARS[name], w.alloc(dvar_bytes(value)))
        w.write(PLAYERS_KB + 8 * 0x14, kbutton(strafe))
        w.write(PLAYERS_KB + 13 * 0x14, kbutton(mlook))
        w.u32(CLIENTS + 0x14, pm_flags)
        w.write(CLIENTS + 0x3420, struct.pack("<2i2ii", dx[0], dx[1], dy[0], dy[1], 0))
        w.write(CLIENTS + 0x3450, struct.pack("<3f", fov, *caps))
        w.write(CLIENTS + 0x34F4, struct.pack("<3f", *angles))
    return run


def mouse_move_inputs():
    out = []
    counts = [0, 1, -1, 4, -9, 60, -300, 2500, -2**31, 2**31 - 1]
    nan = float("nan")
    for _ in range(900):
        values = {"m_filter": rng.random() < 0.3, "cl_mouseAccel": rng.choice((0.0, 0.0, 0.5)),
                  "sensitivity": rng.choice((1.0, 5.0, 0.33, float("inf"))),
                  "m_pitch": rng.choice((0.022, -0.022, 0.0, float("inf"))),
                  "m_yaw": rng.choice((0.022, 0.044, -float("inf"))), "m_forward": rng.choice((0.25, 1.0)),
                  "m_side": rng.choice((0.25, 1.0)), "cl_freelook": rng.random() < 0.8}
        caps = (rng.choice((0.0, 0.0, 90.0, 400.0, -1.0, nan)), rng.choice((0.0, 0.0, 120.0, 360.0, -5.0, nan)))
        cmd = struct.pack("<ii", rng.randrange(1 << 20), rng.randrange(1 << 31)) + bytes(rng.randrange(256) for _ in range(0x24))
        out.append({"args": [0, cmd, ("f", rng.choice((0.016, 0.001, 0.25, 0.0)))],
                    "setup": mouse_move_setup(rng.choice((0, 1, 7, 16, 50)), values, rng.randrange(2),
                                              rng.randrange(2), rng.choice((0, 0, 0x800, 0x801)), rng.randrange(2),
                                              rng.choice((None, 0, 1234, 0x12345)),
                                              (rng.choice(counts), rng.choice(counts)),
                                              (rng.choice(counts), rng.choice(counts)),
                                              rng.choice((1.0, 0.65, nan)), caps,
                                              (rng.uniform(-89, 89), rng.uniform(-720, 720), 0.0),
                                              rng.choice((0, 16, 33)))})
    return out


CASES = [
    {"name": "CL_GetMouseMovementWithSensitivity", "ret": "void", "inputs": movement_inputs()},
    {"name": "CL_MouseMove", "ret": "void", "inputs": mouse_move_inputs(),
     "fakes": {"CL_Demo_IsPlaying": lambda mu: STATE["demo"], "AimAssist_UpdateGamePadInput": aim_assist,
               "CG_GetAirburstMarkDistance": airburst},
     "observe": {"CL_Demo_IsPlaying": "i", "CG_GetAirburstMarkDistance": "iqi"}},  # the 4th: a stack local
]


# -- CL_MouseEvent (the PC layer) ---------------------------------------------------------------------------------

from win32 import HWND, Desktop  # noqa: E402

DESKTOP = Desktop()
G_WV = 0x148A8D6D0
CLIENT_UI = 0x140E1EB90
BYPASS = 0x140E1A680


def mouse_event_setup(catchers, state, bypass, menu, demo_menu, index, dx0, dy0, foreground):
    # every random choice is made here, once: the setup runs before each side's call and must give both the same
    cursor = (rng.randrange(-50, 2000), rng.randrange(-50, 1200))

    def run(w):
        DESKTOP.reset(foreground=foreground, cursor=cursor)
        STATE.update(menu=menu, demo_menu=demo_menu)
        w.ptr(G_WV + 0x10, HWND)
        w.u32(CLIENT_UI + 0x54, catchers)
        w.u32(CLIENT_UI + 0x5C, state)
        w.ptr(BYPASS, w.alloc(dvar_bytes(bypass)))
        w.write(CLIENTS + 0x3420, struct.pack("<2i2ii", dx0, -dx0, dy0, 3, index))
    return run


def mouse_event_inputs():
    out = []
    for catchers in (0, 0x10, 0x11, 0x01):
        for state in (0, 1, 5, 8, 9, 10, 11, -1):
            for menu, demo_menu in ((0, False), (6, False), (6, True), (2, False)):
                for bypass in (False, True):
                    dx, dy = rng.choice(((0, 0), (5, -3), (-2**31, 2**31 - 1), (1, 0)))
                    out.append({"args": [rng.randrange(-100, 1400), rng.randrange(-100, 800), dx, dy],
                                "setup": mouse_event_setup(catchers, state, bypass, menu, demo_menu, rng.randrange(2),
                                                           rng.randrange(-5, 5), rng.randrange(-5, 5), HWND)})
    return out


CASES += [
    {"name": "CL_MouseEvent", "ret": "bool", "inputs": mouse_event_inputs(), "imports": DESKTOP.models(),
     "fakes": {"sub_1400C42A0": lambda mu: STATE["demo_menu"], "UI_GetActiveMenu": lambda mu: STATE["menu"],
               "UI_MouseEvent": lambda mu: None},
     "observe": {"sub_1400C42A0": "i", "UI_GetActiveMenu": "i", "UI_MouseEvent": "iii"}},
]
