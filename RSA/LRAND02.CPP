// Program : 'LongRand' is to get a random integer with*32 bits
// Author  : Avatar
// Date    : 98.03.16
// Filename: LongRand.cpp

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LENGTH 17
#define FULL  0xffffffff
#define RMASK 0x00000001
#define LMASK 0x80000000

void longrand(unsigned int rdl, unsigned long *bin);

void main(void)
{
   clrscr();
   randomize();

   int rdl;
   printf("'Bigrnd' is to get a random integer with rdl*32 bits");
   printf("\n\nKey-in length of integer: ");
   scanf("%d", &rdl);

   unsigned long *lint = new unsigned long[rdl+1];
   longrand(rdl, lint);
   printf("\n\nLong integer:");
   for (int i=(*lint); i>0; i--)
      printf("%010lu ", lint[i]);

   getch();
}
