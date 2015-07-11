#pragma once
#include <utility>

class Lexer
{
private:
	char *src_str = nullptr;
public:
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

	Lexer(const char *str = nullptr);
	void InitLexer(const char *str);
	Lexer::Lexme Lex();
	void PushBack(const Lexme & L);
};
