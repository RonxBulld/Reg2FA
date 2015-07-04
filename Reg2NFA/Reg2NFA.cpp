
#include "Reg2NFA.h"
#include "Lex.h"

NFAPack *ParseStmt();
NFAPack *ParseItem();
NFAPack *ParseReplay(NFAPack *p0);

NFAPack *MakeLink(NFAPack *p0, NFAPack *p1)
{
	p0->Tail->LinkNode(EMPTY_TRANSFORM, p1->Head);
	p0->Tail = p1->Tail;
	delete p1;
	return p0;
}

NFAPack *MakeSelect(NFAPack *p0, NFAPack *p1)
{
	NFANode *n0 = new NFANode();
	NFANode *n1 = new NFANode();

	n0->LinkNode(EMPTY_TRANSFORM, p0->Head);
	n0->LinkNode(EMPTY_TRANSFORM, p1->Head);

	p0->Tail->LinkNode(EMPTY_TRANSFORM, n1);
	p1->Tail->LinkNode(EMPTY_TRANSFORM, n1);

	p0->Head = n0;
	p0->Tail = n1;

	delete p1;
	return p0;
}

NFAPack *MakeClosure(NFAPack *p0)
{
	NFANode *n0 = new NFANode();
	NFANode *n1 = new NFANode();

	n0->LinkNode(EMPTY_TRANSFORM, p0->Head);
	p0->Tail->LinkNode(EMPTY_TRANSFORM, n1);
	n0->LinkNode(EMPTY_TRANSFORM, n1);
	p0->Tail->LinkNode(EMPTY_TRANSFORM, p0->Head);

	p0->Head = n0;
	p0->Tail = n1;

	return p0;
}

NFAPack *MakeClosurePlus(NFAPack *p0)
{
	NFANode *n0 = new NFANode();
	NFANode *n1 = new NFANode();

	n0->LinkNode(EMPTY_TRANSFORM, p0->Head);
	p0->Tail->LinkNode(EMPTY_TRANSFORM, n1);
	p0->Tail->LinkNode(EMPTY_TRANSFORM, p0->Head);

	p0->Head = n0;
	p0->Tail = n1;

	return p0;
}

NFAPack *MakeBasic(char c)
{
	NFANode *n0 = new NFANode();
	NFANode *n1 = new NFANode();
	n0->LinkNode(c, n1);
	NFAPack *p0 = new NFAPack(n0, n1);
	return p0;
}

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
		p0 = MakeClosure(p0);
	}
	else if (l.first == '+')
	{
		p0 = MakeClosurePlus(p0);
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
		p0 = MakeBasic(l.second.c);
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
				p0 = MakeSelect(p0, p1);
			}
		}
		else
		{
			PushBack(l);
		}
		p1 = ParseStmt();
		if (p1 != nullptr)
		{
			p0 = MakeLink(p0, p1);
		}
	}
	return p0;
}

std::set<NFANode*> NodePo;
void NFA2Dot(NFAPack *p)
{
	FILE *f;
	fopen_s(&f, "t.gv", "wt+");
	fprintf(f, "digraph G\n{\n\trankdir = \"LR\";\n");
	for (auto p = NodePo.begin(); p != NodePo.end(); p++)
	{
		NFANode *n = *p;
		for (auto q = n->Edges.begin(); q != n->Edges.end(); q++)
		{
			if ((*q).first != '\0')
				fprintf(f, "\t%d->%d[label=\"%c\"];\n", n->ID, (*q).second->ID, (*q).first);
			else
				fprintf(f, "\t%d->%d[label=\"¦Å\"];\n", n->ID, (*q).second->ID);
		}
		
	}
	fprintf(f, "}");
	fclose(f);
}

int main()
{
	InitLexer("ab(cd+e|f*)*g");
	NFAPack *p = ParseStmt();
	p->Head->Start = true;
	p->Tail->Final = true;
	NFA2Dot(p);
	return 0;
}