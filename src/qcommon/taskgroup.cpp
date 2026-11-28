// Store 14027E8C0: faithful candidate, from the SHA-256 pinned x64 PE.
// Store-only helpers use address labels until their original names are recovered.
// No enhanced/offline policy belongs in this file.
#include "qcommon/taskgroup.h"
#include "universal/q_shared.h"
#include <string.h>

extern TaskGroupRegistry s_taskGroupRegistry;
extern const char s_taskGroupLimitError[];
extern "C" void XAsyncCancel(void *async);

void TaskGroup_Init(TaskGroup *group, const char *name)
{
    int index = 0;
    while (index < 32 && s_taskGroupRegistry.groups[index])
        ++index;
    if (index == 32)
    {
        Com_Error(ERR_FATAL, s_taskGroupLimitError);
        return;
    }

    s_taskGroupRegistry.groups[index] = group;
    s_taskGroupRegistry.names[index] = name;
    group->index = index + 1;

    for (TaskSlot &slot : group->slots)
    {
        if (slot.state != 2)
            sub_14011A600(&slot);
        XAsyncCancel(slot.asyncBlock);
        slot.fieldC8 = 0;
        slot.fieldD0 = 0;
        slot.fieldD8 = 0;
        if (slot.state == 2)
            sub_14020CC10(&slot.record);
        slot.flagBC = 0;
        slot.fieldC0 = 0;
    }

    ++s_taskGroupRegistry.count;
}

// Store 14027E4A0: any occupied slot.
bool sub_14027E4A0(const TaskGroup *group)
{
    for (const TaskSlot &slot : group->slots)
        if (slot.flagBC)
            return true;
    return false;
}

// Store 14027E720: reset all slots, including unused ones. Do not unregister.
void sub_14027E720(TaskGroup *group)
{
    for (TaskSlot &slot : group->slots)
    {
        if (slot.state != 2)
            sub_14011A600(&slot);
        XAsyncCancel(slot.asyncBlock);
        slot.fieldC8 = 0;
        slot.fieldD0 = 0;
        slot.fieldD8 = 0;
        if (slot.state == 2)
            sub_14020CC10(&slot.record);
        slot.flagBC = 0;
        slot.fieldC0 = 0;
    }
}

// Store 14027E860: reset one slot. Preserve the original last two stores' order.
void sub_14027E860(TaskSlot *slot)
{
    if (slot->state != 2)
        sub_14011A600(slot);
    XAsyncCancel(slot->asyncBlock);
    slot->fieldC8 = 0;
    slot->fieldD0 = 0;
    slot->fieldD8 = 0;
    if (slot->state == 2)
        sub_14020CC10(&slot->record);
    slot->fieldC0 = 0;
    slot->flagBC = 0;
}

// Store 14027E9B0: count occupied slots with matching category.
int sub_14027E9B0(const TaskGroup *group, unsigned int category)
{
    int count = 0;
    for (const TaskSlot &slot : group->slots)
        if (slot.flagBC && slot.record.category == category)
            ++count;
    return count;
}

// Store 14027EB30: claim first free slot; the original clears exactly [8, 0xb8).
TaskSlot *sub_14027EB30(TaskGroup *group, unsigned int selector, unsigned int category, int state)
{
    for (TaskSlot &slot : group->slots)
    {
        if (slot.flagBC)
            continue;
        memset(reinterpret_cast<unsigned char *>(&slot) + 8, 0, 0xb0);
        slot.record.category = category;
        slot.selectorBD = static_cast<unsigned char>(selector);
        slot.state = state;
        slot.flagBC = 1;
        return &slot;
    }
    return nullptr;
}

// Store 14027EBE0, 14027EBF0: opaque qword accessors.
unsigned long long sub_14027EBE0(const TaskSlot *slot)
{
    return slot->fieldC0;
}

void sub_14027EBF0(TaskSlot *slot, unsigned long long value)
{
    slot->fieldC0 = value;
}

// Store 14027EC00, 14027EC30: occupied slot predicates.
bool sub_14027EC00(const TaskGroup *group, unsigned int category)
{
    for (const TaskSlot &slot : group->slots)
        if (slot.flagBC && slot.record.category == category)
            return true;
    return false;
}

bool sub_14027EC30(const TaskGroup *group, unsigned int category, unsigned int selector)
{
    for (const TaskSlot &slot : group->slots)
        if (slot.flagBC && slot.record.category == category && slot.selectorBD == selector)
            return true;
    return false;
}

// Store 14027E4D0: state-dependent cancellation of one slot.
bool sub_14027E4D0(TaskSlot *slot)
{
    if (slot->state == 1)
    {
        if (TaskRefObject *object = slot->reference)
            sub_1403F1420(object);
        sub_14027E860(slot);
    }
    else if (slot->state == 2)
        sub_14020C3D0(&slot->record);
    return true;
}

