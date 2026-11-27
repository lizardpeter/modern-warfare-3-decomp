// enhanced/raw_input.h: raw mouse input (enhanced/win32/win_rawinput.cpp, P0001).
#pragma once

#include <windows.h>

// Starts the raw input thread once; later calls return its state. False if the mouse couldn't be registered for raw
// input: IN_Frame then stops the game with an error (raw input is the only mode; no fallback to the cursor).
bool IN_StartRawInput();

// The Windows error code of the failed raw input registration (or thread start).
unsigned int IN_RawInputError();

// Adds one raw mouse report to the counters: relative movement only. Absolute reports (tablets, remote desktop,
// some virtual machines) carry positions, not movement, and are ignored.
void IN_RawInputAdd(const RAWMOUSE *mouse);

// The raw input thread's window procedure: WM_INPUT mouse reports, while the game is the foreground window, go to
// IN_RawInputAdd.
LRESULT WINAPI IN_RawInputWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// The movement since the last call, in device counts; the counters restart at zero.
void IN_TakeRawMouse(int *dx, int *dy);
