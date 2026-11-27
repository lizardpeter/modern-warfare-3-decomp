# Builds

Every build is pinned in `target.toml` (R2 key and SHA-256; for Xbox 360 builds also the SHA-256 of the unpacked
PE). Times are PE link timestamps, UTC.

## Target: Microsoft Store (x64)

| id | file | linked | PDB (not shipped) |
|---|---|---|---|
| `iw5mp_ship` | `mw3/DUMP/iw5mp_ship.exe` | 2024-10-31 16:53:34 | `C:\trees\iw5\game\pc\iw5mp_ship.pdb` {87D8A969-…} age 72 |
| `iw5sp_ship` | `mw3/DUMP/iw5sp_ship.exe` | 2024-10-31 16:52:47 | `C:\trees\iw5\game\pc\iw5sp_ship.pdb` {A68BFB67-…} age 37 |

Package 38985CA0.CallofDutyMW3PCMS 2.0.3.0 (`mw3/DUMP/MicrosoftGame.Config`), `build_num.txt` 20491254. See
`docs/store-build.md`.

## References: Xbox 360, title update 1 (PowerPC)

All twelve executables in `mw3/exes/MW3_TU1/` are XEX2 files (the `.exe` names are the upload's), linked
2011-10-11 between 22:13 and 22:31 from one tree, build 452 (`build_num.txt`; line tables name
`\\infinityward.net\shared\builds\src\iw5\x360\patch\452\src`). Each has a full private PDB (age 1, signature = the
PE timestamp), a linker MAP and an XDB. All are encrypted with the development-kit key and use basic compression;
`tools/unpack/xex.py` turns each into the PE the linker wrote (docs/xbox360.md).

| id | file | configuration | asserts | dev GUI | linked |
|---|---|---|---|---|---|
| `iw5mp_xenon` | `2-iw5mp.exe` | development (primary reference) | yes | yes | 22:15:30 |
| `iw5mp_xenon_ship` | `default_mp.xex` | ship (retail TU1) | no | no | 22:19:02 |
| `iw5mp_xenon_fast_server` | `6-iw5mp_fast_server.exe` | fast server | yes | yes | 22:27:46 |
| `iw5mp_xenon_demo` | `5-iw5mp_demo.exe` | demo | no | no | 22:24:15 |
| `iw5mp_xenon_screenshot` | `a-iw5mp_screenshot.exe` | screenshot | no | yes | 22:31:18 |
| `iw5sp_xenon` | `1-iw5sp.exe` | development | yes | yes | 22:13:45 |
| `iw5sp_xenon_ship` | `default.xex` | ship (retail TU1) | no | no | 22:17:15 |
| `iw5sp_xenon_fast_server` | `3-iw5sp_fast_server.exe` | fast server | yes | yes | 22:26:01 |
| `iw5sp_xenon_demo` | `4-iw5sp_demo.exe` | demo | no | no | 22:20:44 |
| `iw5sp_xenon_demo_replay` | `7-iw5sp_demo_replay.exe` | demo replay | no | no | 22:22:27 |
| `iw5sp_xenon_screenshot` | `9-iw5sp_screenshot.exe` | screenshot | no | yes | 22:29:30 |

Configurations are read from the binaries: source-path strings (asserts) number about 820 in the development and
fast-server builds and about 55 in the others; `devgui` strings about 68 where the dev GUI is compiled in. The
configuration names themselves come from the file names.

`iw5mp_xenon` is the primary reference, as the asserting server was for BO2: most names and strings. The PDB holds
1,764 modules, 28,867 functions with prototypes, source files and line numbers, and the full type graph; the MAP
102,381 publics. `iw5mp_xenon_ship` is the same code in the ship configuration, closest to the Store build's, and
is analysed blind first so its own symbols score the matcher (docs/matching.md).

## Related

| id | file | what | linked |
|---|---|---|---|
| `iw5mp_steam` | `Downloads/plu_exes/iw5mp.exe` | PC multiplayer as Plutonium ships it, x86, Steam, linker 9.00, PDB `c:\trees\iw5-pc-release\game\pc\iw5mp.pdb` | 2018-05-02 15:11:13 |
| `iw5sp_steam` | `Downloads/plu_exes/iw5sp.exe` | PC single player, x86, Steam | 2012-11-30 17:14:20 |
| `iw4mp_xenon_mpbeta` | `mw3/exes/MW3_TU1/mp_beta.xex` | **not MW3**: Modern Warfare 2's multiplayer beta (MAP module `iw4mp_mpbeta`) | 2009-09-03 05:51:13 |

The PC exes are the same platform layer as the Store build (Windows, D3D9) on the 2011 code, without symbols.
Community clients for IW5 name many of their addresses; those names can be carried to the Store build by matching.
