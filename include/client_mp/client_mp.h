// client_mp/client_mp.h, as far as the reconstruction needs it.
#pragma once

#include "qcommon/dvar.h"

enum LocalClientNum_t
{
	LOCAL_CLIENT_INVALID = -1,
	LOCAL_CLIENT_0 = 0,
};

// usercmd_s: no pointers, the same on x64 as in the reference.
struct usercmd_s
{
	int serverTime;
	int buttons;
	int angles[3];
	int weapon;
	int offHand;
	char forwardmove;
	char rightmove;
	unsigned short airburstMarkDistance;
	unsigned short meleeChargeEnt;
	unsigned char meleeChargeDist;
	char selectedLoc[2];
	unsigned char selectedLocAngle;
	char remoteControlAngles[2];
	char remoteControlMove[3];
};
static_assert(sizeof(usercmd_s) == 0x2C, "usercmd_s");

struct kbutton_t
{
	int down[2];
	unsigned int downtime;
	unsigned int msec;
	bool active;
	bool wasPressed;
};
static_assert(sizeof(kbutton_t) == 0x14, "kbutton_t");

// The Store build's playersKb: 30 buttons a client (600 bytes); the indices CL_MouseMove reads.
enum
{
	KB_STRAFE = 8,
	KB_MLOOK = 13,
	NUM_BUTTONS = 30,
};
extern kbutton_t playersKb[1][NUM_BUTTONS];

// playerState_s: only the start.
struct playerState_s
{
	int commandTime;
	int pm_type;
	int pm_time;
	int pm_flags;
};
#define PMF_ON_LADDER_OR_FROZEN 0x800  // CL_MouseMove ignores the mouse while it's set (outside demo playback)

// clientActive_t at its Store build offsets: the reference's minus 0xF8 from the snapshot on (the Xbox aligns the
// snapshot to 128 bytes; the Store has it at +8). Only the fields the reconstruction uses are named.
struct clientActive_t
{
	bool usingAds;
	int timeoutcount;
	playerState_s ps;  // snap.ps
	char unknown18[0x3420 - 0x18];
	int mouseDx[2];
	int mouseDy[2];
	int mouseIndex;
	char unknown3434[0x3450 - 0x3434];
	float cgameFOVSensitivityScale;
	float cgameMaxPitchSpeed;
	float cgameMaxYawSpeed;
	char unknown345C[0x34F4 - 0x345C];
	float clViewangles[3];
};
static_assert(__builtin_offsetof(clientActive_t, ps) == 0x8, "clientActive_t::snap.ps (Store layout)");
static_assert(__builtin_offsetof(clientActive_t, mouseDx) == 0x3420, "clientActive_t::mouseDx (Store layout)");
static_assert(__builtin_offsetof(clientActive_t, mouseIndex) == 0x3430, "clientActive_t::mouseIndex (Store layout)");
static_assert(__builtin_offsetof(clientActive_t, cgameFOVSensitivityScale) == 0x3450,
              "clientActive_t::cgameFOVSensitivityScale (Store layout)");
static_assert(__builtin_offsetof(clientActive_t, cgameMaxYawSpeed) == 0x3458, "clientActive_t::cgameMaxYawSpeed");
static_assert(__builtin_offsetof(clientActive_t, clViewangles) == 0x34F4, "clientActive_t::clViewangles");

extern clientActive_t clients[1];  // the Store build's multiplayer client: one

inline clientActive_t *CL_GetLocalClientGlobals(LocalClientNum_t localClientNum)
{
	(void)localClientNum;
	return &clients[0];
}

// clientStatic_t at its Store offsets: the reference's without devGuiStarted (Com_Error tests uiStarted at +0x114).
struct clientStatic_t
{
	char unknown0[0x114];
	int uiStarted;
	int frametime;
};
static_assert(__builtin_offsetof(clientStatic_t, frametime) == 0x118, "clientStatic_t::frametime (Store layout)");
extern clientStatic_t cls;

extern unsigned int frame_msec;  // client_mp/cl_main_mp.cpp

extern const dvar_t *m_filter;
extern const dvar_t *cl_mouseAccel;
extern const dvar_t *sensitivity;
extern const dvar_t *m_pitch;
extern const dvar_t *m_yaw;
extern const dvar_t *m_forward;
extern const dvar_t *m_side;
extern const dvar_t *cl_freelook;

// aim_assist/aim_assist.h
struct AimInput
{
	float deltaTime;
	float deltaTimeScaled;
	float pitch;
	float pitchAxis;
	float pitchMax;
	float yaw;
	float yawAxis;
	float yawMax;
	float forwardAxis;
	float rightAxis;
	int buttons;
	LocalClientNum_t localClientNum;
};
struct AimOutput
{
	float pitch;
	float yaw;
	float meleeChargeYaw;
	unsigned short meleeChargeEnt;
	unsigned char meleeChargeDist;
};
void AimAssist_UpdateGamePadInput(const AimInput *input, AimOutput *output);

bool CL_Demo_IsPlaying(LocalClientNum_t localClientNum);
int CG_GetAirburstMarkDistance(LocalClientNum_t localClientNum, playerState_s *ps, int buttons,
                               unsigned int *markDistance);
signed char ClampChar(const int i);

void CL_GetMouseMovementWithSensitivity(clientActive_t *cl, float *mx, float *my);
void CL_MouseMove(LocalClientNum_t localClientNum, usercmd_s *cmd, float frametime_base);
