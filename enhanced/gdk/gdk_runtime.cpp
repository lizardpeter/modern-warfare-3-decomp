// BO2-style local runtime bootstrap with a fail-closed GDK interface boundary.
// Only the enhanced DLL itself is resolved; initialization prepares local filesystem
// state. No system runtime, account, license or external provider is opened or queried.
#include "enhanced/gdk.h"

#include <windows.h>

WINBASEAPI void *WINAPI LocalAlloc(UINT flags, size_t bytes);
WINBASEAPI void *WINAPI LocalFree(void *mem);
WINBASEAPI void WINAPI OutputDebugStringA(const char *text);
WINBASEAPI BOOL WINAPI GetModuleHandleExA(DWORD flags, const char *name, HINSTANCE *module);
WINBASEAPI DWORD WINAPI GetModuleFileNameA(HINSTANCE module, char *path, DWORD size);

// The exe's runtime module, set by the loader it replaces (records/names.tsv).
extern HINSTANCE s_xgameRuntimeModule;

namespace
{
// Local filesystem state, independent of every GDK interface.
char s_folder[MAX_PATH];
char s_gamertag[16] = "Unknown Soldier";
}  // namespace

GdkInterface *g_gdkAsync;

const char *Gdk_Gamertag()
{
	return s_gamertag;
}

const char *Gdk_GameFolder()
{
	return s_folder;
}

void *Gdk_Alloc(size_t size)
{
	return LocalAlloc(0x40, size ? size : 1);  // LPTR: fixed, zeroed
}

void Gdk_Free(void *p)
{
	if (p)
		LocalFree(p);
}

void Gdk_Copy(void *dst, const void *src, size_t n)
{
	u8 *d = static_cast<u8 *>(dst);
	const u8 *s = static_cast<const u8 *>(src);
	while (n--)
		*d++ = *s++;
}

void Gdk_Zero(void *dst, size_t n)
{
	u8 *d = static_cast<u8 *>(dst);
	while (n--)
		*d++ = 0;
}

size_t Gdk_StrLen(const char *s)
{
	size_t n = 0;
	while (s && s[n])
		++n;
	return n;
}

bool Gdk_StartsWith(const char *s, const char *prefix)
{
	if (!s)
		return false;
	while (*prefix)
		if (*s++ != *prefix++)
			return false;
	return true;
}

void Gdk_Log(const char *line)
{
	OutputDebugStringA(line);
}

// 1403601E0: resolve the local module only, like T6's enhanced GdkLoadRuntime.
// Never reuse a potentially foreign module value from the original executable.
HRESULT XGameRuntimeLoadModule()
{
	HINSTANCE self = nullptr;
	constexpr DWORD fromAddress = 4, unchangedRefcount = 2;
	s_xgameRuntimeModule = nullptr;
	if (!GetModuleHandleExA(fromAddress | unchangedRefcount,
		reinterpret_cast<const char *>(&XGameRuntimeLoadModule), &self) || !self)
		return GDK_E_GAMERUNTIME_DLL_NOT_FOUND;
	s_xgameRuntimeModule = self;
	return GDK_S_OK;
}

// Initialization is local housekeeping, not an authorization result. Retain IW5's
// existing game-folder convention; no XGameSave or other GDK interface is exposed.
extern "C" HRESULT InitializeApiImpl(u64, u64)
{
	s_folder[0] = '\0';
	HRESULT hr = XGameRuntimeLoadModule();
	if (hr < 0)
		return hr;
	char path[MAX_PATH];
	DWORD n = GetModuleFileNameA(s_xgameRuntimeModule, path, sizeof(path));
	if (!n || n >= sizeof(path))
		return GDK_E_FAIL;
	while (n && path[n - 1] != '\\' && path[n - 1] != '/')
		--n;
	if (!n)
		return GDK_E_FAIL;
	Gdk_Copy(s_folder, path, n);
	s_folder[n] = '\0';
	return GDK_S_OK;
}

extern "C" HRESULT InitializeApiImplEx(u64 version, u64 sdk, u64)
{
	return InitializeApiImpl(version, sdk);
}

extern "C" void UninitializeApiImpl()
{
	// There is no external runtime to shut down.
}

extern "C" void XErrorReport(HRESULT hr, const char *message)
{
	(void)hr;
	if (message)
	{
		Gdk_Log("iw5_enhanced: GDK error: ");
		Gdk_Log(message);
		Gdk_Log("\n");
	}
}

