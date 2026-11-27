// win32/win_local.h, as far as the reconstruction needs it. The PC layer has no reference build: names and layouts
// are black-ops-2-decomp's, which the Store build's accesses match (docs/store-build.md, Mouse input).
#pragma once

#include <windows.h>

#include "qcommon/dvar.h"

struct WinVars_t
{
	HINSTANCE reflib_library;
	int reflib_active;
	HWND hWnd;
	HINSTANCE hInstance;
	int activeApp;
	int isMinimized;
	int recenterMouse;  // the last CL_MouseEvent wanted the cursor centred (gameplay, not a menu)
};
static_assert(__builtin_offsetof(WinVars_t, hWnd) == 0x10, "WinVars_t::hWnd (Store g_wv 148A8D6D0: hWnd at 148A8D6E0)");
static_assert(__builtin_offsetof(WinVars_t, recenterMouse) == 0x28, "WinVars_t::recenterMouse (148A8D6F8)");
extern WinVars_t g_wv;

struct WinMouseVars_t
{
	int oldButtonState;
	POINT oldPos;           // the cursor's screen position at the last read
	bool mouseActive;       // the game window has the mouse
	bool mouseInitialized;  // in_mouse was set at startup
};
extern WinMouseVars_t s_wmv;
extern int s_wantMouse;  // IN_ActivateMouse's argument, kept for IN_Frame (148A8048C)
extern int window_center_x;
extern int window_center_y;
extern const dvar_t *in_mouse;

bool Dvar_GetBool(const char *dvarName);

void IN_ShowSystemCursor(int show);
void IN_Frame();
