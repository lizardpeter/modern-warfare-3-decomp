// universal/com_math.h, as far as the reconstruction needs it (prototypes from the iw5mp_xenon PDB and MAP).
#pragma once

typedef float vec_t;
typedef vec_t vec2_t[2];
typedef vec_t vec3_t[3];

#define PITCH 0
#define YAW 1
#define ROLL 2

signed char ClampChar(const int i);
vec_t Vec2Normalize(vec2_t v);
float vectoyaw(const vec2_t vec);
float vectosignedyaw(const vec2_t vec);
float vectopitch(const vec3_t vec);
float vectosignedpitch(const vec3_t vec);
void vectoangles(const vec3_t vec, vec3_t angles);
void vectosignedangles(const vec3_t vec, vec3_t angles);
void AnglesSubtract(const vec3_t v1, const vec3_t v2, vec3_t v3);
float AngleNormalize360(float angle);
float AngleDelta(float angle1, float angle2);
void AddPointToBounds2D(const vec2_t v, vec2_t mins, vec2_t maxs);
void AxisClear(vec3_t axis[3]);
void AxisCopy(const vec3_t in[3], vec3_t out[3]);
