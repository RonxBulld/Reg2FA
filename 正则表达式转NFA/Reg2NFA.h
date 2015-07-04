#pragma once
#include <cstdio>
#include <map>
#include <set>
#include <deque>
using namespace std;

#define LINK_OPERATOR '\0'
#define EMPTY_TRANSFORM '\0'

#define __DEBUG__ 1

#if __DEBUG__
#	define TRACE(s, ...) (fprintf(stdout, (s), ##__VA_ARGS__));
#else
#	define TRACE(s, ...) ;
#endif

class NFANode;
extern std::set<NFANode*> NodePo;

class NFANode
{
private:
	static int id;
public:
	int ID = 0;
	bool Final = false;
	multimap<char, NFANode*> Edges;
public:
	NFANode()
	{
		this->ID = id++;
		NodePo.insert(this);
	}
	void LinkNode(char TransChar, NFANode *DestNode)
	{
		this->Edges.insert(pair<char, NFANode*>(TransChar, DestNode));
	}
};
int NFANode::id = 0;

class NFAPack
{
public:
	NFANode *Head, *Tail;
	NFAPack(NFANode *Head, NFANode *Tail) :Head(Head), Tail(Tail) {}
};

template<typename T>
class Stack
{
private:
	deque<T> Container;
public:
	const T & top()
	{
		return this->Container.back();
	}
	void pop()
	{
		TRACE("pop %d\n", (int)top());
		this->Container.pop_back();
	}
	void push(const T & v)
	{
		TRACE("push %d\n", *(int*)&v);
		this->Container.push_back(v);
	}
	bool empty()
	{
		return this->Container.empty();
	}
};

