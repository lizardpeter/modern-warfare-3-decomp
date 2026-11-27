// <ctype.h>, as far as the reconstruction needs it.
//
// Build environment, not game source. tolower and toupper are UCRT imports of the Store build, called with a char
// sign-extended to int, as here.
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

int tolower(int c);
int toupper(int c);

#ifdef __cplusplus
}
#endif
