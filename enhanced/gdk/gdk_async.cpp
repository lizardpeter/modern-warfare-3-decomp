// enhanced/gdk/gdk_async.cpp: the async plumbing the local providers complete through (docs/online.md).
//
// XAsync and XTaskQueue are the system runtime's own (gdk_runtime.cpp hands the game Microsoft's interface): the
// local XUser, XGameSave and XNetworking operations begin, complete and hand back their results through the same
// interface the game waits on, so a completion arrives on the caller's queue exactly as Gaming Services would
// deliver it.
#include "enhanced/gdk.h"

namespace
{
template <typename F> F *Slot(u32 offset)
{
	return reinterpret_cast<F *>(g_gdkAsync->vtable[offset / 8]);
}
}  // namespace

HRESULT Gdk_AsyncBegin(XAsyncBlock *async, void *context, const void *identity, const char *identityName,
                       XAsyncProvider *provider)
{
	typedef HRESULT F(GdkInterface *, XAsyncBlock *, void *, const void *, const char *, XAsyncProvider *);
	return Slot<F>(0x38)(g_gdkAsync, async, context, identity, identityName, provider);
}

HRESULT Gdk_AsyncSchedule(XAsyncBlock *async, u32 delayMs)
{
	typedef HRESULT F(GdkInterface *, XAsyncBlock *, u32);
	return Slot<F>(0x48)(g_gdkAsync, async, delayMs);
}

void Gdk_AsyncComplete(XAsyncBlock *async, HRESULT result, size_t requiredBufferSize)
{
	typedef void F(GdkInterface *, XAsyncBlock *, HRESULT, size_t);
	Slot<F>(0x50)(g_gdkAsync, async, result, requiredBufferSize);
}

HRESULT Gdk_AsyncGetResult(XAsyncBlock *async, const void *identity, size_t bufferSize, void *buffer, size_t *used)
{
	typedef HRESULT F(GdkInterface *, XAsyncBlock *, const void *, size_t, void *, size_t *);
	return Slot<F>(0x58)(g_gdkAsync, async, identity, bufferSize, buffer, used);
}

HRESULT Gdk_AsyncGetResultSize(XAsyncBlock *async, size_t *bufferSize)
{
	typedef HRESULT F(GdkInterface *, XAsyncBlock *, size_t *);
	return Slot<F>(0x20)(g_gdkAsync, async, bufferSize);
}

// The shape of every local async API: Begin, then complete from DoWork (on the caller's work port, so the
// completion arrives the way the caller expects) with a result the provider copies out.
namespace
{
struct Immediate
{
	HRESULT result;
	size_t size;
	u8 data[1];
};

HRESULT ImmediateProvider(XAsyncOp op, const XAsyncProviderData *data)
{
	Immediate *im = static_cast<Immediate *>(data->context);
	switch (op)
	{
	case XAsyncOp::Begin:
		return Gdk_AsyncSchedule(data->async, 0);
	case XAsyncOp::DoWork:
		Gdk_AsyncComplete(data->async, im->result, im->result >= 0 ? im->size : 0);
		return GDK_S_OK;
	case XAsyncOp::GetResult:
		Gdk_Copy(data->buffer, im->data, im->size);
		return GDK_S_OK;
	case XAsyncOp::Cleanup:
		Gdk_Free(im);
		return GDK_S_OK;
	default:
		return GDK_S_OK;
	}
}
}  // namespace

HRESULT Gdk_AsyncImmediate(XAsyncBlock *async, const void *identity, HRESULT result, const void *resultData,
                           size_t resultSize)
{
	Immediate *im = static_cast<Immediate *>(Gdk_Alloc(sizeof(Immediate) + resultSize));
	if (!im)
		return GDK_E_OUTOFMEMORY;
	im->result = result;
	im->size = resultSize;
	if (resultSize)
		Gdk_Copy(im->data, resultData, resultSize);
	return Gdk_AsyncBegin(async, im, identity, nullptr, ImmediateProvider);
}
