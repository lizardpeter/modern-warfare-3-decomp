// enhanced/gdk/gdk_runtime.cpp: the enhanced build as the game's GDK runtime (docs/online.md).
//
// The exe loads xgameruntime.dll from System32 only (XGameRuntimeLoadModule, LoadLibraryExA with
// LOAD_LIBRARY_SEARCH_SYSTEM32) and resolves QueryApiImpl, InitializeApiImpl(Ex), XErrorReport and
// UninitializeApiImpl from it. The replacement hands it this DLL instead, which exports the same five entry points.
// QueryApiImpl answers by the two GUIDs each thunk passes (rcx, rdx; analysis/iw5mp_ship/gdk/thunks.tsv):
//
// - local: XUser (and its gamertag interface), XGameSave, XNetworking, XSystem, XGame, XGameInvite, XGameEvent,
//   XGameUI: nothing about the player or the network reaches Microsoft or Activision;
// - the system runtime's: XAsync/XTaskQueue (the plumbing, no network), XStore and XPackage (the game licence and
//   DLC: Microsoft's checks, unchanged) and XGameRuntimeFeature.
//
// Without a system runtime (no Gaming Services) nothing changes from the original: initialization fails and the
// game shows its own "Game Runtime is not installed" message. The enhanced build never runs the game where
// Microsoft's licence check can't.
#include "enhanced/gdk.h"

#include <windows.h>

WINBASEAPI void *WINAPI LocalAlloc(UINT flags, size_t bytes);
WINBASEAPI void *WINAPI LocalFree(void *mem);
WINBASEAPI HINSTANCE WINAPI LoadLibraryExA(const char *name, HANDLE file, DWORD flags);
WINBASEAPI void *WINAPI GetProcAddress(HINSTANCE module, const char *name);
WINBASEAPI BOOL WINAPI GetModuleHandleExA(DWORD flags, const char *name, HINSTANCE *module);
WINBASEAPI DWORD WINAPI GetModuleFileNameA(HINSTANCE module, char *path, DWORD size);
WINBASEAPI HANDLE WINAPI CreateFileA(const char *name, DWORD access, DWORD share, void *security, DWORD disposition,
                                     DWORD flags, HANDLE templ);
WINBASEAPI BOOL WINAPI ReadFile(HANDLE file, void *buffer, DWORD bytes, DWORD *read, void *overlapped);
WINBASEAPI void WINAPI OutputDebugStringA(const char *text);

void *Gdk_WrapStore(void *real);

// The exe's runtime module, set by the loader it replaces (records/names.tsv).
extern HINSTANCE s_xgameRuntimeModule;

