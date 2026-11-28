// enhanced/DW/local_demonware.cpp: the local DemonWare service (docs/online.md, "Local DemonWare").
//
// - Auth: the game posts {"auth_task":44,"iv_seed":N,"title_id":2310} with its Xbox token. The reply issues the
//   service's own tickets ({"auth_task":45,"code":700,...}, tools/demonware/auth.py): a client ticket the game's
//   ticket decoder (1404049D0) reads (magic EFBDADDE, title 906, account, name, a fresh 24-byte session key) and a
//   128-byte server ticket the game hands back to the lobby. The service remembers which session key each server
//   ticket was issued with.
// - Lobby: TCP on a loopback port of its own. Each connection does the protocol 210 handshake
//   (tools/demonware/lobby210.py): the client's 28-byte hello, the service's challenge, the client's auth frame
//   carrying the server ticket and a proof; the proof must match the session key issued with that ticket, then
//   both sides derive the record keys (BDDATA) and exchange AES-128-CBC records with truncated HMAC-SHA1 and a
//   sequence counter per direction (tools/demonware/records210.py).
// - After the handshake the service sends the connection ID header; then each task the game sends gets a reply in
//   order, every record traced to iw5_enhanced.log. Tasks are answered as they're identified (HandleTask); the
//   rest get BD_SERVICE_NOT_AVAILABLE.
#include "enhanced/local_demonware.h"

#include <windows.h>

WINBASEAPI HANDLE WINAPI CreateFileA(const char *name, DWORD access, DWORD share, void *security, DWORD disposition,
                                     DWORD flags, HANDLE templ);
WINBASEAPI BOOL WINAPI GetModuleHandleExA(DWORD flags, const char *name, HINSTANCE *module);
WINBASEAPI DWORD WINAPI GetModuleFileNameA(HINSTANCE module, char *path, DWORD size);
WINBASEAPI void WINAPI InitializeCriticalSection(LPCRITICAL_SECTION section);
WINBASEAPI void *WINAPI LocalAlloc(UINT flags, size_t bytes);
WINBASEAPI void *WINAPI LocalFree(void *mem);
struct FileTime
{
	DWORD dwLowDateTime, dwHighDateTime;
};
WINBASEAPI void WINAPI GetSystemTimeAsFileTime(FileTime *time);

// Winsock (the exe's imports, plus ws2_32.def)
typedef u64 SOCKET;
struct sockaddr_in4
{
	unsigned short family;
	unsigned short port;
	u32 address;
	u8 zero[8];
};
struct WSADATA_
{
	u8 raw[408];
};
extern "C" __declspec(dllimport) int __stdcall WSAStartup(unsigned short, WSADATA_ *);
extern "C" __declspec(dllimport) SOCKET __stdcall socket(int, int, int);
extern "C" __declspec(dllimport) int __stdcall bind(SOCKET, const void *, int);
extern "C" __declspec(dllimport) int __stdcall listen(SOCKET, int);
extern "C" __declspec(dllimport) SOCKET __stdcall accept(SOCKET, void *, int *);
extern "C" __declspec(dllimport) int __stdcall recv(SOCKET, char *, int, int);
extern "C" __declspec(dllimport) int __stdcall send(SOCKET, const char *, int, int);
extern "C" __declspec(dllimport) int __stdcall closesocket(SOCKET);
extern "C" __declspec(dllimport) int __stdcall getsockname(SOCKET, void *, int *);
extern "C" __declspec(dllimport) unsigned short __stdcall htons(unsigned short);
extern "C" __declspec(dllimport) unsigned short __stdcall ntohs(unsigned short);
extern "C" __declspec(dllimport) int __stdcall WSAGetLastError();

