// <string.h>, as far as the reconstruction needs it.
//
// Build environment, not game source. The Store build links the CRT dynamically: these resolve to its import table
// through the hybrid link (tools/build_recon.py), so reconstructed code calls the same CRT the original does.
// strlen and strcmp, which VS2019 expands inline, come from tools/recon/crt_x64.s.
#pragma once

typedef __SIZE_TYPE__ size_t;  // unsigned int on x86, unsigned __int64 on x64

#ifdef __cplusplus
extern "C" {
#endif

void *memset(void *dst, int val, size_t size);
void *memcpy(void *dst, const void *src, size_t size);
void *memmove(void *dst, const void *src, size_t size);
int memcmp(const void *a, const void *b, size_t size);
size_t strlen(const char *str);
int strcmp(const char *a, const char *b);
char *strchr(const char *str, int c);
char *strncpy(char *dst, const char *src, size_t count);

#ifdef __cplusplus
}
#endif

// strcpy and strcat are expanded inline (/Oi): the Store exe imports neither. These are the expansions: a forward
// byte copy (strcat: after a strlen). Forward copying is what makes an overlapping strcpy (Quake 3's
// Info_RemoveKey) work.
static inline char *strcpy(char *dst, const char *src)
{
	char *d = dst;
	while ((*d++ = *src++) != 0)
		;
	return dst;
}

static inline char *strcat(char *dst, const char *src)
{
	strcpy(dst + strlen(dst), src);
	return dst;
}
