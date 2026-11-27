// universal/q_shared.cpp: string utilities (the first reconstructed file; the rest of the file follows).
//
// Each function: the reference VA (iw5mp_xenon) and original line from its PDB, then the Store VA (iw5mp_ship,
// records/names.tsv). Control flow follows the reference; the Store build is the behaviour tested.
#define IW5_ASSERT_FILE "c:\\trees\\iw5build4-iw\\iw5\\code_source\\src\\universal\\q_shared.cpp"
#include <ctype.h>
#include <iw5_assert.h>
#include <string.h>

#include "universal/q_shared.h"

// 8260BB68  universal/q_shared.cpp:662  Store 14031AA40
bool I_islower(int c)
{
	return c >= 'a' && c <= 'z';
}

// 8260BB88  universal/q_shared.cpp:668  (inlined in the Store build)
bool I_isupper(int c)
{
	return c >= 'A' && c <= 'Z';
}

// 8260BBA8  universal/q_shared.cpp:674  (inlined in the Store build)
bool I_isalpha(int c)
{
	return I_islower(c) || I_isupper(c);
}

// 8260BBE0  universal/q_shared.cpp:680  Store 14031AA00
bool I_isdigit(int c)
{
	return c >= '0' && c <= '9';
}

// 8260BC00  universal/q_shared.cpp:686  (inlined in the Store build)
bool I_isalnum(int c)
{
	return I_isalpha(c) || I_isdigit(c);
}

// 8260BC70  universal/q_shared.cpp:692  Store 14031AA10
bool I_isforfilename(int c)
{
	return I_isalnum(c) || c == '_' || c == '-';
}

// 8260BD00  universal/q_shared.cpp:708  Store 14031AD50
void I_strncpyz(char *dest, const char *src, int destsize)
{
	iassert_at(709, src);
	iassert_at(710, dest);
	iassertvalue_at(711, (destsize >= 1), destsize);

	strncpy(dest, src, destsize - 1);
	dest[destsize - 1] = 0;
}

// 8260BDC0  universal/q_shared.cpp:720  Store 14031AD80
int I_strnicmp(const char *s0, const char *s1, int n)
{
	int c0;
	int c1;

	do
	{
		c0 = (unsigned char)*s0++;
		c1 = (unsigned char)*s1++;

		if (!n--)
			return 0;  // strings are equal until end point

		if (c0 != c1)
		{
			if (I_isupper(c0))
				c0 += 'a' - 'A';
			if (I_isupper(c1))
				c1 += 'a' - 'A';
			if (c0 != c1)
				return c0 < c1 ? -1 : 1;
		}
	} while (c0);

	return 0;  // strings are equal
}

// 8260BE60  universal/q_shared.cpp:750  Store 14031AD00
int I_strncmp(const char *s0, const char *s1, int n)
{
	int c0;
	int c1;

	do
	{
		c0 = *s0++;
		c1 = *s1++;

		if (!n--)
			return 0;  // strings are equal until end point

		if (c0 != c1)
			return c0 < c1 ? -1 : 1;
	} while (c0);

	return 0;  // strings are equal
}

// 8260BEB0  universal/q_shared.cpp:770  Store 14031ABB0
const char *I_stristr(const char *s0, const char *substr)
{
	int charIndex;
	int substrIndex;

	iassert_at(774, s0);
	iassert_at(775, substr);

	for (charIndex = 0; s0[charIndex]; ++charIndex)
	{
		for (substrIndex = 0; substr[substrIndex]; ++substrIndex)
		{
			if (tolower(s0[charIndex + substrIndex]) != tolower(substr[substrIndex]))
				break;
		}
		if (!substr[substrIndex])
			return &s0[charIndex];
	}
	return 0;
}

// 8260BF90  universal/q_shared.cpp:794  Store 14031AAA0 (I_strnicmp inlined)
int I_stricmp(const char *s0, const char *s1)
{
	iassert_at(795, s0);
	iassert_at(796, s1);

	return I_strnicmp(s0, s1, 0x7FFFFFFF);
}

