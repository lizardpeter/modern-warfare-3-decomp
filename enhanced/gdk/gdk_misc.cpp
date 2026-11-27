// enhanced/gdk/gdk_misc.cpp: the small GDK interfaces, and the Store interface's one translation (docs/online.md).
//
// - XNetworking: connected (the local DemonWare is reached without leaving the machine), no certificate pinning
//   for a request that never goes out, no connectivity changes.
// - XSystem, XGame: the retail sandbox and the game's own title id (MicrosoftGame.Config, 66CB4BCA).
// - XGameInvite, XGameEvent, XGameUI: invites never arrive, telemetry events go nowhere, the profile card isn't shown.
// - XStore, with a system runtime: Microsoft's own interface, except that XStoreCreateContext gets no user. The
//   game passes the player it signed in, which is the local player here, not an Xbox account the Store knows; with
//   no user, the PC Store uses the account signed in to the Microsoft Store app, as the licence check always does.
#include "enhanced/gdk.h"

HRESULT Gdk_QueryInterface(GdkInterface *, const GdkGuid *, void **);
u32 Gdk_AddRef(GdkInterface *);
u32 Gdk_Release(GdkInterface *);
HRESULT Gdk_NotImplemented();

namespace
{
const char kSecurityIdentity[] = "XNetworkingQuerySecurityInformationForUrlAsync";
const char kSecurity16Identity[] = "XNetworkingQuerySecurityInformationForUrlUtf16Async";
const char kProfileCardIdentity[] = "XGameUiShowPlayerProfileCardAsync";

struct SecurityInformation
{
	u32 enabledHttpSecurityProtocolFlags;
	size_t thumbprintCount;
	void *thumbprints;
};

SecurityInformation Security()
{
	SecurityInformation s = {};
	s.enabledHttpSecurityProtocolFlags = 0x800 | 0x2000;  // WINHTTP_FLAG_SECURE_PROTOCOL_TLS1_2 | TLS1_3
	return s;
}

HRESULT SecurityAsync(GdkInterface *, const char *, XAsyncBlock *async)
{
	SecurityInformation s = Security();
	return Gdk_AsyncImmediate(async, kSecurityIdentity, GDK_S_OK, &s, sizeof(s));
}

HRESULT SecurityAsync16(GdkInterface *, const wchar_t *, XAsyncBlock *async)
{
	SecurityInformation s = Security();
	return Gdk_AsyncImmediate(async, kSecurity16Identity, GDK_S_OK, &s, sizeof(s));
}

HRESULT SecurityResultSize(GdkInterface *, XAsyncBlock *async, size_t *size)
{
	return Gdk_AsyncGetResultSize(async, size);
}

HRESULT SecurityResult(GdkInterface *, XAsyncBlock *async, size_t size, size_t *used, u8 *buffer,
                       SecurityInformation **info)
{
	if (info)
		*info = nullptr;
	HRESULT hr = Gdk_AsyncGetResult(async, kSecurityIdentity, size, buffer, used);
	if (hr >= 0 && info)
		*info = reinterpret_cast<SecurityInformation *>(buffer);
	return hr;
}

HRESULT SecurityResult16(GdkInterface *, XAsyncBlock *async, size_t size, size_t *used, u8 *buffer,
                         SecurityInformation **info)
{
	if (info)
		*info = nullptr;
	HRESULT hr = Gdk_AsyncGetResult(async, kSecurity16Identity, size, buffer, used);
	if (hr >= 0 && info)
		*info = reinterpret_cast<SecurityInformation *>(buffer);
	return hr;
}

HRESULT VerifyServerCertificate(GdkInterface *, void *, const SecurityInformation *)
{
	return GDK_S_OK;
}

struct ConnectivityHint
{
	u32 connectivityLevel;
	u32 connectivityCost;
	u32 ianaInterfaceType;
	bool networkInitialized;
	bool approachingDataLimit;
	bool overDataLimit;
	bool roaming;
};

HRESULT GetConnectivityHint(GdkInterface *, ConnectivityHint *hint)
{
	Gdk_Zero(hint, sizeof(*hint));
	hint->connectivityLevel = 3;  // InternetAccess: the game's services (the local DemonWare) are reachable
	hint->connectivityCost = 1;   // Unrestricted
	hint->ianaInterfaceType = 6;  // Ethernet
	hint->networkInitialized = true;
	return GDK_S_OK;
}

u64 NextToken()
{
	static u64 next;
	return __atomic_add_fetch(&next, 1, __ATOMIC_RELAXED);
}

HRESULT RegisterNever(GdkInterface *, XTaskQueueHandle, void *, void *, XTaskQueueRegistrationToken *token)
{
	token->token = NextToken();
	return GDK_S_OK;
}

bool UnregisterNever(GdkInterface *, u64, bool)
{
	return true;
}

HRESULT GetSandboxId(GdkInterface *, size_t size, char *sandbox, size_t *used)
{
	const char retail[] = "RETAIL";
	if (used)
		*used = sizeof(retail);
	if (size < sizeof(retail))
		return GDK_E_NOT_SUFFICIENT_BUFFER;
	Gdk_Copy(sandbox, retail, sizeof(retail));
	return GDK_S_OK;
}

HRESULT GetTitleId(GdkInterface *, u32 *titleId)
{
	*titleId = 0x66CB4BCA;
	return GDK_S_OK;
}

HRESULT EventWrite(GdkInterface *, void *, const char *, const char *, const char *, const char *, const char *)
{
	return GDK_S_OK;
}

HRESULT ShowProfileCardAsync(GdkInterface *, XAsyncBlock *async, void *, u64)
{
	return Gdk_AsyncImmediate(async, kProfileCardIdentity, GDK_S_OK, nullptr, 0);
}

}  // namespace

