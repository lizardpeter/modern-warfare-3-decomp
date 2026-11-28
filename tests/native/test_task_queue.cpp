#include "enhanced/qcommon/native_task_queue.h"
#include <cassert>
#include <vector>
using namespace iw5::native;

struct Event { int id; Result result; };
struct Fixture
{
    int id;
    std::vector<int> *work;
    std::vector<Event> *done;
    Result result;
    TaskQueue *queue = nullptr;
    TaskId handle = 0;
    bool close_on_work = false;
};

static Result Run(void *opaque)
{
    auto &f = *static_cast<Fixture *>(opaque);
    f.work->push_back(f.id);
    if (f.queue)
    {
        assert(!f.queue->Dispatch(1000)); // no reentrant execution
        assert(!f.queue->Cancel(f.handle)); // already running
        if (f.close_on_work)
            f.queue->Close();
    }
    return f.result;
}

static void Done(void *opaque, Result result)
{
    auto &f = *static_cast<Fixture *>(opaque);
    f.done->push_back({f.id, result});
    if (f.queue)
        assert(!f.queue->Dispatch(1000));
}

int main()
{
    std::vector<int> work;
    std::vector<Event> done;
    TaskQueue q;
    Fixture a{1, &work, &done, Result::Completed, &q};
    Fixture b{2, &work, &done, Result::Failed};
    Fixture c{3, &work, &done, Result::Completed};
    a.handle = q.Submit(10, Run, Done, &a);
    b.handle = q.Submit(5, Run, Done, &b);
    c.handle = q.Submit(10, Run, Done, &c);
    assert(a.handle && b.handle && c.handle && q.Outstanding() == 3);
    assert(!q.Dispatch(4));
    assert(q.Dispatch(5) && work == std::vector<int>{2} && done.empty());
    assert(!q.Cancel(b.handle));
    assert(q.Dispatch(5) && done.size() == 1 && done[0].result == Result::Failed);
    assert(!q.Dispatch(9));
    assert(q.Cancel(c.handle) && !q.Cancel(c.handle));
    assert(q.Dispatch(9) && done.size() == 2 && done[1].id == 3 &&
           done[1].result == Result::Cancelled);
    assert(q.Dispatch(10) && work == (std::vector<int>{2, 1}));
    assert(q.Dispatch(10) && done.size() == 3 && done[2].result == Result::Completed);
    assert(q.Outstanding() == 0 && !q.Dispatch(1000));
    assert(!q.Cancel(a.handle) && !q.Cancel(0));

    // Fixed capacity, stale-handle rejection, and orderly cancellation.
    TaskQueue full;
    TaskId ids[TaskQueue::Capacity];
    for (auto &id : ids)
    {
        id = full.Submit(0, Run, Done, &a);
        assert(id);
    }
    assert(!full.Submit(0, Run, Done, &a));
    assert(full.Cancel(ids[0]) && full.Dispatch(0));
    auto fresh = full.Submit(0, Run, Done, &a);
    assert(fresh && fresh != ids[0] && !full.Cancel(ids[0]));
    const auto before = done.size();
    full.Close();
    assert(!full.Submit(0, Run, Done, &a));
    // Avoid fixture's reentrancy assertions against a different queue.
    a.queue = nullptr;
    while (full.Dispatch(0)) {}
    assert(full.Outstanding() == 0 && done.size() == before + TaskQueue::Capacity);
    for (unsigned i = 0; i < TaskQueue::Capacity; ++i)
        assert(done[before + i].result == Result::Cancelled);

    // Closing from within work must not lose the running task's result.
    TaskQueue closing;
    Fixture d{4, &work, &done, Result::Completed, &closing, 0, true};
    Fixture e{5, &work, &done, Result::Completed};
    d.handle = closing.Submit(1, Run, Done, &d);
    e.handle = closing.Submit(1, Run, Done, &e);
    assert(closing.Dispatch(1));
    assert(closing.Dispatch(1) && done.back().id == 4 && done.back().result == Result::Completed);
    assert(closing.Dispatch(1) && done.back().id == 5 && done.back().result == Result::Cancelled);
    assert(!closing.Dispatch(1) && closing.Outstanding() == 0);
    assert(!closing.Submit(0, nullptr, Done, &d));
}
