// enhanced/libHttpClient/HTTP/Curl/CurlProvider.cpp: no Xbox Live HTTP (P0003; docs/online.md).
//
// The Xbox services library the game links (achievements, presence, social) sends its requests through
// libHttpClient's XCurl provider. In the enhanced build nothing reaches Xbox Live: the provider is created as the
// original creates it but without starting XCurl (curl_global_init, which also loads the system's GDK runtime from
// XCurl.dll), and every request fails at once with E_HC_NO_NETWORK, the library's own "no network" result, which
// the services handle as an offline console does. Xbox services start (XblInitialize succeeds, as the game requires
// before it will run) and simply never get an answer.
#include "libHttpClient/curl_provider.h"

extern "C" __declspec(dllimport) void _Mtx_init_in_situ(void *mutex, int type);
extern "C" __declspec(dllimport) void _CxxThrowException(void *object, void *info);

namespace std
{
extern void *const bad_alloc_vftable[];
extern const char bad_alloc_message[];
}  // namespace std

// std::bad_alloc as its constructor (140363F70) builds it: vftable, "bad allocation", nothing to free
struct BadAlloc
{
	void *const *vftable;
	const char *what;
	unsigned long long doFree;
};

[[noreturn]] static void ThrowBadAlloc(void *throwInfo)
{
	BadAlloc e;
	e.vftable = std::bad_alloc_vftable;
	e.what = std::bad_alloc_message;
	e.doFree = 0;
	_CxxThrowException(&e, throwInfo);
	__builtin_unreachable();
}

namespace xbox
{
namespace httpclient
{
void *http_alloc(unsigned long long size);
extern void *const CurlProvider_vftable[];
extern unsigned char bad_alloc_ThrowInfo[];

// 1403D81D0: the original, without curl_global_init
CurlProviderResult *CurlProvider::Initialize(CurlProviderResult *result)
{
	CurlProvider *provider = static_cast<CurlProvider *>(http_alloc(sizeof(CurlProvider)));
	if (!provider)
		ThrowBadAlloc(bad_alloc_ThrowInfo);
	provider->vftable = CurlProvider_vftable;
	provider->curlMultisHead = nullptr;
	provider->curlMultisSize = 0;
	void **node = static_cast<void **>(http_alloc(0x38));
	if (!node)
		ThrowBadAlloc(bad_alloc_ThrowInfo);
	node[0] = node;
	node[1] = node;
	node[2] = node;
	*reinterpret_cast<unsigned short *>(node + 3) = 0x101;
	provider->curlMultisHead = node;
	_Mtx_init_in_situ(provider->mutex, 2);
	provider->cleanupAsyncBlock = nullptr;
	provider->multiCleanupAsyncBlocks[0] = nullptr;
	provider->multiCleanupAsyncBlocks[1] = nullptr;
	provider->multiCleanupAsyncBlocks[2] = nullptr;
	provider->multiCleanupQueue = nullptr;
	provider->cleanupTasksRemaining = 0;
	result->hr = 0;
	*(unsigned long long *)result->message = 0;
	result->size = 0;
	result->capacity = 15;
	result->message[0] = 0;
	result->allocator = (unsigned char)(unsigned long long)&provider->curlMultisHead;
	result->payload = provider;
	result->hasPayload = true;
	return result;
}

// 1403D83C0: no request is sent. A failed PerformAsync completes the call with that result (libHttpClient's
// HC_CALL perform provider), so the caller sees E_HC_NO_NETWORK.
HRESULT CurlProvider::PerformAsync(void *, XAsyncBlock *)
{
	return (HRESULT)0x89235006;  // E_HC_NO_NETWORK
}
}  // namespace httpclient
}  // namespace xbox
