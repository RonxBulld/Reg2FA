#pragma once
#include <cstdio>

#define __DEBUG__ 1
#define MAX_CHARACTER 128

#include "ENFA.h"
#include "DFA.h"
using namespace std;

#if __DEBUG__
#	define TRACE(s, ...) (fprintf(stderr, (s), ##__VA_ARGS__))
#else
#	define TRACE(s, ...) 
#endif
