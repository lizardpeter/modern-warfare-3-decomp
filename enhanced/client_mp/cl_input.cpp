// enhanced/client_mp/cl_input.cpp: mouse look without acceleration or smoothing (P0001; B0002, B0003), in MW2019's
// units (P0002). From black-ops-2-decomp's P0002/P0003.
//
// CL_MouseMove as in the Store build (src/client_mp/cl_input.cpp), with these changes:
// - MW2019 sensitivity (P0002): one count turns the view by m_sens * 0.0066 degrees, at any field of view, hip fire
//   and aiming down sights alike. The menu's sensitivity (the sensitivity dvar), the cgame's FOV scale and m_yaw no
//   longer apply; m_pitch's sign still inverts the pitch (the menu's option), and 0 still stops it. The moves the
//   mouse makes with +strafe or without free look use the counts scaled as the original's at sensitivity
//   m_sens * 0.3 and the FOV scale 1.
// - The view angles are kept in double precision between calls, so the same movement over more frames (higher
//   polling and frame rates) ends at the same angle. Anything else that changes them (spawning, aim assist) is
//   picked up the next time.
// - No acceleration (P0001): cl_mouseAccel has no effect. No smoothing (P0001): this frame's movement only; m_filter
//   has no effect.
// - The yaw is brought back within [-180, 180) at the end, so its precision doesn't wear down after many turns the
//   same way (B0003).
// The rest (the speed caps, the ladder and frozen check, aim assist, the airburst sight) is as the original.
#include <math.h>

#include "client_mp/client_mp.h"
#include "universal/com_math.h"

// MW2019's mouse scale: degrees per count per unit of m_sens
#define MOUSE_DEGREES_PER_COUNT 0.0066
// The original's turn per scaled count at its default m_yaw: the moves take the counts scaled to it
#define MOUSE_ORIGINAL_YAW 0.022

const dvar_t *m_sens;  // MW2019 units: registered on first use (CL_MouseSensitivity)

// This frame's movement and the start of the next frame's; no averaging with the last frame (m_filter).
static void CL_TakeMouseMovement(clientActive_t *cl, float *mx, float *my)
{
	*mx = (float)cl->mouseDx[cl->mouseIndex];
	*my = (float)cl->mouseDy[cl->mouseIndex];
	cl->mouseIndex ^= 1;
	cl->mouseDx[cl->mouseIndex] = 0;
	cl->mouseDy[cl->mouseIndex] = 0;
}

// Store 1400C90F0, for its other caller (the remote-control view, 1400C9940): this frame's movement scaled by the
// sensitivity and the FOV scale, without cl_mouseAccel or m_filter.
void CL_GetMouseMovementWithSensitivity(clientActive_t *cl, float *mx, float *my)
{
	float scale;

	CL_TakeMouseMovement(cl, mx, my);
	scale = sensitivity->current.value * cl->cgameFOVSensitivityScale;
	*mx = scale * *mx;
	*my = scale * *my;
}

// m_sens, registered the first time the mouse moves the view (the dvar system is up by then). A value the config
// set before that (seta m_sens ...) is kept: registering a dvar that already exists takes its value.
static float CL_MouseSensitivity()
{
	if (!m_sens)
		m_sens = Dvar_RegisterFloat("m_sens", 1.5f, 0.01f, 100.0f, DVAR_ARCHIVE,
		                            "Mouse sensitivity in MW2019 units: 0.0066 degrees per count each, at any "
		                            "field of view");
	return m_sens->current.value;
}

// The view angles (pitch, yaw) as this file last wrote them, in double precision. A float that differs from the
// one written was changed elsewhere: it is taken as it is.
static double s_viewAngles[2];
static float s_viewAnglesWritten[2];

static double CL_ViewAngle(clientActive_t *cl, int axis)
{
	if (!(cl->clViewangles[axis] == s_viewAnglesWritten[axis]))
		s_viewAngles[axis] = cl->clViewangles[axis];
	return s_viewAngles[axis];
}