// 37e56907/bf2346b2
static void *const s_networkingVtable[] = {
	(void *)Gdk_QueryInterface, (void *)Gdk_AddRef, (void *)Gdk_Release,
	(void *)Gdk_NotImplemented,        // 0x18
	(void *)Gdk_NotImplemented,        // 0x20
	(void *)Gdk_NotImplemented,        // 0x28
	(void *)Gdk_NotImplemented,        // 0x30
	(void *)Gdk_NotImplemented,        // 0x38
	(void *)SecurityAsync,             // 0x40
	(void *)SecurityResultSize,        // 0x48
	(void *)SecurityResult,            // 0x50
	(void *)SecurityAsync16,           // 0x58
	(void *)SecurityResultSize,        // 0x60
	(void *)SecurityResult16,          // 0x68
	(void *)VerifyServerCertificate,   // 0x70
	(void *)GetConnectivityHint,       // 0x78
	(void *)RegisterNever,             // 0x80
	(void *)UnregisterNever,           // 0x88
};
GdkInterface g_gdkNetworking = {s_networkingVtable};

static void *const s_systemVtable[] = {
	(void *)Gdk_QueryInterface, (void *)Gdk_AddRef, (void *)Gdk_Release,
	(void *)Gdk_NotImplemented,  // 0x18
	(void *)GetSandboxId,        // 0x20
};
GdkInterface g_gdkSystem = {s_systemVtable};

static void *const s_gameVtable[] = {
	(void *)Gdk_QueryInterface, (void *)Gdk_AddRef, (void *)Gdk_Release,
	(void *)GetTitleId,          // 0x18
};
GdkInterface g_gdkGame = {s_gameVtable};

static void *const s_gameInviteVtable[] = {
	(void *)Gdk_QueryInterface, (void *)Gdk_AddRef, (void *)Gdk_Release,
	(void *)RegisterNever,       // 0x18
	(void *)UnregisterNever,     // 0x20
};
GdkInterface g_gdkGameInvite = {s_gameInviteVtable};

static void *const s_gameEventVtable[] = {
	(void *)Gdk_QueryInterface, (void *)Gdk_AddRef, (void *)Gdk_Release,
	(void *)EventWrite,          // 0x18
};
GdkInterface g_gdkGameEvent = {s_gameEventVtable};

static void *const s_gameUiVtable[] = {
	(void *)Gdk_QueryInterface, (void *)Gdk_AddRef, (void *)Gdk_Release,
	(void *)Gdk_NotImplemented,  // 0x18
	(void *)Gdk_NotImplemented,  // 0x20
	(void *)Gdk_NotImplemented,  // 0x28
	(void *)Gdk_NotImplemented,  // 0x30
	(void *)ShowProfileCardAsync,  // 0x38
};
GdkInterface g_gdkGameUi = {s_gameUiVtable};

// --- XStore through the system runtime ------------------------------------------------------------------------------
//
// A proxy per query (the game releases each interface after one call): every slot jumps to the system object's
// own, with `this` swapped; Release releases the system object and the proxy; XStoreCreateContext drops the user.

struct StoreProxy
{
	void *const *vtable;
	GdkInterface *real;
};

#define IW5_STORE_SLOTS(X)                                                                                         \
	X(4) X(5) X(6) X(7) X(8) X(9) X(10) X(11) X(12) X(13) X(14) X(15) X(16) X(17) X(18) X(19) X(20) X(21)    \
	X(22) X(23) X(24) X(25) X(26) X(27) X(28) X(29) X(30) X(31) X(32) X(33) X(34) X(35) X(36) X(37) X(38) X(39)
#define IW5_STORE_DECLARE(k) extern "C" void iw5_store_forward_##k();
IW5_STORE_SLOTS(IW5_STORE_DECLARE)

asm(R"(
	.text
	.irp k, 4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39
	.globl iw5_store_forward_\k
	.p2align 4
iw5_store_forward_\k:
	movq 8(%rcx), %rcx
	movq (%rcx), %rax
	jmpq *(8*\k)(%rax)
	.endr
)");

static u32 StoreProxyAddRef(StoreProxy *p)
{
	typedef u32 F(GdkInterface *);
	return reinterpret_cast<F *>(p->real->vtable[1])(p->real);
}

static u32 StoreProxyRelease(StoreProxy *p)
{
	typedef u32 F(GdkInterface *);
	GdkInterface *real = p->real;
	Gdk_Free(p);
	return reinterpret_cast<F *>(real->vtable[2])(real);
}

static HRESULT StoreProxyCreateContext(StoreProxy *p, void *, void **context)
{
	typedef HRESULT F(GdkInterface *, void *, void **);
	return reinterpret_cast<F *>(p->real->vtable[3])(p->real, nullptr, context);
}

#define IW5_STORE_ENTRY(k) (void *)iw5_store_forward_##k,
static void *const s_storeProxyVtable[] = {
	(void *)Gdk_QueryInterface, (void *)StoreProxyAddRef, (void *)StoreProxyRelease,
	(void *)StoreProxyCreateContext,  // 0x18 XStoreCreateContext
	IW5_STORE_SLOTS(IW5_STORE_ENTRY)
};

void *Gdk_WrapStore(void *real)
{
	if (!real)
		return nullptr;
	StoreProxy *p = static_cast<StoreProxy *>(Gdk_Alloc(sizeof(StoreProxy)));
	if (!p)
		return real;
	p->real = static_cast<GdkInterface *>(real);
	p->vtable = s_storeProxyVtable;
	return p;
}