// 8260C018  universal/q_shared.cpp:803  Store 14031AA50 (I_strncmp inlined)
int I_strcmp(const char *s0, const char *s1)
{
	iassert_at(804, s0);
	iassert_at(805, s1);

	return I_strncmp(s0, s1, 0x7FFFFFFF);
}

// 8260C0F0  universal/q_shared.cpp:811  Store 14031AB10
int I_stricmpwild(const char *wild, const char *s)
{
	char charWild;
	char charS;
	int delta;

	iassert_at(816, wild);
	iassert_at(817, s);

	do
	{
		charWild = *wild;
		++wild;
		if (charWild == '*')
		{
			if (!*wild)
				return 0;  // a trailing '*' matches the rest
			if (*s && !I_stricmpwild(wild - 1, s + 1))
				return 0;  // the '*' absorbs one more character
			continue;      // the '*' absorbs nothing
		}
		charS = *s;
		++s;
		if (charWild != charS && charWild != '?')
		{
			delta = tolower(charWild) - tolower(charS);
			if (delta)
				return delta < 0 ? -1 : 1;
		}
	} while (charWild);

	return 0;
}

// 8260C208  universal/q_shared.cpp:849  Store 14031AC50
char *I_strlwr(char *s)
{
	char *iter;

	for (iter = s; *iter; ++iter)
	{
		if (I_isupper(*iter))
			*iter += 'a' - 'A';
	}
	return s;
}

// 8260C260  universal/q_shared.cpp:864  (not in the Store build)
char *I_strupr(char *s)
{
	char *iter;

	for (iter = s; *iter; ++iter)
	{
		if (I_islower(*iter))
			*iter += 'A' - 'a';
	}
	return s;
}

// 8260C2B8  universal/q_shared.cpp:879  Store 14031A9D0
int I_iscsym(int c)
{
	return I_isalnum(c) || c == '_';
}

// 8260C300  universal/q_shared.cpp:891  Store 14031AC80
void I_strncat(char *dest, int size, const char *src)
{
	int destLen;

	iassert_at(894, size != sizeof( char * ));

	destLen = (int)strlen(dest);
	if (destLen >= size)
		Com_Error(ERR_FATAL, "\x15I_strncat: already overflowed");
	I_strncpyz(&dest[destLen], src, size - destLen);
}

// 8260C398  universal/q_shared.cpp:910  Store 14031A980
int I_DrawStrlen(const char *str)
{
	const char *s;
	int count;

	s = str;
	count = 0;
	while (*s)
	{
		if (I_IsColorString(s))
		{
			s += 2;
		}
		else
		{
			++count;
			++s;
		}
	}
	return count;
}

// 8260C408  universal/q_shared.cpp:932  Store 14031A920
char *I_CleanStr(char *string)
{
	char *d;
	char *s;
	int c;

	s = string;
	d = string;
	while ((c = *s) != 0)
	{
		if (I_IsColorString(s))
		{
			s++;
		}
		else if (c >= 0x20 && c <= 0x7E)
		{
			*d++ = (char)c;
		}
		s++;
	}
	*d = '\0';

	return string;
}

// 8260C490  universal/q_shared.cpp:959  Store 14031A910
char I_CleanChar(char character)
{
	if (character == (char)0x92)  // right single quotation mark in Windows-1252
		return '\'';
	return character;
}

// 8260C9F8  universal/q_shared.cpp:1262  Store 14031ADF0
void Info_NextPair(const char **head, char *key, char *value)
{
	char *o;
	const char *s;

	s = *head;

	if (*s == '\\')
		s++;
	key[0] = 0;
	value[0] = 0;

	o = key;
	while (*s != '\\')
	{
		if (!*s)
		{
			*o = 0;
			*head = s;
			return;
		}
		*o++ = *s++;
	}
	*o = 0;
	s++;

	o = value;
	while (*s != '\\' && *s)
		*o++ = *s++;
	*o = 0;

	*head = s;
}
