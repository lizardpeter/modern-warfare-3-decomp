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

## Ghidra analysis

- **Chained `.pdata` parts are not functions.** The compiler separates cold and shrink-wrapped regions into chained
  entries (UNW_FLAG_CHAININFO), and a chained entry's parent can itself be chained (nested up to several levels:
  `140009FEF` → `140009FDE` → `140009F80`). `tools/inventory.py` resolves `part_of` to the root function;
  `ApplySymbols.java` creates functions only at the 11,507 roots and Ghidra joins the parts to them by flow.
- **Non-returning discovery is off.** Ghidra's "Non-Returning Functions - Discovered" marked `memcpy`, `memset`,
  `memmove`, `free`, `sqrtf`, `floor`, `__chkstk` and hundreds of game functions non-returning (one with 1,353
  callers ends in a plain `ret`), so every caller stopped at the first such call: 3,419 functions shorter than
  `.pdata`, a quarter of string references outside any function. Known non-returning functions still apply.
- Result: 10,369 of 11,507 functions are exactly their `.pdata` extent with chained parts (90%); 4 are larger
  (1 by more than 16 bytes); of the 1,134 smaller, all but 211 differ by alignment padding, and those checked are
  switch jump tables, which MSVC x64 places in `.text` after the function and Ghidra rightly keeps out of the body.
  14,220 functions in all, 0 decompile failures.

## Code layout

A source file's functions sit together, roughly in alphabetical order of name: `q_shared.cpp`'s run at
`14031A760`-`14031BDA0` goes `Com_sprintf`, `I_CleanChar`, `I_CleanStr`, `I_DrawStrlen`, `I_iscsym`, `I_isdigit`,
`I_isforfilename`, `I_islower`, `I_strcmp`, ... `Info_NextPair`, `Info_SetValueForKey`, ..., `Swap_Init`, `va`.
Functions inlined everywhere (`I_isupper`, `I_isalpha`, `I_isalnum`) or unused (`I_strupr`) are absent. So the gaps
between identified functions are where to look for the rest of a file, and reading decides (`semantic+differential`
in `records/names.tsv`); the position alone identifies nothing.

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
