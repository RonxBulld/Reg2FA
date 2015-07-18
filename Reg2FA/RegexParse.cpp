#include "RegexParse.h"

/**
* Replay : '*'
*        | '+'
*        |
*        ;
*/
NFAPack * Parser::ParseReplay(ENFA *nfa, NFAPack *p0)
{
	// TRACE("%s\n", __func__);
	Lexer::Lexme l = this->Lex->Lex();
	if (l.first == '*')
	{
		p0 = nfa->MakeClosure(p0);
	}
	else if (l.first == '+')
	{
		p0 = nfa->MakeClosurePlus(p0);
	}
	else
	{
		this->Lex->PushBack(l);
	}
	return p0;
}
/**
* Meta : CHAR Replay
*      | CHAR Replay Meta
*      ;
*/
NFAPack * Parser::ParseMeta(ENFA *nfa)
{
	// TRACE("%s\n", __func__);
	Lexer::Lexme l = this->Lex->Lex();
	NFAPack *p0 = nullptr;
	if (l.first == Lexer::Char)
	{
		p0 = nfa->MakeBasic(l.second.c);
		p0 = this->ParseReplay(nfa, p0);
		NFAPack *p1 = this->ParseMeta(nfa);
		if (p1 != nullptr)
		{
			p0 = nfa->MakeLink(p0, EMPTY_TRANSFORM, p1);
		}
	}
	else
	{
		this->Lex->PushBack(l);
	}
	return p0;
}
/**
* Item : Meta
*      | '(' Stmt ')' Replay
*      |
*      ;
*/
NFAPack * Parser::ParseItem(ENFA *nfa)
{
	// TRACE("%s\n", __func__);
	NFAPack *p0 = this->ParseMeta(nfa);
	if (p0 != nullptr)
	{
	}
	else
	{
		Lexer::Lexme l = this->Lex->Lex();
		if (l.first == '(')
		{
			p0 = this->ParseStmt(nfa);
			if (this->Lex->Lex().first != ')')
			{
				// error
				fprintf(stderr, "except - )\n");
			}
			p0 = this->ParseReplay(nfa, p0);
		}
		else
		{
			this->Lex->PushBack(l);
		}
	}
	return p0;
}
/**
* Stmt : Item
*      | Item Stmt
*      | Item '|' Item Stmt
*      ;
*/
NFAPack * Parser::ParseStmt(ENFA *nfa)
{
	// TRACE("%s\n", __func__);
	NFAPack *p0 = this->ParseItem(nfa);
	if (p0 != nullptr)
	{
		NFAPack *p1 = nullptr;
		Lexer::Lexme l = this->Lex->Lex();
		if (l.first == '|')
		{
			p1 = this->ParseItem(nfa);
			if (p1 != nullptr)
			{
				p0 = nfa->MakeSelect(p0, p1);
			}
		}
		else
		{
			this->Lex->PushBack(l);
		}
		p1 = this->ParseStmt(nfa);
		if (p1 != nullptr)
		{
			p0 = nfa->MakeLink(p0, EMPTY_TRANSFORM, p1);
		}
	}
	return p0;
}
