# Online services in the Store build and enhanced isolation

The faithful Store multiplayer executable uses GDK and DemonWare. The enhanced build now
uses BO2-style self-module bootstrap and local filesystem initialization. The local profile and polling completion interfaces are available; the other GDK families remain unavailable. It never loads or forwards to the system runtime, even when Gaming
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
- Except for the local user, gamertag and polling completion interface, `QueryApiImpl` rejects interfaces with E_NOINTERFACE and clears the output pointer;
  a null output pointer gets E_POINTER. XStore, XPackage and other external service families remain unavailable. No system runtime or entitlement provider is invoked.
- The legacy provider is not exposed. The local profile has a session-local ID and name, retained handles, local-ID lookup, one-user capacity and polling completion. The compatibility `XUserGetId` slot returns the existing private DemonWare account key for a valid local handle, and `XUserFindUserById` resolves only that key. `XUserGetLocalId` remains the distinct session-local handle key. Both are independent of Xbox account identity. Classic and modern name queries support the local name with an empty modern suffix. It reports Xbox SignedOut, no Store user, and no privilege or token authority. Local work/completion queues, delayed and immediate callbacks, and the default process queue are implemented. Composite ports and generic XAsync provider operations remain unsupported. No platform authorization is supplied. No sign-in, license or DLC success is fabricated.
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


## Local DemonWare (P0008)

`enhanced/DW/local_demonware.cpp`, inside `iw5_enhanced.dll`, loopback only, started by the first lookup of its host
names. Nothing else to run.

| Step | How the game reaches it |
|---|---|
| Resolve | `BdGetHostByName` (1404148F0): `auth3.prod.demonware.net` and `lsg.*.prod.demonware.net` give 127.0.0.1; every other name (the STUN hosts included) still finds nothing. |
| Auth | The log-on posts `{"auth_task":44,"iv_seed":N,"title_id":2310}` to `https://127.0.0.1:443/auth/` (`makeAuthForXBoxOne` builds the URL from the resolved address). `BdBeginWinHttpRequest` (1403E9FE0) hands that one request to the service and delivers the reply as the WinHTTP path would (response buffer, status 200, finalized BD_DONE). The game only speaks HTTPS, and a loopback HTTPS endpoint would need a certificate installed in Windows, so the auth endpoint is answered in process. |
| Tickets | `{"auth_task":45,"code":700,...}`: a client ticket the game's decoder (1404049D0) reads (title 906, account 0009000000000001, "Unknown Soldier", a fresh 24-byte session key) and a random 128-byte server ticket; the service keeps the session key issued with each server ticket. |
| Lobby | `BdConnectStream` (140449870) connects 127.0.0.1:3074 to the service's own port (any other address still fails). Protocol 210 (proof check and record keys: `IW5_DemonWare210Verify`, `enhanced/DW/lobby210.cpp`): the 28-byte hello, the challenge, the client auth frame with the server ticket and its proof (which must match that ticket's session key), the server's proof, then records: AES-128-CBC with a fresh IV, HMAC-SHA1 truncated to 8 bytes, a counter per direction (BDDATA keys). |
| After the handshake | The connection ID header (type 4, a typed uint64; the task manager waits for it, `bdLobbyService::pump` 1403F5370), then a reply per task in order (type 1: typed transaction ID, typed error, typed task ID, results; `bdRemoteTask::handleTaskReply` 1403F18E0). |

Tasks answered so far: `bdTitleUtilities::getServerTime` (service 0x0C, task 6). Every other task gets
`BD_SERVICE_NOT_AVAILABLE` (108) and is traced (`dw: lobby: message type 86 ...`, `dw: lobby: service ... task ...`),
so the next ones to implement can be read from a log.

`tests/service/run.py` builds the service into a small console program, runs it under Wine (CI: the
`local-demonware` job) and plays the game's part with the reference protocol code in `tools/demonware/`: the auth
reply and client ticket, the handshake with the issued ticket, the connection ID, the server-time reply, a
service-not-available reply, refusal of a corrupt record, a proof from another key and a ticket it didn't issue.

### What a log-on still needs

The log-on (`dwLogOnFrame`) starts only for a signed-in player, and the auth request carries that player's token
from `XUserGetTokenAndSignatureAsync`. With the current GDK boundary (every interface unavailable) the game can't
sign anyone in, so it never reaches the service. Signing in is also where the Store build checks the game licence
(the Store context is created for the signed-in player and the licence queried, `1401A5A10`, `1401A4400`), so the
player has to sign in through a runtime whose Store interface is Microsoft's own. Until then the service is
exercised by `tests/service/run.py`, and the task list it must answer comes from a log on a PC with Gaming Services.


## Connection evidence and map-entry gate

The smoke test logs bounded per-API local XUser return values, without logging identity or token data.
The read-only pinned-PE report `tools/gdk/connection_evidence.py` lists original direct callers and
instructions of the connection functions. Neither static call graphs nor interface GUID queries prove an API ran.
The smoke run now enables original engine logging and an active-action echo, and fails unless both the original
client initialization completion and the active-action echo appear in order. The active-action mechanism comes
from the Xbox reference; absence of these markers on Store means **unverified**, not a diagnosis of the failure.
A passing marker gate still needs screenshot and input review for playable gameplay. The existing local service
protocol tests exercise an isolated synthetic client, not an authenticated retail game session. No Store, Xbox
identity, token or privilege authorization has been added or modified in this pass.

### Observed retail user setup (CI 46124549841)

The actual Store multiplayer run recorded XUserAddAsync and XUserAddResult returning S_OK, followed by XUserGetId returning E_NOINTERFACE. No token request was observed. The original user setup callback rejects a failed Xbox ID lookup; a local identity is not an Xbox account. The subsequent compatibility update supplies the private account key at the numeric ID boundary, without changing the token, privilege, Store or sign-in-state results. The new behavior is pending a live run. The original retail session is still unavailable in the isolated environment. Local lookup, capacity, name components and invalid options are tested independently.

### Synthetic token ABI fixture

`tests/native/test_token_mock.cpp` is a standalone host-only fixture with unmistakably synthetic strings. It uses the real local queue adapter and checks the 32-byte x64 result header, string lengths including terminators, result-buffer bounds, callback dispatch, cancellation and one-shot retrieval. It is built only as a separate CI test executable. No game DLL, runtime vtable, token service, network request or entitlement decision uses this fixture. The production token APIs remain unavailable.
