// Program : Encrypt
// Author  : Avatar
// Date    : 98.03.17
// Filename: Encrypt.cpp

#include <time.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FULL  0xffffffff
#define LMASK 0x80000000
#define RMASK 0x00000001
#define NNINE 0x3b9ac9ff
#define LENGTH 17

int esread(FILE *fp, unsigned long *msg);
void eswrite(FILE *fp1, unsigned long *cpt);
void hexout(unsigned long *num);
void modexp(unsigned long *ex, unsigned long *ev, unsigned long *ep, unsigned long *ew);
void encrypt(void);

void main(void)
{
   clrscr();
   encrypt();
}

