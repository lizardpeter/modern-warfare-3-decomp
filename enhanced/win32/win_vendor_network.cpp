// enhanced/win32/win_vendor_network.cpp: vendor HTTP, nothing leaves the machine; the DemonWare auth request goes to
// the local DemonWare (docs/online.md).
#include "enhanced/local_demonware.h"
#include "enhanced/local_http_route.h"

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
	const wchar_t *Method() { return reinterpret_cast<const wchar_t *>(raw + 0x1088); }
	void *CompletionStream() { return *reinterpret_cast<void **>(raw + 0x48); }
};

}  // namespace

// 1403E9FE0: the auth request to the address the resolver gave for auth3 (https://127.0.0.1:443/auth/) is answered
// by the local DemonWare as the WinHTTP path would deliver it: the body into the response buffer, status 200,
// finalized BD_DONE. Every other request fails to connect before a connection is opened.
extern "C" void BdBeginWinHttpRequest(void *, void *requestObject)
{
	Request *request = static_cast<Request *>(requestObject);
	bool route = iw5::local_http::Equal(request->Url(),"https://127.0.0.1:443/auth/");
#ifdef IW5_CI_LOCAL_RESPONSE
    route = iw5::local_http::IsAuthRoute(request->Url());
    LocalDemonWare::Trace("http: local route %u",route ? 1u : 0u);
#endif
    if (!route || !iw5::local_http::Equal(request->Method(),"POST") || !request->Body() ||
        !request->ResponseBuffer() || !request->ResponseCapacity() || request->Stream() ||
        request->CompletionStream())
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
#ifdef IW5_CI_LOCAL_RESPONSE
    LocalDemonWare::Trace("http: completed in-process status %u bytes %u",code,(unsigned)size);
#endif
	BdCompleteWinHttpRequest(request, 0);
}

extern "C" long WinHttpInitializeRequest(void *)
{
	return cannotConnect;
}