// Shared expansion of the cancellation body in 14027E540, 14027E630 and
// 14027E9E0. The original performs conditional cancellation before reset.
static void CancelOccupiedSlot(TaskSlot &slot)
{
    if (slot.state == 1)
    {
        TaskRefObject *object = slot.reference;
        if (object)
            sub_1403F1420(object);
        if (!object || slot.state != 2)
            sub_14011A600(&slot);
        XAsyncCancel(slot.asyncBlock);
        slot.fieldC8 = 0;
        slot.fieldD0 = 0;
        slot.fieldD8 = 0;
        if (slot.state == 2)
            sub_14020CC10(&slot.record);
        slot.flagBC = 0;
        slot.fieldC0 = 0;
    }
    else if (slot.state == 2)
        sub_14020C3D0(&slot.record);
}

// Store 14027E520: category-only cancellation wrapper.
bool sub_14027E520(TaskGroup *group, unsigned int category)
{
    return sub_14027E630(group, 0, category);
}

// Store 14027E540: cancel occupied slots matching category AND selector.
int sub_14027E540(TaskGroup *group, unsigned int category, unsigned int selector)
{
    for (TaskSlot &slot : group->slots)
        if (slot.flagBC && slot.record.category == category && slot.selectorBD == selector)
            CancelOccupiedSlot(slot);
    return 1;
}

// Store 14027E630: cancel all occupied slots or those in a single category.
bool sub_14027E630(TaskGroup *group, unsigned char all, unsigned int category)
{
    for (TaskSlot &slot : group->slots)
        if (slot.flagBC && (all || slot.record.category == category))
            CancelOccupiedSlot(slot);
    return true;
}

// Store 14027E7B0: the argument points at the first slot, not group header.
// Unlike the cancellation filter, this resets even unoccupied matching slots.
void sub_14027E7B0(TaskSlot *slots, unsigned int selector)
{
    for (unsigned int i = 0; i < 32; ++i)
    {
        TaskSlot &slot = slots[i];
        if (slot.selectorBD != selector)
            continue;
        if (slot.state != 2)
            sub_14011A600(&slot);
        XAsyncCancel(slot.asyncBlock);
        slot.fieldC8 = 0;
        slot.fieldD0 = 0;
        slot.fieldD8 = 0;
        if (slot.state == 2)
            sub_14020CC10(&slot.record);
        slot.flagBC = 0;
        slot.fieldC0 = 0;
    }
}

// Store 14027E9E0: cancel occupied work, reset all slots and unregister.
// Caller must supply an already registered group, as in the original.
void sub_14027E9E0(TaskGroup *group)
{
    for (TaskSlot &slot : group->slots)
        if (slot.flagBC)
            CancelOccupiedSlot(slot);
    sub_14027E720(group);
    unsigned int index = group->index - 1;
    group->index = 0;
    s_taskGroupRegistry.groups[index] = nullptr;
    s_taskGroupRegistry.names[index] = nullptr;
    --s_taskGroupRegistry.count;
}

// Store 14027EAE0: the shipped wrapper retries the same allocation once.
TaskSlot *sub_14027EAE0(TaskGroup *group, unsigned int selector, unsigned int category)
{
    TaskSlot *slot = sub_14027EB30(group, selector, category, 1);
    return slot ? slot : sub_14027EB30(group, selector, category, 1);
}

extern "C" long XAsyncGetStatus(void *async, unsigned char wait);
using TaskCallback = void (*)(TaskSlot *);

// Store 14027EC70: poll original async status and dispatch the original
// terminal callback, then release the slot. Pending work stays untouched.
// This is a faithful implementation, not a substitute platform provider.
void sub_14027EC70(TaskGroup *group)
{
    for (TaskSlot &slot : group->slots)
    {
        if (!slot.flagBC)
            continue;
        long status = XAsyncGetStatus(slot.asyncBlock, slot.asyncBlock[0x38]);
        if (status == static_cast<long>(0x8000000au))
            continue;
        __atomic_exchange_n(&slot.fieldC8, status < 0 ? 2u : 1u, __ATOMIC_SEQ_CST);
        unsigned int result = __atomic_load_n(&slot.fieldC8, __ATOMIC_RELAXED);
        unsigned long long callback;
        if (result == 1)
            callback = slot.fieldD0;
        else if (result == 2)
            callback = slot.fieldD8;
        else
            continue;
        if (callback)
            reinterpret_cast<TaskCallback>(callback)(&slot);
        if (slot.state != 2)
            sub_14011A600(&slot);
        XAsyncCancel(slot.asyncBlock);
        slot.fieldD8 = 0;
        slot.fieldD0 = 0;
        slot.fieldC8 = 0;
        if (slot.state == 2)
            sub_14020CC10(&slot.record);
        slot.flagBC = 0;
        slot.fieldC0 = 0;
    }
}