namespace
{
constexpr SOCKET kInvalidSocket = ~SOCKET(0);
constexpr u32 kTitle = 0x906;
constexpr u32 kProtocol = 210;
constexpr size_t kTicketSize = 128;
constexpr size_t kAuthBody = (83 + kTicketSize * 8 + 7) / 8;  // 139
constexpr int kMaxTickets = 64;
constexpr u64 kAccount = 0x0009000000000001ull;  // the local player (the XUID the local XUser reports)

CRITICAL_SECTION s_lock;
bool s_started;
unsigned short s_port;
SOCKET s_listener = kInvalidSocket;
HANDLE s_log = INVALID_HANDLE_VALUE;

struct Ticket
{
	u8 server[kTicketSize];
	u8 sessionKey[24];
	bool used;
};
Ticket s_tickets[kMaxTickets];
int s_nextTicket;

// --- small helpers: no CRT here ------------------------------------------------------------------------------------

void Copy(void *d, const void *s, size_t n)
{
	u8 *a = static_cast<u8 *>(d);
	const u8 *b = static_cast<const u8 *>(s);
	while (n--)
		*a++ = *b++;
}

bool Equal(const void *x, const void *y, size_t n)
{
	const u8 *a = static_cast<const u8 *>(x), *b = static_cast<const u8 *>(y);
	u8 diff = 0;
	while (n--)
		diff |= *a++ ^ *b++;
	return !diff;
}

size_t Length(const char *s)
{
	size_t n = 0;
	while (s && s[n])
		++n;
	return n;
}

bool SameText(const char *a, const char *b)
{
	for (;; ++a, ++b)
	{
		char x = *a >= 'A' && *a <= 'Z' ? *a + 32 : *a, y = *b >= 'A' && *b <= 'Z' ? *b + 32 : *b;
		if (x != y)
			return false;
		if (!x)
			return true;
	}
}

bool EndsWith(const char *s, const char *suffix)
{
	size_t n = Length(s), m = Length(suffix);
	return n >= m && SameText(s + n - m, suffix);
}

bool StartsWithText(const char *s, const char *prefix)
{
	for (; *prefix; ++s, ++prefix)
	{
		char x = *s >= 'A' && *s <= 'Z' ? *s + 32 : *s;
		if (x != *prefix)
			return false;
	}
	return true;
}

void Put32(u8 *p, u32 v)
{
	p[0] = (u8)v, p[1] = (u8)(v >> 8), p[2] = (u8)(v >> 16), p[3] = (u8)(v >> 24);
}

void Put64(u8 *p, u64 v)
{
	Put32(p, (u32)v);
	Put32(p + 4, (u32)(v >> 32));
}

u32 UnixTime()
{
	FileTime now;
	GetSystemTimeAsFileTime(&now);
	u64 ticks = (u64)now.dwHighDateTime << 32 | now.dwLowDateTime;  // 100 ns since 1601
	return (u32)(ticks / 10000000ull - 11644473600ull);
}

u32 Get32(const u8 *p)
{
	return p[0] | p[1] << 8 | p[2] << 16 | (u32)p[3] << 24;
}

// --- the trace -------------------------------------------------------------------------------------------------------

struct Out
{
	char *p, *end;
	void Char(char c)
	{
		if (p < end)
			*p++ = c;
	}
	void Text(const char *s)
	{
		while (s && *s)
			Char(*s++);
	}
	void Unsigned(u64 v, int base, int width = 0)
	{
		char digits[24];
		int n = 0;
		do
			digits[n++] = "0123456789abcdef"[v % base];
		while ((v /= base) && n < 24);
		while (n < width)
			digits[n++] = '0';
		while (n)
			Char(digits[--n]);
	}
};

void OpenLog()
{
	if (s_log != INVALID_HANDLE_VALUE)
		return;
	HINSTANCE self = nullptr;
	GetModuleHandleExA(4 | 2, reinterpret_cast<const char *>(&OpenLog), &self);
	char path[MAX_PATH + 32];
	DWORD n = GetModuleFileNameA(self, path, MAX_PATH);
	while (n && path[n - 1] != '\\' && path[n - 1] != '/')
		--n;
	Copy(path + n, "iw5_enhanced.log", 17);
	constexpr DWORD appendData = 4, openAlways = 4;  // FILE_APPEND_DATA, OPEN_ALWAYS
	s_log = CreateFileA(path, appendData, FILE_SHARE_READ | 2, nullptr, openAlways, 0, nullptr);
}

// --- tickets ---------------------------------------------------------------------------------------------------------

const Ticket *FindTicket(const u8 *server)
{
	for (const Ticket &t : s_tickets)
		if (t.used && Equal(t.server, server, kTicketSize))
			return &t;
	return nullptr;
}

// --- base64 and the auth JSON ----------------------------------------------------------------------------------------

size_t Base64(const u8 *in, size_t n, char *out)
{
	static const char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	size_t o = 0;
	for (size_t i = 0; i < n; i += 3)
	{
		u32 v = in[i] << 16 | (i + 1 < n ? in[i + 1] << 8 : 0) | (i + 2 < n ? in[i + 2] : 0);
		out[o++] = alphabet[v >> 18 & 63];
		out[o++] = alphabet[v >> 12 & 63];
		out[o++] = i + 1 < n ? alphabet[v >> 6 & 63] : '=';
		out[o++] = i + 2 < n ? alphabet[v & 63] : '=';
	}
	return o;
}

// The number after "key": in a flat JSON object, or false.
bool JsonNumber(const char *body, size_t size, const char *key, u64 *value)
{
	size_t k = Length(key);
	for (size_t i = 0; i + k + 2 < size; ++i)
	{
		if (body[i] != '"' || !Equal(body + i + 1, key, k) || body[i + 1 + k] != '"')
			continue;
		size_t j = i + 2 + k;
		while (j < size && (body[j] == ' ' || body[j] == ':'))
			++j;
		u64 v = 0;
		bool any = false;
		while (j < size && body[j] >= '0' && body[j] <= '9')
			v = v * 10 + (body[j++] - '0'), any = true;
		if (any)
			*value = v;
		return any;
	}
	return false;
}

struct Writer
{
	char *p, *end;
	bool ok = true;
	void Text(const char *s, size_t n)
	{
		if ((size_t)(end - p) < n)
		{
			ok = false;
			return;
		}
		Copy(p, s, n);
		p += n;
	}
	void Text(const char *s)
	{
		Text(s, Length(s));
	}
	void Number(u64 v)
	{
		char buf[24];
		int n = 0;
		do
			buf[n++] = '0' + v % 10;
		while ((v /= 10) && n < 24);
		while (n)
			Text(&buf[--n], 1);
	}
};

// --- the lobby -------------------------------------------------------------------------------------------------------

bool ReadAll(SOCKET s, u8 *p, size_t n)
{
	while (n)
	{
		int got = recv(s, reinterpret_cast<char *>(p), (int)n, 0);
		if (got <= 0)
			return false;
		p += got;
		n -= got;
	}
	return true;
}

bool WriteAll(SOCKET s, const u8 *p, size_t n)
{
	while (n)
	{
		int sent = send(s, reinterpret_cast<const char *>(p), (int)n, 0);
		if (sent <= 0)
			return false;
		p += sent;
		n -= sent;
	}
	return true;
}

// P_SHA1-style expansion: HMAC(secret, previous || label || counter), counters from 1.
void Expand(const u8 secret[20], const char *label, u8 *out, size_t length)
{
	u8 previous[20];
	size_t have = 0, previousSize = 0;
	for (u8 counter = 1; have < length; ++counter)
	{
		const u8 *parts[] = {previous, reinterpret_cast<const u8 *>(label), &counter};
		size_t sizes[] = {previousSize, Length(label), 1};
		LocalCrypto::HmacSha1Parts(secret, 20, parts, sizes, 3, previous);
		previousSize = 20;
		size_t take = length - have < 20 ? length - have : 20;
		Copy(out + have, previous, take);
		have += take;
	}
}

struct Records
{
	u8 receiveMac[20], sendMac[20], receiveKey[16], sendKey[16];
	u32 receiveCounter = 1, sendCounter = 1;
	u32 frameLimit;
};

// One record from the game: size, AB 85, counter, IV, AES-CBC ciphertext, 8-byte MAC.
bool ReadRecord(SOCKET s, Records &r, u8 *type, u8 *payload, size_t capacity, size_t *size)
{
	u8 head[4];
	if (!ReadAll(s, head, 4))
		return false;
	u32 length = Get32(head);
	if (length < 2 + 4 + 16 + 16 + 8 || length > r.frameLimit || length > 0x10000)
	{
		LocalDemonWare::Trace("lobby: record length %u rejected", length);
		return false;
	}
	u8 *frame = static_cast<u8 *>(LocalAlloc(0x40, length + 4));
	Copy(frame, head, 4);
	bool ok = ReadAll(s, frame + 4, length);
	size_t cipher = length - 2 - 4 - 16 - 8;
	u8 mac[20];
	ok = ok && frame[4] == 0xAB && frame[5] == 0x85 && Get32(frame + 6) == r.receiveCounter && cipher % 16 == 0 &&
	     LocalCrypto::HmacSha1(r.receiveMac, 20, frame, length + 4 - 8, mac) && Equal(mac, frame + length + 4 - 8, 8);
	u8 *plain = static_cast<u8 *>(LocalAlloc(0x40, cipher ? cipher : 16));
	ok = ok && LocalCrypto::Aes128Cbc(false, r.receiveKey, frame + 10, frame + 26, cipher, plain);
	u32 messageSize = ok ? Get32(plain) : 0;
	ok = ok && cipher >= 5 && messageSize <= cipher - 5 && messageSize <= capacity;
	if (ok)
	{
		*type = plain[4];
		Copy(payload, plain + 5, messageSize);
		*size = messageSize;
		++r.receiveCounter;
	}
	else
		LocalDemonWare::Trace("lobby: invalid record (counter %u)", r.receiveCounter);
	LocalFree(plain);
	LocalFree(frame);
	return ok;
}

bool WriteRecord(SOCKET s, Records &r, u8 type, const u8 *payload, size_t size)
{
	size_t plainSize = (size + 5 + 15) / 16 * 16;
	size_t wire = 2 + 4 + 16 + plainSize + 8;
	if (wire > r.frameLimit)
		return false;
	u8 *frame = static_cast<u8 *>(LocalAlloc(0x40, wire + 4));
	u8 *plain = static_cast<u8 *>(LocalAlloc(0x40, plainSize));
	Put32(plain, (u32)size);
	plain[4] = type;
	Copy(plain + 5, payload, size);
	Put32(frame, (u32)wire);
	frame[4] = 0xAB;
	frame[5] = 0x85;
	Put32(frame + 6, r.sendCounter);
	bool ok = LocalCrypto::Random(frame + 10, 16) &&
	          LocalCrypto::Aes128Cbc(true, r.sendKey, frame + 10, plain, plainSize, frame + 26);
	u8 mac[20];
	ok = ok && LocalCrypto::HmacSha1(r.sendMac, 20, frame, wire + 4 - 8, mac);
	if (ok)
	{
		Copy(frame + wire + 4 - 8, mac, 8);
		ok = WriteAll(s, frame, wire + 4);
		++r.sendCounter;
	}
	LocalFree(plain);
	LocalFree(frame);
	return ok;
}

void TraceBytes(const char *what, const u8 *p, size_t n)
{
	char hex[2 * 96 + 1];
	size_t shown = n < 96 ? n : 96;
	for (size_t i = 0; i < shown; ++i)
	{
		hex[2 * i] = "0123456789abcdef"[p[i] >> 4];
		hex[2 * i + 1] = "0123456789abcdef"[p[i] & 15];
	}
	hex[2 * shown] = 0;
	LocalDemonWare::Trace("%s (%u bytes): %s%s", what, (u32)n, hex, n > shown ? "..." : "");
}

// --- tasks -----------------------------------------------------------------------------------------------------------
//
// Each handler writes its results (count, count, then each result, typed) and returns the error code; services
// the game hasn't been seen to need answer BD_SERVICE_NOT_AVAILABLE, as an unavailable DemonWare service does.

constexpr u32 kServiceNotAvailable = 108;

void Results(u8 *&p, u32 count)
{
	p[0] = 8;
	Put32(p + 1, count);
	p[5] = 8;
	Put32(p + 6, count);
	p += count ? 10 : 5;
}

u32 HandleTask(u8 service, u8 task, const u8 *, size_t, u8 *out, size_t capacity, size_t *size)
{
	u8 *p = out;
	*size = 0;
	if (capacity < 64)
		return kServiceNotAvailable;
	if (service == 0x0C && task == 6)  // bdTitleUtilities::getServerTime: one typed uint32, the Unix time
	{
		Results(p, 1);
		p[0] = 8;
		Put32(p + 1, UnixTime());
		p += 5;
		*size = p - out;
		return 0;
	}
	return kServiceNotAvailable;
}

// The protocol 210 handshake, then records until the game hangs up.
void Session(SOCKET s, u64 connectionId)
{
	u8 hello[28];
	if (!ReadAll(s, hello, sizeof(hello)) || Get32(hello) != 200 || Get32(hello + 4) != 200 ||
	    Get32(hello + 8) != kProtocol || Get32(hello + 12) != kProtocol)
	{
		LocalDemonWare::Trace("lobby: not a protocol 210 hello");
		return;
	}
	Records r;
	r.frameLimit = Get32(hello + 16);
	if (r.frameLimit < 2 + kAuthBody + 8 || r.frameLimit > 65535)
	{
		LocalDemonWare::Trace("lobby: frame limit %u rejected", r.frameLimit);
		return;
	}
	// Challenge: 16 00 00 00 AB 81, protocol, connection id, nonce.
	u8 challenge[26];
	Put32(challenge, 22);
	challenge[4] = 0xAB;
	challenge[5] = 0x81;
	Put32(challenge + 6, kProtocol);
	Put64(challenge + 10, connectionId);
	if (!LocalCrypto::Random(challenge + 18, 8) || !WriteAll(s, challenge, sizeof(challenge)))
		return;
	// Client auth: size, AB 82, the 139-byte bit-packed body (header, title, IV seed, ticket), 8-byte proof.
	u8 auth[4 + 2 + kAuthBody + 8];
	if (!ReadAll(s, auth, sizeof(auth)) || Get32(auth) != sizeof(auth) - 4 || auth[4] != 0xAB || auth[5] != 0x82)
	{
		LocalDemonWare::Trace("lobby: not a client auth frame");
		return;
	}
	const u8 *body = auth + 6;
	auto bits = [&](u32 at, u32 count) {
		u64 v = 0;
		for (u32 i = 0; i < count; ++i)
			v |= (u64)(body[(at + i) / 8] >> ((at + i) % 8) & 1) << i;
		return v;
	};
	u8 ticket[kTicketSize];
	for (u32 i = 0; i < kTicketSize; ++i)
		ticket[i] = (u8)bits(83 + 8 * i, 8);
	if (bits(0, 9) != 0x107 || bits(9, 5) != 8 || bits(46, 5) != 8 || bits(14, 32) != kTitle)
	{
		LocalDemonWare::Trace("lobby: client auth header rejected");
		return;
	}
	EnterCriticalSection(&s_lock);
	const Ticket *issued = FindTicket(ticket);
	u8 sessionKey[24];
	if (issued)
		Copy(sessionKey, issued->sessionKey, 24);
	LeaveCriticalSection(&s_lock);
	if (!issued)
	{
		LocalDemonWare::Trace("lobby: ticket not issued by this service");
		return;
	}
	// transcript: (210, 210, frame limit, client nonce) || challenge || auth frame without its proof
	u8 transcript[20 + sizeof(challenge) + sizeof(auth) - 8];
	Put32(transcript, kProtocol);
	Put32(transcript + 4, kProtocol);
	Put32(transcript + 8, r.frameLimit);
	Copy(transcript + 12, hello + 20, 8);
	Copy(transcript + 20, challenge, sizeof(challenge));
	Copy(transcript + 20 + sizeof(challenge), auth, sizeof(auth) - 8);
	u8 digest[20], secret[20], proof[16];
	LocalCrypto::Sha1(transcript, sizeof(transcript), digest);
	LocalCrypto::HmacSha1(digest, 20, sessionKey, 24, secret);
	Expand(secret, "CLIENTCHAL", proof, 16);
	if (!Equal(proof, auth + sizeof(auth) - 8, 8))
	{
		LocalDemonWare::Trace("lobby: client proof doesn't match the ticket's session key");
		return;
	}
	u8 done[14];
	Put32(done, 10);
	done[4] = 0xAB;
	done[5] = 0x83;
	Copy(done + 6, proof + 8, 8);
	if (!WriteAll(s, done, sizeof(done)))
		return;
	u8 material[72];
	Expand(secret, "BDDATA", material, sizeof(material));
	Copy(r.receiveMac, material, 20);
	Copy(r.sendMac, material + 20, 20);
	Copy(r.receiveKey, material + 40, 16);
	Copy(r.sendKey, material + 56, 16);
	LocalDemonWare::Trace("lobby: connection %u authenticated (frame limit %u)", (u32)connectionId, r.frameLimit);

	// The connection ID header (type 4: a typed uint64), which the task manager needs before it sends tasks
	// (bdLobbyService::pump, 1403F5370).
	u8 id[9] = {10};
	Put64(id + 1, connectionId);
	if (!WriteRecord(s, r, 4, id, sizeof(id)))
		return;

	u8 *payload = static_cast<u8 *>(LocalAlloc(0x40, 0x10000));
	u8 *reply = static_cast<u8 *>(LocalAlloc(0x40, 0x10000));
	u64 transaction = 0;
	for (;;)
	{
		u8 type = 0;
		size_t size = 0;
		if (!ReadRecord(s, r, &type, payload, 0x10000, &size))
			break;
		char what[32] = "lobby: message type ";
		Out o = {what + 20, what + sizeof(what) - 1};
		o.Unsigned(type, 16, 2);
		*o.p = 0;
		TraceBytes(what, payload, size);
		// A task (0x86, bdLobbyConnection::sendTask210): service id, a typed uint8 (3) task id, the arguments.
		if (type != 0x86 || size < 3 || payload[1] != 3)
			continue;
		u8 service = payload[0], task = payload[2];
		size_t replySize = 0;
		u32 status = HandleTask(service, task, payload + 3, size - 3, reply + 16, 0x10000 - 16, &replySize);
		// The reply (type 1, bdRemoteTask::handleTaskReply 1403F18E0): typed transaction ID, typed error, and on
		// success the typed task id and the task's results (written by HandleTask after the header).
		u8 header[16];
		header[0] = 10;
		Put64(header + 1, ++transaction);
		header[9] = 8;
		Put32(header + 10, status);
		size_t headerSize = 14;
		if (status == 0)
		{
			header[14] = 3;
			header[15] = task;
			headerSize = 16;
		}
		u8 *out = reply + 16 - headerSize;
		Copy(out, header, headerSize);
		LocalDemonWare::Trace("lobby: service %x task %u: status %u, %u bytes of results", service, task, status,
		                      (u32)replySize);
		if (!WriteRecord(s, r, 1, out, headerSize + (status == 0 ? replySize : 0)))
			break;
	}
	LocalFree(reply);
	LocalFree(payload);
	LocalDemonWare::Trace("lobby: connection %u closed", (u32)connectionId);
}

struct SessionArgs
{
	SOCKET socket;
	u64 id;
};

DWORD WINAPI SessionThread(void *p)
{
	SessionArgs args = *static_cast<SessionArgs *>(p);
	LocalFree(p);
	Session(args.socket, args.id);
	closesocket(args.socket);
	return 0;
}

DWORD WINAPI AcceptThread(void *)
{
	for (u64 id = 1;; ++id)
	{
		SOCKET client = accept(s_listener, nullptr, nullptr);
		if (client == kInvalidSocket)
		{
			LocalDemonWare::Trace("lobby: accept failed (%u)", (u32)WSAGetLastError());
			Sleep(100);
			continue;
		}
		SessionArgs *args = static_cast<SessionArgs *>(LocalAlloc(0x40, sizeof(SessionArgs)));
		args->socket = client;
		args->id = id;
		CloseHandle(CreateThread(nullptr, 0, SessionThread, args, 0, nullptr));
	}
}
}  // namespace

