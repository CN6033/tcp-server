#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stdio.h>
#include <string.h>

static void DEBUG(const char* filename, int line){
	printf("@%s:%d\n", filename, line);
}

static void PINT(int i)
{
	printf(":%d\n", i);
}

static void PERRNO(int i)
{
	printf(":%s\n", strerror(i));
}

static void PSTR(const char* str)
{
	printf(":%s\n", str);
}

#endif
