// qcommon/dvar.h, as far as the reconstruction needs it.
#pragma once

union DvarValue
{
	bool enabled;
	int integer;
	unsigned int unsignedInt;
	float value;
	float vector[4];
	const char *string;
	unsigned char color[4];
};

// The Store build's layout, from its accesses: the current value at +0x10 (the Xbox reference has a description
// pointer before the flags, and its current value at +0x0C). Only the fields the reconstruction reads are named.
struct dvar_t
{
	const char *name;
	unsigned int flags;
	unsigned char type;
	bool modified;
	DvarValue current;
};
static_assert(__builtin_offsetof(dvar_t, current) == 0x10, "dvar_t::current (Store layout)");

#define DVAR_ARCHIVE 0x1  // saved in the config (seta)

const dvar_t *Dvar_RegisterFloat(const char *dvarName, float value, float min, float max, unsigned short flags,
                                 const char *description);
