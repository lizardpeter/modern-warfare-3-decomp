// Assert macros as they expand in the original Xbox 360 development build (iw5mp_xenon).
//
// Build environment, not game source. A failed assert calls MyAssertHandler(__FILE__, __LINE__, type, fmt, ...)
// (universal/assertive.cpp; it returns nothing, and the code carries on after it). The reconstruction's line numbers
// differ from the original's, so each call site gives the ORIGINAL line, and each .cpp defines IW5_ASSERT_FILE as the
// original __FILE__ string ("c:\\trees\\iw5build4-iw\\iw5\\code_source\\src\\universal\\q_shared.cpp"). Each macro
// reproduces one argument shape seen in the reference build's calls.
//
// The Store builds compiled their asserts out: tools/build_recon.py defines IW5_NO_ASSERTS for every build whose
// target.toml entry doesn't say `asserts = true`, and the macros vanish, expression and all. Only x64 Store builds
// are linked today, so these shapes are documentation of the reference until a build with asserts is.
#pragma once

void MyAssertHandler(const char *filename, int line, int type, const char *fmt, ...);  // universal/assertive.cpp

#ifndef IW5_NO_ASSERTS
// MyAssertHandler(file, line, 0, "%s", "expr")
#define iassert_at(line, expr) \
	do { if (!(expr)) MyAssertHandler(IW5_ASSERT_FILE, line, 0, "%s", #expr); } while (0)

// MyAssertHandler(file, line, 0, "%s\n\t(value) = %i", "expr", value): an assert that prints one int
#define iassertvalue_at(line, expr, value) \
	do { if (!(expr)) MyAssertHandler(IW5_ASSERT_FILE, line, 0, "%s\n\t(" #value ") = %i", #expr, value); } while (0)
#else
#define iassert_at(...) ((void)0)
#define iassertvalue_at(...) ((void)0)
#endif
