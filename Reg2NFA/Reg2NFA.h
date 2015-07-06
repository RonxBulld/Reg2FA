#pragma once
#include <cstdio>
#include "ENFA.h"
using namespace std;


#define __DEBUG__ 1

#if __DEBUG__
#	define TRACE(s, ...) (fprintf(stdout, (s), ##__VA_ARGS__));
#else
#	define TRACE(s, ...) ;
#endif