namespace
{
// Bounded, debugger-only telemetry: no account identifiers, tokens, file paths or
// network operations. The GUID bytes are in Windows memory order, not text order.
// Repeated status polls must not consume the budget before a different
// interface is first encountered. Retain the first occurrence of each pair.
struct QueryPair
{
    GdkGuid iid, cls;
    bool hasIid, hasClass;
};
constexpr u32 kQueryTraceLimit = 128;
QueryPair s_seenQueries[kQueryTraceLimit];
u32 s_queryTraceCount;
u32 s_queryTraceLock;

bool EqualGuid(const GdkGuid &saved, const GdkGuid *incoming, bool present)
{
    if (present != (incoming != nullptr))
        return false;
    if (!present)
        return true;
    const u8 *a = reinterpret_cast<const u8 *>(&saved);
    const u8 *b = reinterpret_cast<const u8 *>(incoming);
    for (u32 i = 0; i < sizeof(GdkGuid); ++i)
        if (a[i] != b[i])
            return false;
    return true;
}

bool RememberQuery(const GdkGuid *iid, const GdkGuid *cls)
{
    while (__atomic_exchange_n(&s_queryTraceLock, 1u, __ATOMIC_ACQUIRE))
    {
    }
    bool first = false;
    if (s_queryTraceCount < kQueryTraceLimit)
    {
        u32 i = 0;
        for (; i < s_queryTraceCount; ++i)
        {
            const auto &pair = s_seenQueries[i];
            if (EqualGuid(pair.iid, iid, pair.hasIid) &&
                EqualGuid(pair.cls, cls, pair.hasClass))
                break;
        }
        if (i == s_queryTraceCount)
        {
            auto &pair = s_seenQueries[s_queryTraceCount++];
            pair.hasIid = iid != nullptr;
            pair.hasClass = cls != nullptr;
            if (iid)
                Gdk_Copy(&pair.iid, iid, sizeof(GdkGuid));
            if (cls)
                Gdk_Copy(&pair.cls, cls, sizeof(GdkGuid));
            first = true;
        }
    }
    __atomic_store_n(&s_queryTraceLock, 0u, __ATOMIC_RELEASE);
    return first;
}

char *TraceGuid(char *out, const GdkGuid *guid)
{
	const char hex[] = "0123456789abcdef";
	const u8 *bytes = reinterpret_cast<const u8 *>(guid);
	for (u32 i = 0; i < sizeof(GdkGuid); ++i)
	{
		*out++ = bytes ? hex[bytes[i] >> 4] : '-';
		*out++ = bytes ? hex[bytes[i] & 15] : '-';
	}
	return out;
}

void TraceRejectedQuery(const GdkGuid *iid, const GdkGuid *cls)
{
	if (!RememberQuery(iid, cls))
		return;
	const char prefix[] = "iw5_gdk_rejected iid_le=";
	const char separator[] = " class_le=";
	char line[sizeof(prefix) + 32 + sizeof(separator) + 32 + 1];
	char *p = line;
	for (const char *s = prefix; *s; ++s)
		*p++ = *s;
	p = TraceGuid(p, iid);
	for (const char *s = separator; *s; ++s)
		*p++ = *s;
	p = TraceGuid(p, cls);
	*p++ = '\n';
	*p = '\0';
	Gdk_Log(line);
}
}  // namespace

// Only offline profile and polling completion interfaces are exposed.
extern GdkInterface g_iw5LocalUser, g_iw5LocalGamertag, g_iw5LocalAsync;
#ifdef IW5_CI_LOCAL_RESPONSE
extern GdkInterface g_iw5CiLocalNetwork;
#endif
namespace {
constexpr GdkGuid userIid = {0x01acd177,0x91f9,0x4763,{0xa3,0x8e,0xcc,0xbb,0x55,0xce,0x32,0xe0}};
constexpr GdkGuid userClass = {0x079415e3,0x6727,0x437f,{0x8e,0x9d,0x8f,0x8f,0x9b,0x24,0x39,0xf7}};
constexpr GdkGuid tagClass = {0xcef4fac0,0x7676,0x4a94,{0xa1,0x19,0x4c,0x43,0xf9,0xeb,0x5b,0x74}};
constexpr GdkGuid asyncIid = {0x073b7dcb,0x1fcf,0x4030,{0x94,0xbe,0xe3,0xc9,0xeb,0x62,0x34,0x28}};
#ifdef IW5_CI_LOCAL_RESPONSE
constexpr GdkGuid networkIid = {0x37e56907,0x2f10,0x41e8,{0xb7,0x2f,0x36,0xed,0xb1,0x85,0x33,0x1a}};
constexpr GdkGuid networkClass = {0xbf2346b2,0x39af,0x4658,{0xb5,0xea,0x44,0x71,0x3c,0x7e,0x83,0xb3}};
#endif
}

extern "C" HRESULT QueryApiImpl(const GdkGuid *iid, const GdkGuid *cls, void **result)
{
	if (!result)
		return GDK_E_POINTER;
	*result = nullptr;
	if (EqualGuid(userIid, iid, true) && EqualGuid(userClass, cls, true))
        *result = &g_iw5LocalUser;
    else if (EqualGuid(userIid, iid, true) && EqualGuid(tagClass, cls, true))
        *result = &g_iw5LocalGamertag;
    else if (EqualGuid(asyncIid, iid, true) && EqualGuid(asyncIid, cls, true))
        *result = &g_iw5LocalAsync;
#ifdef IW5_CI_LOCAL_RESPONSE
    else if (EqualGuid(networkIid,iid,true) && EqualGuid(networkClass,cls,true))
        *result = &g_iw5CiLocalNetwork;
#endif
    if (*result) return GDK_S_OK;
	TraceRejectedQuery(iid, cls);
	return GDK_E_NOINTERFACE;
}

// IUnknown for the local interfaces: static objects, nothing to count.
HRESULT Gdk_QueryInterface(GdkInterface *, const GdkGuid *, void **out)
{
	if (out)
		*out = nullptr;
	return GDK_E_NOINTERFACE;
}

u32 Gdk_AddRef(GdkInterface *)
{
	return 1;
}

u32 Gdk_Release(GdkInterface *)
{
	return 1;
}

HRESULT Gdk_NotImplemented()
{
	return GDK_E_NOTIMPL;
}
