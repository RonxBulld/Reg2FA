#pragma once
#include <vector>
#include "Reg2FA.h"

class DFA
{
private:
	inline bool IsValidState(int s);
	inline bool IsValidInput(char c);
	std::set<int> Alphabet;
public:
	std::vector<int*> DFATable;
	int StartState;
	std::vector<bool> FinalTable;

	void AppendState();
	void SetTransform(int src, char c, int dest);
	void SetFinal(int s);
	void Minimum();
	int Move(int s, char c);
	bool IsFinalState(int s);
	const char *StringMove(const char *str);
	void ToDot(const char *title, const char *file);
	~DFA();
};