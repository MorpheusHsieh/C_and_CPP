// Program : 'SR01' is to do shift right 1 bit
// Author  : Avatar
// Date    : 98.05.04

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#define LENGTH 5
#define RMASK 0x80000000
#define LMASK 0x00000001

void hexout(unsigned long *num);
void bigrand(unsigned int bits, unsigned long *sl);
void shiftright(unsigned long *sr);

void main(void)
{
   clrscr();
   randomize();
   printf("Procedure 'ShiftRight' is to do shift right 1 bit.\n\n");

   unsigned long *sr = new unsigned long [LENGTH+1];
   printf("\nSR: ");   bigrand(65, sr);   hexout(sr);
   for (int j=0; j<34; j++) {
      shiftright(sr);
      printf("SR[%lx]: ", *sr);   hexout(sr);
      getch();
   }
   getch();
}

