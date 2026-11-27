"""libHttpClient/HTTP/Curl/CurlProvider.cpp: HrFromCurle and CurlProvider::Initialize (src/libHttpClient/...), against
the Store build. Initialize runs with curl_global_init returning each interesting CURLcode, the allocator hook
answering from a bump allocator (or failing, which throws: the case ends in the throw import), and _Mtx_init_in_situ
observed; the Result buffer, the provider's bytes and every call compare.
"""
import struct

from unicorn import x86_const as X

from harness import SCRATCH

ARGS = (X.UC_X86_REG_RCX, X.UC_X86_REG_RDX)
STATE = {"curl": 0, "next": 0, "fail_at": -1, "count": 0}
ALLOC = SCRATCH + 0x8000


def setup(curl, fail_at=-1):
    def apply(w):
        STATE.update(curl=curl, next=ALLOC, fail_at=fail_at, count=0)
    return apply


def curl_global_init(m, mu):
    m.observed.append(("curl_global_init", mu.reg_read(ARGS[0]) & 0xFFFFFFFF))
    mu.reg_write(X.UC_X86_REG_RAX, STATE["curl"])


def mtx_init(m, mu):
    m.observed.append(("_Mtx_init_in_situ", mu.reg_read(ARGS[0]) - ALLOC, mu.reg_read(ARGS[1]) & 0xFFFFFFFF))


def throw(m, mu):
    m.observed.append(("_CxxThrowException",))
    mu.emu_stop()


def http_alloc(mu):
    size = mu.reg_read(ARGS[0])
    at = 0 if STATE["count"] == STATE["fail_at"] else STATE["next"]
    STATE["count"] += 1
    if at:
        STATE["next"] += (size + 15) & ~15
    mu.reg_write(X.UC_X86_REG_RAX, at)


CODES = [0, 1, 2, 0x2B, 0x7FFFFFFF, -1]

CASES = [
    {"name": "HrFromCurle@httpclient@xbox", "ret": "int", "inputs": [[c] for c in CODES + list(range(3, 100, 7))]},
    {"name": "Initialize@CurlProvider@httpclient@xbox", "ret": "uint64",
     "imports": {"XCurl.dll!curl_global_init": (curl_global_init, 0), "MSVCP140.dll!_Mtx_init_in_situ": (mtx_init, 0),
                 "VCRUNTIME140.dll!_CxxThrowException": (throw, 0)},
     "fakes": {"http_alloc@httpclient@xbox": http_alloc},
     "observe": {"http_alloc@httpclient@xbox": "q"},
     "inputs": [{"args": [bytes(0x40)], "setup": setup(c)} for c in CODES] +
               [{"args": [bytes(0x40)], "setup": setup(0, f)} for f in (0, 1)]},
]
