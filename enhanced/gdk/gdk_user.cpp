// enhanced/gdk/gdk_user.cpp: XUser for one local player (docs/online.md).
//
// The original signs the player in to Xbox Live (XUserAddAsync with the Windows account) and gets Xbox tokens for
// DemonWare and Xbox services. Here there is one player, always signed in: "Unknown Soldier" unless
// iw5_enhanced.ini names another (`name=`), with a fixed local XUID, every privilege, and a token only for the local
// DemonWare's auth URL. Anything else asks for a token for an Xbox Live service, which gets "network unreachable":
// nothing leaves the machine with the player's identity.
#include "enhanced/gdk.h"

HRESULT Gdk_QueryInterface(GdkInterface *, const GdkGuid *, void **);
u32 Gdk_AddRef(GdkInterface *);
u32 Gdk_Release(GdkInterface *);
HRESULT Gdk_NotImplemented();

struct XUser
{
	u32 tag;
};
XUser g_gdkLocalUser = {0x52455355};  // "USER"

namespace
{
typedef XUser *XUserHandle;

constexpr HRESULT kNetworkUnreachable = (HRESULT)0x800704CF;  // HRESULT_FROM_WIN32(ERROR_NETWORK_UNREACHABLE)
constexpr char kAuthUrl[] = "https://auth3.prod.demonware.net";  // bdHTTPAuthService's token URL (140405490)

// One identity per API: the result calls name the same one as the begin calls (XAsyncGetResult checks).
const char kAddIdentity[] = "XUserAddAsync";
const char kPictureIdentity[] = "XUserGetGamerPictureAsync";
const char kPrivilegeIdentity[] = "XUserResolvePrivilegeWithUiAsync";
const char kIssueIdentity[] = "XUserResolveIssueWithUiAsync";
const char kTokenIdentity[] = "XUserGetTokenAndSignatureAsync";

bool IsLocal(XUserHandle user)
{
	return user == &g_gdkLocalUser;
}

HRESULT DuplicateHandle(GdkInterface *, XUserHandle user, XUserHandle *out)
{
	if (!out)
		return GDK_E_POINTER;
	*out = user;
	return IsLocal(user) ? GDK_S_OK : GDK_E_INVALIDARG;
}

void CloseUserHandle(GdkInterface *, XUserHandle) {}

int CompareUsers(GdkInterface *, XUserHandle a, XUserHandle b)
{
	return a == b ? 0 : a < b ? -1 : 1;
}

HRESULT AddAsync(GdkInterface *, u32, XAsyncBlock *async)
{
	XUserHandle user = &g_gdkLocalUser;
	return Gdk_AsyncImmediate(async, kAddIdentity, GDK_S_OK, &user, sizeof(user));
}

HRESULT AddResult(GdkInterface *, XAsyncBlock *async, XUserHandle *user)
{
	*user = nullptr;
	return Gdk_AsyncGetResult(async, kAddIdentity, sizeof(*user), user, nullptr);
}

HRESULT GetLocalId(GdkInterface *, XUserHandle user, u64 *localId)
{
	*localId = 1;
	return IsLocal(user) ? GDK_S_OK : GDK_E_INVALIDARG;
}

HRESULT GetId(GdkInterface *, XUserHandle user, u64 *id)
{
	*id = Gdk_LocalXuid();
	return IsLocal(user) ? GDK_S_OK : GDK_E_INVALIDARG;
}

HRESULT GetIsGuest(GdkInterface *, XUserHandle, bool *guest)
{
	*guest = false;
	return GDK_S_OK;
}

HRESULT GetState(GdkInterface *, XUserHandle user, u32 *state)
{
	*state = IsLocal(user) ? 0 : 2;  // SignedIn, SignedOut
	return GDK_S_OK;
}

HRESULT GamerPictureAsync(GdkInterface *, XUserHandle, u32, XAsyncBlock *async)
{
	return Gdk_AsyncImmediate(async, kPictureIdentity, kNetworkUnreachable, nullptr, 0);
}

HRESULT GamerPictureResultSize(GdkInterface *, XAsyncBlock *async, size_t *size)
{
	return Gdk_AsyncGetResultSize(async, size);
}

HRESULT GamerPictureResult(GdkInterface *, XAsyncBlock *async, size_t size, void *buffer, size_t *used)
{
	return Gdk_AsyncGetResult(async, kPictureIdentity, size, buffer, used);
}

HRESULT GetAgeGroup(GdkInterface *, XUserHandle, u32 *group)
{
	*group = 3;  // Adult
	return GDK_S_OK;
}

HRESULT CheckPrivilege(GdkInterface *, XUserHandle, u32, u32, bool *has, u32 *reason)
{
	if (has)
		*has = true;
	if (reason)
		*reason = 0;  // None
	return GDK_S_OK;
}

HRESULT ResolvePrivilegeAsync(GdkInterface *, XUserHandle, u32, u32, XAsyncBlock *async)
{
	return Gdk_AsyncImmediate(async, kPrivilegeIdentity, GDK_S_OK, nullptr, 0);
}

HRESULT ResolvePrivilegeResult(GdkInterface *, XAsyncBlock *async)
{
	return Gdk_AsyncGetResult(async, kPrivilegeIdentity, 0, nullptr, nullptr);
}

HRESULT ResolveIssueAsync(GdkInterface *, XUserHandle, const char *, XAsyncBlock *async)
{
	return Gdk_AsyncImmediate(async, kIssueIdentity, GDK_S_OK, nullptr, 0);
}

HRESULT ResolveIssueResult(GdkInterface *, XAsyncBlock *async)
{
	return Gdk_AsyncGetResult(async, kIssueIdentity, 0, nullptr, nullptr);
}

// The token result: XUserGetTokenAndSignatureData followed by the token and signature strings, its pointers into
// the caller's own buffer. The local DemonWare (its auth endpoint) accepts this token for the local player.
struct TokenData
{
	size_t tokenSize;
	size_t signatureSize;
	const char *token;
	const char *signature;
};

struct TokenOp
{
	HRESULT result;
	size_t tokenSize;  // with the terminator
	char token[96];
};

size_t TokenResultBytes(const TokenOp *op)
{
	return sizeof(TokenData) + op->tokenSize + 1;  // an empty signature
}

HRESULT TokenProvider(XAsyncOp op, const XAsyncProviderData *data)
{
	TokenOp *t = static_cast<TokenOp *>(data->context);
	switch (op)
	{
	case XAsyncOp::Begin:
		return Gdk_AsyncSchedule(data->async, 0);
	case XAsyncOp::DoWork:
		Gdk_AsyncComplete(data->async, t->result, t->result >= 0 ? TokenResultBytes(t) : 0);
		return GDK_S_OK;
	case XAsyncOp::GetResult:
	{
		TokenData *out = static_cast<TokenData *>(data->buffer);
		char *strings = reinterpret_cast<char *>(out + 1);
		Gdk_Copy(strings, t->token, t->tokenSize);
		strings[t->tokenSize] = 0;
		out->tokenSize = t->tokenSize;
		out->signatureSize = 1;
		out->token = strings;
		out->signature = strings + t->tokenSize;
		return GDK_S_OK;
	}
	case XAsyncOp::Cleanup:
		Gdk_Free(t);
		return GDK_S_OK;
	default:
		return GDK_S_OK;
	}
}

void AppendHex(char *&p, u64 v)
{
	for (int shift = 60; shift >= 0; shift -= 4)
		*p++ = "0123456789ABCDEF"[(v >> shift) & 15];
}

HRESULT TokenAsync(GdkInterface *, XUserHandle user, u32, const char *, const char *url, size_t, const void *, size_t,
                   const void *, XAsyncBlock *async)
{
	TokenOp *t = static_cast<TokenOp *>(Gdk_Alloc(sizeof(TokenOp)));
	if (!t)
		return GDK_E_OUTOFMEMORY;
	if (IsLocal(user) && Gdk_StartsWith(url, kAuthUrl))
	{
		// "XBL3.0 x=<xuid>;iw5-local": the shape of an Xbox token header, naming the local player.
		char *p = t->token;
		const char prefix[] = "XBL3.0 x=";
		Gdk_Copy(p, prefix, sizeof(prefix) - 1);
		p += sizeof(prefix) - 1;
		AppendHex(p, Gdk_LocalXuid());
		const char suffix[] = ";iw5-local";
		Gdk_Copy(p, suffix, sizeof(suffix));
		p += sizeof(suffix);
		t->tokenSize = p - t->token;
		t->result = GDK_S_OK;
	}
	else
		t->result = kNetworkUnreachable;
	return Gdk_AsyncBegin(async, t, kTokenIdentity, nullptr, TokenProvider);
}

HRESULT TokenResultSize(GdkInterface *, XAsyncBlock *async, size_t *size)
{
	return Gdk_AsyncGetResultSize(async, size);
}

HRESULT TokenResult(GdkInterface *, XAsyncBlock *async, size_t size, void *buffer, TokenData **data, size_t *used)
{
	if (data)
		*data = nullptr;
	HRESULT hr = Gdk_AsyncGetResult(async, kTokenIdentity, size, buffer, used);
	if (hr >= 0 && data)
		*data = static_cast<TokenData *>(buffer);
	return hr;
}

HRESULT RegisterForChangeEvent(GdkInterface *, XTaskQueueHandle, void *, void *, XTaskQueueRegistrationToken *token)
{
	static u64 next;
	token->token = __atomic_add_fetch(&next, 1, __ATOMIC_RELAXED);  // the local player never changes
	return GDK_S_OK;
}

bool UnregisterForChangeEvent(GdkInterface *, u64, bool)
{
	return true;
}

bool IsStoreUser(GdkInterface *, XUserHandle user)
{
	return IsLocal(user);
}

HRESULT GetGamertag(GdkInterface *, XUserHandle user, u32, size_t size, char *gamertag, size_t *used)
{
	const char *name = Gdk_Gamertag();
	size_t n = Gdk_StrLen(name) + 1;
	if (used)
		*used = n;
	if (!IsLocal(user))
		return GDK_E_INVALIDARG;
	if (size < n)
		return GDK_E_NOT_SUFFICIENT_BUFFER;
	Gdk_Copy(gamertag, name, n);
	return GDK_S_OK;
}
}  // namespace

