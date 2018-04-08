// Program : 'Level prime01' is to get a strong prime spn
// Author  : Avatar
// Date    : 98.04.17

#include <math.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LENGTH 33
#define FULL  0xffffffff
#define RMASK 0x00000001
#define LMASK 0x80000000

void decout(unsigned long num[]);
void hexout(unsigned long num[]);

void levelprime(unsigned int splen, unsigned long *spn);

void main(void)
{
   clrscr();
   randomize();

   unsigned int splen;
   printf("'Level-2 Prime' is to get a strong prime.");
   printf("\n\nKey-in bits of number: ");
   scanf("%d", &splen);

   unsigned long *spn = new unsigned long[LENGTH+1];
   levelprime(splen, spn);
   printf("\nLevel-03 prime is ");
   hexout(spn);
   getch();
}






