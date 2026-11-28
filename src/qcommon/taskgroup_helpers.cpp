// Store 14011A600 and 14020CC10. The original source files and symbol names
// are unresolved; these are the observed ABI and instruction-level semantics.
#include "qcommon/taskgroup.h"

// 14011A600: release the optional reference using the original locked decrement.
// Reload the slot's pointer after the decrement, as the original does.
void sub_14011A600(TaskSlot *slot)
{
    TaskRefObject *object = slot->reference;
    if (!object)
        return;
    if (__atomic_fetch_sub(&object->references, 1u, __ATOMIC_SEQ_CST) == 1u)
    {
        object = slot->reference;
        if (object)
            object->vtable->destroy(object, 1);
    }
    slot->reference = nullptr;
}

// 14020CC10: clear the original record's 32-bit field at +0x5c only.
void sub_14020CC10(TaskRecord *record)
{
    record->field5C = 0;
}

// Store 1403F1420: original remote-object cancellation state store.
void sub_1403F1420(TaskRefObject *object)
{
    object->field1C = 5;
}

// Store 14020CC00: transition the embedded record's two status fields.
void sub_14020CC00(TaskRecord *record)
{
    record->field5C = 2;
    record->field60 = 4;
}

// Store 14020CC20: valid link states are exactly 1 through 6 (unsigned compare).
bool sub_14020CC20(const TaskRecordLink *link)
{
    return link->state - 1u <= 5u;
}

extern TaskRecordLink s_taskRecordLinks[24];

// Store 14020C3D0: detach all active, valid links referencing this record.
// Do not assume uniqueness or stop after the first match.
void sub_14020C3D0(TaskRecord *record)
{
    for (TaskRecordLink &link : s_taskRecordLinks)
    {
        if (!link.active || !sub_14020CC20(&link) || link.record != record)
            continue;
        if (sub_14020CC20(&link))
            sub_14020CC00(link.record);
        link.active = 0;
    }
}
