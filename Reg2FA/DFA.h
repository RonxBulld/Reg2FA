#pragma once
#include <vector>
#include "Reg2FA.h"

class DFA
{
private:
	inline bool IsValidState(int s);
public:
	std::vector<int*> DFATable;
	std::vector<bool> FinalTable;

	void AppendState();
	void SetTransform(int src, char c, int dest);
	void SetFinal(int s);
	void ToDot(const char *file);
};