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

void NFA2DFA(ENFA *nfa, DFA *dfa)
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

	dfa->AppendState();

	while (!WorkList.empty())
	{
		std::set<int> q = WorkList.front();
		WorkList.pop();
		for (int c = 0; c < MAX_CHARACTER; c++)
		{
			if (c == EMPTY_TRANSFORM)
				continue;
			std::set<int> *t_tmp = nfa->Move(q, c);
			std::set<int> *t = nfa->EClosure(*t_tmp);
			if (t->empty())
				continue;
			if (Q.insert(*t).second == true)
			{
				SCDictionary.insert(std::pair<std::set<int>, int>(*t, SCDictionary.size()));
				WorkList.push(*t);

				dfa->AppendState();
				if (t->find(nfa->FinalState) != t->end())
					dfa->SetFinal(SCDictionary[*t]);
			}
			dfa->SetTransform(SCDictionary[q], c, SCDictionary[*t]);
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

	// NFA->ToDot("t.gv");
	DFA *dfa = new DFA();
	NFA2DFA(NFA, dfa);
	dfa->ToDot("t.gv");
	return 0;
}