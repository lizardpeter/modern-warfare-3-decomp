#pragma once

// Store x64 layout recovered from TaskGroup_Init (14027E8C0).
// Unknown bytes retain their original storage; names here describe observed
// accesses, not inferred Xbox/PDB types.
struct TaskRefObject;
struct TaskRefVtable
{
    // Deleting destructor: observed first vtable slot, flag 1. Return value unused.
    void (*destroy)(TaskRefObject *, unsigned int);
};
struct TaskRefObject
{
    const TaskRefVtable *vtable;
    unsigned int references;
};
struct TaskRecord
{
    unsigned char opaque00[0x5c];
    unsigned int field5C;
    unsigned char opaque60[8];
    unsigned int category;
};
struct TaskSlot
{
    int state;
    unsigned char opaque04[4];
    TaskRefObject *volatile reference;
    unsigned char asyncBlock[0x40];
    TaskRecord record;
    unsigned char flagBC;
    unsigned char selectorBD;
    unsigned char opaqueBE[2];
    unsigned long long fieldC0;
    unsigned int fieldC8;
    unsigned char opaqueCC[4];
    unsigned long long fieldD0;
    unsigned long long fieldD8;
};

struct TaskGroup
{
    unsigned int index;
    unsigned char opaque04[4];
    TaskSlot slots[32];
};

struct TaskGroupRegistry
{
    unsigned int count;
    unsigned char opaque04[4];
    TaskGroup *groups[32];
    const char *names[32];
};

static_assert(sizeof(TaskSlot) == 0xe0);
static_assert(__builtin_offsetof(TaskSlot, asyncBlock) == 0x10);
static_assert(__builtin_offsetof(TaskRefObject, references) == 8);
static_assert(sizeof(TaskRecord) == 0x6c);
static_assert(__builtin_offsetof(TaskRecord, field5C) == 0x5c);
static_assert(__builtin_offsetof(TaskRecord, category) == 0x68);
static_assert(__builtin_offsetof(TaskSlot, reference) == 8);
static_assert(__builtin_offsetof(TaskSlot, record) == 0x50);
static_assert(__builtin_offsetof(TaskSlot, selectorBD) == 0xbd);
static_assert(__builtin_offsetof(TaskSlot, flagBC) == 0xbc);
static_assert(__builtin_offsetof(TaskSlot, fieldC0) == 0xc0);
static_assert(__builtin_offsetof(TaskSlot, fieldC8) == 0xc8);
static_assert(__builtin_offsetof(TaskSlot, fieldD0) == 0xd0);
static_assert(__builtin_offsetof(TaskSlot, fieldD8) == 0xd8);
static_assert(__builtin_offsetof(TaskGroup, slots) == 8);
static_assert(sizeof(TaskGroup) == 0x1c08);
static_assert(__builtin_offsetof(TaskGroupRegistry, groups) == 8);
static_assert(__builtin_offsetof(TaskGroupRegistry, names) == 0x108);

void TaskGroup_Init(TaskGroup *group, const char *name);

// Original symbol names for these Store-only routines have not been recovered.
// Address-based labels remain until corroborating evidence identifies them.
void sub_14011A600(TaskSlot *slot);
void sub_14020CC10(TaskRecord *record);
bool sub_14027E4A0(const TaskGroup *group);
void sub_14027E720(TaskGroup *group);
void sub_14027E860(TaskSlot *slot);
int sub_14027E9B0(const TaskGroup *group, unsigned int category);
TaskSlot *sub_14027EB30(TaskGroup *group, unsigned int selector, unsigned int category, int state);
unsigned long long sub_14027EBE0(const TaskSlot *slot);
void sub_14027EBF0(TaskSlot *slot, unsigned long long value);
bool sub_14027EC00(const TaskGroup *group, unsigned int category);
bool sub_14027EC30(const TaskGroup *group, unsigned int category, unsigned int selector);
