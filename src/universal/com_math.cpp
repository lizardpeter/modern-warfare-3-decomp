// universal/com_math.cpp: angles from vectors, angle arithmetic, small vector and axis helpers.
//
// Each function: the reference VA (iw5mp_xenon) and original line from its PDB, then the Store VA (iw5mp_ship,
// records/names.tsv). Float arithmetic is written as the Store build performs it, which differs from the Xbox
// build's: the Store keeps AngleDelta and friends in float where the Xbox code widens to double, and adds 0 or 360
// without a branch (so -0.0 comes back +0.0), as /fp:fast code does. The tests compare results bit for bit.
#define IW5_ASSERT_FILE "c:\\trees\\iw5build4-iw\\iw5\\code_source\\src\\universal\\com_math.cpp"
#include <iw5_assert.h>
#include <math.h>

#include "universal/com_math.h"

// atan2 results are scaled in double by this float (57.2957763671875) and rounded to float once
#define RAD2DEG (180.0f / M_PI)

// 825D8AB8  universal/com_math.cpp:813  Store 14030B160
vec_t Vec2Normalize(vec2_t v)
{
	float length;
	float ilength;

	length = sqrtf(v[0] * v[0] + v[1] * v[1]);
	ilength = 1.0f / (-length >= 0.0f ? 1.0f : length);  // I_fsel(-length, 1.0f, length): a NaN length divides
	v[0] = v[0] * ilength;
	v[1] = v[1] * ilength;
	return length;
}

// 825D8F08  universal/com_math.cpp:946  Store 14030BBA0
float vectoyaw(const vec2_t vec)
{
	float yaw;

	if (vec[YAW] == 0.0f && vec[PITCH] == 0.0f)
		return 0.0f;

	yaw = (float)(atan2(vec[YAW], vec[PITCH]) * RAD2DEG);
	return yaw + (yaw < 0.0f ? 360.0f : 0.0f);
}

// 825D8F90  universal/com_math.cpp:969  Store 14030BB60
float vectosignedyaw(const vec2_t vec)
{
	float yaw;

	if (vec[YAW] == 0.0f && vec[PITCH] == 0.0f)
		return 0.0f;

	yaw = (float)(atan2(vec[YAW], vec[PITCH]) * RAD2DEG);
	iassert_at(980, yaw >= -180);
	iassert_at(981, yaw <= 180);
	return yaw;
}

// 825D9070  universal/com_math.cpp:994  Store 14030B8D0
float vectopitch(const vec3_t vec)
{
	float forward;
	float pitch;

	if (vec[YAW] == 0.0f && vec[PITCH] == 0.0f)
		return -vec[2] < 0.0f ? 270.0f : 90.0f;

	forward = sqrtf(vec[0] * vec[0] + vec[1] * vec[1]);
	pitch = (float)(atan2(vec[2], forward) * -RAD2DEG);
	return pitch + (pitch < 0.0f ? 360.0f : 0.0f);
}

// 825D9120  universal/com_math.cpp:1019  Store 14030BAB0
float vectosignedpitch(const vec3_t vec)
{
	float forward;

	if (vec[YAW] == 0.0f && vec[PITCH] == 0.0f)
		return -vec[2] < 0.0f ? -90.0f : 90.0f;

	forward = sqrtf(vec[0] * vec[0] + vec[1] * vec[1]);
	return (float)(atan2(vec[2], forward) * -RAD2DEG);
}

// 825D91B8  universal/com_math.cpp:1043  Store 14030B780
void vectoangles(const vec3_t vec, vec3_t angles)
{
	float forward;
	float yaw;
	float pitch;

	if (vec[YAW] == 0.0f && vec[PITCH] == 0.0f)
	{
		yaw = 0.0f;
		pitch = -vec[2] < 0.0f ? 270.0f : 90.0f;
	}
	else
	{
		yaw = (float)(atan2(vec[YAW], vec[PITCH]) * RAD2DEG);
		yaw = yaw + (yaw < 0.0f ? 360.0f : 0.0f);
		forward = sqrtf(vec[0] * vec[0] + vec[1] * vec[1]);
		pitch = (float)(atan2(vec[2], forward) * -RAD2DEG);
		pitch = pitch + (pitch < 0.0f ? 360.0f : 0.0f);
	}
	angles[YAW] = yaw;
	angles[PITCH] = pitch;
	angles[ROLL] = 0.0f;
}

