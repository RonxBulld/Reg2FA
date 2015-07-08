#pragma once

#include <set>
#include <map>
#include <vector>

#include "Reg2FA.h"

#define EMPTY_TRANSFORM '\0'

class NFAPack
{
public:
	int Head, Tail;
	NFAPack(int Head, int Tail) :Head(Head), Tail(Tail) {}
};

class ENFA
{
public:
	std::set<int> States;
	std::map<char, int> Alphabet;
	int StartState;
	int FinalState;
	std::vector<std::set<int>**> TransMatrix;

	int NewState();
	void LinkNode(int src, char c, int dest);
	void ToDot(const char *file);

	NFAPack * MakeBasic(char c);
	NFAPack * MakeLink(NFAPack *p0, char c, NFAPack *p1);
	NFAPack * MakeSelect(NFAPack *p0, NFAPack *p1);
	NFAPack * MakeClosure(NFAPack *p0);
	NFAPack * MakeClosurePlus(NFAPack *p0);

	const std::set<int> & Move(int q, char c);
	std::set<int> * Move(std::set<int> &Q, char c);
	std::set<int> * ENFA::EClosure(std::set<int> &Q);
};
