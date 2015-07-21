#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
using namespace std;
typedef vector<int> RuleLine;
typedef vector<RuleLine> Product;

string Grammer = "";
map<string, int> TokenMap;
map<int, Product> GrammerMap;
enum TokenType { _the_head_token_type = 256, GrammerBegin, OtherProduct, GrammerEnd, _the_tail_token_type };

inline void LexicalError()
{
	cerr << "Lexical Error." << endl;
}
inline void GrammerError()
{
	cerr << "GrammerError." << endl;
}

int ungettok(int p = -1)
{
	static int prev_tok = -1;
	swap(p, prev_tok);
	return p;
}
unsigned int gettok(istringstream &iss)
{
	{
		int tmp = ungettok();
		if (tmp != -1)
			return tmp;
	}
	unsigned char c = '\0';
	do { iss >> c; } while (isspace(c));
	if (c == '\'')
	{
		iss >> c;
		if (iss.get() != '\'')
		{
			LexicalError();
		}
		return (unsigned int)c;
	}
	else if (isalpha(c) || c == '_')
	{
		string TokenName;
		do
		{
			TokenName.append(1, c);
			iss >> c;
		} while (isalnum(c) || c == '_');
		iss.unget();
		TokenMap[TokenName] = TokenMap.size() + _the_tail_token_type;
		return TokenMap[TokenName];
	}
	else if (c == ':')
	{
		return GrammerBegin;
	}
	else if (c == '|')
	{
		return OtherProduct;
	}
	else if (c == ';')
	{
		return GrammerEnd;
	}
	else
	{
		LexicalError();
		return (unsigned int)c;
	}
}
bool ParseRuleLine(istringstream &iss, Product &Prod)
{
	int L;
	RuleLine rl;
	do
	{
		L = gettok(iss);
		rl.push_back(L);
	} while ((L > _the_head_token_type && L < _the_tail_token_type) && !iss.eof());
	ungettok(L);
	rl.pop_back();
	Prod.push_back(rl);
	return true;
}
bool ParseProduct(istringstream &iss)
{
	int ProductHead = gettok(iss);
	if (ProductHead <= _the_tail_token_type)
	{
		GrammerError();
		return false;
	}
	Product Prod;
	do
	{
		if (gettok(iss) != GrammerBegin) { GrammerError(); return false; }
		if (ParseRuleLine(iss, Prod) == false) return false;
		int tmp = gettok(iss);
		if (tmp == GrammerEnd) {
			break;
		} else if (tmp == OtherProduct) {
			continue;
		} else {
			GrammerError();
			return false;
		}
	} while (1);
	map<int, Product>::iterator gph = GrammerMap.find(ProductHead);
	if (gph == GrammerMap.end())
	{
		GrammerMap[ProductHead] = Prod;
	}
	else
	{
		gph->second.insert(gph->second.end(), Prod.begin(), Prod.end());
	}
	return true;
}

int main()
{
	istringstream iss(Grammer);
	int End = gettok(iss);
	while (End != EOF || !iss.eof())
	{
		ungettok(End);
		if (ParseProduct(iss) == false)
			return 1;
	}
	return 0;
}