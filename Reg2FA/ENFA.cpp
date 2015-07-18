#include <cstdlib>
#include "ENFA.h"

// --- Unitilies ---------------------------

ENFA::~ENFA()
{
	for (std::vector<std::set<int>**>::iterator sp = this->TransMatrix.begin(); sp != this->TransMatrix.end(); sp++)
	{
		for (int i = 0; i < MAX_CHARACTER; i++)
		{
			if ((*sp)[i] != nullptr)
				delete (*sp)[i];
		}
		delete *sp;
	}
}

// --- Basic -----------------------------

int ENFA::NewState()
{
	std::set<int>** np = (std::set<int>**)std::malloc(sizeof(std::set<int>**) * MAX_CHARACTER);
	if (np == nullptr)
	{
		// error
		throw new std::exception("Out of memory.");
	}
	for (int i = 0; i < MAX_CHARACTER; i++)
	{
		np[i] = nullptr;
	}
	this->TransMatrix.push_back(np);
	return this->TransMatrix.size() - 1;
}

void ENFA::LinkNode(int src, char c, int dest)
{
	if (src < 0 || src > ((int)this->TransMatrix.size() - 1) || dest < 1 || dest > ((int)this->TransMatrix.size() - 1))
	{
		// error
		throw new std::exception("No such state number.");
	}
	this->Alphabet[c] = this->Alphabet.size();
	if (this->TransMatrix[src][c] == nullptr)
		this->TransMatrix[src][c] = new std::set<int>();
	this->TransMatrix[src][c]->insert(dest);
}

// --- Advanced --------------------------

NFAPack * ENFA::MakeBasic(char c)
{
	int src = this->NewState();
	int dest = this->NewState();
	this->LinkNode(src, c, dest);
	NFAPack *np = new NFAPack(src, dest);

	return np;
}

NFAPack * ENFA::MakeLink(NFAPack *p0, char c, NFAPack *p1)
{
	this->LinkNode(p0->Tail, c, p1->Head);
	p0->Tail = p1->Tail;

	delete p1;
	return p0;
}

NFAPack * ENFA::MakeSelect(NFAPack *p0, NFAPack *p1)
{
	int n0 = this->NewState();
	int n1 = this->NewState();

	this->LinkNode(n0, EMPTY_TRANSFORM, p0->Head);
	this->LinkNode(n0, EMPTY_TRANSFORM, p1->Head);

	this->LinkNode(p0->Tail, EMPTY_TRANSFORM, n1);
	this->LinkNode(p1->Tail, EMPTY_TRANSFORM, n1);

	p0->Head = n0;
	p0->Tail = n1;

	delete p1;
	return p0;
}

NFAPack * ENFA::MakeClosure(NFAPack *p0)
{
	int n0 = this->NewState();
	int n1 = this->NewState();

	this->LinkNode(n0, EMPTY_TRANSFORM, p0->Head);
	this->LinkNode(p0->Tail, EMPTY_TRANSFORM, n1);
	this->LinkNode(n0, EMPTY_TRANSFORM, n1);
	this->LinkNode(p0->Tail, EMPTY_TRANSFORM, p0->Head);

	p0->Head = n0;
	p0->Tail = n1;

	return p0;
}

NFAPack * ENFA::MakeClosurePlus(NFAPack *p0)
{
	int n0 = this->NewState();
	int n1 = this->NewState();

	this->LinkNode(n0, EMPTY_TRANSFORM, p0->Head);
	this->LinkNode(p0->Tail, EMPTY_TRANSFORM, n1);
	this->LinkNode(p0->Tail, EMPTY_TRANSFORM, p0->Head);

	p0->Head = n0;
	p0->Tail = n1;

	return p0;
}

const std::set<int> & ENFA::Move(int q, char c)
{
	if (q < 0 || q >= (int)this->TransMatrix.size())
	{
		throw new std::exception("Transform start state error.");
	}
	if (this->TransMatrix[q][c] == nullptr)
	{
		return *(new std::set<int>());
	}
	return (*this->TransMatrix[q][c]);
}

std::set<int> * ENFA::Move(std::set<int> &Q, char c)
{
	std::set<int> *qn = new std::set<int>();
	for (auto p = Q.begin(); p != Q.end(); p++)
	{
		std::set<int> tmp = this->Move(*p, c);
		qn->insert(tmp.begin(), tmp.end());
	}
	return qn;
}

std::set<int> * ENFA::EClosure(std::set<int> &Q)
{
	std::set<int> *qn = this->Move(Q, EMPTY_TRANSFORM);
	qn->insert(Q.begin(), Q.end());
	if (qn->size() > Q.size())
	{
		std::set<int> *nqn = this->EClosure(*qn);
		delete qn;
		return nqn;
	}
	else
		return qn;
}

void ENFA::ToDot(const char *title, const char *file)
{
	FILE *f;
	fopen_s(&f, file, "wt+");
	if (f == nullptr)
		throw new std::exception("Cannot create/rewrite dot file.");
	fprintf(f, "digraph G\n{\n\trankdir = \"LR\";\n");
	fprintf(f, "\tnode[shape=circle];\n");
	fprintf(f, "\t%d[shape=doublecircle];\n", this->FinalState);
	fprintf(f, "\ttitle[shape=box label=\"RegEx = %s\"];\n", title);
	for (int i = 0; i < (int)this->TransMatrix.size(); i++)
	{
		for (int c = 0; c < MAX_CHARACTER; c++)
		{
			std::set<int> *p = this->TransMatrix[i][c];
			if (p == nullptr)
				continue;
			for (auto s = p->begin(); s != p->end(); s++)
			{
				if (c != '\0')
					fprintf(f, "\t%d->%d[label=\"%c\"];\n", i, *s, c);
				else
					fprintf(f, "\t%d->%d[label=\"¦Å\"];\n", i, *s);
			}
		}

	}
	fprintf(f, "}");
	fclose(f);
}

