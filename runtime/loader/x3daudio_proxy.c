// runtime/loader/x3daudio_proxy.c: X3DAudio1_7.dll in the game folder, which loads the enhanced build.
// From black-ops-2-decomp's XINPUT1_4.dll loader (MW3's Store exe imports no XInput).
//
// iw5mp_ship.exe and iw5sp_ship.exe import X3DAudioInitialize and X3DAudioCalculate from X3DAudio1_7.dll, which the
// Store package doesn't ship (it comes from the DirectX runtime in the system folder) and which isn't one of Windows'
// KnownDLLs, so Windows looks in the game's folder first. This DLL forwards both to the system's X3DAudio1_7.dll, so
// audio works as before. When the process is the supported multiplayer executable (checked by PE timestamp and image
// size), it loads iw5_enhanced.dll from the same folder and points each exe function the enhanced build replaces at
// its replacement (the DLL's iw5_redirects table, tools/runtime_link.py): a 14-byte jump at the function's entry,
// written only if every replaced function still starts with the bytes the build saw. Multiplayer stops if its
// enhanced build cannot be installed; other executables (single player) only get forwarding.
// Nothing on disk is changed; deleting the two DLLs restores the game.
//
// What happened is written to iw5_enhanced.log next to the DLL.
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef int BOOL;
typedef unsigned long long ULONG_PTR;
typedef unsigned long long SIZE_T;
typedef void *HANDLE;
typedef void *HMODULE;
typedef unsigned short WCHAR;
typedef void (*FARPROC)(void);

#define WINAPI __stdcall
#define DLL_PROCESS_ATTACH 1
#define PAGE_EXECUTE_READWRITE 0x40
#define GENERIC_WRITE 0x40000000
#define CREATE_ALWAYS 2
#define FILE_ATTRIBUTE_NORMAL 0x80
#define INVALID_HANDLE_VALUE ((HANDLE)(long long)-1)
#define MAX_PATH 260

__declspec(dllimport) HMODULE WINAPI LoadLibraryW(const WCHAR *name);
__declspec(dllimport) FARPROC WINAPI GetProcAddress(HMODULE module, const char *name);
__declspec(dllimport) HMODULE WINAPI GetModuleHandleW(const WCHAR *name);
__declspec(dllimport) DWORD WINAPI GetModuleFileNameW(HMODULE module, WCHAR *path, DWORD size);
__declspec(dllimport) DWORD WINAPI GetSystemDirectoryW(WCHAR *path, DWORD size);
__declspec(dllimport) BOOL WINAPI VirtualProtect(void *address, SIZE_T size, DWORD protect, DWORD *old);
__declspec(dllimport) HANDLE WINAPI GetCurrentProcess(void);
__declspec(dllimport) BOOL WINAPI TerminateProcess(HANDLE process, DWORD exitCode);
__declspec(dllimport) void WINAPI OutputDebugStringA(const char *text);
__declspec(dllimport) BOOL WINAPI FlushInstructionCache(HANDLE process, const void *address, SIZE_T size);
__declspec(dllimport) BOOL WINAPI DisableThreadLibraryCalls(HMODULE module);
__declspec(dllimport) HANDLE WINAPI CreateFileW(const WCHAR *name, DWORD access, DWORD share, void *security,
                                               DWORD disposition, DWORD flags, HANDLE templ);
__declspec(dllimport) BOOL WINAPI WriteFile(HANDLE file, const void *data, DWORD size, DWORD *written, void *overlapped);
__declspec(dllimport) BOOL WINAPI CloseHandle(HANDLE handle);

// -- the system X3DAudio1_7.dll's exports; each export of this DLL jumps through its slot (x3daudio_stubs.s)
FARPROC x3daudio_slots[2];
static const char *const x3daudio_names[2] = {"X3DAudioInitialize", "X3DAudioCalculate"};
static BOOL s_x3daudioLoaded;

