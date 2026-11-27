# Roadmap

The enhanced build is the faithful code plus deliberate, documented changes (AGENTS.md). Nothing here is built yet.
The enhanced-build items are carried over from the Ghosts, BO2 and BO3 decomps' plans, to confirm for MW3. Each
lists what must be decompiled first.

## Tooling

- **PowerPC reference scan in `tools/match/match.py`** (docs/matching.md). Needs: nothing; first step after the
  Ghidra analyses of `iw5mp_xenon` and `iw5mp_ship` are committed.
- **VMX128 for Ghidra**: a SLEIGH extension so the Xenon vector code disassembles and decompiles
  (docs/xbox360.md).
- **Differential harness** for the Store build, from black-ops-2-decomp's (docs/verification.md).

## Enhanced build

- **No official servers.** Cut every path to DemonWare, XCurl/WinHTTP services and Xbox services so the game never
  connects to them; keep the engine's own UDP networking for peer and LAN play and a future own server. Needs:
  the online layer's call sites (`bdNet`, the DemonWare hosts, XCurl and WinHTTP callers; docs/store-build.md).
- **Loading.** A DLL in the game's own folder, never a system file; the Store exe has a fixed base and no
  relocations. Needs: the entry points the replacements hook.
- **Raw mouse input** with no acceleration or smoothing, handling high DPI and polling rates, and never silently
  falling back to cursor input; MW2019-style sensitivity and FOV. Needs: `win_input`, `cl_input`.
- **Frame rate.** Uncapped, with game behaviour independent of frame rate; refresh rates above 240 Hz. Needs:
  `Com_Frame`, the client frame and `com_maxfps` paths, and every frame-time-dependent calculation (file them in
  `records/bugs.tsv` as `precision`/`limit`).
- **Latency.** Flip-model presentation, CPU late latch, then NVIDIA Reflex; new options as in-game graphics settings.
  Needs: the D3D9 device and present path.
- **Renderer.** A native Vulkan renderer replacing D3D9, no translation layers. Needs: `gfx_d3d`.
- **Security fixes** to network and file input handling, from `records/bugs.tsv` and `docs/security.md` (created
  with the first finding).
