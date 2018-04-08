// Program : 'ShiftLeft01' is to shift 1 bit left of an integer sl
// Author  : Avatar
// Date    : 98.04.07

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#define BITS   17
#define LENGTH 33
#define LMASK 0x80000000
#define RMASK 0x00000001

void hexout(unsigned long num[]);
void shiftleft(unsigned long sl[]);
void bigrand(unsigned int bits, unsigned long lint[]);

void main(void)
{
   clrscr();
   randomize();
   printf("\nProcedure 'ShiftLeft' is to shift 1 bit left of an integer sl.");

   unsigned long *lint1 = new unsigned long[LENGTH];
   bigrand(BITS, lint1);
   printf("\n\nNumber 1: ");
   hexout(lint1);

   shiftleft(lint1);
   printf("\nShiftLeft over.\nbumber: ");
   hexout(lint1);

   getch();
}