namespace
{
typedef HRESULT QueryApiImplFn(const GdkGuid *, const GdkGuid *, void **);
typedef HRESULT InitializeApiImplExFn(u64, u64, u64);
typedef HRESULT InitializeApiImplFn(u64, u64);
typedef void UninitializeApiImplFn();

HINSTANCE s_self;
HINSTANCE s_system;  // the system xgameruntime.dll, if Windows has one
QueryApiImplFn *s_systemQuery;
InitializeApiImplExFn *s_systemInitializeEx;
InitializeApiImplFn *s_systemInitialize;
UninitializeApiImplFn *s_systemUninitialize;
bool s_initialized;
char s_folder[MAX_PATH];
char s_gamertag[16] = "Unknown Soldier";  // a classic gamertag: at most 15 characters

constexpr GdkGuid kAsync = {0x073b7dcb, 0x1fcf, 0x4030, {0x94, 0xbe, 0xe3, 0xc9, 0xeb, 0x62, 0x34, 0x28}};
constexpr GdkGuid kUser = {0x01acd177, 0x91f9, 0x4763, {0xa3, 0x8e, 0xcc, 0xbb, 0x55, 0xce, 0x32, 0xe0}};
constexpr GdkGuid kUserImpl = {0x079415e3, 0x6727, 0x437f, {0x8e, 0x9d, 0x8f, 0x8f, 0x9b, 0x24, 0x39, 0xf7}};
constexpr GdkGuid kUserGamertag = {0xcef4fac0, 0x7676, 0x4a94, {0xa1, 0x19, 0x4c, 0x43, 0xf9, 0xeb, 0x5b, 0x74}};
constexpr GdkGuid kGameSave = {0x704c3f58, 0xe629, 0x4cc2, {0xb1, 0x97, 0x30, 0x51, 0x1b, 0x99, 0x6f, 0xe2}};
constexpr GdkGuid kNetworking = {0x37e56907, 0x2f10, 0x41e8, {0xb7, 0x2f, 0x36, 0xed, 0xb1, 0x85, 0x33, 0x1a}};
constexpr GdkGuid kStore = {0x0dd112ac, 0x7c24, 0x448c, {0xb9, 0x2b, 0x39, 0x60, 0xfb, 0x5b, 0xd3, 0x0c}};
constexpr GdkGuid kPackage = {0xaf406016, 0xe850, 0x4aa8, {0xa8, 0x8d, 0x2f, 0x3d, 0xcb, 0x9d, 0xac, 0x7e}};
constexpr GdkGuid kFeature = {0x8836fe87, 0xedb9, 0x4fe3, {0x8d, 0xad, 0x05, 0xf0, 0xd2, 0xcd, 0x5b, 0x40}};
constexpr GdkGuid kSystem = {0xe349bd1a, 0xfc20, 0x4e40, {0xb9, 0x9c, 0x41, 0x78, 0xcc, 0x6b, 0x40, 0x9f}};
constexpr GdkGuid kGame = {0x973a344e, 0x24bf, 0x4d0f, {0x84, 0x57, 0x56, 0xc5, 0x34, 0x89, 0x2b, 0x29}};
constexpr GdkGuid kGameInvite = {0x0651aae2, 0x4012, 0x4077, {0xbf, 0x84, 0x8b, 0x90, 0x97, 0x09, 0x0e, 0x2c}};
constexpr GdkGuid kGameEvent = {0xbbfbdcc7, 0xbfe7, 0x409b, {0xa5, 0xca, 0xed, 0xf0, 0x54, 0x96, 0x0b, 0x4d}};
constexpr GdkGuid kGameUi = {0xdfcd4649, 0x4ff8, 0x4043, {0xba, 0x07, 0x35, 0xd6, 0x07, 0xdf, 0x98, 0xb0}};

bool Same(const GdkGuid *a, const GdkGuid &b)
{
	if (!a)
		return false;
	const u8 *x = reinterpret_cast<const u8 *>(a), *y = reinterpret_cast<const u8 *>(&b);
	for (int i = 0; i < 16; ++i)
		if (x[i] != y[i])
			return false;
	return true;
}

// iw5_enhanced.ini next to the DLL, one setting: `name=...` (the local player's gamertag)
void ReadSettings()
{
	char path[MAX_PATH + 32];
	size_t n = Gdk_StrLen(s_folder);
	Gdk_Copy(path, s_folder, n);
	Gdk_Copy(path + n, "iw5_enhanced.ini", 17);
	HANDLE file = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
	if (file == INVALID_HANDLE_VALUE)
		return;
	char text[4096];
	DWORD read = 0;
	ReadFile(file, text, sizeof(text) - 1, &read, nullptr);
	CloseHandle(file);
	text[read] = 0;
	for (char *line = text; *line;)
	{
		char *end = line;
		while (*end && *end != '\n' && *end != '\r')
			++end;
		char saved = *end;
		*end = 0;
		while (*line == ' ' || *line == '\t')
			++line;
		if (Gdk_StartsWith(line, "name="))
		{
			const char *value = line + 5;
			size_t len = Gdk_StrLen(value);
			while (len && (value[len - 1] == ' ' || value[len - 1] == '\t'))
				--len;
			if (len)
			{
				if (len > 15)
					len = 15;
				Gdk_Copy(s_gamertag, value, len);
				s_gamertag[len] = 0;
			}
		}
		*end = saved;
		line = *end ? end + 1 : end;
	}
}
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

// 1403601E0: hand the exe this DLL as its runtime. The original loads System32's xgameruntime.dll and fails with
// E_GAMERUNTIME_DLL_NOT_FOUND without one; this always succeeds, and InitializeApiImpl(Ex) below decides what the
// system's runtime is used for.
HRESULT XGameRuntimeLoadModule()
{
	if (!s_xgameRuntimeModule)
	{
		constexpr DWORD fromAddress = 4, unchangedRefcount = 2;  // GET_MODULE_HANDLE_EX_FLAG_*
		GetModuleHandleExA(fromAddress | unchangedRefcount, reinterpret_cast<const char *>(&XGameRuntimeLoadModule),
		                   &s_xgameRuntimeModule);
	}
	return s_xgameRuntimeModule ? GDK_S_OK : GDK_E_GAMERUNTIME_DLL_NOT_FOUND;
}

static HRESULT Initialize(u64 version, u64 sdk, u64 flags, bool extended)
{
	if (s_initialized)
		return GDK_S_OK;
	constexpr DWORD fromAddress = 4, unchangedRefcount = 2;
	GetModuleHandleExA(fromAddress | unchangedRefcount, reinterpret_cast<const char *>(&Initialize), &s_self);
	DWORD n = GetModuleFileNameA(s_self, s_folder, sizeof(s_folder));
	while (n && s_folder[n - 1] != '\\' && s_folder[n - 1] != '/')
		--n;
	s_folder[n] = 0;
	ReadSettings();

	// The system runtime, exactly as the original would have loaded it (System32 only).
	s_system = LoadLibraryExA("xgameruntime.dll", nullptr, 0x800);
	if (!s_system)
		return GDK_E_GAMERUNTIME_DLL_NOT_FOUND;  // as the original, which needs the system runtime to start
	{
		s_systemQuery = reinterpret_cast<QueryApiImplFn *>(GetProcAddress(s_system, "QueryApiImpl"));
		s_systemInitializeEx = reinterpret_cast<InitializeApiImplExFn *>(GetProcAddress(s_system, "InitializeApiImplEx"));
		s_systemInitialize = reinterpret_cast<InitializeApiImplFn *>(GetProcAddress(s_system, "InitializeApiImpl"));
		s_systemUninitialize = reinterpret_cast<UninitializeApiImplFn *>(GetProcAddress(s_system, "UninitializeApiImpl"));
		HRESULT hr = GDK_E_GAMERUNTIME_DLL_NOT_FOUND;
		if (s_systemQuery && s_systemUninitialize)
		{
			if (extended && s_systemInitializeEx)
				hr = s_systemInitializeEx(version, sdk, flags);
			else if (s_systemInitialize)
				hr = s_systemInitialize(version, sdk);
		}
		if (hr < 0)
		{
			// A runtime that won't start is no runtime: report it like the original would, so the game's own
			// "Game Runtime initialization failed" message names the system's error.
			return hr;
		}
		void *async = nullptr;
		if (s_systemQuery(&kAsync, &kAsync, &async) < 0 || !async)
			return GDK_E_GAMERUNTIME_DLL_NOT_FOUND;
		g_gdkAsync = static_cast<GdkInterface *>(async);  // held for the process's life
	}
	Gdk_Log("iw5_enhanced: GDK: player, saves and network local; async, Store and packages from the system runtime\n");
	s_initialized = true;
	return GDK_S_OK;
}

extern "C" HRESULT InitializeApiImplEx(u64 version, u64 sdk, u64 flags)
{
	return Initialize(version, sdk, flags, true);
}

extern "C" HRESULT InitializeApiImpl(u64 version, u64 sdk)
{
	return Initialize(version, sdk, 0, false);
}

extern "C" void UninitializeApiImpl()
{
	if (s_system && s_systemUninitialize)
		s_systemUninitialize();
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

extern "C" HRESULT QueryApiImpl(const GdkGuid *first, const GdkGuid *second, void **result)
{
	if (!result)
		return GDK_E_POINTER;
	*result = nullptr;
	GdkInterface *local = nullptr;
	if (Same(first, kUser))
		local = Same(second, kUserGamertag) ? &g_gdkUserGamertag : Same(second, kUserImpl) ? &g_gdkUser : nullptr;
	else if (Same(first, kGameSave))
		local = &g_gdkGameSave;
	else if (Same(first, kNetworking))
		local = &g_gdkNetworking;
	else if (Same(first, kSystem))
		local = &g_gdkSystem;
	else if (Same(first, kGame))
		local = &g_gdkGame;
	else if (Same(first, kGameInvite))
		local = &g_gdkGameInvite;
	else if (Same(first, kGameEvent))
		local = &g_gdkGameEvent;
	else if (Same(first, kGameUi))
		local = &g_gdkGameUi;
	if (local)
	{
		*result = local;
		return GDK_S_OK;
	}
	if (s_systemQuery && Same(first, kStore))
	{
		HRESULT hr = s_systemQuery(first, second, result);
		if (hr >= 0)
			*result = Gdk_WrapStore(*result);  // XStoreCreateContext without the local player (gdk_misc.cpp)
		return hr;
	}
	if (s_systemQuery && (Same(first, kAsync) || Same(first, kPackage) || Same(first, kFeature)))
		return s_systemQuery(first, second, result);
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
