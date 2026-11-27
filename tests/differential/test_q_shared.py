"""universal/q_shared.cpp: the string utilities (src/universal/q_shared.cpp), against the Store build.

Inputs adapted from black-ops-2-decomp's tests of the same functions. The Store build compiled its asserts out, so
there are no assert hooks, and no null-pointer inputs: its copies have no checks, and both sides would fault.
"""
import random
import struct

from harness import SCRATCH

rng = random.Random(2011)
LAYOUT = SCRATCH + 0x90000  # Info_NextPair's string, behind the head pointer passed by reference


def returns(value):
    return lambda mu: value


def cstr(s):
    return (s.encode("latin1") if isinstance(s, str) else s) + b"\0"


def ints(values):
    return [[v] for v in values]


SMALL_INTS = list(range(-300, 300)) + [-2**31, 2**31 - 1, 0x92, -0x6E, 0x192]

STRINGS = ["", "a", "A", "abc", "ABC", "abd", "ab", "abcd", "Hello World", "hello world", "HELLO WORLD!", "^1Red",
           "[{+attack}]", "\x7f\x80\xfe\xff", "zz", "@[`{", "0123456789", "mixed CASE 123", "\x92quote\x92"]

INFO = ["", "\\", "\\name\\player", "\\name\\player\\rate\\25000", "name\\player\\rate\\25000",
        "\\a\\1\\b\\2\\c\\3", "\\A\\upper\\a\\lower", "\\key\\", "\\key", "\\\\empty\\x\\y", "\\k\\v\\\\",
        "\\sv_hostname\\^1My ^7Server\\g_gametype\\tdm\\mapname\\mp_dome", "\\x\\1\\x\\2", "k\\v", "\\only"]


def string_pairs():
    pairs = [(a, b) for a in STRINGS for b in STRINGS[:8]]
    for _ in range(150):
        a = "".join(chr(rng.choice(b"aAbB\x80\xffz[{^0")) for _ in range(rng.randrange(6)))
        b = "".join(chr(rng.choice(b"aAbB\x80\xffz[{^0")) for _ in range(rng.randrange(6)))
        pairs.append((a, b))
    return pairs


def random_text(n, alphabet="abcXYZ09 ^^[{}\\;\"./*?\x01\x7f\x80\x92\xff"):
    return "".join(rng.choice(alphabet) for _ in range(n))


CASES = [
    {"name": "I_islower", "ret": "bool", "inputs": ints(SMALL_INTS)},
    {"name": "I_isdigit", "ret": "bool", "inputs": ints(SMALL_INTS)},
    {"name": "I_isforfilename", "ret": "bool", "inputs": ints(SMALL_INTS)},
    {"name": "I_iscsym", "ret": "int", "inputs": ints(SMALL_INTS)},
    {"name": "I_strncpyz", "ret": "void",
     "inputs": [[b"\xcc" * 40, cstr(s), n] for s in STRINGS for n in (1, 2, 3, 5, 40)]},
    {"name": "I_strnicmp", "ret": "int",
     "inputs": [[cstr(a), cstr(b), n] for a, b in string_pairs() for n in (0, 2, 0x7FFFFFFF)]},
    {"name": "I_strncmp", "ret": "int",
     "inputs": [[cstr(a), cstr(b), n] for a, b in string_pairs() for n in (0, 2, 0x7FFFFFFF)]},
    {"name": "I_stristr", "ret": "cstr", "inputs": [[cstr(a), cstr(b)] for a, b in string_pairs()]},
    {"name": "I_stricmp", "ret": "int", "inputs": [[cstr(a), cstr(b)] for a, b in string_pairs()]},
    {"name": "I_strcmp", "ret": "int", "inputs": [[cstr(a), cstr(b)] for a, b in string_pairs()]},
    {"name": "I_stricmpwild", "ret": "int",
     "inputs": [[cstr(w), cstr(s)] for w in ["*", "", "a*", "*c", "a?c", "A*C", "*b*", "??", "?", "a*b*c", "*.ff",
                                             "mp_*", "**", "abc", "ab?", "\x80*", "[{*"]
                for s in ["", "abc", "ABC", "ab", "abd", "a", "mp_dome", "code.ff", "aXbYc", "\x80\x81"]]},
    {"name": "I_strlwr", "ret": "cstr", "inputs": [[cstr(s)] for s in STRINGS]},
    {"name": "I_strncat", "ret": "void",
     "inputs": [[cstr(d).ljust(48, b"\xcc"), size, cstr(s)] for d in ["", "abc", "x" * 10]
                for size in (4, 11, 12, 16, 48) for s in ["", "de", "longer string"]],
     "fakes": {"Com_Error": returns(None)}, "observe": {"Com_Error": "is"}},
    {"name": "I_DrawStrlen", "ret": "int",
     "inputs": [[cstr(s)] for s in STRINGS + ["^", "^^", "^^1", "^1", "^a", "a^1b^2c", "^9^0^/^:^;^<", "^1^"]]},
    {"name": "I_CleanStr", "ret": "cstr",
     "inputs": [[cstr(s)] for s in STRINGS + ["^^1x", "^^^11a", "a\tb\nc", "^", "~\x7f ", "^1^2^3", "^a", "^;^<"]] +
               [[cstr(random_text(rng.randrange(30)))] for _ in range(60)]},
    {"name": "I_CleanChar", "ret": "char", "inputs": ints(range(-128, 128))},
    {"name": "Info_NextPair", "ret": "void",
     "inputs": [{"args": [struct.pack("<Q", LAYOUT), b"\xcc" * 64, b"\xcc" * 64],
                 "setup": (lambda w, s=s: w.write(LAYOUT, cstr(s)))} for s in INFO]},
]
