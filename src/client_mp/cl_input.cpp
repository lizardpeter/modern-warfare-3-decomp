// client_mp/cl_input.cpp: the mouse movement that becomes view angles.
//
// Each function: the reference VA (iw5mp_xenon) and original line from its PDB, then the Store VA (iw5mp_ship,
// records/names.tsv). The Store build is the behaviour tested (docs/store-build.md, Mouse input).
#define IW5_ASSERT_FILE "c:\\trees\\iw5build4-iw\\iw5\\code_source\\src\\client_mp\\cl_input.cpp"
#include <iw5_assert.h>
#include <math.h>
#include <string.h>

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

// 8233A620  client_mp/cl_input.cpp:1170  Store 1400C9640
// The mouse's movement into the view angles (or, while strafing or without mouse look, into the move axes), each
// turn capped by the cgame's speed limit for this frame. The cap is minss/maxss in the Store build, so a NaN turn
// passes through it (Ghidra shows the two as <= comparisons, which would clamp it). The Store revision then runs aim assist on the new angles,
// with no stick input, and takes the airburst mark distance (the Xbox reference clears the melee charge instead).
void CL_MouseMove(LocalClientNum_t localClientNum, usercmd_s *cmd, float frametime_base)
{
	clientActive_t *cl;
	float mx;
	float my;
	float cap;
	float capped;
	AimInput aimInput;
	AimOutput aimOutput;
	unsigned int markDistance;

	cl = CL_GetLocalClientGlobals(localClientNum);
	if (!frame_msec)
		return;
	CL_GetMouseMovementWithSensitivity(cl, &mx, &my);
	if ((cl->ps.pm_flags & PMF_ON_LADDER_OR_FROZEN) && !CL_Demo_IsPlaying(localClientNum))
		return;

	if (mx != 0.0f || my != 0.0f)
	{
		if (playersKb[localClientNum][KB_STRAFE].active)
		{
			cmd->rightmove = ClampChar(cmd->rightmove + (int)floor(mx * m_side->current.value + 0.5f));
		}
		else
		{
			mx = mx * m_yaw->current.value;
			if (cl->cgameMaxYawSpeed > 0.0f)
			{
				cap = (float)frame_msec * cl->cgameMaxYawSpeed * 0.001f;
				capped = cap < mx ? cap : mx;         // minss: a NaN mx passes through
				mx = -cap > capped ? -cap : capped;  // maxss
			}
			cl->clViewangles[1] = cl->clViewangles[1] - mx;
		}

		if ((!playersKb[localClientNum][KB_MLOOK].active && !cl_freelook->current.enabled)
		    || playersKb[localClientNum][KB_STRAFE].active)
		{
			cmd->forwardmove = ClampChar(cmd->forwardmove - (int)floor(my * m_forward->current.value + 0.5f));
		}
		else
		{
			my = my * m_pitch->current.value;
			if (cl->cgameMaxPitchSpeed > 0.0f)
			{
				cap = (float)frame_msec * cl->cgameMaxPitchSpeed * 0.001f;
				capped = cap < my ? cap : my;         // minss: a NaN my passes through
				my = -cap > capped ? -cap : capped;  // maxss
			}
			cl->clViewangles[0] = cl->clViewangles[0] + my;
		}
	}

	aimInput.deltaTime = frametime_base;
	aimInput.deltaTimeScaled = (float)cls.frametime * 0.001f;
	aimInput.pitch = cl->clViewangles[0];
	aimInput.pitchAxis = 0.0f;
	aimInput.pitchMax = 0.0f;
	aimInput.yaw = cl->clViewangles[1];
	aimInput.yawAxis = 0.0f;
	aimInput.yawMax = 0.0f;
	aimInput.forwardAxis = 0.0f;
	aimInput.rightAxis = 0.0f;
	aimInput.buttons = cmd->buttons;
	aimInput.localClientNum = LOCAL_CLIENT_0;
	AimAssist_UpdateGamePadInput(&aimInput, &aimOutput);
	cl->clViewangles[0] = aimOutput.pitch;
	cl->clViewangles[1] = aimOutput.yaw;
	// SetMeleeChargeCmd (inlined)
	cmd->meleeChargeEnt = aimOutput.meleeChargeEnt;
	cmd->meleeChargeDist = aimOutput.meleeChargeDist;

	// CL_UpdateAirburstMarkDistance (inlined)
	if (CG_GetAirburstMarkDistance(localClientNum, &cl->ps, cmd->buttons, &markDistance))
	{
		cmd->buttons |= 0x8000000;
		cmd->airburstMarkDistance = (unsigned short)markDistance;
	}
}
