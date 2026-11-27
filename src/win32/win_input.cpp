// win32/win_input.cpp: the PC mouse layer (no reference build: black-ops-2-decomp's names, the Store build's code).
#include <windows.h>

#include "client_mp/client_mp.h"
#include "win32/win_local.h"

// Store 140337040
// Steps ShowCursor's count to 0 (shown) or -1 (hidden).
void IN_ShowSystemCursor(int show)
{
	int target;
	int count;

	target = show ? 0 : -1;
	for (count = ShowCursor(show); count != target; count = ShowCursor(count < target))
		;
}

// Store 140336FB0
void IN_RecenterMouse()
{
	RECT windowRect;

	GetWindowRect(g_wv.hWnd, &windowRect);
	window_center_x = (windowRect.left + windowRect.right) / 2;
	window_center_y = (windowRect.top + windowRect.bottom) / 2;
	SetCursorPos(window_center_x, window_center_y);
}

// Store 140336CD0 (IN_MouseMove inlined)
// The cursor's movement since the last read, to CL_MouseEvent; while the game wants it (not a menu), the cursor goes
// back to the window's centre.
static inline void IN_MouseMove()
{
	POINT curPos;
	int dx;
	int dy;
	int x;
	int y;

	if (GetForegroundWindow() != g_wv.hWnd)
		return;
	GetCursorPos(&curPos);
	dx = curPos.x - s_wmv.oldPos.x;
	dy = curPos.y - s_wmv.oldPos.y;
	s_wmv.oldPos = curPos;
	ScreenToClient(g_wv.hWnd, &curPos);
	x = curPos.x * cls.displayWidth / cls.windowWidth;  // imul / idiv: 32-bit signed
	y = curPos.y * cls.displayHeight / cls.windowHeight;
	g_wv.recenterMouse = CL_MouseEvent(x, y, dx, dy);
	if (g_wv.recenterMouse && (dx || dy))
	{
		IN_RecenterMouse();
		s_wmv.oldPos.x = window_center_x;
		s_wmv.oldPos.y = window_center_y;
	}
}

// Store 140336CD0
// The mouse each frame: the ClickToContinue dvar clicks for the player; the mouse is taken while in_mouse is set and
// the game window is in front (and given back when IN_ActivateMouse asked for it not to be); then its movement.
void IN_Frame()
{
	if (Dvar_GetBool("ClickToContinue"))
		PostMessageA(g_wv.hWnd, WM_LBUTTONDOWN, MK_LBUTTON, 0);
	if (!s_wmv.mouseInitialized)
		return;

	if (!s_wantMouse)
	{
		if (s_wmv.mouseActive)
		{
			s_wmv.mouseActive = false;
			IN_ShowSystemCursor(TRUE);
		}
		return;
	}

	if (in_mouse->current.enabled && (s_wmv.mouseActive || GetForegroundWindow() == g_wv.hWnd))
	{
		if (!s_wmv.mouseActive)
		{
			s_wmv.mouseActive = true;
			if ((unsigned)(clientUIActives[0].connectionState - CA_CINEMATIC) <= CA_LOADING - CA_CINEMATIC)
			{
				IN_ShowSystemCursor(FALSE);
				sub_1400C7DA0(0);
			}
		}
	}
	else
	{
		s_wmv.mouseActive = false;
	}
	IN_MouseMove();
}
