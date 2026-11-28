#include <windows.h>

WINBASEAPI LONG WINAPI BCryptOpenAlgorithmProvider(void **, const wchar_t *, const wchar_t *, ULONG);
WINBASEAPI LONG WINAPI BCryptCloseAlgorithmProvider(void *, ULONG);
WINBASEAPI LONG WINAPI BCryptCreateHash(void *, void **, unsigned char *, ULONG, const unsigned char *, ULONG, ULONG);
WINBASEAPI LONG WINAPI BCryptHashData(void *, const unsigned char *, ULONG, ULONG);
WINBASEAPI LONG WINAPI BCryptFinishHash(void *, unsigned char *, ULONG, ULONG);
WINBASEAPI LONG WINAPI BCryptDestroyHash(void *);

namespace
{
constexpr unsigned protocol = 210;
constexpr unsigned title = 0x906;
constexpr unsigned authSize = 153;
constexpr unsigned helloSize = 28;
constexpr unsigned challengeSize = 26;
constexpr unsigned sha1Size = 20;

unsigned Read32(const unsigned char *bytes)
{
	return unsigned(bytes[0]) | unsigned(bytes[1]) << 8 |
	       unsigned(bytes[2]) << 16 | unsigned(bytes[3]) << 24;
}

void Write32(unsigned char *bytes, unsigned value)
{
	for (unsigned i = 0; i < 4; ++i)
		bytes[i] = static_cast<unsigned char>(value >> (8 * i));
}

unsigned ReadBits(const unsigned char *bytes, unsigned bit, unsigned count)
{
	unsigned value = 0;
	for (unsigned i = 0; i < count; ++i)
		value |= unsigned((bytes[(bit + i) / 8] >> ((bit + i) % 8)) & 1) << i;
	return value;
}

void Copy(unsigned char *dst, const unsigned char *src, unsigned length)
{
	for (unsigned i = 0; i < length; ++i)
		dst[i] = src[i];
}

bool Hash(void *provider, const unsigned char *key, ULONG keySize, const unsigned char *data,
          ULONG dataSize, unsigned char *digest)
{
	void *hash;
	if (BCryptCreateHash(provider, &hash, nullptr, 0, key, keySize, 0) < 0)
		return false;
	const bool ok = BCryptHashData(hash, data, dataSize, 0) >= 0 &&
	                BCryptFinishHash(hash, digest, sha1Size, 0) >= 0;
	BCryptDestroyHash(hash);
	return ok;
}

bool Expand(void *provider, const unsigned char *secret, const char *label, unsigned labelSize,
            unsigned char *out, unsigned length)
{
	unsigned char block[sha1Size];
	unsigned char message[sha1Size + 11];
	unsigned previousSize = 0;
	for (unsigned counter = 1; length; ++counter)
	{
		for (unsigned i = 0; i < labelSize; ++i)
			message[previousSize + i] = static_cast<unsigned char>(label[i]);
		message[previousSize + labelSize] = static_cast<unsigned char>(counter);
		if (!Hash(provider, secret, sha1Size, message, previousSize + labelSize + 1, block))
			return false;
		const unsigned taken = length < sha1Size ? length : sha1Size;
		Copy(out, block, taken);
		out += taken;
		length -= taken;
		Copy(message, block, sha1Size);
		previousSize = sha1Size;
	}
	return true;
}
}

// The caller supplies the session key of a ticket actually issued by its own
// service. The wire helper performs no transport or platform requests.
extern "C" bool IW5_DemonWare210Verify(const unsigned char *hello, unsigned helloLength,
                                      const unsigned char *challenge, unsigned challengeLength,
                                      const unsigned char *auth, unsigned authLength,
                                      const unsigned char *sessionKey, unsigned char *reply,
                                      unsigned char *recordKeys)
{
	if (!hello || !challenge || !auth || !sessionKey || !reply || !recordKeys ||
	    helloLength != helloSize || challengeLength != challengeSize || authLength != authSize ||
	    Read32(hello) != 200 || Read32(hello + 4) != 200 ||
	    Read32(hello + 8) != protocol || Read32(hello + 12) != protocol ||
	    Read32(hello + 16) < authSize - 4 || Read32(hello + 16) > 65535 ||
	    Read32(challenge) != challengeSize - 4 || challenge[4] != 0xab || challenge[5] != 0x81 ||
	    Read32(challenge + 6) != protocol ||
	    Read32(auth) != authSize - 4 || auth[4] != 0xab || auth[5] != 0x82)
		return false;
	const auto *body = auth + 6;
	if (ReadBits(body, 0, 9) != 0x107 || ReadBits(body, 9, 5) != 8 ||
	    ReadBits(body, 14, 32) != title || ReadBits(body, 46, 5) != 8)
		return false;

	unsigned char transcript[helloSize - 8 + challengeSize + authSize - 8];
	Copy(transcript, hello + 8, helloSize - 8);
	Copy(transcript + helloSize - 8, challenge, challengeSize);
	Copy(transcript + helloSize - 8 + challengeSize, auth, authSize - 8);
	unsigned char digest[sha1Size];
	void *sha1;
	if (BCryptOpenAlgorithmProvider(&sha1, L"SHA1", nullptr, 0) < 0)
		return false;
	const bool hashed = Hash(sha1, nullptr, 0, transcript, sizeof(transcript), digest);
	BCryptCloseAlgorithmProvider(sha1, 0);
	void *hmac;
	if (!hashed || BCryptOpenAlgorithmProvider(&hmac, L"SHA1", nullptr, 8) < 0)
		return false;
	unsigned char secret[sha1Size], proof[16], keys[72];
	bool ok = Hash(hmac, digest, sha1Size, sessionKey, 24, secret) &&
	          Expand(hmac, secret, "CLIENTCHAL", 10, proof, sizeof(proof));
	if (ok)
	{
		unsigned difference = 0;
		for (unsigned i = 0; i < 8; ++i)
			difference |= proof[i] ^ auth[authSize - 8 + i];
		ok = difference == 0 && Expand(hmac, secret, "BDDATA", 6, keys, sizeof(keys));
	}
	BCryptCloseAlgorithmProvider(hmac, 0);
	if (ok)
	{
		Write32(reply, 10);
		reply[4] = 0xab;
		reply[5] = 0x83;
		Copy(reply + 6, proof + 8, 8);
		Copy(recordKeys, keys, sizeof(keys));
	}
	return ok;
}
