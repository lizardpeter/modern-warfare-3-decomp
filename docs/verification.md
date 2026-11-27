# Verification

`matches` means a passing automated test that runs the reconstruction and the original on the same inputs and
compares behaviour (optimised builds won't byte-match). `tools/check_records.py` fails CI if `records/functions.tsv`
claims `matches` without a harness to show it.

## Plan

The harness arrives with the first reconstructed file, started from black-ops-2-decomp's (`tests/differential/`:
Unicorn, hybrid-linked DLL from `tools/build_recon.py` and `tools/symbols.py`, whose unreconstructed calls resolve to
the original exe; `tests/replay/` for native side-by-side runs of Store code). Start with self-contained files
(`universal/q_shared.cpp`, `universal/com_math.cpp`) to prove it.

- **Store build (x64): the target, and what `matches` is measured against.** Same toolchain as BO2's Store build
  (clang targeting x64 Windows, original prototypes, MSVC ABI), so the BO2 harness applies with the build ids and
  `iw5` names changed.
- **Xbox 360 references (PowerPC): read, not executed, at first.** They supply names, types, line order and the
  original control flow. Unicorn runs big-endian PowerPC 64, so differential tests against a reference are possible
  later (clang `--target=powerpc64-unknown-unknown` with 32-bit pointers is not an MSVC ABI; any such harness needs
  its own calling-convention bridge). Add it only if the Store build can't answer a question the reference can, e.g.
  code the Store build dropped.

Reconstruct in the reference's original order: each function tagged `// <VA>  <file>:<line>`, using the reference
VA and its PDB line, plus the Store VA once matched.
