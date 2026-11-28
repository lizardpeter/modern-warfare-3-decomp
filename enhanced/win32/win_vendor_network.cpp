// enhanced/win32/win_vendor_network.cpp: vendor HTTP, nothing leaves the machine; the DemonWare auth request goes to
// the local DemonWare (docs/online.md).
#include "enhanced/local_demonware.h"

extern "C" void BdCompleteWinHttpRequest(void *request, long result);

namespace
{
constexpr long cannotConnect = static_cast<long>(0x80072efdu);

// bdHTTPResponseWinHTTP, as BeginHttpRequest, SendHttpRequest and the status callback use it (1403E9FE0,
// 1403EA660, 1403EA9B0).
struct Request
{
	unsigned char raw[0x20d0 + 0x1000];
	unsigned char *&ResponseBuffer() { return *reinterpret_cast<unsigned char **>(raw + 0x20); }
	unsigned &ResponseCapacity() { return *reinterpret_cast<unsigned *>(raw + 0x28); }
	const char *Body() { return *reinterpret_cast<const char **>(raw + 0x30); }
	unsigned BodySize() { return *reinterpret_cast<unsigned *>(raw + 0x38); }
	void *Stream() { return *reinterpret_cast<void **>(raw + 0x40); }
	unsigned &HttpCode() { return *reinterpret_cast<unsigned *>(raw + 0x14); }
	unsigned &StatusCode() { return *reinterpret_cast<unsigned *>(raw + 0x54); }
	bool &HaveStatus() { return *reinterpret_cast<bool *>(raw + 0x50); }
	unsigned &Received() { return *reinterpret_cast<unsigned *>(raw + 0x60); }
	const wchar_t *Url() { return reinterpret_cast<const wchar_t *>(raw + 0x88); }
};

bool Prefix(const wchar_t *s, const char *p)
{
	for (; *p; ++s, ++p)
		if (*s != *p)
			return false;
	return true;
}

bool Contains(const wchar_t *s, const char *p)
{
	for (; *s; ++s)
		if (Prefix(s, p))
			return true;
	return false;
}
}  // namespace

// 1403E9FE0: the auth request to the address the resolver gave for auth3 (https://127.0.0.1:443/auth/) is answered
// by the local DemonWare as the WinHTTP path would deliver it: the body into the response buffer, status 200,
// finalized BD_DONE. Every other request fails to connect before a connection is opened.
extern "C" void BdBeginWinHttpRequest(void *, void *requestObject)
{
	Request *request = static_cast<Request *>(requestObject);
	if (!Prefix(request->Url(), "https://127.0.0.1:") || !Contains(request->Url(), "/auth/") || !request->Body() ||
	    !request->ResponseBuffer() || request->Stream())
	{
		BdCompleteWinHttpRequest(request, cannotConnect);
		return;
	}
	size_t size = 0;
	unsigned code = LocalDemonWare::Authorize(request->Body(), request->BodySize(), nullptr,
	                                          reinterpret_cast<char *>(request->ResponseBuffer()),
	                                          request->ResponseCapacity(), &size);
	request->StatusCode() = code;
	request->HttpCode() = code;
	request->HaveStatus() = true;
	request->Received() = (unsigned)size;
	BdCompleteWinHttpRequest(request, 0);
}

extern "C" long WinHttpInitializeRequest(void *)
{
	return cannotConnect;
}
