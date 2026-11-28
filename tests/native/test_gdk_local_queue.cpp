// Portable regression for the actual GDK queue adapter, with a deterministic host scheduler.
#include "enhanced/gdk_local_queue.h"
#include <cassert>
#include <cstring>
#include <vector>
#include <utility>
static u64 tick;
static std::vector<std::pair<unsigned long (*)(void *),void *>> workers;
u64 QueueNow(){ return tick; }
void QueueSleep(u32 n){ tick += n; }
bool QueueSpawn(unsigned long (*fn)(void *), void *ctx){ workers.emplace_back(fn,ctx); return true; }
void Gdk_Zero(void *p,size_t n){ std::memset(p,0,n); }
static void Pump(){ while (!workers.empty()) { auto w=workers.front(); workers.erase(workers.begin()); w.first(w.second); } }
struct Event { unsigned calls=0; bool canceled=false; };
static void Callback(void *p,bool canceled){ auto &e=*static_cast<Event *>(p); ++e.calls; e.canceled=canceled; }
static void Terminated(void *p){ ++*static_cast<unsigned *>(p); }
int main(){
    XTaskQueueHandle q=nullptr, dup=nullptr;
    assert(LocalQueueCreate(nullptr,0,0,&q)==0 && q);
    assert(LocalQueueDuplicate(nullptr,q,&dup)==0 && dup==q);
    LocalQueueClose(nullptr,q);
    Event a,b;
    assert(LocalQueueDelay(nullptr,dup,0,10,&a,Callback)==0);
    assert(LocalQueueSubmit(nullptr,dup,1,&b,Callback)==0);
    assert(!LocalQueueDispatch(nullptr,dup,0,0));
    assert(LocalQueueDispatch(nullptr,dup,1,0) && b.calls==1 && !b.canceled);
    assert(!LocalQueueDispatch(nullptr,dup,0,9));
    assert(LocalQueueDispatch(nullptr,dup,0,1) && a.calls==1 && !a.canceled);
    Event c; unsigned terminated=0;
    assert(LocalQueueDelay(nullptr,dup,0,100,&c,Callback)==0);
    assert(LocalQueueTerminate(nullptr,dup,false,&terminated,Terminated)==0 && !terminated);
    assert(LocalQueueDispatch(nullptr,dup,0,0) && c.calls==1 && c.canceled && terminated==1);
    assert(LocalQueueSubmit(nullptr,dup,0,&c,Callback)!=0);
    LocalQueueClose(nullptr,dup);
    assert(!LocalQueueDispatch(nullptr,dup,0,0));
    assert(LocalQueueCreate(nullptr,3,1,&q)==0);
    Event d,e;
    assert(LocalQueueSubmit(nullptr,q,0,&d,Callback)==0 && d.calls==1);
    assert(LocalQueueDelay(nullptr,q,1,3,&e,Callback)==0 && !e.calls);
    Pump(); assert(e.calls==1 && !e.canceled);
    LocalQueueSetCurrent(nullptr,q);
    assert(LocalQueueCurrent(nullptr,&dup) && dup==q);
    LocalQueueClose(nullptr,dup); LocalQueueClose(nullptr,q);
    assert(LocalQueueCurrent(nullptr,&dup));
    LocalQueueSetCurrent(nullptr,nullptr);
    LocalQueueClose(nullptr,dup);
    assert(!LocalQueueCurrent(nullptr,&dup) && !dup);
    for(unsigned i=0;i<100;++i){assert(LocalQueueCreate(nullptr,0,0,&q)==0); LocalQueueClose(nullptr,q);}
}
