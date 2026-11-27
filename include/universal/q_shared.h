// universal/q_shared.h, as far as the reconstruction needs it (prototypes from the iw5mp_xenon PDB).
#pragma once

bool I_islower(int c);
bool I_isupper(int c);
bool I_isalpha(int c);
bool I_isdigit(int c);
bool I_isalnum(int c);
bool I_isforfilename(int c);
void I_strncpyz(char *dest, const char *src, int destsize);
int I_strnicmp(const char *s0, const char *s1, int n);
int I_strncmp(const char *s0, const char *s1, int n);
const char *I_stristr(const char *s0, const char *substr);
int I_stricmp(const char *s0, const char *s1);
int I_strcmp(const char *s0, const char *s1);
int I_stricmpwild(const char *wild, const char *s);
char *I_strlwr(char *s);
char *I_strupr(char *s);
int I_iscsym(int c);
void I_strncat(char *dest, int size, const char *src);
int I_DrawStrlen(const char *str);
char *I_CleanStr(char *string);
char I_CleanChar(char character);
void Info_NextPair(const char **head, char *key, char *value);

// qcommon/qcommon.h
enum errorParm_t
{
	ERR_FATAL = 0x0,
	ERR_DROP = 0x1,
	ERR_SERVERDISCONNECT = 0x2,
	ERR_DISCONNECT = 0x3,
	ERR_SCRIPT = 0x4,
	ERR_SCRIPT_DROP = 0x5,
	ERR_LOCALIZATION = 0x6,
	ERR_MAPLOADERRORSUMMARY = 0x7,
};
void Com_Error(errorParm_t code, const char *fmt, ...);

// A color escape: '^' followed by '0'..';' (q_shared.h I_IsColorString)
#define Q_COLOR_ESCAPE '^'
inline bool I_IsColorString(const char *p)
{
	return p && *p == Q_COLOR_ESCAPE && p[1] && p[1] != Q_COLOR_ESCAPE && p[1] >= '0' && p[1] <= ';';
}
