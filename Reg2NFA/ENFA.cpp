#include <cstdlib>
#include "ENFA.h"

// --- Unitilies ---------------------------

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
	this->Alphabet.insert(c);
	if (*(this->TransMatrix[src] + c) == nullptr)
		this->TransMatrix[src][c] = new std::set<int>();
	std::set<int>** p = this->TransMatrix[src];
	std::set<int>* q = *(p + c);
	q->insert(dest);
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