// 825D92B0  universal/com_math.cpp:1083  Store 14030B9A0
void vectosignedangles(const vec3_t vec, vec3_t angles)
{
	float forward;
	float yaw;
	float pitch;

	if (vec[YAW] == 0.0f && vec[PITCH] == 0.0f)
	{
		yaw = 0.0f;
		pitch = -vec[2] < 0.0f ? -90.0f : 90.0f;
	}
	else
	{
		yaw = (float)(atan2(vec[YAW], vec[PITCH]) * RAD2DEG);
		forward = sqrtf(vec[0] * vec[0] + vec[1] * vec[1]);
		pitch = (float)(atan2(vec[2], forward) * -RAD2DEG);
	}
	angles[YAW] = yaw;
	angles[PITCH] = pitch;
	angles[ROLL] = 0.0f;
}

// 825DC790  universal/com_math.cpp:2540  Store 140306CB0
void AnglesSubtract(const vec3_t v1, const vec3_t v2, vec3_t v3)
{
	float a;

	a = (v1[0] - v2[0]) * (1.0f / 360.0f);
	v3[0] = (a - (float)floor(a + 0.5f)) * 360.0f;
	a = (v1[1] - v2[1]) * (1.0f / 360.0f);
	v3[1] = (a - (float)floor(a + 0.5f)) * 360.0f;
	a = (v1[2] - v2[2]) * (1.0f / 360.0f);
	v3[2] = (a - (float)floor(a + 0.5f)) * 360.0f;
}

// 825DC850  universal/com_math.cpp:2555  Store 140306C50
float AngleNormalize360(float angle)
{
	float scaledAngle;
	float result;
	float wrapped;

	scaledAngle = angle * (1.0f / 360.0f);
	result = (scaledAngle - (float)floor(scaledAngle)) * 360.0f;
	wrapped = result - 360.0f;  // the float rounding of a tiny negative angle can give exactly 360
	return wrapped < 0.0f ? result : wrapped;
}

// 825DC8A8  universal/com_math.cpp:2575  Store 140306C00
float AngleDelta(float angle1, float angle2)
{
	float a;

	a = (angle1 - angle2) * (1.0f / 360.0f);
	return (a - (float)floor(a + 0.5f)) * 360.0f;
}

// 825DC928  universal/com_math.cpp:2591  Store 140306B30
void AddPointToBounds2D(const vec2_t v, vec2_t mins, vec2_t maxs)
{
	if (v[0] < mins[0])
		mins[0] = v[0];
	if (v[0] > maxs[0])
		maxs[0] = v[0];
	if (v[1] < mins[1])
		mins[1] = v[1];
	if (v[1] > maxs[1])
		maxs[1] = v[1];
}

// 825DCB68  universal/com_math.cpp:2658  Store 140306DA0
void AxisClear(vec3_t axis[3])
{
	axis[0][0] = 1.0f;
	axis[0][1] = 0.0f;
	axis[0][2] = 0.0f;
	axis[1][0] = 0.0f;
	axis[1][1] = 1.0f;
	axis[1][2] = 0.0f;
	axis[2][0] = 0.0f;
	axis[2][1] = 0.0f;
	axis[2][2] = 1.0f;
}

// 825DCBA0  universal/com_math.cpp:2677  Store 140306DD0
void AxisCopy(const vec3_t in[3], vec3_t out[3])
{
	out[0][0] = in[0][0];
	out[0][1] = in[0][1];
	out[0][2] = in[0][2];
	out[1][0] = in[1][0];
	out[1][1] = in[1][1];
	out[1][2] = in[1][2];
	out[2][0] = in[2][0];
	out[2][1] = in[2][1];
	out[2][2] = in[2][2];
}
