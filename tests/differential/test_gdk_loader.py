"""Compare the original GDK loader against the faithful candidate, with a model Win32 loader.

No Windows services are started and no actual DLL is loaded by this test.
Compare the HRESULT, original module global and exact DLL name, handle, flags and calls.
"""
from unicorn import x86_const as X

STATE = {}
MODULE_GLOBAL = 0x14A706898
MODULES = (0, 0x00007FFA12340000, 0x00007FFB56780000)


def setup(initial, returned):
    def apply(w):
        STATE["returned"] = returned
        w.ptr(MODULE_GLOBAL, initial)
    return apply


def load_library_ex(m, mu):
    name = m.read_cstr(mu.reg_read(X.UC_X86_REG_RCX))
    file = mu.reg_read(X.UC_X86_REG_RDX)
    flags = mu.reg_read(X.UC_X86_REG_R8) & 0xffffffff
    m.observed.append(("LoadLibraryExA", name, file, flags))
    mu.reg_write(X.UC_X86_REG_RAX, STATE["returned"])


CASES = [
    {"name": "XGameRuntimeLoadModule", "ret": "int", "builds": ["iw5mp_ship"],
     "imports": {"kernel32.dll!LoadLibraryExA": (load_library_ex, 0)},
     "inputs": [{"args": [], "setup": setup(initial, returned)}
                for initial in MODULES for returned in MODULES]},
]
