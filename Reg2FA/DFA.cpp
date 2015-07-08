#include "DFA.h"

inline bool DFA::IsValidState(int s)
{
	return (s >= 0 && s < this->DFATable.size());
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
}

void DFA::SetFinal(int s)
{
	if (!IsValidState(s))
	{
		throw new std::exception("Bad state when set final.");
	}
	this->FinalTable[s] = true;
}

void DFA::ToDot(const char *file)
{
	FILE *f;
	fopen_s(&f, "t.gv", "wt+");
	fprintf(f, "digraph G\n{\n\trankdir = \"LR\";\n");
	fprintf(f, "\tnode[shape=circle];\n");
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
