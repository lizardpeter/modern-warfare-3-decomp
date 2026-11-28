// Process-local scheduling only. No account or platform service calls.
#include "enhanced/gdk_local_queue.h"
#ifdef IW5_QUEUE_HOST_TEST
extern u64 QueueNow();
extern void QueueSleep(u32);
extern bool QueueSpawn(unsigned long (*)(void *), void *);
#define QCALL
#else
#include <windows.h>
WINBASEAPI unsigned long long WINAPI GetTickCount64();
WINBASEAPI BOOL WINAPI QueueUserWorkItem(LPTHREAD_START_ROUTINE, void *, unsigned long);
static u64 QueueNow() { return GetTickCount64(); }
static void QueueSleep(u32 ms) { Sleep(ms); }
static bool QueueSpawn(DWORD (WINAPI *work)(void *), void *ctx)
{
    return QueueUserWorkItem(work, ctx, 0) != 0;
}
#define QCALL WINAPI
#endif
namespace {
constexpr u32 capacity = 64, queueCount = 16;
constexpr HRESULT closedError = (HRESULT)0x80070006u;
struct Queue;
struct Port { Queue *owner; u32 index, mode, active; };
struct Work {
    XTaskQueueCallback *fn;
    void *context;
    u64 due, sequence;
    u32 port;
};
struct Queue {
    u32 refs, handles;
    bool terminating, notifying, notified;
    u32 pending;
    u64 sequence;
    Port ports[2];
    Work work[capacity];
    XTaskQueueTerminatedCallback *terminated;
    void *terminateContext;
};
Queue queues[queueCount]{};
Queue *processQueue;
bool processInitialized;
u32 gate;
void Lock() { while (__atomic_exchange_n(&gate, 1u, __ATOMIC_ACQUIRE)) {} }
void Unlock() { __atomic_store_n(&gate, 0u, __ATOMIC_RELEASE); }
XTaskQueueHandle Handle(Queue *q) { return reinterpret_cast<XTaskQueueHandle>(q); }
Queue *Find(XTaskQueueHandle h)
{
    for (auto &q : queues) if (Handle(&q) == h && q.handles) return &q;
    return nullptr;
}
Queue *New(u32 workMode, u32 completionMode)
{
    for (auto &q : queues) {
        if (q.refs) continue;
        Gdk_Zero(&q, sizeof(q));
        q.refs = q.handles = 1;
        q.ports[0] = {&q,0,workMode,0};
        q.ports[1] = {&q,1,completionMode,0};
        return &q;
    }
    return nullptr;
}
void Drop(Queue *q) { --q->refs; }
void DropHandle(Queue *q) { --q->handles; Drop(q); }
void AddHandle(Queue *q) { ++q->handles; ++q->refs; }
Queue *Acquire(XTaskQueueHandle h)
{
    Lock();
    Queue *q = Find(h);
    if (q) ++q->refs;
    Unlock();
    return q;
}
void Release(Queue *q) { Lock(); Drop(q); Unlock(); }
void Notify(Queue *q)
{
    Lock();
    if (!q->terminating || q->notifying || q->notified || q->pending ||
        q->ports[0].active || q->ports[1].active) {
        Unlock(); return;
    }
    q->notifying = true;
    auto fn = q->terminated;
    auto ctx = q->terminateContext;
    Unlock();
    if (fn) fn(ctx);
    Lock();
    q->notified = true;
    q->notifying = false;
    Unlock();
}
bool DispatchOne(Queue *q, u32 port)
{
    Lock();
    Port &p = q->ports[port];
    if (p.mode == 2 && p.active) { Unlock(); return false; }
    Work *selected = nullptr;
    const u64 now = QueueNow();
    for (auto &w : q->work) {
        if (!w.fn || w.port != port || (!q->terminating && w.due > now)) continue;
        if (!selected || w.due < selected->due ||
            (w.due == selected->due && w.sequence < selected->sequence)) selected = &w;
    }
    if (!selected) { Unlock(); return false; }
    Work w = *selected;
    selected->fn = nullptr;
    --q->pending;
    ++p.active;
    bool canceled = q->terminating;
    Unlock();
    w.fn(w.context, canceled);
    Lock();
    --p.active;
    Drop(q); // pending work reference
    Unlock();
    Notify(q);
    return true;
}
unsigned long QCALL Worker(void *ctx)
{
    Port *p = static_cast<Port *>(ctx);
    Queue *q = p->owner;
    for (;;) {
        if (DispatchOne(q,p->index)) break;
        Lock();
        bool pending = false;
        for (auto &w : q->work) if (w.fn && w.port == p->index) { pending = true; break; }
        Unlock();
        if (!pending) break;
        QueueSleep(1);
    }
    Release(q);
    return 0;
}
}
HRESULT LocalQueueCreate(GdkInterface *, u32 a, u32 b, XTaskQueueHandle *out)
{
    if (!out) return GDK_E_POINTER;
    *out = nullptr;
    if (a > 3 || b > 3) return GDK_E_INVALIDARG;
    Lock(); Queue *q = New(a,b); Unlock();
    if (!q) return GDK_E_OUTOFMEMORY;
    *out = Handle(q);
    return GDK_S_OK;
}
HRESULT LocalQueueComposite(GdkInterface *, XTaskQueuePortHandle, XTaskQueuePortHandle, XTaskQueueHandle *out)
{
    if (!out) return GDK_E_POINTER;
    *out = nullptr;
    return GDK_E_NOTIMPL; // shared port ownership is not implemented
}
HRESULT LocalQueuePort(GdkInterface *, XTaskQueueHandle h, u32 port, XTaskQueuePortHandle *out)
{
    if (!out) return GDK_E_POINTER;
    *out = nullptr;
    Lock(); Queue *q = Find(h);
    if (q && port < 2) *out = reinterpret_cast<XTaskQueuePortHandle>(&q->ports[port]);
    Unlock();
    return *out ? GDK_S_OK : GDK_E_INVALIDARG;
}
HRESULT LocalQueueDuplicate(GdkInterface *, XTaskQueueHandle h, XTaskQueueHandle *out)
{
    if (!out) return GDK_E_POINTER;
    *out = nullptr;
    Lock(); Queue *q = Find(h);
    if (q) { AddHandle(q); *out = h; }
    Unlock();
    return q ? GDK_S_OK : GDK_E_INVALIDARG;
}
void LocalQueueClose(GdkInterface *, XTaskQueueHandle h)
{
    Lock(); Queue *q = Find(h);
    if (q) DropHandle(q);
    Unlock();
}
bool LocalQueueDispatch(GdkInterface *, XTaskQueueHandle h, u32 port, u32 timeout)
{
    if (port > 1) return false;
    Queue *q = Acquire(h);
    if (!q) return false;
    const u64 start = QueueNow();
    bool ran = false;
    for (;;) {
        if (DispatchOne(q,port)) { ran = true; break; }
        Lock();
        bool stopped = q->terminating;
        Unlock();
        if (stopped || !timeout || (timeout != 0xffffffffu && QueueNow()-start >= timeout)) break;
        QueueSleep(1);
    }
    Release(q);
    return ran;
}
HRESULT LocalQueueDelay(GdkInterface *, XTaskQueueHandle h, u32 port, u32 delay, void *context, XTaskQueueCallback *fn)
{
    if (port > 1 || !fn) return GDK_E_INVALIDARG;
    Lock();
    Queue *q = Find(h);
    if (!q || q->terminating) { Unlock(); return closedError; }
    Port &p = q->ports[port];
    if (p.mode == 3 && !delay) {
        ++q->refs; ++p.active;
        Unlock();
        fn(context,false);
        Lock(); --p.active; Unlock();
        Notify(q); Release(q);
        return GDK_S_OK;
    }
    Work *slot = nullptr;
    for (auto &w : q->work) if (!w.fn) { slot = &w; break; }
    if (!slot) { Unlock(); return GDK_E_OUTOFMEMORY; }
    *slot = {fn,context,QueueNow()+delay,++q->sequence,port};
    ++q->pending; ++q->refs;
    if (p.mode != 0) {
        ++q->refs;
        if (!QueueSpawn(Worker,&p)) {
            slot->fn = nullptr; --q->pending; Drop(q); Drop(q);
            Unlock(); return GDK_E_FAIL;
        }
    }
    Unlock();
    return GDK_S_OK;
}
HRESULT LocalQueueSubmit(GdkInterface *i, XTaskQueueHandle h, u32 port, void *ctx, XTaskQueueCallback *fn)
{
    return LocalQueueDelay(i,h,port,0,ctx,fn);
}
HRESULT LocalQueueTerminate(GdkInterface *, XTaskQueueHandle h, bool wait, void *context, XTaskQueueTerminatedCallback *callback)
{
    Queue *q = Acquire(h);
    if (!q) return GDK_E_INVALIDARG;
    Lock();
    if (q->terminating) { Unlock(); Release(q); return GDK_E_INVALIDARG; }
    q->terminating = true; q->terminated = callback; q->terminateContext = context;
    Unlock();
    Notify(q);
    if (wait) {
        for (;;) {
            // Manual ports need a dispatcher; the waiting caller may drain them.
            if (q->ports[0].mode == 0) DispatchOne(q,0);
            if (q->ports[1].mode == 0) DispatchOne(q,1);
            Notify(q);
            Lock(); bool done = q->notified; Unlock();
            if (done) break;
            QueueSleep(1);
        }
    }
    Release(q);
    return GDK_S_OK;
}
bool LocalQueueCurrent(GdkInterface *, XTaskQueueHandle *out)
{
    if (!out) return false;
    *out = nullptr;
    Lock();
    if (!processInitialized) { processQueue = New(1,1); processInitialized = processQueue != nullptr; }
    if (processQueue) { AddHandle(processQueue); *out = Handle(processQueue); }
    Unlock();
    return *out != nullptr;
}
void LocalQueueSetCurrent(GdkInterface *, XTaskQueueHandle h)
{
    Lock();
    Queue *q = Find(h);
    if (h && !q) { Unlock(); return; }
    if (q) AddHandle(q);
    if (processQueue) DropHandle(processQueue);
    processQueue = q; processInitialized = true;
    Unlock();
}
