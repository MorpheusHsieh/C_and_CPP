// Program : 'SL01' is to do shift 1 bit left
// Author  : Morpheus
// Date    : 98.05.04

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#define LENGTH 5

void hexout(unsigned long *num);
void bigrand(unsigned int bits, unsigned long *sl);
void shiftleft(unsigned long *sl);

void main(void)
{
   clrscr();
   randomize();
   printf("Procedure 'ShiftLeft' is to do shift left 1 bit.\n\n");

   unsigned long *sl = new unsigned long [LENGTH+1];
   bigrand(16, sl);
   printf("\nN1: "); hexout(sl);

   for (int i=0; i<32; i++) {
      shiftleft(sl);
      printf("\nShift over: "); hexout(sl); getch();
   }
   getch();
}


