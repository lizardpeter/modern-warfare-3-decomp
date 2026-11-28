// Store 14027E8C0: faithful candidate, from the SHA-256 pinned x64 PE.
// The two opaque callees are still original engine functions. Their names,
// prototypes beyond the observed pointer argument, and semantics are unresolved.
// No enhanced/offline policy belongs in this file.
#include "qcommon/taskgroup.h"
#include "universal/q_shared.h"

extern TaskGroupRegistry s_taskGroupRegistry;
extern const char s_taskGroupLimitError[];
extern void sub_14011A600(void *slot);
extern void sub_14020CC10(void *object);
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
            sub_14020CC10(slot.opaque50);
        slot.flagBC = 0;
        slot.fieldC0 = 0;
    }

    ++s_taskGroupRegistry.count;
}
