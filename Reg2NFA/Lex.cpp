#include <cstring>
#include "Lex.h"

static char *src_str;
void InitLexer(const char *str)
{
	src_str = (char*)str;
}

char *strndup(char *s, int n)
{
	int l = std::strlen(s);
	l = n < l ? n : l;
	char *ns = (char*)std::malloc(l + 1);
	strncpy_s(ns, l + 1, s, l);
	return ns;
}

Lexme Lex()
{
	Lexme lm;
	switch (*src_str)
	{
	case '|':
	case '*': case '+':
	case '(': case ')':
		lm.first = (enum LexType)*src_str;
		lm.second.c = *src_str++;
		break;
	case '\0':
		lm.first = Eof;
		lm.second.c = *src_str++;
		break;
	default:
		lm.first = Char;
		lm.second.c = *src_str++;
		break;
	}
	return lm;
}

void PushBack(const Lexme & L)
{
	if (L.first == Char || L.first == Eof || L.first < 127)
	{
		src_str--;
	}
}
