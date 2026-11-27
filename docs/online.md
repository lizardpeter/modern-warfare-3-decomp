# Online services in the Store build, and the local replacement

What the Store multiplayer exe (`iw5mp_ship`) talks to, mapped so the enhanced build can answer it locally: the
game starts, signs in, connects and plays through its own menus, with no launch options, and nothing leaves the
machine for Activision's or Xbox's servers. Ported in spirit from black-ops-2-decomp (its P0010, P0013-P0016), whose
Store build carries the same GDK and DemonWare layers, but built in from the start: no command-line switches and no
separate service to run.

## The GDK (XGameRuntime)

The exe links the 2024 PC GDK's thunk library (`xgameruntime.lib`, edition 240308, version 0x6336 = 25398 passed to
`InitializeApiImplEx`). Every GDK call is a thunk that asks the query wrapper `XGameRuntimeQueryApi` (140360220), which calls the runtime's `QueryApiImpl` for an interface and calls one
vtable slot. The runtime is loaded by `XGameRuntimeLoadModule` (1403601E0) with
`LoadLibraryExA("xgameruntime.dll", 0, LOAD_LIBRARY_SEARCH_SYSTEM32)`: only from System32, so a DLL in the game
folder can't stand in for it the way the X3DAudio loader does; the enhanced build redirects that one function.

All 91 thunks are named from the library itself (`tools/gdk/name_thunks.py` -> `analysis/iw5mp_ship/gdk/thunks.tsv`,
matched by interface GUIDs and slot; `records/names.tsv`, method `library`):

| Area | APIs used | Used for |
|---|---|---|
| XAsync, XTaskQueue, XThread | 19 | the async plumbing every other API completes through |
| XUser | 24 | sign-in (`XUserAddAsync`), gamertag, XUID, privileges, the DemonWare token (`XUserGetTokenAndSignatureAsync` for `https://auth3.prod.demonware.net`) |
| XGameSave | 12 | the player's saves (a GDK save container, synced by the Xbox app) |
| XStore | 10 | the game licence (`XStoreQueryGameLicenseAsync`), DLC licences |
| XPackage | 8 | DLC packages: enumerate, mount, install events |
| XNetworking | 10 | connectivity hint, TLS certificate checks for DemonWare HTTP |
| XGameInvite, XGameEvent, XGameUi, XSystem, XGame | 7 | invites, telemetry events, profile card, sandbox, title id |

## DemonWare

Compiled in whole (bdCore, bdNet, bdSocket with DTLS and NAT traversal, bdLobby with auth, storage, stats, teams,
counters, profiles, matchmaking, messaging, rich presence, content streaming, anti-cheat, event log, bandwidth test).
Log-on is `dwLogOnFrame` (1401145C0) per controller, from `Live_ControllerFrame`:

1. `dwLogOnStart` (140114C40): rate-limited start, state 1.
2. `dwLogOnUpdate` (1401148E0), state 3: resolve `auth3.prod.demonware.net` (`dwResolveHost`, bdGetHostByName).
3. State 4: `bdHTTPAuthService::authorizeXBoxOne` (title 0x906): gets the user's Xbox token for the auth URL from
   XUser, then posts it over WinHTTP (`bdHTTPWorkerWinHTTPImpl`, https, port 443).
4. State 5: wait for the auth service; status 700 is success.
5. State 6: resolve `lsg.2310.prod.demonware.net`, `bdLobbyService::connect` on TCP 3074 with the auth ticket.
6. State 7: wait for the lobby (`bdLobbyService::getStatus` == 2), then state 8, logged on: stats download, party,
   presence.

Other hosts: `mw3-stun.us/eu.demonware.net` and `stun.{us,eu,jp,au}.demonware.net` (NAT discovery), XCurl and WinHTTP
callers.

## The replacement (enhanced build)

Done so far (P0003, P0004):

- **The local GDK runtime, `iw5_enhanced.dll` itself** (`enhanced/gdk/`). `XGameRuntimeLoadModule` (1403601E0)
  returns the DLL, which exports `QueryApiImpl`, `InitializeApiImpl(Ex)`, `XErrorReport` and `UninitializeApiImpl`.
  It loads the system's `xgameruntime.dll` exactly as the original would (System32 only) and:
  - answers **locally**: XUser (one player, "Unknown Soldier" unless `iw5_enhanced.ini` next to the DLLs says
    `name=...`; signed in; XUID 0009000000000001; every privilege; an Xbox-style token only for the DemonWare auth
    URL, "network unreachable" for any Xbox Live URL), XGameSave (`players\<container>\<blob>` in the game folder,
    written to `.tmp` files and moved into place), XNetworking (connected, nothing pinned), XSystem (RETAIL),
    XGame (title 66CB4BCA), XGameInvite, XGameEvent, XGameUI;
  - **forwards** XAsync/XTaskQueue, XStore, XPackage and XGameRuntimeFeature to Gaming Services, unchanged. The
    game licence (`XStoreQueryGameLicenseAsync`, checked after sign-in) and DLC licences are Microsoft's checks.
    The one translation: `XStoreCreateContext` gets no user instead of the local player, so the Store uses the
    account signed in to the Microsoft Store app.
  - **Without Gaming Services the game doesn't start**, as the original ("Game Runtime is not installed"). The
    enhanced build never runs the game where the licence check can't run.
- **No Xbox Live HTTP** (`enhanced/libHttpClient/`). The Xbox services library (XSAPI, statically linked; the game
  quits if `XblInitialize` fails) sends through libHttpClient's XCurl provider. `CurlProvider::Initialize`
  (1403D81D0) builds the provider without `curl_global_init` (XCurl never starts, and doesn't load the system runtime
  itself), and `CurlProvider::PerformAsync` (1403D83C0) returns `E_HC_NO_NETWORK` for every request.

Checked under Wine (with a stand-in for Gaming Services that was then removed, see below): the game starts, the
local runtime answers every query, the Xbox services initialise, the multiplayer menu comes up, Play signs in the
local player, and the DemonWare log-on starts and fails with "server not available": there's no local DemonWare
yet.

Why the stand-in went: without Gaming Services the Store interfaces don't exist, the game's licence query can't run,
and the game goes on to sign in anyway. A runtime that made that possible would run the game unlicensed on any PC
without Gaming Services, so the enhanced build requires the system runtime, as the original does. Testing past the
main menu therefore needs Windows with Gaming Services.

Next:

- **A local DemonWare inside the DLL, started on its own.** Loopback only: `auth3` and `lsg` resolve to 127.0.0.1,
  the auth request goes to the local auth endpoint instead of https on the internet, and the lobby (TCP 3074) is the
  original protocol, encrypted with the ticket the local auth issued. It answers what the game asks for after
  log-on (time, storage and publisher files, stats, teams, counters, profiles, matchmaking for parties, presence,
  messaging, anti-cheat challenges, event log), each from the reference build's DemonWare.
- **Everything else cut:** STUN, UPnP and remaining WinHTTP callers fail cleanly, like a machine with no internet;
  the engine's own UDP networking stays for playing with others.