static void CL_SetViewAngle(clientActive_t *cl, int axis, double angle)
{
	s_viewAngles[axis] = angle;
	s_viewAnglesWritten[axis] = (float)angle;
	cl->clViewangles[axis] = s_viewAnglesWritten[axis];
}

// The view's yaw back into [-180, 180) by whole turns (B0003): exact, and the commands' short angles don't change,
// since they wrap at 360.
static void CL_WrapViewYaw(clientActive_t *cl)
{
	double yaw = CL_ViewAngle(cl, 1);
	if (yaw >= 180.0 || yaw < -180.0)
	{
		yaw = fmod(yaw, 360.0);  // exact: the remainder of two doubles is representable
		if (yaw >= 180.0)
			yaw -= 360.0;
		else if (yaw < -180.0)
			yaw += 360.0;
		CL_SetViewAngle(cl, 1, yaw);
	}
}

// A speed cap the cgame sets (vehicles, turrets): the turn limited to cap degrees a second over this frame
static double CL_CapTurn(double turn, float cap)
{
	double limit;

	if (!(cap > 0.0f))
		return turn;
	limit = (double)frame_msec * cap * 0.001;
	return turn > limit ? limit : turn < -limit ? -limit : turn;
}

void CL_MouseMove(LocalClientNum_t localClientNum, usercmd_s *cmd, float frametime_base)
{
	clientActive_t *cl;
	float countsX;
	float countsY;
	float sens;
	double degreesPerCount;
	float scale;
	float mx;
	float my;
	float pitchScale;
	AimInput aimInput;
	AimOutput aimOutput;
	unsigned int markDistance;

	cl = CL_GetLocalClientGlobals(localClientNum);
	if (!frame_msec)
		return;  // the movement waits for the next frame, as in the original
	CL_TakeMouseMovement(cl, &countsX, &countsY);
	sens = CL_MouseSensitivity();
	if ((cl->ps.pm_flags & PMF_ON_LADDER_OR_FROZEN) && !CL_Demo_IsPlaying(localClientNum))
		return;

	degreesPerCount = (double)sens * MOUSE_DEGREES_PER_COUNT;
	scale = sens * (float)(MOUSE_DEGREES_PER_COUNT / MOUSE_ORIGINAL_YAW);  // the original's sensitivity * FOV scale
	mx = countsX * scale;
	my = countsY * scale;
	if (mx != 0.0f || my != 0.0f)
	{
		if (playersKb[localClientNum][KB_STRAFE].active)
			cmd->rightmove = ClampChar(cmd->rightmove + (int)floor(mx * m_side->current.value + 0.5f));
		else
			CL_SetViewAngle(cl, 1, CL_ViewAngle(cl, 1) - CL_CapTurn(countsX * degreesPerCount, cl->cgameMaxYawSpeed));

		if ((!playersKb[localClientNum][KB_MLOOK].active && !cl_freelook->current.enabled)
		    || playersKb[localClientNum][KB_STRAFE].active)
		{
			cmd->forwardmove = ClampChar(cmd->forwardmove - (int)floor(my * m_forward->current.value + 0.5f));
		}
		else
		{
			pitchScale = m_pitch->current.value;  // its sign: inverted (the menu's option), or 0: no pitch
			CL_SetViewAngle(cl, 0, CL_ViewAngle(cl, 0)
			                + CL_CapTurn(countsY * degreesPerCount
			                             * (pitchScale > 0.0f ? 1.0 : pitchScale < 0.0f ? -1.0 : 0.0),
			                             cl->cgameMaxPitchSpeed));
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
	cmd->meleeChargeEnt = aimOutput.meleeChargeEnt;
	cmd->meleeChargeDist = aimOutput.meleeChargeDist;
	CL_WrapViewYaw(cl);  // after aim assist, which may turn the view too

	if (CG_GetAirburstMarkDistance(localClientNum, &cl->ps, cmd->buttons, &markDistance))
	{
		cmd->buttons |= 0x8000000;
		cmd->airburstMarkDistance = (unsigned short)markDistance;
	}
}
