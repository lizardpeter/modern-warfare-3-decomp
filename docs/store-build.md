# The Microsoft Store build (target)

Package `38985CA0.CallofDutyMW3PCMS` 2.0.3.0 (`MicrosoftGame.Config`), Store id 9NTL0QDWZ4FS, `build_num.txt`
20491254. The dump in `mw3/DUMP/` holds `iw5sp_ship.exe`, `iw5mp_ship.exe` (the config's multiplayer-protocol
executable), `gamelaunchhelper.exe`, `XCurl.dll`, `bink2w64.dll`, the zone files and the GDK assets.

## The exes

| | `iw5mp_ship` | `iw5sp_ship` |
|---|---|---|
| linked | 2024-10-31 16:53:34 | 2024-10-31 16:52:47 |
| linker | 14.29 (VS2019 16.11 toolset) | 14.29 |
| image base | 0x140000000, no relocations | same |
| entry | `14043B244` | `1403D6B30` |
| `.pdata` | 11,507 functions + 7,688 chained parts | |
| PDB (not shipped) | `C:\trees\iw5\game\pc\iw5mp_ship.pdb` age 72 | `...\iw5sp_ship.pdb` age 37 |

Both are plain PE files: normal section entropy, no packer, no Arxan, no Steam. They link the dynamic VS2019 CRT
(`MSVCP140`, `VCRUNTIME140`, `VCRUNTIME140_1`, the UCRT api sets), as BO2's and Ghosts' Store builds do. A `.xbld`
section is the GDK's build marker.

## What the port changed (from the imports and strings so far)

- **Renderer:** still **Direct3D 9** (`d3d9.dll`), unlike BO2's Store build (D3D11). Audio through `DSOUND` and
  `X3DAudio1_7`; video through `bink2w64.dll`.
- **Online:** `XCurl.dll` (the GDK's libcurl: easy and multi interfaces), `WINHTTP`, `CRYPT32` and `bcrypt` (hashes,
  symmetric keys, random). DemonWare is still compiled in: `bdNet`, `auth3.prod.demonware.net`,
  `lsg.2310.prod.demonware.net`, the `mw3-stun` and regional `stun` hosts.
- **Platform:** `XGameRuntime` (its error strings), the `xbox::services` C++ API (achievements manager and service
  classes in RTTI), app-state change notifications (`api-ms-win-core-psm-appnotify`).
- **Bink 2** in place of the Xbox 360's Bink 1.

Inventory every Store-only difference here as it is found: these are where `src/` follows the Store build rather
than the Xbox 360 reference.
