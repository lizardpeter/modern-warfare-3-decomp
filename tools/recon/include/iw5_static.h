// Static data in the hybrid build.
//
// Build environment, not game source. A `static` variable in the original (file scope or inside a
// function) lives at a fixed address in the original binary, but it has no public symbol, so it can't
// be linked by name like globals can. While the reconstruction is tested against the original, it
// must read and write that same storage. So reconstructed code declares original statics like this:
//
//     #define IW5_SOURCE "universal/com_math.cpp"   // once per file: the PDB source path
//
//     IW5_STATIC unsigned int holdrand IW5_AT("", "holdrand");
//     IW5_STATIC const float sRGBtoXYZ[4][4] IW5_AT("", "sRGBtoXYZ") IW5_INIT(= { ... });
//
//     int BoxOnPlaneSide(...)
//     {
//         IW5_STATIC int bops_initialized IW5_AT("BoxOnPlaneSide", "bops_initialized");
//
// IW5_AT(scope, name): scope is "" for a file static, or the enclosing function's name.
// tools/build_recon.py resolves every such symbol to the address in analysis/<build>/pdb/statics.tsv.
// IW5_INIT keeps the original initializer readable in the source; in the hybrid build the original
// binary's storage already holds it.
#pragma once

#define IW5_STATIC extern
#ifdef IW5_REPLAY
// tests/replay builds for the host (ELF), where '@' in a symbol is a version: the statics keep their C++ names there
// (the test defines the ones the function under test reaches)
#define IW5_AT(scope, name)
#else
#define IW5_AT(scope, name) __asm__("iw5s@" IW5_SOURCE "@" scope "@" name)
#endif
#define IW5_INIT(...)

// A global (public) variable the reconstructed file defines in the original. While the build is hybrid-linked its
// storage stays in the original binary, found by its decorated name in the MAP; IW5_INIT keeps the initializer in the
// source:
//
//     IW5_GLOBAL const vec4_t colorBlack IW5_INIT(= { 0.0f, 0.0f, 0.0f, 1.0f });
#define IW5_GLOBAL extern

// A thread-local original variable (`__declspec(thread)`), declared with IW5_STATIC / IW5_GLOBAL:
//
//     IW5_STATIC IW5_THREAD void **g_dwTlsIndex IW5_AT("", "g_dwTlsIndex");
//
// tools/build_recon.py places it in the DLL's .tls at its offset in the original's TLS block. The original's
// variables are constant-initialized; saying so keeps clang (as MSVC 2019 for an extern thread_local) from guarding
// each access with the on-demand dynamic TLS initialization (__tls_guard, __dyn_tls_on_demand_init).
#define IW5_THREAD __declspec(thread) __attribute__((require_constant_initialization))
