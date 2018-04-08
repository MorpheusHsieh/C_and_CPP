// Program : 'ShiftRight01' is to shift 1 bit Right of an integer rl
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
void bigrand(unsigned int bits, unsigned long lint[]);

void main(void)
{
   clrscr();
   randomize();
   printf("\nProcedure 'ShiftRight' is to rl <- rl/2.");

   unsigned long *lint1 = new unsigned long[LENGTH+1];
   bigrand(BITS, lint1);
   printf("\n\nNumber 1: ");
   hexout(lint1);

   shiftright(lint1);
   printf("\nShiftRight over.\nNumber: ");
   hexout(lint1);

   getch();
}


