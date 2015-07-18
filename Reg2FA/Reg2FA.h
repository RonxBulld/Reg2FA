#pragma once
#include <cstdio>

#ifdef _MSC_VER
#	define FOPEN(fp,name,mode) (fopen_s(&fp, name, mode))
#	define STRNCPY(dest,size,src,max) (strncpy_s(dest, size, src, max))
#else
#	define FOPEN(fp,name,mode) (fp = fopen(name, mode))
#	define STRNCPY(dest,size,src,max) (strncpy(dest, src, max))
#endif

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
