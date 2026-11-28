// Local profile only. No Xbox identity, token, privilege or entitlement authority.
// Profile results complete locally; completion callbacks use the local queue.
#include "enhanced/gdk.h"
#include "enhanced/gdk_local_queue.h"
HRESULT Gdk_QueryInterface(GdkInterface *, const GdkGuid *, void **);
u32 Gdk_AddRef(GdkInterface *);
u32 Gdk_Release(GdkInterface *);
namespace {
struct Profile { u32 references; };
Profile profile{};
using Handle = Profile *;
constexpr u64 marker = 0x4957354C4F43414Cull;
const char identity[] = "IW5LocalProfileAdd";
struct Completion {
    u64 tag;
    const void *identity;
    HRESULT result;
    u32 size;
    u64 payload;
};
static_assert(sizeof(Completion) == 32, "local completion ABI");
static_assert(sizeof(XAsyncBlock::internal) == 32, "XAsync ABI");
bool Read(XAsyncBlock *block, Completion &c)
{
    if (!block) return false;
    Gdk_Copy(&c, block->internal, sizeof(c));
    return c.tag == marker && c.identity == identity;
}
void AddCompleted(void *ctx, bool)
{
    // The profile operation is complete before notification is queued.
    // Queue termination must not retroactively change its published result.
    auto *block = static_cast<XAsyncBlock *>(ctx);
    if (block->callback) block->callback(block);
}
HRESULT AddAsync(GdkInterface *, u32 options, XAsyncBlock *block)
{
    if (!block) return GDK_E_POINTER;
    if ((options & ~7u) || ((options & 1u) && (options & 4u))) return GDK_E_INVALIDARG;

    Completion old{};
    if (Read(block, old)) return GDK_E_INVALIDARG;
    Completion c{marker, identity, GDK_S_OK, sizeof(Handle), reinterpret_cast<u64>(&profile)};
    XTaskQueueHandle queue = nullptr;
    if (block->queue) {
        HRESULT hr = LocalQueueDuplicate(nullptr,block->queue,&queue);
        if (hr < 0) return hr;
    } else if (block->callback && !LocalQueueCurrent(nullptr,&queue)) {
        return GDK_E_NOTIMPL;
    }
    Gdk_Copy(block->internal, &c, sizeof(c));
    HRESULT hr = GDK_S_OK;
    if (block->callback) {
        hr = LocalQueueSubmit(nullptr,queue,1,block,AddCompleted);
        if (hr < 0) Gdk_Zero(block->internal,sizeof(block->internal));
    }
    if (queue) LocalQueueClose(nullptr,queue);
    return hr;
}
HRESULT Status(GdkInterface *, XAsyncBlock *block, bool)
{
    Completion c{};
    return Read(block, c) ? c.result : GDK_E_INVALIDARG;
}
HRESULT Size(GdkInterface *, XAsyncBlock *block, size_t *out)
{
    if (!out) return GDK_E_POINTER;
    *out = 0;
    Completion c{};
    if (!Read(block, c)) return GDK_E_INVALIDARG;
    *out = c.size;
    return c.result;
}
void Cancel(GdkInterface *, XAsyncBlock *block)
{
    // The operation completed at submission; cancellation is not retroactive.
    (void)block;
}
bool Retain(bool existing)
{
    u32 n = __atomic_load_n(&profile.references, __ATOMIC_ACQUIRE);
    do {
        if ((existing && !n) || n == 0xffffffffu) return false;
    } while (!__atomic_compare_exchange_n(&profile.references, &n, n+1,
                                           false, __ATOMIC_ACQ_REL, __ATOMIC_ACQUIRE));
    return true;
}
bool Valid(Handle h)
{
    return h == &profile && __atomic_load_n(&profile.references, __ATOMIC_ACQUIRE);
}
HRESULT AddResult(GdkInterface *, XAsyncBlock *block, Handle *out)
{
    if (!out) return GDK_E_POINTER;
    *out = nullptr;
    Completion c{};
    if (!Read(block, c)) return GDK_E_INVALIDARG;
    if (c.result < 0) { Gdk_Zero(block->internal,sizeof(block->internal)); return c.result; }
    if (c.size != sizeof(Handle)) return GDK_E_INVALIDARG;
    if (!Retain(false)) return GDK_E_FAIL;
    *out = &profile;
    Gdk_Zero(block->internal, sizeof(block->internal));
    return GDK_S_OK;
}
HRESULT Duplicate(GdkInterface *, Handle h, Handle *out)
{
    if (!out) return GDK_E_POINTER;
    *out = nullptr;
    if (h != &profile || !Retain(true)) return GDK_E_INVALIDARG;
    *out = h;
    return GDK_S_OK;
}
void Close(GdkInterface *, Handle h)
{
    if (h != &profile) return;
    u32 n = __atomic_load_n(&profile.references, __ATOMIC_ACQUIRE);
    while (n && !__atomic_compare_exchange_n(&profile.references, &n, n-1,
                                            false, __ATOMIC_ACQ_REL, __ATOMIC_ACQUIRE)) {}
}
int Compare(GdkInterface *, Handle a, Handle b)
{
    auto x = reinterpret_cast<u64>(a), y = reinterpret_cast<u64>(b);
    return x == y ? 0 : x < y ? -1 : 1;
}
HRESULT LocalId(GdkInterface *, Handle h, u64 *out)
{
    if (!out) return GDK_E_POINTER;
    *out = 0;
    if (!Valid(h)) return GDK_E_INVALIDARG;
    *out = 1;
    return GDK_S_OK;
}
HRESULT XboxId(GdkInterface *, Handle h, u64 *out)
{
    if (!out) return GDK_E_POINTER;
    *out = 0;
    return Valid(h) ? GDK_E_NOINTERFACE : GDK_E_INVALIDARG;
}
HRESULT Guest(GdkInterface *, Handle h, bool *out)
{
    if (!out) return GDK_E_POINTER;
    *out = false;
    return Valid(h) ? GDK_S_OK : GDK_E_INVALIDARG;
}
HRESULT State(GdkInterface *, Handle h, u32 *out)
{
    if (!out) return GDK_E_POINTER;
    *out = 2; // Xbox services SignedOut: local identity is not Xbox sign-in.
    return Valid(h) ? GDK_S_OK : GDK_E_INVALIDARG;
}
HRESULT Age(GdkInterface *, Handle h, u32 *out)
{
    if (!out) return GDK_E_POINTER;
    *out = 0; // Unknown.
    return Valid(h) ? GDK_S_OK : GDK_E_INVALIDARG;
}
HRESULT Gamertag(GdkInterface *, Handle h, u32 kind, size_t capacity, char *out, size_t *used)
{
    if (used) *used = 0;
    if (!Valid(h) || kind > 3) return GDK_E_INVALIDARG;
    const char *name = Gdk_Gamertag();
    size_t count = Gdk_StrLen(name) + 1;
    if (used) *used = count;
    if (capacity < count) return GDK_E_NOT_SUFFICIENT_BUFFER;
    if (!out) return GDK_E_POINTER;
    Gdk_Copy(out, name, count);
    return GDK_S_OK;
}
HRESULT Privilege(GdkInterface *, Handle, u32, u32, bool *allowed, u32 *reason)
{
    if (allowed) *allowed = false;
    if (reason) *reason = 0;
    return GDK_E_NOINTERFACE;
}
HRESULT Unavailable() { return GDK_E_NOINTERFACE; }
void Noop() {}
bool False() { return false; }
#define V(f) reinterpret_cast<void *>(f)
#define U V(Unavailable)
static void *const userSlots[] = {
    V(Gdk_QueryInterface), V(Gdk_AddRef), V(Gdk_Release),
    V(Duplicate), V(Close), V(Compare), U, V(AddAsync), V(AddResult),
    V(LocalId), U, V(XboxId), U, V(Guest), V(State), U,
    U, U, U, V(Age), V(Privilege), U, U, U, U, U,
    U, U, U, U, U, U, U, U, V(False), U, U, U, U, U, U, U, V(False)
};
static_assert(sizeof(userSlots)/sizeof(*userSlots) == 0x150/8+1, "XUser slot count");
static void *const tagSlots[] = {V(Gdk_QueryInterface), V(Gdk_AddRef), V(Gdk_Release), V(Gamertag)};
static void *const asyncSlots[] = {
    V(Gdk_QueryInterface), V(Gdk_AddRef), V(Gdk_Release),
    V(Status), V(Size), V(Cancel), U, U, U, U, V(Noop), U,
    V(LocalQueueCreate), V(LocalQueueComposite), V(LocalQueuePort), V(LocalQueueDuplicate), V(LocalQueueDispatch),
    V(LocalQueueClose), V(LocalQueueSubmit), V(LocalQueueDelay), U, U, V(LocalQueueTerminate), U, U,
    V(LocalQueueCurrent), V(LocalQueueSetCurrent), U, U, V(Noop)
};
static_assert(sizeof(asyncSlots)/sizeof(*asyncSlots) == 0xe8/8+1, "XAsync slot count");
#undef U
#undef V
}
GdkInterface g_iw5LocalUser = {userSlots};
GdkInterface g_iw5LocalGamertag = {tagSlots};
GdkInterface g_iw5LocalAsync = {asyncSlots};
