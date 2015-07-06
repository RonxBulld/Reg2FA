#include <cstdlib>
#include "ENFA.h"

// --- Unitilies ---------------------------
ENFA::ENFA()
{
	for (int i = 0; i <= 127; i++)
		this->Alphabet[i] = -1;
}

void ENFA::InitAlphabet(const char *s)
{
	for (int i = 0; s[i] != '\0'; i++)
	{
		this->InitAlphabet(s[i]);
	}
}

void ENFA::InitAlphabet(char c)
{
	if (c <= 127)
		this->Alphabet[(int)c] = this->AlphabetCounter++;
}

char ENFA::FindAlpha(int AlphaID)
{
	for (char p = 0; p <= 127; p++)
	{
		if (this->Alphabet[p] == AlphaID)
			return p;
	}
	// error
	throw new std::exception("Except character id");
	return '\0';
}

int ENFA::RequestAlphaID(char c)
{
	if (this->Alphabet[c] != -1)
		return this->Alphabet[c];
	else
		throw new std::exception("This character is not in alphabet.");
}

// --- Basic -----------------------------

int ENFA::NewState()
{
	std::set<int>** np = (std::set<int>**)std::malloc(sizeof(std::set<int>**) * this->AlphabetCounter);
	if (np == nullptr)
	{
		// error
		throw new std::exception("Out of memory.");
	}
	for (int i = 0; i < this->AlphabetCounter; i++)
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
	int index = this->RequestAlphaID(c);
	if (index == -1)
	{
		// error
		throw new std::exception("No such alpha in Alphabet.");
	}
	if (*(this->TransMatrix[src] + index) == nullptr)
		this->TransMatrix[src][index] = new std::set<int>();
	std::set<int>** p = this->TransMatrix[src];
	std::set<int>* q = *(p + index);
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
