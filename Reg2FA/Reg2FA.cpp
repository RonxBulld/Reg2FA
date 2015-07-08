#include <queue>
#include "Reg2FA.h"
#include "RegexParse.h"

ENFA *NFA;

void PrintSet(const std::set<int> *s)
{
	TRACE("{ ");
	for (auto ip = s->begin(); ip != s->end(); ip++)
	{
		TRACE("%d ", *ip);
	}
	TRACE("}");
}

void NFA2DFA(ENFA *nfa)
{
	/*
	q0←ε-closure({n0});
	Q←q0;
	WorkList←{q0};
	while (WorkList≠∅) do
		remove q from WorkList;
		for each character c∈Σ do
			t←ε-closure(Delta(q,c));
			T[q,c]←t;
			if t∉Q then
				add t to Q and to WorkList;
		end;
	end;
	*/

	std::set<int> q0 = *nfa->EClosure(*(new std::set<int>({ nfa->StartState })));
	std::set<std::set<int>> Q({ q0 });
	std::queue<std::set<int>> WorkList({ q0 });
	
	std::map<std::set<int>, int> SCDictionary;
	SCDictionary.insert(std::pair<std::set<int>, int>(q0, 0));

	std::vector<int*> DFATable;
	int *tp = (int*)std::malloc(sizeof(int) * nfa->Alphabet.size());
	std::memset(tp, -1, sizeof(int) * nfa->Alphabet.size());
	DFATable.push_back(tp);

	while (!WorkList.empty())
	{
		std::set<int> q = WorkList.front();
		WorkList.pop();
		for (auto cp = nfa->Alphabet.begin(); cp != nfa->Alphabet.end(); cp++)
		{
			std::set<int> *t_tmp = nfa->Move(q, cp->first);
			std::set<int> *t = nfa->EClosure(*t_tmp);
			if (t->empty())
				continue;
			// TRACE("T["); PrintSet(&q); TRACE(", %d] = ", cp->first); PrintSet(t); TRACE("\n");
			if (Q.insert(*t).second == true)
			{
				SCDictionary.insert(std::pair<std::set<int>, int>(*t, SCDictionary.size()));

				int *tp = (int*)std::malloc(sizeof(int) * nfa->Alphabet.size());
				std::memset(tp, -1, sizeof(int) * nfa->Alphabet.size());
				DFATable.push_back(tp);

				WorkList.push(*t);
			}

			DFATable[SCDictionary[q]][cp->second] = SCDictionary[*t];
			TRACE("T[%d, %d(%d)] = %d\n", SCDictionary[q], cp->first, cp->second, DFATable[SCDictionary[q]][cp->second]);
		}
	}
}

int main()
{
	NFA = new ENFA();
	InitLexer("ab(cd+e|f*)*g");

	NFAPack *p = ParseStmt();
	NFA->StartState = p->Head;
	NFA->FinalState = p->Tail;

	NFA->ToDot("t.gv");
	NFA2DFA(NFA);
	return 0;
}