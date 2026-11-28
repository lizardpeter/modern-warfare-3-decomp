"""Store TaskGroup_Init: compare bytes, original registry writes and call order.

The two opaque callees are deliberately modeled, not reconstructed. This is a
leaf-level contract comparison, not a claim of end-to-end task behavior.
"""
import struct
from unicorn import x86_const as X

REGISTRY = 0x144B09760
SLOT = 0xe0
GROUP_SIZE = 8 + SLOT * 32
STATE = {}


def fixture(states, occupied, count, change_state):
    def setup(w):
        registry = bytearray([0xa7] * 0x208)
        struct.pack_into("<I", registry, 0, count)
        for i in range(32):
            struct.pack_into("<Q", registry, 8 + 8*i, 0x12340000 + i*8 if i in occupied else 0)
            struct.pack_into("<Q", registry, 0x108 + 8*i, 0x23450000 + i*8)
        w.write(REGISTRY, registry)
        STATE["change"] = change_state
    group = bytearray([0x5a] * GROUP_SIZE)
    for i, value in enumerate(states):
        struct.pack_into("<i", group, 8 + i*SLOT, value)
    return {"args": [bytes(group), b"task fixture\0"], "setup": setup}


def first_helper(mu):
    if STATE["change"]:
        ptr = mu.reg_read(X.UC_X86_REG_RCX)
        mu.mem_write(ptr, struct.pack("<i", 2))


def no_op(mu):
    pass


CASES = [{
    "name": "TaskGroup_Init",
    "ret": "void",
    "builds": ["iw5mp_ship"],
    "fakes": {"sub_14011A600": first_helper, "XAsyncCancel": no_op,
              "sub_14020CC10": no_op, "Com_Error": no_op},
    "observe": {"sub_14011A600": "q", "XAsyncCancel": "q",
                "sub_14020CC10": "q", "Com_Error": "is"},
    "inputs": [fixture(states, occupied, count, change)
               for states in ([0]*32, [2]*32, [i%4 for i in range(32)])
               for occupied in (set(), {0, 1, 3}, set(range(31)), set(range(32)))
               for count in (0, 7, 0xffffffff)
               for change in (False, True)]
}]
