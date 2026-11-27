# Online services in the Store build and enhanced isolation

The faithful Store multiplayer executable uses GDK and DemonWare. The enhanced build now
intentionally treats GDK as unavailable: it never loads or forwards to the system runtime,
even on a machine where Gaming Services is installed. It does not fabricate authorization.
Startup may stop at the original runtime-unavailable error; playable offline mode is not
claimed by this change.

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

## Enhanced build: fail-closed GDK boundary

- `XGameRuntimeLoadModule` clears the runtime module and returns the runtime-not-found error.
- Both initialization exports return the same error. Shutdown is a no-op.
- `QueryApiImpl` rejects every interface with E_NOINTERFACE and clears the output pointer;
  a null output pointer gets E_POINTER. This applies to XUser, XAsync, XTaskQueue, XStore,
  XPackage and every other GDK family. No system runtime or entitlement provider is invoked.
- Existing local provider source is retained for linkage and research, but not exposed by
  this boundary. No sign-in, license or DLC success is fabricated.
- The existing libHttpClient XCurl provider still rejects all HTTP operations locally.

This is a GDK-boundary change, not proof of process-wide network isolation. Independent
WinHTTP, DemonWare, STUN and other native networking paths require separate auditing.
Neither offline gameplay nor Windows runtime validation is established by this change.
