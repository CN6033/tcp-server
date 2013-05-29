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
