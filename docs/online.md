# Online services in the Store build and enhanced isolation

The faithful Store multiplayer executable uses GDK and DemonWare. The enhanced build now
uses BO2-style self-module bootstrap and local filesystem initialization, while every GDK
interface is unavailable. It never loads or forwards to the system runtime, even when Gaming
Services is installed. No authorization is fabricated; a game feature requiring unavailable
authorization can still fail. Playable offline mode is not claimed by this change.

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

### Emulated online protocol

The Store log-on path at `140114B7C` calls `bdLobbyService::connect` at `1403F3980`. Its constructor
sets the flag at connection offset `0x98`, so the connect callback sends the 28-byte protocol 210
hello at `1404087F0`. BO2's emulated lobby uses the legacy 200 handshake and cannot serve this
client unchanged. MW3's client explicitly rejects the legacy reply while waiting for the 210 server header.

`tools/demonware/auth.py` encodes the Store auth response and packed title `0x906` client ticket.
`tools/demonware/lobby210.py` decodes the 210 hello and client proof, constructs the server challenge,
and derives the verification and record key material. `tools/demonware/records210.py` uses the
directional AES-CBC and HMAC keys from the Store record paths at `140408BB0` and `140406E20`.
The differential check runs the original Store ticket deserializer at `1404049D0` and key expansion
at `140406B40` with modeled HMAC-SHA1 primitives. Record tests cover both directions, invalid MACs
and repeated sequences. The game still needs a configured service transport and the native Online task handlers.

## Enhanced build: fail-closed GDK boundary

- `XGameRuntimeLoadModule` resolves the enhanced DLL itself by address, never a system DLL.
- Both initialization exports initialize the local game-directory path using the enhanced
  module's filename, or return an error if that fails. Shutdown is a no-op.
- This mirrors BO2's split between local startup and rejected interface queries, while retaining
  IW5's existing game-folder convention rather than T6's LOCALAPPDATA path.
- `QueryApiImpl` rejects every interface with E_NOINTERFACE and clears the output pointer;
  a null output pointer gets E_POINTER. This applies to XUser, XAsync, XTaskQueue, XStore,
  XPackage and every other GDK family. No system runtime or entitlement provider is invoked.
- Existing local provider source is retained for linkage and research, but not exposed by
  this boundary. No sign-in, license or DLC success is fabricated.
- The existing libHttpClient XCurl provider still rejects all HTTP operations locally.

## Engine startup and vendor transport redirects

The Store engine's initialization at `14026D290` calls `1401A2CA0` before loading
`code_pre_gfx_mp`. The latter calls `XblInitialize` with no explicit task queue.
With the GDK boundary active, the process task queue is unavailable, so the Xbox Live
SDK returns `0x800701AB` and the engine takes its shutdown path.

The enhanced replacement initializes the game directory and the same five engine
task groups with native helpers. The platform queues remain absent and the Store's
sign-in and entitlement paths continue to receive unavailable results. If local
initialization itself fails, the startup routine still reports failure.

| Store entry | Enhanced behavior |
|---|---|
| `1404148F0` DemonWare DNS | Returns zero addresses before Winsock, including for official authentication, lobby and STUN names. |
| `140414D00` DemonWare datagram | Returns the vendor socket's normal failure result before `sendto` or `WSASendTo`. |
| `140449870` DemonWare TCP connect | Returns the vendor socket's connection-failure result before Winsock, including for numeric addresses. |
| `1403E9FE0` DemonWare WinHTTP start | Completes the original request with `ERROR_WINHTTP_CANNOT_CONNECT` before opening a connection. |
| `1403DF670` libHttpClient WinHTTP start | Returns the connection-failure HRESULT before opening the request. |

The engine's own UDP paths remain available. These redirects cover the identified
vendor call paths. Process-wide isolation requires a network trace.


## Faithful reconstruction and verification

The first faithful loader candidate lives in `src/gdk/xgameruntime_thunks.cpp`; its
enhanced override remains separate. The candidate restores the original lazy loader
and HRESULT behavior described in records/names.tsv, not any local service policy.
It is not marked matched pending the original-vs-reconstruction differential results.
The test models the Win32 load API and compares the call arguments, returned HRESULT
and writes to the original module global over cached, successful and failed loads.

The CI job also exports the pinned original instructions for the three loader/query
functions and all named thunks as an artifact using `tools/gdk/disassemble_runtime.py`.
This is evidence for reconstructing the remaining bodies rather than assuming that
matching a GUID and slot proves complete control flow. The enhanced rejection test
reads every actual IID/class pair from the generated thunk catalogue.
