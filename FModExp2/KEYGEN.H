//Key128b.h

#ifndef _Key128b
#define _Key128b

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ModExp.h"

void _fastcall bigrand(unsigned int ,unsigned long *);
int  _fastcall primetest(unsigned long *);
void _fastcall multiply(unsigned long *,unsigned long *,unsigned long *);
void _fastcall division(unsigned long *,unsigned long *,unsigned long *,unsigned long *);
void _fastcall gcd(unsigned long *,unsigned long *,unsigned long *);
void _fastcall inverse(unsigned long *,unsigned long *,unsigned long *);
void _fastcall getprime(unsigned int ,unsigned long *);
void _fastcall level_prime(unsigned int ,unsigned long *);
void _fastcall strongprime(unsigned int ,unsigned long *);
//extern void _fastcall _genkeys128b(char*,char *);
extern int  _fastcall _genkeys(int keylen, char* pkey,char *mkey);

#endif
