# Verification

`matches` means a passing automated test that runs the reconstruction and the original on the same inputs and
compares behaviour (optimised builds won't byte-match). `tools/check_records.py` fails CI if `records/functions.tsv`
claims `matches` without a harness to show it.

## compiles: the hybrid-link build

`tools/build_recon.py [--build iw5mp_ship]` compiles every `.cpp` under `src/` with clang for x64 Windows (VS2019
16.11 compatibility, MSVC ABI and name mangling) and links `build/<build>/recon.dll`. Ported from
black-ops-2-decomp's Store path (slot table from black-ops-3-decomp, TLS from ghosts-decomp). Only the x64 Store
builds are linked: the Xbox 360 references aren't run.

- Exports: every function the Store build identifies (`tools/symbols.py`: `records/names.tsv`, then the matcher's
  anchored pairs, resolved by scoped plain name, so the Xbox MAP's 32-bit decorations don't matter).
- Hybrid link: anything the reconstruction uses but doesn't define resolves to the Store exe. Functions go through a
  slot table the DLL's entry point (`iw5_hybrid_entry`) rebases, imports through the exe's import table, exe data
  through slot loads (`-mcmodel=large`, `movabsq` rewritten). Not identified yet: a `ud2` trap. `hybrid.tsv` lists
  every dependency; the link fails if code holds an exe address directly.
- Build environment (`tools/recon/`, not game source): `crt_x64.s` (`strlen`, `strcmp`, which VS2019 expands
  inline, and `__chkstk`); minimal CRT headers; `iw5_assert.h` (MW3's `MyAssertHandler` shapes; the Store builds
  have no asserts, so `IW5_NO_ASSERTS` compiles them away); `iw5_static.h` for original statics and globals.
  Headers carried over from BO2 still describe VS2010 in places and are trimmed as they come into use.

## matches: differential tests

`tests/differential/run.py [--build iw5mp_ship] [--check-records] [-k text]` maps the Store exe and `recon.dll`
into one Unicorn machine (`harness.py`, from black-ops-2-decomp) and runs each exported function and its original
on the same inputs. Cases live in `test_*.py` as `CASES`, keyed by plain name. A case passes when every input gives
the same return value (floats bit for bit), the same bytes in every buffer passed by pointer, the same writes to
either image's globals and the same observed calls. Inputs on which the original faults are skipped; a
reconstruction that reaches a trap is `blocked`. Imports are stubs returning 0 unless modelled: `memset`,
`memcpy`, `memmove`, `strchr`, `strlen`, `atoi`, `tolower`, `toupper` (the C locale) and `strncpy`, which fills a
page at a time so a negative count turned `size_t` faults as the real one would.

`--check-records` fails if `records/functions.tsv` claims `matches` for a function that didn't pass. CI
(`.github/workflows/check.yml`, job `reconstruction`) builds the DLL and runs the tests with it on every push.

First file: `src/universal/q_shared.cpp`, the string utilities (17 functions, `test_q_shared.py`, all passing).
A deliberate change (`I_CleanChar` on 0x93, signed bytes in `I_strnicmp`) fails, so the comparison discriminates.
Each function is identified in the Store build by reading (`records/names.tsv`, `semantic+differential`): the
Store copies of a file's functions sit together, roughly in alphabetical order, and the gaps between matched ones
hold the rest of the file (docs/store-build.md). Reading also corrects the matcher: its callee pair
`LiveStorage_SetPostDownloadCommand` at `14031AD50` is `I_strncpyz`.

Second: `src/universal/com_math.cpp`, the angle and vector functions the input code uses and their neighbours (13
functions, `test_com_math.py`, about 8,700 inputs including signed zeros, denormals, infinities and NaNs with
payloads), all bit for bit. The Store build's float code is the model, not the Xbox's: it keeps `AngleDelta` and
friends in float, scales `atan2` by `180.0f / M_PI` computed in float (57.2957763671875) and adds 0 or 360 without a
branch, so `vectoyaw` returns +0.0 where `if (yaw < 0) yaw += 360` would keep -0.0; `Vec2Normalize`'s divisor is a
select (`-length >= 0 ? 1 : length`) whose NaN behaviour Ghidra's rendering of the Xbox code hides. Each of those
changed back fails its test. The harness models `atan2` and `floor` (doubles in XMM registers).

Third: the mouse's command side in `src/client_mp/cl_input.cpp`: `CL_GetMouseMovementWithSensitivity` (with
`CL_GetMouseMovement` inlined) and `CL_MouseMove` (900 inputs including infinite dvars and NaN movement), with
`ClampChar`. Read the Store's instructions for float clamps and selects, not only Ghidra's pseudocode: its speed cap
is `minss`/`maxss`, which pass a NaN movement through, and Ghidra renders the pair as `<=` comparisons that would
clamp it. Written as `cap < mx ? cap : mx` and `-cap > capped ? -cap : capped`, clang gives the same results.
Pass stack addresses out of `observe` signatures: a callee's pointer to the caller's local differs between the two
builds' frames.

Fourth: the PC mouse layer, `src/win32/win_input.cpp` (`IN_Frame` with `IN_MouseMove` inlined,
`IN_RecenterMouse`, `IN_ShowSystemCursor`) and `CL_MouseEvent`, on a model desktop (`tests/differential/win32.py`):
one window, the cursor, the foreground window and ShowCursor's count; every user32 call is recorded with its
arguments, so both sides must make the same calls in the same order. With this the Store's whole mouse path is
reconstructed and matches (docs/store-build.md, Mouse input). Pitfalls met on the way, each a test bug, not a
reconstruction bug:
- make every random choice when the input is built, never in its setup: the setup runs before each side's call;
- observe a string argument as `s` and a callee's pointer to the caller's local not at all: the two builds' literals
  and frames are at different addresses;
- models do 32-bit arithmetic where Windows does (a cursor at the int limits wraps).

Reconstruct in the reference's original order: each function tagged `// <reference VA>  <file>:<line>` with the
Store VA once identified. Control flow follows the reference; where the Store build differs (13 years of patches,
the PC platform layer), the Store build is the truth.

## Xbox 360 references

Read, not executed: they supply names, types, line order, assert expressions and the original control flow. Unicorn
runs big-endian PowerPC, so tests against a reference are possible later, with a calling-convention bridge (clang has
no Xenon MSVC ABI). Add it only if the Store build can't answer a question the reference can.
