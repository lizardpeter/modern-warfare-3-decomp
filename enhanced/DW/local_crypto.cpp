// enhanced/DW/local_crypto.cpp: the local DemonWare's cryptography through Windows' CNG, as the game's own
// DemonWare uses it (bcrypt.dll is already one of the exe's imports).
#include "enhanced/local_demonware.h"

typedef void *BCRYPT_ALG_HANDLE;
typedef void *BCRYPT_HASH_HANDLE;
typedef void *BCRYPT_KEY_HANDLE;
typedef long NTSTATUS;
extern "C" __declspec(dllimport) NTSTATUS __stdcall BCryptOpenAlgorithmProvider(BCRYPT_ALG_HANDLE *, const wchar_t *,
                                                                                 const wchar_t *, unsigned long);
extern "C" __declspec(dllimport) NTSTATUS __stdcall BCryptCloseAlgorithmProvider(BCRYPT_ALG_HANDLE, unsigned long);
extern "C" __declspec(dllimport) NTSTATUS __stdcall BCryptCreateHash(BCRYPT_ALG_HANDLE, BCRYPT_HASH_HANDLE *, u8 *,
                                                                      unsigned long, u8 *, unsigned long, unsigned long);
extern "C" __declspec(dllimport) NTSTATUS __stdcall BCryptHashData(BCRYPT_HASH_HANDLE, const u8 *, unsigned long,
                                                                    unsigned long);
extern "C" __declspec(dllimport) NTSTATUS __stdcall BCryptFinishHash(BCRYPT_HASH_HANDLE, u8 *, unsigned long,
                                                                      unsigned long);
extern "C" __declspec(dllimport) NTSTATUS __stdcall BCryptDestroyHash(BCRYPT_HASH_HANDLE);
extern "C" __declspec(dllimport) NTSTATUS __stdcall BCryptGenerateSymmetricKey(BCRYPT_ALG_HANDLE, BCRYPT_KEY_HANDLE *,
                                                                                u8 *, unsigned long, u8 *,
                                                                                unsigned long, unsigned long);
extern "C" __declspec(dllimport) NTSTATUS __stdcall BCryptSetProperty(void *, const wchar_t *, u8 *, unsigned long,
                                                                       unsigned long);
extern "C" __declspec(dllimport) NTSTATUS __stdcall BCryptEncrypt(BCRYPT_KEY_HANDLE, const u8 *, unsigned long, void *,
                                                                   u8 *, unsigned long, u8 *, unsigned long,
                                                                   unsigned long *, unsigned long);
extern "C" __declspec(dllimport) NTSTATUS __stdcall BCryptDecrypt(BCRYPT_KEY_HANDLE, const u8 *, unsigned long, void *,
                                                                   u8 *, unsigned long, u8 *, unsigned long,
                                                                   unsigned long *, unsigned long);
extern "C" __declspec(dllimport) NTSTATUS __stdcall BCryptDestroyKey(BCRYPT_KEY_HANDLE);
extern "C" __declspec(dllimport) NTSTATUS __stdcall BCryptGenRandom(BCRYPT_ALG_HANDLE, u8 *, unsigned long,
                                                                     unsigned long);

namespace
{
constexpr unsigned long kHmacFlag = 8;           // BCRYPT_ALG_HANDLE_HMAC_FLAG
constexpr unsigned long kSystemRng = 2;          // BCRYPT_USE_SYSTEM_PREFERRED_RNG

bool Hash(bool hmac, const u8 *key, size_t keySize, const u8 *const *parts, const size_t *sizes, int count, u8 out[20])
{
	BCRYPT_ALG_HANDLE alg = nullptr;
	if (BCryptOpenAlgorithmProvider(&alg, L"SHA1", nullptr, hmac ? kHmacFlag : 0) < 0)
		return false;
	BCRYPT_HASH_HANDLE hash = nullptr;
	bool ok = BCryptCreateHash(alg, &hash, nullptr, 0, const_cast<u8 *>(key), (unsigned long)keySize, 0) >= 0;
	for (int i = 0; ok && i < count; ++i)
		ok = !sizes[i] || BCryptHashData(hash, parts[i], (unsigned long)sizes[i], 0) >= 0;
	ok = ok && BCryptFinishHash(hash, out, 20, 0) >= 0;
	if (hash)
		BCryptDestroyHash(hash);
	BCryptCloseAlgorithmProvider(alg, 0);
	return ok;
}
}  // namespace

namespace LocalCrypto
{
bool Sha1(const u8 *data, size_t size, u8 out[20])
{
	const u8 *parts[] = {data};
	size_t sizes[] = {size};
	return Hash(false, nullptr, 0, parts, sizes, 1, out);
}

bool HmacSha1(const u8 *key, size_t keySize, const u8 *data, size_t size, u8 out[20])
{
	const u8 *parts[] = {data};
	size_t sizes[] = {size};
	return Hash(true, key, keySize, parts, sizes, 1, out);
}

bool HmacSha1Parts(const u8 *key, size_t keySize, const u8 *const *parts, const size_t *sizes, int count, u8 out[20])
{
	return Hash(true, key, keySize, parts, sizes, count, out);
}

// Whole blocks, no padding: the lobby records pad themselves.
bool Aes128Cbc(bool encrypt, const u8 key[16], const u8 iv[16], const u8 *in, size_t size, u8 *out)
{
	if (size % 16)
		return false;
	BCRYPT_ALG_HANDLE alg = nullptr;
	if (BCryptOpenAlgorithmProvider(&alg, L"AES", nullptr, 0) < 0)
		return false;
	wchar_t mode[] = L"ChainingModeCBC";
	bool ok = BCryptSetProperty(alg, L"ChainingMode", reinterpret_cast<u8 *>(mode), sizeof(mode), 0) >= 0;
	BCRYPT_KEY_HANDLE handle = nullptr;
	ok = ok && BCryptGenerateSymmetricKey(alg, &handle, nullptr, 0, const_cast<u8 *>(key), 16, 0) >= 0;
	u8 chain[16];
	for (int i = 0; i < 16; ++i)
		chain[i] = iv[i];
	unsigned long done = 0;
	if (ok)
		ok = (encrypt ? BCryptEncrypt(handle, in, (unsigned long)size, nullptr, chain, 16, out, (unsigned long)size, &done, 0)
		              : BCryptDecrypt(handle, in, (unsigned long)size, nullptr, chain, 16, out, (unsigned long)size, &done, 0)) >= 0 &&
		     done == size;
	if (handle)
		BCryptDestroyKey(handle);
	BCryptCloseAlgorithmProvider(alg, 0);
	return ok;
}

bool Random(u8 *out, size_t size)
{
	return BCryptGenRandom(nullptr, out, (unsigned long)size, kSystemRng) >= 0;
}
}  // namespace LocalCrypto