namespace LocalDemonWare
{
void Trace(const char *format, ...)
{
	char line[512];
	Out o = {line, line + sizeof(line) - 2};
	o.Text("dw: ");
	__builtin_va_list args;
	__builtin_va_start(args, format);
	for (const char *f = format; *f; ++f)
	{
		if (*f != '%')
		{
			o.Char(*f);
			continue;
		}
		switch (*++f)
		{
		case 's':
			o.Text(__builtin_va_arg(args, const char *));
			break;
		case 'u':
			o.Unsigned(__builtin_va_arg(args, u32), 10);
			break;
		case 'x':
			o.Unsigned(__builtin_va_arg(args, u32), 16);
			break;
		case '%':
			o.Char('%');
			break;
		default:
			o.Char('?');
			break;
		}
	}
	__builtin_va_end(args);
	o.Char('\r');
	o.Char('\n');
	OpenLog();
	DWORD written = 0;
	if (s_log != INVALID_HANDLE_VALUE)
		WriteFile(s_log, line, (DWORD)(o.p - line), &written, nullptr);
}

bool IsServiceHost(const char *name)
{
	if (!name)
		return false;
	return SameText(name, "auth3.prod.demonware.net") ||
	       (StartsWithText(name, "lsg.") && EndsWith(name, ".prod.demonware.net"));
}

bool Start()
{
	static long once;
	if (__atomic_exchange_n(&once, 1, __ATOMIC_ACQ_REL))
	{
		while (!__atomic_load_n(&s_started, __ATOMIC_ACQUIRE) && __atomic_load_n(&once, __ATOMIC_ACQUIRE) == 1)
			Sleep(1);
		return s_port != 0;
	}
	InitializeCriticalSection(&s_lock);
	WSADATA_ data;
	WSAStartup(0x0202, &data);
	s_listener = socket(2, 1, 6);  // AF_INET, SOCK_STREAM, IPPROTO_TCP
	sockaddr_in4 at = {};
	at.family = 2;
	at.address = kLoopback;  // loopback only: nothing outside this PC can connect
	int size = sizeof(at);
	if (s_listener == kInvalidSocket || bind(s_listener, &at, sizeof(at)) != 0 || listen(s_listener, 8) != 0 ||
	    getsockname(s_listener, &at, &size) != 0)
	{
		Trace("could not open the lobby socket (%u)", (u32)WSAGetLastError());
		if (s_listener != kInvalidSocket)
			closesocket(s_listener);
		__atomic_store_n(&s_started, true, __ATOMIC_RELEASE);
		return false;
	}
	s_port = ntohs(at.port);
	CloseHandle(CreateThread(nullptr, 0, AcceptThread, nullptr, 0, nullptr));
	Trace("local DemonWare: lobby on 127.0.0.1:%u", s_port);
	__atomic_store_n(&s_started, true, __ATOMIC_RELEASE);
	return true;
}

unsigned short LobbyPort()
{
	return s_port;
}

u32 Authorize(const char *body, size_t size, const char *authorization, char *reply, size_t capacity,
              size_t *replySize)
{
	*replySize = 0;
	u64 task = 0, seed = 0, title = 0;
	if (!JsonNumber(body, size, "auth_task", &task) || !JsonNumber(body, size, "iv_seed", &seed) ||
	    !JsonNumber(body, size, "title_id", &title) || task != 44 || title != kTitle)
	{
		Trace("auth: request rejected");
		return 400;
	}
	(void)authorization;  // the local player's token (enhanced/gdk/gdk_user.cpp); one player, nothing to check
	Start();
	EnterCriticalSection(&s_lock);
	Ticket &t = s_tickets[s_nextTicket++ % kMaxTickets];
	bool ok = LocalCrypto::Random(t.server, kTicketSize) && LocalCrypto::Random(t.sessionKey, 24);
	t.used = ok;
	u8 sessionKey[24], server[kTicketSize];
	Copy(sessionKey, t.sessionKey, 24);
	Copy(server, t.server, kTicketSize);
	LeaveCriticalSection(&s_lock);
	if (!ok)
		return 500;
	// The client ticket (1404049D0): magic, 0, title, 0, 0, 0, account, name[64], session key[24], padding[7].
	u8 client[128] = {};
	Put32(client, 0xEFBDADDE);
	Put32(client + 5, kTitle);
	Put64(client + 25, kAccount);
	const char *name = "Unknown Soldier";
	Copy(client + 33, name, Length(name));
	Copy(client + 97, sessionKey, 24);
	char clientText[176], serverText[176];
	size_t clientSize = Base64(client, sizeof(client), clientText);
	size_t serverSize = Base64(server, sizeof(server), serverText);
	Writer w = {reply, reply + capacity};
	w.Text("{\"auth_task\":45,\"code\":700,\"iv_seed\":");
	w.Number(seed);
	w.Text(",\"client_ticket\":\"");
	w.Text(clientText, clientSize);
	w.Text("\",\"server_ticket\":\"");
	w.Text(serverText, serverSize);
	w.Text("\",\"extra_data\":\"{\\\"sbx\\\":\\\"RETAIL\\\",\\\"dpi\\\":\\\"MW3\\\"}\"}");
	if (!w.ok)
		return 500;
	*replySize = w.p - reply;
	Trace("auth: tickets issued (iv seed %u)", (u32)seed);
	return 200;
}
}  // namespace LocalDemonWare
