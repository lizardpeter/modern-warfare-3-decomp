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
