#pragma once
#include <utility>
enum LexType
{
	Char = 128,

	Eof = 255,
};

union LexValue
{
	char c;
	int i;
	char *s;
};

typedef std::pair<enum LexType, union LexValue> Lexme;

void InitLexer(const char *str);
Lexme Lex();
void PushBack(const Lexme & L);