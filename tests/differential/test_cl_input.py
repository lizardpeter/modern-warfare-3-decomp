"""client_mp/cl_input.cpp: the mouse movement (src/client_mp/cl_input.cpp), against the Store build.

The dvars are built per input: a dvar_t (Store layout, current value at +0x10) in scratch memory, its address written
into the exe's dvar pointer (records/names.tsv), as the game's registration would.
"""
import random
import struct

rng = random.Random(3200)

FRAME_MSEC = 0x140E1EB70
DVARS = {"m_filter": 0x1411D0B38, "cl_mouseAccel": 0x1411D0B08, "sensitivity": 0x1411D0AF8}
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


CASES = [
    {"name": "CL_GetMouseMovementWithSensitivity", "ret": "void", "inputs": movement_inputs()},
]
