#pragma once

#include "Reg2FA.h"
#include "Lex.h"

class Parser
{
private:
	Lexer *Lex;
public:
	Parser(Lexer *Lex) :Lex(Lex) {}
	NFAPack *ParseStmt(ENFA *NFA);
	NFAPack *ParseItem(ENFA *NFA);
	NFAPack *ParseMeta(ENFA *NFA);
	NFAPack *ParseReplay(ENFA *NFA, NFAPack *p0);
};
