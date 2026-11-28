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

LINKS = 0x14489c380
RECORD = OBJECTS + 0x8000
OTHER_RECORD = OBJECTS + 0x8100
SUCCESS_CB = 0x14011A680
FAILURE_CB = 0x14011A510
STATUS_VALUES = (0, 1, 0x80004005, 0x8000000a, 0x80004002, 0x7fffffff)


def link_data(w, record, other):
    # 24 entries; include duplicate matching links, inactive links, wrong
    # records, valid boundary states and out-of-range/underflow states.
    table = bytearray([0x5a] * (24 * 0x28))
    for i in range(24):
        table[i*0x28] = i % 3 != 0
        struct.pack_into("<I", table, i*0x28+4, (0, 1, 6, 7, 0xffffffff, 3)[i%6])
        struct.pack_into("<Q", table, i*0x28+0x10, other if i%4 == 0 else record)
    w.write(LINKS, table)


def link_case():
    def setup(w):
        w.write(RECORD, bytes([0x5a])*0x6c)
        w.write(OTHER_RECORD, bytes([0x6b])*0x6c)
        link_data(w, RECORD, OTHER_RECORD)
    return {"args": [RECORD], "setup": setup}


def cancel_case(group, counts, args=(), single=False, slots=False, registry_index=None):
    data = bytearray(group)
    if registry_index is not None:
        struct.pack_into("<I", data, 0, registry_index)
    if slots:
        data = data[8:]
    def setup(w):
        setup_objects(w, counts)
        first_slot = w.m.heap if single or slots else w.m.heap+8
        link_data(w, first_slot+0x50, first_slot+SLOT+0x50)
        if registry_index is not None:
            registry = bytearray([0x5a] * 0x208)
            struct.pack_into("<I", registry, 0, 12)
            struct.pack_into("<Q", registry, 8+(registry_index-1)*8, w.m.heap)
            w.write(REGISTRY, registry)
    return {"args": [bytes(data), *args], "setup": setup}


def cancel_groups():
    for states in STATES:
        for occupied in (set(), {0, 3, 19}, set(range(32))):
            for refs in (False, True):
                yield group_data(states, occupied, refs)


def poll_case(states, occupied, refs, callbacks):
    group, counts = group_data(states, occupied, refs)
    data = bytearray(group)
    for i in range(32):
        at = 8+i*SLOT
        data[at+0x10] = i % len(STATUS_VALUES)
        data[at+0x48] = (i*13)&0xff
        struct.pack_into("<QQ", data, at+0xd0, SUCCESS_CB if callbacks and i%3 != 0 else 0,
                         FAILURE_CB if callbacks and i%4 != 0 else 0)
    return {"args": [bytes(data)], "setup": lambda w: setup_objects(w, counts)}


def async_status(mu):
    ptr = mu.reg_read(X.UC_X86_REG_RCX)
    return STATUS_VALUES[mu.mem_read(ptr, 1)[0]]


def on_callback(mu, expected):
    slot = mu.reg_read(X.UC_X86_REG_RCX)
    assert struct.unpack("<I", mu.mem_read(slot+0xc8, 4))[0] == expected
    # Model callback-driven state transition, which original code rechecks.
    if mu.mem_read(slot+0xbd, 1)[0] % 2 == 0:
        mu.mem_write(slot, struct.pack("<i", 2))


def status_link(state):
    data = bytearray(0x28)
    struct.pack_into("<I", data, 4, state)
    return bytes(data)


CASES += [
    {"name": "sub_1403F1420", "ret": "void",
     "inputs": [[bytes([value])*0x20] for value in (0, 1, 0x5a, 0xff)]},
    {"name": "sub_14020CC00", "ret": "void",
     "inputs": [[bytes([value])*0x6c] for value in (0, 1, 0x5a, 0xff)]},
    {"name": "sub_14020CC20", "ret": "bool",
     "inputs": [[status_link(state)] for state in (0, 1, 2, 3, 4, 5, 6, 7, 0xffffffff, 0x80000000)]},
    {"name": "sub_14020C3D0", "ret": "void", "inputs": [link_case()]},
    {"name": "sub_14027E4D0", "ret": "bool", "fakes": RESET_FAKES, "observe": RESET_OBSERVE,
     "inputs": [cancel_case(slot_data(state, 1, 3, 7, REFS if count is not None else None),
                            [count], single=True)
                for state in (0, 1, 2, 3) for count in COUNTS]},
    {"name": "sub_14027E520", "ret": "bool", "fakes": RESET_FAKES, "observe": RESET_OBSERVE,
     "inputs": [cancel_case(g, counts, (category,))
                for g, counts in cancel_groups() for category in (0, 3, 4)]},
    {"name": "sub_14027E540", "ret": "int", "fakes": RESET_FAKES, "observe": RESET_OBSERVE,
     "inputs": [cancel_case(g, counts, (category, selector))
                for g, counts in cancel_groups()
                for category, selector in ((0, 0), (3, 33), (3, 209), (3, 0xffffffff))]},
    {"name": "sub_14027E630", "ret": "bool", "fakes": RESET_FAKES, "observe": RESET_OBSERVE,
     "inputs": [cancel_case(g, counts, (all_, category))
                for g, counts in cancel_groups() for all_, category in ((0, 0), (0, 3), (1, 0), (255, 4))]},
    {"name": "sub_14027E7B0", "ret": "void", "fakes": RESET_FAKES, "observe": RESET_OBSERVE,
     "inputs": [cancel_case(g, counts, (selector,), slots=True)
                for g, counts in cancel_groups() for selector in (0, 33, 209, 256)]},
    {"name": "sub_14027E9E0", "ret": "void", "fakes": RESET_FAKES, "observe": RESET_OBSERVE,
     "inputs": [cancel_case(g, counts, registry_index=i)
                for g, counts in cancel_groups() for i in (1, 17, 32)]},
    {"name": "sub_14027EAE0", "ret": "uint64",
     "inputs": [[g, selector, category] for g in GROUPS
                for selector in (0, 255, 256) for category in (0, 3, 0xffffffff)]},
    {"name": "sub_14027EC70", "ret": "void",
     "fakes": dict(RESET_FAKES, **{"XAsyncGetStatus": async_status}) |
              {SUCCESS_CB: lambda mu: on_callback(mu, 1), FAILURE_CB: lambda mu: on_callback(mu, 2)},
     "observe": dict(RESET_OBSERVE, XAsyncGetStatus="qb") | {SUCCESS_CB: "q", FAILURE_CB: "q"},
     "inputs": [poll_case(states, occupied, refs, callbacks)
                for states in STATES for occupied in (set(), {0, 3, 19}, set(range(32)))
                for refs in (False, True) for callbacks in (False, True)]},
]
