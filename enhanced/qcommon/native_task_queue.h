#pragma once
// New enhanced infrastructure, not a reconstruction of XAsync or XTaskQueue.
// Single owner thread; callers supply monotonic time. No platform APIs or
// entitlement/user/account state. Explicit terminal notification on dispatch.
namespace iw5::native
{
using Tick = unsigned long long;
using TaskId = unsigned long long;
enum class Result { Completed, Failed, Cancelled };
using Work = Result (*)(void *);
using Completion = void (*)(void *, Result);

class TaskQueue
{
public:
    static constexpr unsigned Capacity = 32;
    TaskQueue() = default;
    TaskQueue(const TaskQueue &) = delete;
    TaskQueue &operator=(const TaskQueue &) = delete;

    // Zero means invalid submission or full/closed queue.
    TaskId Submit(Tick due, Work work, Completion complete, void *context)
    {
        if (closed_ || !work || !complete)
            return 0;
        for (auto &slot : slots_)
        {
            if (slot.state != State::Free)
                continue;
            // IDs never repeat during the lifetime of this queue.
            if (next_ == 0)
                return 0;
            slot = {next_++, due, work, complete, context, State::Waiting, Result::Failed};
            return slot.id;
        }
        return 0;
    }

    // Once work is running, it must finish; cancellation is not retroactive.
    bool Cancel(TaskId id)
    {
        for (auto &slot : slots_)
        {
            if (id && slot.id == id && slot.state == State::Waiting)
            {
                slot.state = State::Ready;
                slot.result = Result::Cancelled;
                return true;
            }
        }
        return false;
    }

    // Reject new work; deliver one cancellation completion for every queued task.
    // Work already executing is allowed to complete.
    void Close()
    {
        closed_ = true;
        for (auto &slot : slots_)
            if (slot.state == State::Waiting)
                Cancel(slot.id);
    }

    // Dispatch one callback. Ready completions precede work, both in stable ID
    // order for equal deadlines. Reentrant dispatch is explicitly refused.
    bool Dispatch(Tick now)
    {
        if (dispatching_)
            return false;
        Slot *selected = nullptr;
        for (auto &slot : slots_)
            if (slot.state == State::Ready && (!selected || slot.id < selected->id))
                selected = &slot;
        if (selected)
        {
            auto complete = selected->complete;
            auto context = selected->context;
            auto result = selected->result;
            *selected = {};
            dispatching_ = true;
            complete(context, result);
            dispatching_ = false;
            return true;
        }
        for (auto &slot : slots_)
        {
            if (slot.state != State::Waiting || slot.due > now)
                continue;
            if (!selected || slot.due < selected->due ||
                (slot.due == selected->due && slot.id < selected->id))
                selected = &slot;
        }
        if (!selected)
            return false;
        selected->state = State::Running;
        dispatching_ = true;
        selected->result = selected->work(selected->context);
        selected->state = State::Ready;
        dispatching_ = false;
        return true;
    }

    unsigned Outstanding() const
    {
        unsigned n = 0;
        for (const auto &slot : slots_)
            if (slot.state != State::Free)
                ++n;
        return n;
    }

private:
    enum class State { Free, Waiting, Running, Ready };
    struct Slot
    {
        TaskId id{};
        Tick due{};
        Work work{};
        Completion complete{};
        void *context{};
        State state{};
        Result result{};
    };
    Slot slots_[Capacity]{};
    TaskId next_ = 1;
    bool closed_ = false;
    bool dispatching_ = false;
};
} // namespace iw5::native
