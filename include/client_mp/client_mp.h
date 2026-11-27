// client_mp/client_mp.h, as far as the reconstruction needs it.
#pragma once

#include "qcommon/dvar.h"

// clientActive_t: only the mouse fields, at their Store build offsets (from CL_GetMouseMovementWithSensitivity
// 1400C90F0; the Xbox reference has them at 0x3518/0x3520/0x3528 and 0x3548).
struct clientActive_t
{
	char unknown0[0x3420];
	int mouseDx[2];
	int mouseDy[2];
	int mouseIndex;
	char unknown3434[0x3450 - 0x3434];
	float cgameFOVSensitivityScale;
};
static_assert(__builtin_offsetof(clientActive_t, mouseDx) == 0x3420, "clientActive_t::mouseDx (Store layout)");
static_assert(__builtin_offsetof(clientActive_t, mouseIndex) == 0x3430, "clientActive_t::mouseIndex (Store layout)");
static_assert(__builtin_offsetof(clientActive_t, cgameFOVSensitivityScale) == 0x3450,
              "clientActive_t::cgameFOVSensitivityScale (Store layout)");

extern unsigned int frame_msec;  // client_mp/cl_main_mp.cpp

extern const dvar_t *m_filter;
extern const dvar_t *cl_mouseAccel;
extern const dvar_t *sensitivity;

void CL_GetMouseMovementWithSensitivity(clientActive_t *cl, float *mx, float *my);