static void LoadSystemX3DAudio(void)
{
	WCHAR path[MAX_PATH];
	DWORD n = GetSystemDirectoryW(path, MAX_PATH - 20);
	const char *name = "\\X3DAudio1_7.dll";
	for (DWORD i = 0; name[i]; ++i)
		path[n++] = (WCHAR)name[i];
	path[n] = 0;
	HMODULE real = n < MAX_PATH ? LoadLibraryW(path) : 0;
	s_x3daudioLoaded = real != 0;
	for (int i = 0; i < 2; ++i)
		x3daudio_slots[i] = real ? GetProcAddress(real, x3daudio_names[i]) : 0;
	s_x3daudioLoaded = s_x3daudioLoaded && x3daudio_slots[0] && x3daudio_slots[1];
}

// -- the log
static HANDLE s_log = INVALID_HANDLE_VALUE;

static void Log(const char *text)
{
	DWORD n = 0, written;
	while (text[n])
		++n;
	if (s_log != INVALID_HANDLE_VALUE)
		WriteFile(s_log, text, n, &written, 0);
}

static void LogHex(unsigned long long v)
{
	char buf[19] = "0x";
	for (int i = 0; i < 16; ++i)
		buf[2 + i] = "0123456789ABCDEF"[(v >> (60 - 4 * i)) & 0xF];
	buf[18] = 0;
	Log(buf);
}

// -- the enhanced build
struct Redirect
{
	unsigned long long rva;     // the exe function
	unsigned long long target;  // its replacement in iw5_enhanced.dll
	BYTE prologue[16];          // the exe function's first bytes when the build was made
};

struct Redirects
{
	unsigned long long count;
	struct Redirect entries[1];
};

struct RedirectIdentity
{
	DWORD timestamp;
	DWORD imageSize;
};

static BOOL Fail(const char *message)
{
	Log(message);
	OutputDebugStringA(message);
	if (s_log != INVALID_HANDLE_VALUE)
		CloseHandle(s_log);
	// Terminate even if the proxy was loaded dynamically or some jumps were already written.
	TerminateProcess(GetCurrentProcess(), 1114);  // ERROR_DLL_INIT_FAILED
	return 0;
}

static BOOL SameName(const WCHAR *path, const char *name)
{
	while (*name)
	{
		WCHAR ch = *path++;
		if (ch >= 'A' && ch <= 'Z')
			ch += 'a' - 'A';
		if (ch != (WCHAR)*name++)
			return 0;
	}
	return *path == 0;
}

