"""win32/win_input.cpp: the PC mouse layer (src/win32/win_input.cpp), against the Store build, on a model desktop
(win32.py): every user32 call and its arguments compared."""
import random
import struct

from test_cl_input import CLIENT_UI, CLIENTS, G_WV, dvar_bytes
from win32 import HWND, Desktop

rng = random.Random(8000)
DESKTOP = Desktop()
STATE = {}
S_WMV = 0x148A80478          # oldButtonState, oldPos (+4), mouseActive (+0xC), mouseInitialized (+0xD)
S_WANT_MOUSE = 0x148A8048C
WINDOW_CENTER = (0x148A80474, 0x148A80488)
IN_MOUSE = 0x148A80460
CLS = 0x141128D10


def frame_setup(click, initialized, want, active, enabled, foreground, state, cursor, old, size, show_count, recenter):
    def run(w):
        DESKTOP.reset(foreground=foreground, cursor=cursor, show_count=show_count,
                      client=(108, 81, size[0], size[1]))
        STATE.update(click=click, recenter=recenter)
        w.ptr(G_WV + 0x10, HWND)
        w.write(S_WMV, struct.pack("<iiiBB", 0, old[0], old[1], active, initialized))
        w.u32(S_WANT_MOUSE, want)
        w.ptr(IN_MOUSE, w.alloc(dvar_bytes(enabled)))
        w.u32(CLIENT_UI + 0x5C, state)
        w.write(CLS + 0x188, struct.pack("<ii", 1920, 1080))
        w.write(CLS + 0x19C, struct.pack("<ii", *size))
        w.write(CLIENTS + 0x3420, bytes(20))
    return run


def frame_inputs():
    out = []
    for _ in range(600):
        size = rng.choice(((1920, 1080), (1280, 720), (2560, 1440), (3440, 1440)))
        old = rng.choice(((740, 410), (0, 0), (-1, 5000)))
        cursor = rng.choice((old, (old[0] + 3, old[1] - 1), (rng.randrange(-200, 4000), rng.randrange(-200, 2200)),
                             (-2**31, 2**31 - 1)))
        out.append({"args": [], "setup": frame_setup(rng.random() < 0.1, rng.random() < 0.9, rng.random() < 0.85,
                                                     rng.randrange(2), rng.random() < 0.85,
                                                     rng.choice((HWND, HWND, HWND, 0x99)),
                                                     rng.choice((0, 1, 8, 9, 10)), cursor, old, size,
                                                     rng.choice((0, -1, 2, -3)), rng.choice((0, 1)))})
    return out


def recenter_setup(window):
    def run(w):
        DESKTOP.reset(window=window)
        w.ptr(G_WV + 0x10, HWND)
    return run


CASES = [
    {"name": "IN_ShowSystemCursor", "ret": "void", "imports": DESKTOP.models(),
     "inputs": [{"args": [show], "setup": (lambda w, c=count: DESKTOP.reset(show_count=c))}
                for show in (0, 1, 2, -1) for count in (-3, -1, 0, 1, 4)]},
    {"name": "IN_RecenterMouse", "ret": "void", "imports": DESKTOP.models(),
     "inputs": [{"args": [], "setup": recenter_setup(r)}
                for r in ((100, 50, 1380, 770), (0, 0, 1920, 1080), (-1921, -5, 1, 1081), (3, 3, 4, 4),
                          (-2**31, 0, 2**31 - 1, 7))]},
    {"name": "IN_Frame", "ret": "void", "inputs": frame_inputs(), "imports": DESKTOP.models(),
     "fakes": {"Dvar_GetBool": lambda mu: STATE["click"], "CL_MouseEvent": lambda mu: STATE["recenter"],
               "sub_1400C7DA0": lambda mu: None},
     "observe": {"Dvar_GetBool": "s", "CL_MouseEvent": "iiii", "sub_1400C7DA0": "i"}},
]
