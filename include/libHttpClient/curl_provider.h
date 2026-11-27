// include/libHttpClient/curl_provider.h: libHttpClient's XCurl provider as the Store build links it (the GDK's
// libHttpClient, statically linked through the Xbox services library; source in microsoft/libHttpClient,
// Source/HTTP/Curl/CurlProvider.cpp). Layouts read from the Store build's own code (docs/online.md).
#pragma once

typedef long HRESULT;
struct XAsyncBlock;

namespace xbox
{
namespace httpclient
{
// Result<HC_UNIQUE_PTR<CurlProvider>>: HRESULT, an error-message std::string (MSVC layout), the payload.
struct CurlProviderResult
{
	HRESULT hr;
	unsigned int pad;
	char message[16];             // std::string's buffer (small-string storage)
	unsigned long long size;      // +0x18
	unsigned long long capacity;  // +0x20
	unsigned char allocator;      // +0x28: the http_stl_allocator (empty, its byte copied)
	unsigned char pad2[7];
	struct CurlProvider *payload; // +0x30
	bool hasPayload;              // +0x38
};
static_assert(__builtin_offsetof(CurlProviderResult, payload) == 0x30, "Result layout");

struct CurlProvider
{
	void *const *vftable;
	void *curlMultisHead;  // http_internal_map<XTaskQueuePortHandle, ...>: the list's sentinel node
	unsigned long long curlMultisSize;
	unsigned char mutex[0x50];             // std::mutex (_Mtx_init_in_situ)
	XAsyncBlock *cleanupAsyncBlock;        // +0x68
	void *multiCleanupAsyncBlocks[3];      // +0x70 http_internal_vector<XAsyncBlock>
	void *multiCleanupQueue;               // +0x88
	unsigned long long cleanupTasksRemaining;  // +0x90

	static CurlProviderResult *Initialize(CurlProviderResult *result);
	HRESULT PerformAsync(void *callHandle, XAsyncBlock *async);
};
static_assert(sizeof(CurlProvider) == 0x98, "CurlProvider layout");

HRESULT HrFromCurle(int code);
}  // namespace httpclient
}  // namespace xbox