u64 Gdk_LocalXuid()
{
	return 0x0009000000000001ull;  // fixed, so saves and stats don't depend on the chosen name
}

// Slots from analysis/iw5mp_ship/gdk/thunks.tsv (01acd177/079415e3).
static void *const s_userVtable[] = {
	(void *)Gdk_QueryInterface, (void *)Gdk_AddRef, (void *)Gdk_Release,
	(void *)DuplicateHandle,        // 0x18
	(void *)CloseUserHandle,        // 0x20
	(void *)CompareUsers,           // 0x28
	(void *)Gdk_NotImplemented,     // 0x30 XUserGetMaxUsers
	(void *)AddAsync,               // 0x38
	(void *)AddResult,              // 0x40
	(void *)GetLocalId,             // 0x48
	(void *)Gdk_NotImplemented,     // 0x50 XUserFindUserByLocalId
	(void *)GetId,                  // 0x58
	(void *)Gdk_NotImplemented,     // 0x60 XUserFindUserById
	(void *)GetIsGuest,             // 0x68
	(void *)GetState,               // 0x70
	(void *)Gdk_NotImplemented,     // 0x78
	(void *)GamerPictureAsync,      // 0x80
	(void *)GamerPictureResultSize, // 0x88
	(void *)GamerPictureResult,     // 0x90
	(void *)GetAgeGroup,            // 0x98
	(void *)CheckPrivilege,         // 0xA0
	(void *)ResolvePrivilegeAsync,  // 0xA8
	(void *)ResolvePrivilegeResult, // 0xB0
	(void *)TokenAsync,             // 0xB8
	(void *)TokenResultSize,        // 0xC0
	(void *)TokenResult,            // 0xC8
	(void *)Gdk_NotImplemented,     // 0xD0
	(void *)Gdk_NotImplemented,     // 0xD8
	(void *)Gdk_NotImplemented,     // 0xE0
	(void *)ResolveIssueAsync,      // 0xE8
	(void *)ResolveIssueResult,     // 0xF0
	(void *)Gdk_NotImplemented,     // 0xF8
	(void *)Gdk_NotImplemented,     // 0x100
	(void *)RegisterForChangeEvent, // 0x108
	(void *)UnregisterForChangeEvent, // 0x110
	(void *)Gdk_NotImplemented,     // 0x118
	(void *)Gdk_NotImplemented,     // 0x120
	(void *)Gdk_NotImplemented,     // 0x128
	(void *)Gdk_NotImplemented,     // 0x130
	(void *)Gdk_NotImplemented,     // 0x138
	(void *)Gdk_NotImplemented,     // 0x140
	(void *)Gdk_NotImplemented,     // 0x148
	(void *)IsStoreUser,            // 0x150
};
GdkInterface g_gdkUser = {s_userVtable};

// 01acd177/cef4fac0: the interface XUserGetGamertag is on (slot 0x18).
static void *const s_userGamertagVtable[] = {
	(void *)Gdk_QueryInterface, (void *)Gdk_AddRef, (void *)Gdk_Release,
	(void *)GetGamertag,            // 0x18
};
GdkInterface g_gdkUserGamertag = {s_userGamertagVtable};
