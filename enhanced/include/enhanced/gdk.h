// enhanced/include/enhanced/gdk.h: the enhanced build's local GDK runtime (docs/online.md).
//
// The Store exe reaches the GDK only through QueryApiImpl: each API is a thunk (xgameruntime.lib, named in
// analysis/iw5mp_ship/gdk/thunks.tsv) that asks for an interface by two GUIDs and calls one vtable slot, passing the
// API's own arguments after `this`. The enhanced build loads itself as the runtime (XGameRuntimeLoadModule) and
// answers those queries: XUser, XGameSave, XNetworking and the small APIs locally; the async plumbing (XAsync,
// XTaskQueue), XStore, XPackage and runtime features from the system's xgameruntime.dll, so the licence and DLC
// checks are Microsoft's own. Without a system runtime the game doesn't start, as the original.
#pragma once

typedef long HRESULT;
typedef unsigned long long u64;
typedef unsigned int u32;
typedef unsigned char u8;
typedef __SIZE_TYPE__ size_t;

#define GDK_S_OK ((HRESULT)0)
#define GDK_S_FALSE ((HRESULT)1)
#define GDK_E_FAIL ((HRESULT)0x80004005)
#define GDK_E_NOTIMPL ((HRESULT)0x80004001)
#define GDK_E_NOINTERFACE ((HRESULT)0x80004002)
#define GDK_E_POINTER ((HRESULT)0x80004003)
#define GDK_E_ABORT ((HRESULT)0x80004004)
#define GDK_E_INVALIDARG ((HRESULT)0x80070057)
#define GDK_E_OUTOFMEMORY ((HRESULT)0x8007000E)
#define GDK_E_PENDING ((HRESULT)0x8000000A)
#define GDK_E_NOT_SUFFICIENT_BUFFER ((HRESULT)0x8007007A)
#define GDK_E_NOT_FOUND ((HRESULT)0x80070490)              // HRESULT_FROM_WIN32(ERROR_NOT_FOUND)
#define GDK_E_FILE_NOT_FOUND ((HRESULT)0x80070002)
#define GDK_E_GAMERUNTIME_DLL_NOT_FOUND ((HRESULT)0x89240101)
#define GDK_E_GAMEUSER_NO_TOKEN_REQUIRED ((HRESULT)0x8015DC09)  // no token for that URL: the request needs none

struct GdkGuid
{
	u32 a;
	unsigned short b, c;
	u8 d[8];
};

// XAsync.h, XAsyncProvider.h, XTaskQueue.h
typedef struct XTaskQueueObject *XTaskQueueHandle;
typedef struct XTaskQueuePortObject *XTaskQueuePortHandle;
struct XAsyncBlock;
typedef void XAsyncCompletionRoutine(XAsyncBlock *asyncBlock);
struct XAsyncBlock
{
	XTaskQueueHandle queue;
	void *context;
	XAsyncCompletionRoutine *callback;
	u8 internal[sizeof(void *) * 4];
};
enum class XAsyncOp : u32 { Begin, DoWork, GetResult, Cancel, Cleanup };
struct XAsyncProviderData
{
	XAsyncBlock *async;
	size_t bufferSize;
	void *buffer;
	void *context;
};
typedef HRESULT XAsyncProvider(XAsyncOp op, const XAsyncProviderData *data);
enum class XTaskQueueDispatchMode : u32 { Manual, ThreadPool, SerializedThreadPool, Immediate };
enum class XTaskQueuePort : u32 { Work, Completion };
struct XTaskQueueRegistrationToken
{
	u64 token;
};
typedef void XTaskQueueCallback(void *context, bool canceled);
typedef void XTaskQueueTerminatedCallback(void *context);

// The interface objects QueryApiImpl hands out: COM-shaped, `this` first.
struct GdkInterface
{
	void *const *vtable;
};

// The async plumbing every local provider completes through: the system runtime's XAsync interface (gdk_async.cpp;
// slots as thunks.tsv names them).
HRESULT Gdk_AsyncBegin(XAsyncBlock *async, void *context, const void *identity, const char *identityName,
                       XAsyncProvider *provider);
HRESULT Gdk_AsyncSchedule(XAsyncBlock *async, u32 delayMs);
void Gdk_AsyncComplete(XAsyncBlock *async, HRESULT result, size_t requiredBufferSize);
HRESULT Gdk_AsyncGetResult(XAsyncBlock *async, const void *identity, size_t bufferSize, void *buffer, size_t *used);
HRESULT Gdk_AsyncGetResultSize(XAsyncBlock *async, size_t *bufferSize);

// Begins an operation that completes at once with `result` and `resultSize` bytes, copied from `result_data` by
// the provider's GetResult. The common shape of every local async API.
HRESULT Gdk_AsyncImmediate(XAsyncBlock *async, const void *identity, HRESULT result, const void *resultData,
                           size_t resultSize);

// Interfaces (gdk_*.cpp): XUser, XUser's gamertag, XGameSave, XNetworking, and the small ones.
extern GdkInterface g_gdkUser;
extern GdkInterface g_gdkUserGamertag;
extern GdkInterface g_gdkGameSave;
extern GdkInterface g_gdkNetworking;
extern GdkInterface g_gdkSystem;
extern GdkInterface g_gdkGame;
extern GdkInterface g_gdkGameInvite;
extern GdkInterface g_gdkGameEvent;
extern GdkInterface g_gdkGameUi;

// The system runtime's XAsync interface, set at initialization.
extern GdkInterface *g_gdkAsync;

// Settings (iw5_enhanced.ini next to the DLL): the local player's name.
const char *Gdk_Gamertag();
// The folder the DLL is in, with a trailing backslash.
const char *Gdk_GameFolder();

// The local player: one user, signed in.
struct XUser;
extern XUser g_gdkLocalUser;
u64 Gdk_LocalXuid();

// Small helpers (gdk_runtime.cpp): no CRT here.
void *Gdk_Alloc(size_t size);
void Gdk_Free(void *p);
void Gdk_Copy(void *dst, const void *src, size_t n);
void Gdk_Zero(void *dst, size_t n);
size_t Gdk_StrLen(const char *s);
bool Gdk_StartsWith(const char *s, const char *prefix);
void Gdk_Log(const char *line);
