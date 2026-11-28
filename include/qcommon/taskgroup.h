#pragma once

// Store x64 layout recovered from TaskGroup_Init (14027E8C0).
// Unknown bytes retain their original storage; names here describe observed
// accesses, not inferred Xbox/PDB types.
struct TaskSlot
{
    int state;
    unsigned char opaque04[0x0c];
    unsigned char asyncBlock[0x40];
    unsigned char opaque50[0x6c];
    unsigned char flagBC;
    unsigned char opaqueBD[3];
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
static_assert(__builtin_offsetof(TaskSlot, opaque50) == 0x50);
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
