// client_mp/cl_input.cpp: the mouse movement that becomes view angles.
//
// Each function: the reference VA (iw5mp_xenon) and original line from its PDB, then the Store VA (iw5mp_ship,
// records/names.tsv). The Store build is the behaviour tested (docs/store-build.md, Mouse input).
#define IW5_ASSERT_FILE "c:\\trees\\iw5build4-iw\\iw5\\code_source\\src\\client_mp\\cl_input.cpp"
#include <iw5_assert.h>
#include <math.h>

#include "client_mp/client_mp.h"

// 8233A3C8  client_mp/cl_input.cpp:1128  (inlined in the Store build)
// This frame's mouse movement (with m_filter, averaged with the last frame's), and starts the next frame's.
static inline void CL_GetMouseMovement(clientActive_t *cl, float *mx, float *my)
{
	iassert_at(1129, mx);
	iassert_at(1130, my);

	if (m_filter->current.enabled)
	{
		*mx = (float)(cl->mouseDx[0] + cl->mouseDx[1]) * 0.5f;
		*my = (float)(cl->mouseDy[0] + cl->mouseDy[1]) * 0.5f;
	}
	else
	{
		*mx = (float)cl->mouseDx[cl->mouseIndex];
		*my = (float)cl->mouseDy[cl->mouseIndex];
	}
	cl->mouseIndex ^= 1;
	cl->mouseDx[cl->mouseIndex] = 0;
	cl->mouseDy[cl->mouseIndex] = 0;
}

// 8233A530  client_mp/cl_input.cpp:1149  Store 1400C90F0
// The movement scaled by sensitivity, cl_mouseAccel (by the movement's speed in counts per millisecond) and the
// field of view's scale.
void CL_GetMouseMovementWithSensitivity(clientActive_t *cl, float *mx, float *my)
{
	float rate;
	float accelSensitivity;

	CL_GetMouseMovement(cl, mx, my);
	rate = sqrtf(*mx * *mx + *my * *my) / (float)frame_msec;
	accelSensitivity = (rate * cl_mouseAccel->current.value + sensitivity->current.value) * cl->cgameFOVSensitivityScale;
	*mx = accelSensitivity * *mx;
	*my = accelSensitivity * *my;
}
