// Program : 'Sub01' is to do shift right 1 bit
// Author  : Avatar
// Date    : 98.05.04

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#define LENGTH 5
#define FULL  0xffffffff

void hexout(unsigned long *num);
void bigrand(unsigned int bits, unsigned long *sl);
void Sub(unsigned long *p1, unsigned long *p2);

void main(void)
{
   clrscr();
   randomize();
   printf("Procedure 'Sub' is to do ma <-- ma - mb.\n\n");

   unsigned long *p1 = new unsigned long [LENGTH+1];
   printf("\nP1: ");   bigrand(64, p1);   hexout(p1);

   unsigned long *p2 = new unsigned long [LENGTH+1];
   printf("\nP2: ");   bigrand(65, p2);   hexout(p2);

   Sub(p1, p2);
   printf("P1 - P2: ");   hexout(p1);
   getch();
}

