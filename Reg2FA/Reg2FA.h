#pragma once
#include <cstdio>
#include "ENFA.h"
using namespace std;


#define __DEBUG__ 1
#define MAX_CHARACTER 128

#if __DEBUG__
#	define TRACE(s, ...) (fprintf(stderr, (s), ##__VA_ARGS__))
#else
#	define TRACE(s, ...) 
#endif
