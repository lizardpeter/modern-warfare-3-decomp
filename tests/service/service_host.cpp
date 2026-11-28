// tests/service/service_host.cpp: the local DemonWare on its own, for tests/service/run.py.
//
// Built from the same enhanced/DW sources as iw5_enhanced.dll into a small Windows console program (run under Wine
// in CI), it starts the service, answers one auth request the way the game's redirected WinHTTP request would
// reach it, prints the lobby port and the auth reply, and keeps serving until killed: the test then talks to the
// lobby as the game's client does.
#include "enhanced/local_demonware.h"

typedef void *HANDLE;
extern "C" __declspec(dllimport) HANDLE __stdcall GetStdHandle(unsigned long);
extern "C" __declspec(dllimport) int __stdcall WriteFile(HANDLE, const void *, unsigned long, unsigned long *, void *);
extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned);

static void Print(const char *s, size_t n)
{
	unsigned long written = 0;
	WriteFile(GetStdHandle((unsigned long)-11), s, (unsigned long)n, &written, nullptr);
}

static void PrintText(const char *s)
{
	size_t n = 0;
	while (s[n])
		++n;
	Print(s, n);
}

extern "C" int mainCRTStartup()
{
	if (!LocalDemonWare::Start())
	{
		PrintText("start failed\n");
		ExitProcess(1);
	}
	char port[8];
	int n = 0;
	unsigned v = LocalDemonWare::LobbyPort();
	char digits[8];
	do
		digits[n++] = '0' + v % 10;
	while (v /= 10);
	int k = 0;
	while (n)
		port[k++] = digits[--n];
	PrintText("port ");
	Print(port, k);
	PrintText("\n");
	static const char request[] = "{\"auth_task\":\"44\",\"iv_seed\":\"305419896\",\"title_id\":\"2310\"}";
	static char reply[0x1400];
	size_t size = 0;
	unsigned code = LocalDemonWare::Authorize(request, sizeof(request) - 1, "XBL3.0 x=0009000000000001;iw5-local",
	                                          reply, sizeof(reply), &size);
	PrintText(code == 200 ? "auth " : "auth failed\n");
	Print(reply, size);
	PrintText("\n");
	for (;;)
		Sleep(1000);
}
