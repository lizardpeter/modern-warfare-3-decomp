# The enhanced build in the game

The enhanced build (`enhanced/`, `records/patches.tsv`) loads into the original Store multiplayer executable
`iw5mp_ship.exe` (package 2.0.3.0, linked 2024-10-31). Single player is not supported yet: it runs unchanged.
Ported from black-ops-2-decomp's loader; MW3's Store exe imports no XInput, so the loader is an `X3DAudio1_7.dll`
instead of BO2's `XINPUT1_4.dll`.

## Install

1. Build, or download from a CI run of `Checks` (artifact `iw5mp_ship-enhanced`):

       python3 tools/build_runtime.py     # -> build/iw5mp_ship/runtime/X3DAudio1_7.dll, iw5_enhanced.dll

2. Copy both DLLs into the folder that holds `iw5mp_ship.exe`. The game has to run from a folder you can add files
   to (your own unpacked copy): the Store's install folder is protected.
3. Start multiplayer. `iw5_enhanced.log` next to the DLLs says what was installed.

To go back to the original game, delete the two DLLs. Nothing else on disk or in the system is changed.

## How it loads

`iw5mp_ship.exe` imports `X3DAudioInitialize` and `X3DAudioCalculate` from `X3DAudio1_7.dll`. The Store package
doesn't ship that DLL (it comes from the DirectX runtime in the system folder) and it isn't one of Windows'
KnownDLLs, so Windows looks in the game's folder first. The loader there (`runtime/loader/x3daudio_proxy.c`):

- forwards both functions to the system's `X3DAudio1_7.dll`, so audio is as before (if the system copy is missing,
  the game couldn't run without the loader either; the loader says so instead of crashing later);
- checks the executable by its PE timestamp and image size: only `iw5mp_ship.exe` 2.0.3.0 gets the enhanced build;
  any other executable (single player) only gets the forwarding;
- loads `iw5_enhanced.dll`, checks that its redirect table was built for this executable and that every function it
  replaces still starts with the bytes the build saw, and only then writes a 14-byte jump at each replaced function's
  entry to its replacement in the DLL. If anything doesn't match, the game stops with a message in the log rather
  than running half-patched.

In the game the DLL runs only its replacements: every other reconstructed function it holds is a jump into the
exe's own copy (exe first, `tools/runtime_link.py`). The build checks that no code reachable from a replacement can
reach anything not identified in the exe yet.

## Multiplayer enhancements

**P0001, raw mouse input:**
- **Raw input.** The view turns by the mouse's own counts, read on a thread of its own from Windows' raw input. The
  original turned by how far the Windows cursor moved, so Windows' pointer speed and "Enhance pointer precision"
  acceleration applied, and a fast flick could be cut short at the screen's edge (B0001).
- **No acceleration, no smoothing.** `cl_mouseAccel` and `m_filter` do nothing (B0002), the remote-control views
  included.
- **High DPI and polling rates.** Counts are added up in 64-bit counters, never rounded, and each report is handled
  on the input thread, not the game's, so an 8 kHz mouse costs the game nothing. Nothing is lost between frames.
- **No fallback.** If Windows refuses raw input, the game stops with an error naming the Windows error code; it never
  quietly goes back to the cursor.
- Menus keep the cursor as before.

**P0002, MW2019 sensitivity:**
- **`m_sens`, in MW2019's units.** One count turns the view by `m_sens` x 0.0066 degrees: the same number gives the
  same cm/360 as in Modern Warfare (2019). The default is 1.5 (about 29 cm per 360 degrees at 3200 DPI); the range is
  0.01 to 100, and it's saved in the config like any archived setting (`seta m_sens 1.5`, or `/m_sens 1.5` in the
  console).
- **The same at any field of view.** Hip fire and aiming down sights turn alike; the FOV scale doesn't apply (B0004).
  The menu's mouse sensitivity no longer turns the view; set `m_sens` instead.
- **Inverted look** follows the menu's option (`m_pitch`'s sign), as before.
- **Exact over many frames.** The view angles are kept in double precision between frames, so a movement split over
  more frames (higher polling and frame rates) ends at the same angle, and the yaw is kept within +-180 degrees, so
  small movements stay exact however long you play (B0003).

## Testing

`tests/differential/run.py --enhanced` shows every function the enhanced build doesn't replace still matches the
original; `tests/enhanced/run.py` checks each replacement against its patch's promises (`records/patches.tsv`),
including the loader's jumps applied to the emulated exe. Both run in CI.
