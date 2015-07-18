#include <queue>
#include "Reg2FA.h"
#include "RegexParse.h"

/*
void PrintSet(const std::set<int> *s)
{
	TRACE("{ ");
	for (auto ip = s->begin(); ip != s->end(); ip++)
	{
		TRACE("%d ", *ip);
	}
	TRACE("}");
}
*/

class LexRegEx
{
private:
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
		dfa->StartState = 0;

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
				delete t_tmp;
				if (t->empty())
				{
					delete t;
					continue;
				}
				if (Q.insert(*t).second == true)
				{
					SCDictionary.insert(std::pair<std::set<int>, int>(*t, SCDictionary.size()));
					WorkList.push(*t);

					dfa->AppendState();
					if (t->find(nfa->FinalState) != t->end())
						dfa->SetFinal(SCDictionary[*t]);
				}
				dfa->SetTransform(SCDictionary[q], c, SCDictionary[*t]);
				delete t;
			}
		}
	}

	DFA dfa;
public:
	bool SetRegex(const char *regex)
	{
		ENFA nfa;
		Lexer lex(regex);
		Parser parse(&lex);
		NFAPack *p = parse.ParseStmt(&nfa);
		if (p == nullptr)
			return false;
		nfa.StartState = p->Head;
		nfa.FinalState = p->Tail;
		// nfa.ToDot(regex, "t.gv");
		delete p;

		this->NFA2DFA(&nfa, &this->dfa);
		this->dfa.Minimum();
		this->dfa.ToDot(regex, "t.gv");

		return true;
	}
	const char * Match(const char *str)
	{
		const char *next = this->dfa.StringMove(str);
		char *match_str = (char*)std::malloc(sizeof(char) * (next - str + 1));
		if (match_str == nullptr)
			throw new std::exception("Out of memory.");
		STRNCPY(match_str, next - str + 1, str, next - str);
		return match_str;
	}
};

int main(int argc, char **argv)
{
	char *test = new char[10];
	LexRegEx re;
	int RunCode = 0;
	if (argc == 4)
	{
		// re.SetRegex("ab(cd+e|f*)*g");
		// printf("%s\n", re.Match("abgfff"));
		re.SetRegex(argv[1]);
		const char *v = re.Match(argv[2]);
		if (std::strcmp(argv[3], v) == 0)
			RunCode = 0;
		else
			RunCode = 1;
	}
	else
	{
		printf("Usage: %s <Regular-Expressions> <Test-String> <Verify-String>\n", argv[0]);
		RunCode = 2;
	}
	return RunCode;
}