// Enhanced GDK isolation boundary. The original system runtime is intentionally unavailable.
// All loader, initialization and interface requests fail locally; no provider or entitlement
// is synthesized, and no system runtime is opened or queried. See docs/online.md.
#include "enhanced/gdk.h"

#include <windows.h>

WINBASEAPI void *WINAPI LocalAlloc(UINT flags, size_t bytes);
WINBASEAPI void *WINAPI LocalFree(void *mem);
WINBASEAPI void WINAPI OutputDebugStringA(const char *text);

// The exe's runtime module, set by the loader it replaces (records/names.tsv).
extern HINSTANCE s_xgameRuntimeModule;

namespace
{
// Retain the inert provider helpers for the enhanced DLL's existing link contract.
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

// 1403601E0: behave as though the runtime is absent, even if installed on the host.
HRESULT XGameRuntimeLoadModule()
{
	s_xgameRuntimeModule = nullptr;
	return GDK_E_GAMERUNTIME_DLL_NOT_FOUND;
}

extern "C" HRESULT InitializeApiImplEx(u64, u64, u64)
{
	return GDK_E_GAMERUNTIME_DLL_NOT_FOUND;
}

extern "C" HRESULT InitializeApiImpl(u64, u64)
{
	return GDK_E_GAMERUNTIME_DLL_NOT_FOUND;
}

extern "C" void UninitializeApiImpl()
{
	// No runtime was initialized.
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

extern "C" HRESULT QueryApiImpl(const GdkGuid *, const GdkGuid *, void **result)
{
	if (!result)
		return GDK_E_POINTER;
	*result = nullptr;
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
