// enhanced/include/enhanced/local_demonware.h: the local DemonWare inside iw5_enhanced.dll (docs/online.md).
//
// Loopback only. The game's own DemonWare client reaches it through three redirects: the resolver answers
// 127.0.0.1 for the auth and lobby host names (enhanced/DW/bd_platform_socket.cpp), the auth request the game sends
// over WinHTTP to https://127.0.0.1:443/auth/ is answered in process (enhanced/win32/win_vendor_network.cpp: the
// game only speaks HTTPS, and a loopback HTTPS endpoint would need a certificate installed in Windows), and the
// lobby connection to 127.0.0.1:3074 goes to the service's own TCP port.
#pragma once

typedef unsigned char u8;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef __SIZE_TYPE__ size_t;

namespace LocalDemonWare
{
constexpr u32 kLoopback = 0x0100007F;  // 127.0.0.1 as the resolver writes it (network byte order)
constexpr unsigned short kLobbyPort = 3074;

// The host names the service answers for.
bool IsServiceHost(const char *name);
// Start the service once (idempotent). False if its socket couldn't be opened.
bool Start();
// The loopback TCP port the lobby listens on (after Start).
unsigned short LobbyPort();
// The auth endpoint: request body (JSON) and Authorization header in, reply body out. Returns the HTTP status.
u32 Authorize(const char *body, size_t size, const char *authorization, char *reply, size_t capacity,
              size_t *replySize);
// One line in iw5_enhanced.log, prefixed "dw: ".
void Trace(const char *format, ...);
}  // namespace LocalDemonWare

// enhanced/DW/local_crypto.cpp: SHA-1, HMAC-SHA1, AES-128-CBC and random bytes through Windows' CNG (bcrypt).
namespace LocalCrypto
{
bool Sha1(const u8 *data, size_t size, u8 out[20]);
bool HmacSha1(const u8 *key, size_t keySize, const u8 *data, size_t size, u8 out[20]);
// HMAC-SHA1 over several pieces.
bool HmacSha1Parts(const u8 *key, size_t keySize, const u8 *const *parts, const size_t *sizes, int count, u8 out[20]);
bool Aes128Cbc(bool encrypt, const u8 key[16], const u8 iv[16], const u8 *in, size_t size, u8 *out);
bool Random(u8 *out, size_t size);
}  // namespace LocalCrypto
