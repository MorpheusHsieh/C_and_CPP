// Program : 'Add01' is to do p1 <-- p1 + p2
// Author  : Avatar
// Date    : 98.04.07

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#define BITS   1024
#define LENGTH 33
#define LMASK 0x80000000
#define RMASK 0x00000001

void hexout(unsigned long num[]);
void shiftright(unsigned long rl[]);
void add(unsigned long p1[], unsigned long p2[]);
void bigrand(unsigned int bits, unsigned long lint[]);

void main(void)
{
   clrscr();
   randomize();
   printf("\nProcedure 'Add' is to do pl <-- p1 + p2.");

   unsigned long *lint1 = new unsigned long[LENGTH+1];
   bigrand(BITS, lint1);
   printf("\n\nNumber 1: ");
   hexout(lint1);

   unsigned long *lint2 = new unsigned long[LENGTH+1];
   bigrand(BITS, lint2);
   printf("\nNumber 2: ");
   hexout(lint2);

   add(lint1, lint2);
   printf("\nSum: ");
   hexout(lint1);

   getch();
}


