"""universal/com_math.cpp: angles from vectors, angle arithmetic, small vector and axis helpers
(src/universal/com_math.cpp), against the Store build. Floats compare bit for bit: results, and every buffer.

The inputs aim at where float code differs: signed zeros, denormals, infinities, NaNs (quiet, with payload, negative),
values on and just off multiples of 180 and 360, huge magnitudes where the 1/360 scaling loses everything, and random
values across exponents.
"""
import math
import random
import struct

rng = random.Random(1170)


def f32(x):
    return struct.unpack("<f", struct.pack("<f", x))[0]


def from_bits(b):
    return struct.unpack("<f", struct.pack("<I", b))[0]


NAN, NEG_NAN, NAN_PAYLOAD = from_bits(0x7FC00000), from_bits(0xFFC00000), from_bits(0x7FC12345)
SPECIAL = [0.0, -0.0, 1.0, -1.0, 0.5, -0.5, 1e-45, -1e-45, 1.17549435e-38, 3.4028235e38, -3.4028235e38,
           math.inf, -math.inf, NAN, NEG_NAN, NAN_PAYLOAD, 1e-7, -1e-7, 1e-30]
ANGLES = [k * 90.0 + d for k in range(-9, 10) for d in (0.0, 1e-5, -1e-5, 0.4999, 45.0)] + \
         [359.99997, 360.0, 360.00003, -359.99997, -1e-6, -1e-8, 179.99998, 180.0, -180.0, 1e7, -1e7, 16777216.0,
          1e20, -1e20]


def rand_float():
    kind = rng.randrange(4)
    if kind == 0:
        return f32(rng.uniform(-720.0, 720.0))
    if kind == 1:
        return f32(rng.uniform(-1.0, 1.0))
    if kind == 2:
        return f32(math.copysign(10.0 ** rng.uniform(-40, 38), rng.choice((-1, 1))))
    return rng.choice(SPECIAL)


SCALARS = SPECIAL + ANGLES + [rand_float() for _ in range(400)]


def vec(*xs):
    return struct.pack(f"<{len(xs)}f", *xs)


def vectors(n, count=500):
    out = [vec(*([0.0] * n)), vec(*([-0.0] * n))]
    for a in SPECIAL[:13]:
        for b in (0.0, -0.0, 1.0, -1.0, a):
            out.append(vec(*([a, b, -a][:n])))
            out.append(vec(*([0.0, 0.0, a][:n])))
            out.append(vec(*([-0.0, -0.0, b][:n])))
    for x, y in ((1, 0), (0, 1), (-1, 0), (0, -1), (1, 1), (-1, -1), (-1, 1e-30), (-1, -1e-30), (3, 4)):
        out.append(vec(*([x, y, 0.5][:n])))
    out += [vec(*[rand_float() for _ in range(n)]) for _ in range(count)]
    return out


def floats(values):
    return [[("f", x)] for x in values]


def pairs():
    out = [[("f", a), ("f", b)] for a in SPECIAL for b in (0.0, -0.0, 360.0, -180.0, NAN, 1.0)]
    out += [[("f", a), ("f", f32(a - d))] for a in ANGLES for d in (0.0, 180.0, -180.0, 359.0, 1e-3)]
    return out + [[("f", rand_float()), ("f", rand_float())] for _ in range(500)]


CASES = [
    {"name": "ClampChar", "ret": "char", "inputs": [[v] for v in list(range(-300, 300)) + [-2**31, 2**31 - 1, 1000, -1000]]},
    {"name": "Vec2Normalize", "ret": "float", "inputs": [[v] for v in vectors(2)]},
    {"name": "vectoyaw", "ret": "float", "inputs": [[v] for v in vectors(2)]},
    {"name": "vectosignedyaw", "ret": "float", "inputs": [[v] for v in vectors(2)]},
    {"name": "vectopitch", "ret": "float", "inputs": [[v] for v in vectors(3)]},
    {"name": "vectosignedpitch", "ret": "float", "inputs": [[v] for v in vectors(3)]},
    {"name": "vectoangles", "ret": "void", "inputs": [[v, b"\xcc" * 12] for v in vectors(3)]},
    {"name": "vectosignedangles", "ret": "void", "inputs": [[v, b"\xcc" * 12] for v in vectors(3)]},
    {"name": "AnglesSubtract", "ret": "void",
     "inputs": [[a, b, b"\xcc" * 12] for a, b in zip(vectors(3, 300), reversed(vectors(3, 300)))]},
    {"name": "AngleNormalize360", "ret": "float", "inputs": floats(SCALARS)},
    {"name": "AngleDelta", "ret": "float", "inputs": pairs()},
    {"name": "AddPointToBounds2D", "ret": "void",
     "inputs": [[p, vec(*lo), vec(*hi)] for p in vectors(2, 150)
                for lo, hi in (((0.0, 0.0), (0.0, 0.0)), ((-1.0, 5.0), (1.0, 6.0)), ((NAN, -0.0), (NAN, 0.0)),
                               ((math.inf, math.inf), (-math.inf, -math.inf)))]},
    {"name": "AxisClear", "ret": "void", "inputs": [[b"\xcc" * 36], [bytes(36)]]},
    {"name": "AxisCopy", "ret": "void",
     "inputs": [[vec(*[rand_float() for _ in range(9)]), b"\xcc" * 36] for _ in range(50)] +
               [[struct.pack("<9I", *[0x7F800001, 0xFFBFFFFF, 0x80000000, 1, 0x7FC12345, 0, 0, 0, 0]),
                 b"\xcc" * 36]]},
]
