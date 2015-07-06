#pragma once

#include <set>
#include <map>
#include <vector>

#define EMPTY_TRANSFORM '\0'
#define MAX_CHARACTER 128

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
	std::set<char> Alphabet;
	int StartState;
	int FinalState;
	std::vector<std::set<int>**> TransMatrix;

	int NewState();
	void LinkNode(int src, char c, int dest);

	NFAPack * MakeBasic(char c);
	NFAPack * MakeLink(NFAPack *p0, char c, NFAPack *p1);
	NFAPack * MakeSelect(NFAPack *p0, NFAPack *p1);
	NFAPack * MakeClosure(NFAPack *p0);
	NFAPack * MakeClosurePlus(NFAPack *p0);
};
