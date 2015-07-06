
#include "Reg2NFA.h"
#include "Lex.h"

NFAPack *ParseStmt();
NFAPack *ParseItem();
NFAPack *ParseReplay(NFAPack *p0);

ENFA *NFA;
/**
 * Replay : '*'
 *        | '+'
 *        |
 *        ;
 */
NFAPack *ParseReplay(NFAPack *p0)
{
	TRACE("%s\n", __func__);
	Lexme l = Lex();
	if (l.first == '*')
	{
		p0 = NFA->MakeClosure(p0);
	}
	else if (l.first == '+')
	{
		p0 = NFA->MakeClosurePlus(p0);
	}
	else
	{
		PushBack(l);
	}
	return p0;
}
/**
 * Item : CHAR Replay
 *      | '(' Stmt ')' Replay
 *      |
 *      ;
 */
NFAPack *ParseItem()
{
	TRACE("%s\n", __func__);
	Lexme l = Lex();
	NFAPack *p0 = nullptr;
	if (l.first == Char)
	{
		p0 = NFA->MakeBasic(l.second.c);
		p0 = ParseReplay(p0);
	}
	else if (l.first == '(')
	{
		p0 = ParseStmt();
		if (Lex().first != ')')
		{
			// error
			fprintf(stderr, "except - )\n");
		}
		p0 = ParseReplay(p0);
	}
	else
	{
		PushBack(l);
	}
	return p0;
}
/**
 * Stmt : Item
 *      | Item Stmt
 *      | Item '|' Item Stmt
 *      ;
 */
NFAPack *ParseStmt()
{
	TRACE("%s\n", __func__);
	NFAPack *p0 = ParseItem();
	if (p0 != nullptr)
	{
		NFAPack *p1 = nullptr;
		Lexme l = Lex();
		if (l.first == '|')
		{
			p1 = ParseItem();
			if (p1 != nullptr)
			{
				p0 = NFA->MakeSelect(p0, p1);
			}
		}
		else
		{
			PushBack(l);
		}
		p1 = ParseStmt();
		if (p1 != nullptr)
		{
			p0 = NFA->MakeLink(p0, EMPTY_TRANSFORM, p1);
		}
	}
	return p0;
}

void NFA2Dot(NFAPack *p)
{
	FILE *f;
	fopen_s(&f, "t.gv", "wt+");
	fprintf(f, "digraph G\n{\n\trankdir = \"LR\";\n");
	for (int i = 0; i < NFA->TransMatrix.size(); i++)
	{
		for (int c = 0; c < NFA->AlphabetCounter; c++)
		{
			std::set<int> *p = NFA->TransMatrix[i][c];
			if (p == nullptr)
				continue;
			for (auto s = p->begin(); s != p->end(); s++)
			{
				char ch = NFA->FindAlpha(c);
				if (ch != '\0')
					fprintf(f, "\t%d->%d[label=\"%c\"];\n", i, *s, ch);
				else
					fprintf(f, "\t%d->%d[label=\"¦Å\"];\n", i, *s);
			}
		}
		
	}
	fprintf(f, "}");
	fclose(f);
}

int main()
{
	NFA = new ENFA();
	NFA->InitAlphabet(EMPTY_TRANSFORM);
	NFA->InitAlphabet("abcdefg");
	InitLexer("ab(cd+e|f*)*g");

	NFAPack *p = ParseStmt();

	NFA->StartState = p->Head;
	NFA->FinalState = p->Tail;
	NFA2Dot(p);
	return 0;
}