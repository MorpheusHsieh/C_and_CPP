// Program : 'Getkeys' is to generate a pairs key
// Author  : Avatar
// Date    : 98.04.16

#include <math.h>
#include <time.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LENGTH 33
#define MinKeyLen   32
#define MaxKeyLen 1024
#define FULL  0xffffffff
#define RMASK 0x00000001
#define LMASK 0x80000000

typedef unsigned long LINT[34];
int count, cpass;
char uname[20];
FILE *fptr1, *fptr2, *fopen();
LINT up, uq, un, ue, udp, udq;
clock_t time1, time2;

void getkeys(void);

void main(void)
{
   clrscr();
   randomize();
   getkeys();
}



