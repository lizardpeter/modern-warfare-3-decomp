// src/libHttpClient/HTTP/Curl/CurlProvider.cpp: libHttpClient's XCurl provider, as linked into the Store build.
#include "libHttpClient/curl_provider.h"

extern "C" __declspec(dllimport) int curl_global_init(long flags);
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
void *http_alloc(unsigned long long size);  // the library's allocator hook (http_memory)
extern void *const CurlProvider_vftable[];
extern unsigned char bad_alloc_ThrowInfo[];

// 1403D8190
HRESULT HrFromCurle(int code)
{
	if (code == 0)
		return 0;  // CURLE_OK
	if (code == 0x2B)
		return (HRESULT)0x80070057;  // CURLE_BAD_FUNCTION_ARGUMENT: E_INVALIDARG
	return (HRESULT)0x80004005;      // E_FAIL
}

// 1403D81D0  CurlProvider.cpp: Initialize
CurlProviderResult *CurlProvider::Initialize(CurlProviderResult *result)
{
	HRESULT hr = HrFromCurle(curl_global_init(3));  // CURL_GLOBAL_ALL
	if (hr < 0)
	{
		result->hr = hr;
		*(unsigned long long *)result->message = 0;
		result->size = 0;
		result->capacity = 15;
		result->message[0] = 0;
		result->hasPayload = false;
		return result;
	}
	CurlProvider *provider = static_cast<CurlProvider *>(http_alloc(sizeof(CurlProvider)));
	if (!provider)
		ThrowBadAlloc(bad_alloc_ThrowInfo);
	provider->vftable = CurlProvider_vftable;
	provider->curlMultisHead = nullptr;
	provider->curlMultisSize = 0;
	void **node = static_cast<void **>(http_alloc(0x38));  // the list's sentinel: next, prev, parent, colors
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
	// the empty allocator's byte, copied from a stack slot that holds &provider->curlMultisHead by then
	result->allocator = (unsigned char)(unsigned long long)&provider->curlMultisHead;
	result->payload = provider;
	result->hasPayload = true;
	return result;
}
}  // namespace httpclient
}  // namespace xbox
