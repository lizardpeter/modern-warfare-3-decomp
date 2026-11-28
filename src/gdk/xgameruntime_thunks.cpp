// Faithful reconstruction candidate for the Store x64 GDK thunk library.
// 1403601E0 (63 bytes), records/names.tsv: lazy System32-only runtime load.
// The enhanced build intentionally overrides this function in enhanced/gdk/.
// Preserve original loader behavior here; do not implement offline policy in src/.
#include <windows.h>

WINBASEAPI HINSTANCE WINAPI LoadLibraryExA(const char *name, HANDLE file, DWORD flags);
extern HINSTANCE s_xgameRuntimeModule;

long XGameRuntimeLoadModule()
{
	if (!s_xgameRuntimeModule)
		s_xgameRuntimeModule = LoadLibraryExA("xgameruntime.dll", nullptr, 0x800);
	return s_xgameRuntimeModule ? 0 : static_cast<long>(0x89240101u);
}
