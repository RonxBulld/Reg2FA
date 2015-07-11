#pragma once
#include <vector>
#include "Reg2FA.h"

class DFA
{
private:
	inline bool IsValidState(int s);
	std::set<int> Alphabet;
public:
	std::vector<int*> DFATable;
	int StartState;
	std::vector<bool> FinalTable;

	void AppendState();
	void SetTransform(int src, char c, int dest);
	void SetFinal(int s);
	void Minimum();
	void ToDot(const char *file);
	~DFA();
};