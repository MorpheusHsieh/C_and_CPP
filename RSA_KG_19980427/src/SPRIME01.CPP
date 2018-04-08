// Program : 'Strongeprime' is to get a strong prime spn
// Author  : Avatar
// Date    : 98.04.17
// Filename: Sprime01.cpp

#include <math.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LENGTH 33
#define FULL  0xffffffff
#define RMASK 0x00000001
#define LMASK 0x80000000

int count;

void decout(unsigned long num[]);
void hexout(unsigned long num[]);

void strongprime(unsigned int splen, unsigned long *spn);

void main(void)
{
   clrscr();
   randomize();

   unsigned int splen;
   printf("'StrongPrime' is to get a strong primespn.");
   printf("\n\nKey-in bits of number: ");
   scanf("%d", &splen);

   unsigned long *spn = new unsigned long[LENGTH];
   strongprime(splen, spn);
   printf("\n\nSPn(Hex): ");
   hexout(spn);   printf("\nSpn(Dec): ");   decout(spn);
   getch();
}






