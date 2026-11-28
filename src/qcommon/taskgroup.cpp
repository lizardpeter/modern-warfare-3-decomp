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