static BOOL Install(HMODULE self)
{
	BYTE *exe = (BYTE *)GetModuleHandleW(0);
	const BYTE *nt = exe + *(const DWORD *)(exe + 0x3C);
	DWORD timestamp = *(const DWORD *)(nt + 8);
	DWORD imageSize = *(const DWORD *)(nt + 0x18 + 0x38);
	const char *dllName;
	const char *logName;
	const char *banner;
	if (timestamp == 0x6723B60Eu && imageSize == 0x0A75F000u)
	{
		dllName = "iw5_enhanced.dll";
		logName = "iw5_enhanced.log";
		banner = "iw5_enhanced: iw5mp_ship.exe 2.0.3.0\r\n";
	}
	else
	{
		WCHAR exePath[MAX_PATH];
		DWORD end = GetModuleFileNameW(0, exePath, MAX_PATH);
		if (!end || end >= MAX_PATH)
			return Fail("IW5 loader: could not identify the executable.\r\n");
		DWORD start = end;
		while (start && exePath[start - 1] != '\\')
			--start;
		if (!SameName(exePath + start, "iw5mp_ship.exe"))
			return 1;  // single player and anything else: forwarding only
		return Fail("IW5 loader: unsupported multiplayer executable version.\r\n");
	}

	WCHAR path[MAX_PATH + 32];
	DWORD n = GetModuleFileNameW(self, path, MAX_PATH);
	if (!n || n >= MAX_PATH)
		return Fail("IW5 loader: could not find the proxy DLL directory.\r\n");
	while (n && path[n - 1] != '\\')
		--n;
	for (DWORD i = 0; ; ++i)
	{
		path[n + i] = (WCHAR)logName[i];
		if (!logName[i])
			break;
	}
	s_log = CreateFileW(path, GENERIC_WRITE, 1, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	Log(banner);

	for (DWORD i = 0; ; ++i)
	{
		path[n + i] = (WCHAR)dllName[i];
		if (!dllName[i])
			break;
	}
	HMODULE enhanced = LoadLibraryW(path);
	if (!enhanced)
		return Fail("IW5 loader: could not load the enhanced DLL.\r\n");
	const struct Redirects *table = (const struct Redirects *)GetProcAddress(enhanced, "iw5_redirects");
	const struct RedirectIdentity *identity =
		(const struct RedirectIdentity *)GetProcAddress(enhanced, "iw5_redirects_identity");
	if (!table || !identity)
		return Fail("IW5 loader: the enhanced DLL has no compatible redirect table.\r\n");

	const BYTE *dll = (const BYTE *)enhanced;
	const BYTE *dllNt = dll + *(const DWORD *)(dll + 0x3C);
	DWORD dllSize = *(const DWORD *)(dllNt + 0x18 + 0x38);
	ULONG_PTR tableOffset = (ULONG_PTR)table - (ULONG_PTR)dll;
	ULONG_PTR identityOffset = (ULONG_PTR)identity - (ULONG_PTR)dll;
	if (dllSize < sizeof(*identity) || identityOffset > dllSize - sizeof(*identity) ||
	    tableOffset > identityOffset || identityOffset - tableOffset < sizeof(table->count))
		return Fail("IW5 loader: invalid enhanced DLL redirect table.\r\n");
	ULONG_PTR entriesSize = identityOffset - tableOffset - sizeof(table->count);
	if (!table->count || entriesSize % sizeof(struct Redirect) ||
	    table->count != entriesSize / sizeof(struct Redirect))
		return Fail("IW5 loader: invalid enhanced DLL redirect table.\r\n");
	if (identity->timestamp != timestamp || identity->imageSize != imageSize)
		return Fail("IW5 loader: the enhanced DLL does not match this executable.\r\n");

	// Check every entry before writing any jumps.
	for (unsigned long long i = 0; i < table->count; ++i)
	{
		const struct Redirect *r = &table->entries[i];
		if (r->rva > imageSize - sizeof(r->prologue) || r->target - (ULONG_PTR)dll >= dllSize)
			return Fail("IW5 loader: invalid enhanced DLL redirect address.\r\n");
		for (int b = 0; b < 16; ++b)
			if (exe[r->rva + b] != r->prologue[b])
				return Fail("IW5 loader: the enhanced DLL does not match this executable.\r\n");
	}
	for (unsigned long long i = 0; i < table->count; ++i)
	{
		const struct Redirect *r = &table->entries[i];
		BYTE *code = exe + r->rva;
		DWORD old;
		if (!VirtualProtect(code, 14, PAGE_EXECUTE_READWRITE, &old))
			return Fail("IW5 loader: could not make a redirect writable.\r\n");
		BYTE jump[14] = {0xFF, 0x25, 0, 0, 0, 0};  // jmp qword ptr [rip + 0]
		for (int b = 0; b < 8; ++b)
			jump[6 + b] = (BYTE)(r->target >> (8 * b));
		for (int b = 0; b < 14; ++b)
			code[b] = jump[b];
		if (!VirtualProtect(code, 14, old, &old))
			return Fail("IW5 loader: could not restore executable page protection.\r\n");
		if (!FlushInstructionCache(GetCurrentProcess(), code, 14))
			return Fail("IW5 loader: could not flush the instruction cache.\r\n");
		Log("replaced ");
		LogHex((unsigned long long)(ULONG_PTR)code);
		Log("\r\n");
	}
	Log("enhanced build active\r\n");
	if (s_log != INVALID_HANDLE_VALUE)
		CloseHandle(s_log);
	return 1;
}

BOOL WINAPI ProxyMain(HMODULE self, DWORD reason, void *reserved)
{
	(void)reserved;
	if (reason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(self);
		LoadSystemX3DAudio();
		if (!s_x3daudioLoaded)
		{
			// without the system's X3DAudio the game's audio can't start: say so instead of crashing in it later
			OutputDebugStringA("IW5 loader: the system X3DAudio1_7.dll (DirectX runtime) could not be loaded.\r\n");
			return 0;
		}
		return Install(self);
	}
	return 1;
}
