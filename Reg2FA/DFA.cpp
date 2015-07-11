#include <list>
#include "DFA.h"

#define IS_FINAL -1
#define IS_NOT_FINAL -2

inline bool DFA::IsValidState(int s)
{
	return (s >= 0 && s < this->DFATable.size());
}

inline bool DFA::IsValidInput(char c)
{
	return (c >= 0 && c < MAX_CHARACTER);
}

void DFA::AppendState()
{
	int *tp = (int*)std::malloc(sizeof(int) * MAX_CHARACTER);
	std::memset(tp, -1, sizeof(int) * MAX_CHARACTER);
	this->DFATable.push_back(tp);
	this->FinalTable.push_back(false);
}

void DFA::SetTransform(int src, char c, int dest)
{
	if (c <= 0 || c >= MAX_CHARACTER)
	{
		throw new std::exception("Bad character.");
	}
	if (!IsValidState(src))
	{
		throw new std::exception("Bad source state.");
	}
	if (!IsValidState(dest))
	{
		throw new std::exception("Bad destination state.");
	}
	if (this->DFATable[src][c] != -1)
	{
		throw new std::exception("Multiple destination state.");
	}
	this->DFATable[src][c] = dest;
	this->Alphabet.insert(c);
}

void DFA::SetFinal(int s)
{
	if (!IsValidState(s))
	{
		throw new std::exception("Bad state when set final.");
	}
	this->FinalTable[s] = true;
}

void DFA::Minimum()
{
	std::map<std::set<int>, int> ConvertTemp;
	int n = this->DFATable.size();
	std::vector<int> NewStatePoint(n, -1);
	std::vector<int*> NewDFATable;
	std::vector<bool> NewFinalTable;
	for (int s = 0; s < this->DFATable.size(); s++)
	{
		std::set<int> tmp;
		tmp.insert(this->FinalTable[s] ? IS_FINAL : IS_NOT_FINAL);
		for (auto cp = this->Alphabet.begin(); cp != this->Alphabet.end(); cp++)
		{
			tmp.insert(this->DFATable[s][*cp]);
		}
		if (ConvertTemp.insert(std::pair<std::set<int>, int>(tmp, ConvertTemp.size())).second == true)
		{
			NewDFATable.push_back(this->DFATable[s]);
			NewFinalTable.push_back(this->FinalTable[s]);
		}
		else
		{
			delete this->DFATable[s];
		}
		NewStatePoint[s] = ConvertTemp[tmp];
		tmp.clear();
	}
	this->StartState = NewStatePoint[this->StartState];
	// 1. Remove unnessary state (prev did)
	// 2. Rewrite all tranform table
	// 3. Rewrite start state and final state collection (prev did)
	for (int i = 0; i < NewDFATable.size(); i++)
	{
		for (auto cp = this->Alphabet.begin(); cp != this->Alphabet.end(); cp++)
		{
			int os = NewDFATable[i][*cp];
			if (os != -1)
				NewDFATable[i][*cp] = NewStatePoint[os];
		}
	}
	this->DFATable.swap(NewDFATable);
}

int DFA::Move(int s, char c)
{
	if (IsValidState(s) == false)
	{
		throw new std::exception("Bad source state.");
	}
	else if (IsValidInput(c) == false)
	{
		throw new std::exception("Bad input character.");
	}
	return this->DFATable[s][c];
}

bool DFA::IsFinalState(int s)
{
	if (IsValidState(s) == false)
	{
		throw new std::exception("Bad source state.");
	}
	return this->FinalTable[s];
}

const char *DFA::StringMove(const char *str)
{
	int cur_state = 0;
	char *cur_p = (char*)str;
	
	int prev_match_state = 0;
	char *next_match_char = cur_p;
	
	while (*cur_p != '\0')
	{
		cur_state = this->DFATable[cur_state][*cur_p++];
		if (cur_state == -1)
		{
			cur_p = next_match_char;
			break;
		}
		if (this->FinalTable[cur_state] == true)
		{
			next_match_char = cur_p;
			prev_match_state = cur_state;
		}
	}

	return (const char *)cur_p;
}

void DFA::ToDot(const char *title, const char *file)
{
	FILE *f;
	fopen_s(&f, "t.gv", "wt+");
	fprintf(f, "digraph G\n{\n\trankdir = \"LR\";\n");
	fprintf(f, "\tnode[shape=circle];\n");
	fprintf(f, "\ttitle[shape=box label=\"RegEx = %s\"];\n", title);
	for (int i = 0; i < this->FinalTable.size(); i++)
		if (this->FinalTable[i])
			fprintf(f, "\t%d[shape=doublecircle];\n", i);
	for (int s = 0; s < this->DFATable.size(); s++)
	{
		for (int c = 0; c < MAX_CHARACTER; c++)
		{
			if (this->DFATable[s][c] != -1)
				if (c != '\0')
				fprintf(f, "\t%d->%d[label=\"%c\"];\n", s, this->DFATable[s][c], c);
				else
					fprintf(f, "\t%d->%d[label=\"¦Å\"];\n", s, this->DFATable[s][c]);
		}
	}
	fprintf(f, "}");
	fclose(f);
}

DFA::~DFA()
{
	for (auto sp = this->DFATable.begin(); sp != this->DFATable.end(); sp++)
	{
		delete *sp;
	}
}
