#include <cstring>
#include "Lex.h"

Lexer::Lexer(const char *str)
{
	this->InitLexer(str);
}

void Lexer::InitLexer(const char *str)
{
	this->src_str = (char*)str;
}

/*
char *strndup(char *s, int n)
{
	int l = std::strlen(s);
	l = n < l ? n : l;
	char *ns = (char*)std::malloc(l + 1);
	strncpy_s(ns, l + 1, s, l);
	return ns;
}
*/

Lexer::Lexme Lexer::Lex()
{
	Lexer::Lexme lm;
	switch (*this->src_str)
	{
	case '|':
	case '*': case '+':
	case '(': case ')':
		lm.first = (enum LexType)*this->src_str;
		lm.second.c = *this->src_str++;
		break;
	case '\0':
		lm.first = Lexer::Eof;
		lm.second.c = *this->src_str++;
		break;
	default:
		lm.first = Lexer::Char;
		lm.second.c = *this->src_str++;
		break;
	}
	return lm;
}

void Lexer::PushBack(const Lexer::Lexme & L)
{
	if (L.first == Lexer::Char || L.first == Lexer::Eof || L.first < 128)
	{
		this->src_str--;
	}
}
