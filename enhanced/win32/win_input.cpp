// enhanced/win32/win_input.cpp: the mouse turns the view by raw device counts (P0001; B0001).
//
// IN_Frame as in the Store build (src/win32/win_input.cpp), except what CL_MouseEvent is given to turn by: the raw
// input thread's device counts since the last frame, in place of the cursor's movement. The cursor is still read
// for menus, which keep their absolute pointer, and still recentred while the game has the mouse, so clicks stay in
// the window. The counts are taken every frame, so movement made while the game doesn't have the mouse is dropped,
// never added to a later frame.
#include <windows.h>

#include "client_mp/client_mp.h"
#include "enhanced/raw_input.h"
#include "universal/q_shared.h"
#include "win32/win_local.h"

void IN_RecenterMouse();

void IN_Frame()
{
	int rawDx;
	int rawDy;
	POINT curPos;
	int dx;
	int dy;
	int x;
	int y;

	// The enhanced build turns the view only by raw counts; there is no quiet fallback to the cursor.
	if (!IN_StartRawInput())
		Com_Error(ERR_FATAL, "\x15" "Raw mouse input could not be registered (Windows error %u). The enhanced build "
		                     "needs it: remove X3DAudio1_7.dll and iw5_enhanced.dll from the game folder to play "
		                     "without it.",
		          IN_RawInputError());
	IN_TakeRawMouse(&rawDx, &rawDy);  // always: counts never wait for a later frame

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

	// IN_MouseMove
	if (GetForegroundWindow() != g_wv.hWnd)
		return;
	if (!GetCursorPos(&curPos))
		curPos = s_wmv.oldPos;  // never consume an uninitialized cursor position
	dx = curPos.x - s_wmv.oldPos.x;
	dy = curPos.y - s_wmv.oldPos.y;
	s_wmv.oldPos = curPos;
	ScreenToClient(g_wv.hWnd, &curPos);
	x = curPos.x * cls.displayWidth / cls.windowWidth;
	y = curPos.y * cls.displayHeight / cls.windowHeight;
	// menus use the cursor's position; the game adds the counts for CL_MouseMove
	g_wv.recenterMouse = CL_MouseEvent(x, y, rawDx, rawDy);
	if (g_wv.recenterMouse && (dx || dy))
	{
		IN_RecenterMouse();
		s_wmv.oldPos.x = window_center_x;
		s_wmv.oldPos.y = window_center_y;
	}
}
