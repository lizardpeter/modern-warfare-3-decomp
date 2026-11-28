"""Task manager Store functions, compared to original x64 code.

Only the external cancellation API, fatal handler and deleting destructor are
modeled; slot reference release and embedded record reset execute real code.
"""
import struct
from unicorn import x86_const as X
from harness import OBJECTS

REGISTRY = 0x144B09760
SLOT = 0xe0
GROUP_SIZE = 8 + SLOT * 32
VTABLE = OBJECTS + 0x100
REFS = OBJECTS + 0x1000
# A mapped original entry used exclusively as the fake virtual destructor target.
DTOR = 0x14011A650


def setup_objects(w, counts):
    w.ptr(VTABLE, DTOR)
    for i, count in enumerate(counts):
        if count is not None:
            w.write(REFS + i*0x20, struct.pack("<QI", VTABLE, count))


def slot_data(state, flag, category, selector, ref=None):
    data = bytearray([0x5a] * SLOT)
    struct.pack_into("<i", data, 0, state)
    struct.pack_into("<Q", data, 8, ref or 0)
    struct.pack_into("<I", data, 0xb8, category)
    data[0xbc] = flag
    data[0xbd] = selector
    return data


def group_data(states, occupied, refs=False):
    data = bytearray([0x5a] * 8)
    counts = [None, 1, 2, 0, 0xffffffff, 0x80000000, 3, None] * 4
    for i, state in enumerate(states):
        data.extend(slot_data(state, 0x81 if i in occupied else 0,
                              i % 4, (i*11) & 0xff,
                              REFS+i*0x20 if refs and counts[i] is not None else None))
    return bytes(data), counts


def fixture(states, occupied, count, refs):
    group, counts = group_data(states, {i for i in range(32) if i%3}, refs)
    def setup(w):
        registry = bytearray([0xa7] * 0x208)
        struct.pack_into("<I", registry, 0, count)
        for i in range(32):
            struct.pack_into("<Q", registry, 8+8*i, 0x12340000+i*8 if i in occupied else 0)
            struct.pack_into("<Q", registry, 0x108+8*i, 0x23450000+i*8)
        w.write(REGISTRY, registry)
        setup_objects(w, counts)
    return {"args": [group, b"task fixture\0"], "setup": setup}


def no_op(mu):
    pass


def destructor(mu):
    assert mu.reg_read(X.UC_X86_REG_RDX) & 0xffffffff == 1
    obj = mu.reg_read(X.UC_X86_REG_RCX)
    assert struct.unpack("<I", mu.mem_read(obj+8, 4))[0] == 0


def ref_case(count):
    data = slot_data(1, 1, 0, 0, REFS if count is not None else None)
    return {"args": [bytes(data)], "setup": lambda w: setup_objects(w, [count])}


def reset_slot_case(state, count):
    data = slot_data(state, 1, 3, 7, REFS if count is not None else None)
    return {"args": [bytes(data)], "setup": lambda w: setup_objects(w, [count])}


def reset_group_case(states, refs):
    group, counts = group_data(states, set(range(32)), refs)
    return {"args": [group], "setup": lambda w: setup_objects(w, counts)}


STATES = ([0]*32, [2]*32, [1]*32, [i%4 for i in range(32)])
OCCUPIED = (set(), {0}, {31}, {0, 3, 19}, set(range(31)), set(range(32)))
COUNTS = (None, 0, 1, 2, 3, 0x80000000, 0xffffffff)
RESET_FAKES = {"XAsyncCancel": no_op, DTOR: destructor}
RESET_OBSERVE = {"XAsyncCancel": "q", DTOR: "qi"}
GROUPS = [group_data([i%4 for i in range(32)], occupied)[0] for occupied in OCCUPIED]


CASES = [
    {"name": "TaskGroup_Init", "ret": "void",
     "fakes": dict(RESET_FAKES, Com_Error=no_op),
     "observe": dict(RESET_OBSERVE, Com_Error="is"),
     "inputs": [fixture(states, occupied, count, refs)
                for states in STATES
                for occupied in (set(), {0, 1, 3}, set(range(31)), set(range(32)))
                for count in (0, 7, 0xffffffff) for refs in (False, True)]},
    {"name": "sub_14011A600", "ret": "void",
     "fakes": {DTOR: destructor}, "observe": {DTOR: "qi"},
     "inputs": [ref_case(count) for count in COUNTS]},
    {"name": "sub_14020CC10", "ret": "void",
     "inputs": [[bytes([value])*0x6c] for value in (0, 1, 0x5a, 0xff)]},
    {"name": "sub_14027E4A0", "ret": "bool", "inputs": [[g] for g in GROUPS]},
    {"name": "sub_14027E720", "ret": "void", "fakes": RESET_FAKES, "observe": RESET_OBSERVE,
     "inputs": [reset_group_case(states, refs) for states in STATES for refs in (False, True)]},
    {"name": "sub_14027E860", "ret": "void", "fakes": RESET_FAKES, "observe": RESET_OBSERVE,
     "inputs": [reset_slot_case(state, count) for state in (0, 1, 2, 3) for count in COUNTS]},
    {"name": "sub_14027E9B0", "ret": "int",
     "inputs": [[g, category] for g in GROUPS for category in (0, 1, 2, 3, 4, 0xffffffff)]},
    {"name": "sub_14027EB30", "ret": "uint64",
     "inputs": [[g, selector, category, state] for g in GROUPS
                for selector in (0, 255, 256, 0xffffffff)
                for category in (0, 3, 0xffffffff) for state in (0, 1, 2, 3)]},
    {"name": "sub_14027EBE0", "ret": "uint64",
     "inputs": [[bytes([value])*SLOT] for value in (0, 1, 0x5a, 0xff)]},
    {"name": "sub_14027EBF0", "ret": "void",
     "inputs": [[bytes([fill])*SLOT, value] for fill in (0, 0x5a)
                for value in (0, 1, 0x123456789abcdef0, 0xffffffffffffffff)]},
    {"name": "sub_14027EC00", "ret": "bool",
     "inputs": [[g, category] for g in GROUPS for category in (0, 1, 2, 3, 4, 0xffffffff)]},
    {"name": "sub_14027EC30", "ret": "bool",
     "inputs": [[g, category, selector] for g in GROUPS
                for category in (0, 1, 2, 3, 4, 0xffffffff)
                for selector in (0, 11, 33, 209, 85, 255, 256, 0xffffffff)]},
]
