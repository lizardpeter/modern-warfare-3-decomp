#include "enhanced/qcommon/native_task_queue.h"
#include <cassert>
#include <vector>
#include <stdexcept>
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

struct ThrowingFixture
{
    TaskQueue *queue;
    TaskId handle = 0;
    TaskId replacement = 0;
    unsigned work = 0;
    unsigned done = 0;
    bool throw_work = false;
    bool throw_done = false;
    bool replace_on_throw = false;
    Result observed = Result::Completed;
};

static Result ThrowingWork(void *opaque)
{
    auto &f = *static_cast<ThrowingFixture *>(opaque);
    ++f.work;
    assert(!f.queue->Dispatch(0));
    assert(!f.queue->Cancel(f.handle));
    if (f.throw_work)
        throw std::runtime_error("work");
    return Result::Completed;
}

static void ThrowingDone(void *opaque, Result result)
{
    auto &f = *static_cast<ThrowingFixture *>(opaque);
    ++f.done;
    f.observed = result;
    assert(!f.queue->Dispatch(0));
    assert(!f.queue->Cancel(f.handle));
    if (f.throw_done)
    {
        if (f.replace_on_throw)
        {
            f.replacement = f.queue->Submit(0, ThrowingWork, ThrowingDone, &f);
            assert(f.replacement && f.replacement != f.handle);
        }
        throw std::runtime_error("completion");
    }
}

static void TestCallbackExceptions()
{
    TaskQueue queue;
    ThrowingFixture f{&queue};
    f.throw_work = true;
    f.handle = queue.Submit(0, ThrowingWork, ThrowingDone, &f);
    assert(f.handle);
    bool caught = false;
    try { queue.Dispatch(0); }
    catch (const std::runtime_error &) { caught = true; }
    assert(caught && f.work == 1 && f.done == 0 && queue.Outstanding() == 1);
    assert(!queue.Cancel(f.handle));
    assert(queue.Dispatch(0) && f.done == 1 && f.observed == Result::Failed);
    assert(queue.Outstanding() == 0 && !queue.Dispatch(0));

    // A completion may submit replacement work before throwing. Its old
    // slot is consumed, and the new work remains independently dispatchable.
    f.throw_work = false;
    f.throw_done = true;
    f.replace_on_throw = true;
    f.handle = queue.Submit(0, ThrowingWork, ThrowingDone, &f);
    assert(f.handle && queue.Dispatch(0));
    caught = false;
    try { queue.Dispatch(0); }
    catch (const std::runtime_error &) { caught = true; }
    assert(caught && f.work == 2 && f.done == 2 && queue.Outstanding() == 1);
    assert(!queue.Cancel(f.handle));
    f.handle = f.replacement;
    f.throw_done = false;
    assert(queue.Dispatch(0) && queue.Dispatch(0));
    assert(f.work == 3 && f.done == 3 && f.observed == Result::Completed);
    assert(queue.Outstanding() == 0 && !queue.Dispatch(0));

    // A cancellation completion throwing after Close must not block draining.
    f.handle = queue.Submit(99, ThrowingWork, ThrowingDone, &f);
    assert(f.handle);
    ThrowingFixture other{&queue};
    other.handle = queue.Submit(99, ThrowingWork, ThrowingDone, &other);
    assert(other.handle);
    queue.Close();
    f.throw_done = true;
    f.replace_on_throw = false;
    caught = false;
    try { queue.Dispatch(0); }
    catch (const std::runtime_error &) { caught = true; }
    assert(caught && f.observed == Result::Cancelled && queue.Outstanding() == 1);
    assert(queue.Dispatch(0) && other.observed == Result::Cancelled);
    assert(queue.Outstanding() == 0);
}

int main()
{
    TestCallbackExceptions();
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
