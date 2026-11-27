# Working in modern-warfare-3-decomp

Decompilation of Call of Duty: Modern Warfare 3 (2011, IW5). The final target is the **Microsoft Store build** (x64,
`kind = "retail"` in `target.toml`), multiplayer (`iw5mp_ship`) first. The references are the **Xbox 360 title
update 1 builds** (PowerPC, full PDBs and MAPs, October 2011); the PC Steam exes Plutonium ships (x86) are `related`
helpers. The layout and method follow `lizardpeter/black-ops-2-decomp` and `lizardpeter/ghosts-decomp`; this repo
owns its copy of the tools.

The one big difference from those repos: **the references are a different architecture from the target**
(PowerPC vs x64), not just a different compiler. Names, types, source files and line numbers carry over directly
from the PDBs; code is matched by content and call graph, never by instruction shape (docs/matching.md,
docs/xbox360.md).

## The one rule: generated vs authored

| Kind | Where | Who writes it |
|---|---|---|
| **Generated** | `analysis/<build>/`, `STATUS.md` | Only the scripts in `tools/` (and the Ghidra workflow). Never hand-edit. |
| **Authored** | `src/`, `include/`, `enhanced/`, `tests/`, `docs/`, `records/` | People and agents. |

If a generated file is wrong, fix the script that wrote it and re-run. Don't patch the output.

| Path | Responsibility |
|---|---|
| `target.toml` | Every original build: kind, architecture, R2 keys, SHA-256 (and `unpacked_sha256` for XEX builds), PDB identity, Ghidra language, `analyze` flag |
| `analysis/<build>/` | PE inventory (`pe.json`, `pdata.tsv` from x64 or Xenon `.pdata`), PDB/MAP tables (`pdb/`, `map/`), Ghidra exports (`ghidra/`, saved project split under 100 MB), `functions.tsv`, `pseudocode/`, `disasm/`, `match/`, `logs/` |
| `src/`, `include/` | Faithful behaviour, including original quirks, mirroring the original `code_source/src` tree |
| `enhanced/` | (with the first enhancement) overrides for intentional improvements, same layout as `src/` |
| `records/` | `functions.tsv` (status), `names.tsv` (identifications in builds without symbols), `bugs.tsv`, later `patches.tsv` |
| `tests/differential/` | (with the first reconstructed file) reconstructions against the original code (docs/verification.md) |
| `tools/` | Fetching and XEX unpacking, inventory, PDB extraction, Ghidra scripts, matching, records check, status and graph generation |

## Addresses

Upper-case hexadecimal without `0x`, zero-padded to at least eight digits: Xbox 360 builds at 0x82000000 read
`82260CB0`, x64 builds at 0x140000000 read `14043B244`. Every tool uses that form. A reference address applies to
that reference build only.

## Builds

- **Store (target):** `iw5mp_ship`, `iw5sp_ship`. Package 38985CA0.CallofDutyMW3PCMS 2.0.3.0, linked 2024-10-31 with
  the VS2019 16.11 toolset, GDK, no protection, no Steam, no relocations. Still Direct3D 9. See `docs/store-build.md`.
- **References:** `iw5mp_xenon` (development configuration: asserts, dev GUI, PDB + MAP; primary) and its ship
  configuration `iw5mp_xenon_ship` (the retail TU1 `default_mp.xex`, analysed blind: the matcher's ground truth),
  plus the fast-server, demo and screenshot configurations and the single-player set. Xbox 360, VS2010, PowerPC,
  stored as XEX2 and unpacked by `tools/unpack/xex.py`. See `docs/builds.md`, `docs/xbox360.md`.
- **Related:** `iw5mp_steam`, `iw5sp_steam` (Plutonium's PC exes, x86), and `iw4mp_xenon_mpbeta` (the upload's
  `mp_beta.xex` is MW2's 2009 multiplayer beta, not MW3).

## Function status ladder

`located → named → typed → decompiled → compiles → matches → integrated`

- `located`: entry and size known (`.pdata` gives nearly every Xenon function and every non-leaf x64 function;
  Ghidra the rest)
- `named`: original name known (PDB/MAP, or a recorded identification in `records/names.tsv`)
- `typed`: original prototype known
- `decompiled` / `compiles`: authored source exists / builds in CI
- `matches`: passes the differential test against that build's original code. Optimised builds won't byte-match;
  the test compares behaviour. `tools/check_records.py` (CI) fails if `records/functions.tsv` claims more than the
  tests show; until the harness exists, nothing can claim `matches`.
- `integrated`: linked into the runnable reconstruction

`STATUS.md` and the `re_iw5` graph import set are computed from `analysis/` and `records/` (`python tools/status.py`,
`python tools/graph/sync.py`). Don't track progress anywhere else.

## Identifications

Every cross-build identification in `records/names.tsv` records its method, evidence and confidence. Never match on
address position across differently linked builds, and never on instruction shape across architectures. Methods:
`string`, `constant`, `named`, `callee`, `caller` (`tools/match/match.py`), `semantic+differential` (by reading). See
`docs/matching.md`.

## Faithful and enhanced

`src/` is original behaviour, bugs included. Each change for the enhanced build goes in `enhanced/` and is recorded in
`records/patches.tsv` (both created with the first enhancement). Planned improvements live in `docs/roadmap.md`. Never
fix a bug in `src/`; record it in `records/bugs.tsv` (the enhanced build's fix queue: anything in the original worth
fixing or improving, not only outright bugs) and fix it in `enhanced/`.

Where the Store build differs from the Xbox 360 reference (PC platform layer, D3D9 on PC, GDK services, 13 years of
patches), the Store build is the truth: `src/` reproduces the target. Record such differences in
`docs/store-build.md`.

## Originals

Game files are never committed. They live in R2 under the keys in `target.toml`, and every tool verifies the SHA-256
(`python tools/fetch_originals.py <build> originals`). For an XEX build, the fetched XEX is unpacked to the PE the
linker wrote and checked against `unpacked_sha256`; derived binaries are regenerated, never stored. Helpers for
reading the bucket are not part of this repo.

## Ghidra pipeline

`.github/workflows/ghidra.yml` runs on push when `target.toml` or the pipeline changes, for builds with
`analyze = true` (or the builds named in a manual run). It extracts PDB/MAP tables (`tools/pdb/extract.py`), imports
each build (Xbox 360 builds with `ghidra_language`), creates and names every function the PDB, MAP and `.pdata` know
(`ApplySymbols.java`), points Ghidra's PDB analyzer at the PDB unless `ghidra_pdb = false`, exports, decompiles in
shards and commits `analysis/<build>/`. Set `analyze` back to `false` once a build's analysis is committed.

`pdb/types.h` holds one definition per type name; `pdb/type-variants.txt` lists the types the PDB defines more than
once with different layouts (42 in `iw5mp_xenon`). Xbox 360 layouts are 32-bit big-endian: pointers are 4 bytes,
so x64 Store layouts differ wherever a struct holds a pointer (`tools/pdb/layout64.py` predicts them; check against
the Store exe's own accesses).
