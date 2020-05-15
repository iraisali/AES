#include<getopt.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<string.h>
typedef unsigned char uchar;

uchar* options(int argc, char* argv[], int *N, int *decrypt, int *mode, int *verb, uchar *key);
int info(const char* format, ...);


extern int VerboseMode;
extern int createAAD;
extern int AADsize;


