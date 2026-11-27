// <math.h>, as far as the reconstruction needs it.
//
// Build environment, not game source. The Store build calls the UCRT's math functions through its import table
// (api-ms-win-crt-math-l1-1-0.dll: atan2, floor, sqrtf, ...), and the hybrid link reaches the same imports.
// sqrtf and fabsf are intrinsics, as VS2019 treats them: sqrtss / andps (-fno-math-errno keeps sqrtss inline; the
// Store code calls sqrtf only on its errno path for a negative argument, where both give a NaN).
#pragma once

#define M_PI 3.14159265358979323846f  // a float: 180.0f / M_PI is 57.2957763671875, the Store build's constant

#ifdef __cplusplus
extern "C" {
#endif

double acos(double);
double asin(double);
double atan(double);
double atan2(double, double);
double cos(double);
double sin(double);
double tan(double);
double exp(double);
double log(double);
double pow(double, double);
double sqrt(double);
double floor(double);
double ceil(double);
double fabs(double);
double fmod(double, double);

float acosf(float);
float asinf(float);
float atanf(float);
float atan2f(float, float);
float cosf(float);
float expf(float);
float logf(float);
float powf(float, float);
float sinf(float);
float tanf(float);
float floorf(float);
float ceilf(float);
float fmodf(float, float);

#ifdef __cplusplus
}
#endif

#define sqrtf(x) __builtin_sqrtf(x)
#define fabsf(x) __builtin_fabsf(x)
