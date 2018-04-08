// Program : 'Sub01' is to do p1 <-- p1 - p2
// Author  : Avatar
// Date    : 98.04.07

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#define BITS      32
#define MAXBITS 1024
#define LENGTH    33
#define FULL  0xffffffff
#define LMASK 0x80000000
#define RMASK 0x00000001

void hexout(unsigned long num[]);
void shiftright(unsigned long rl[]);
void sub(unsigned long p1[], unsigned long p2[]);
void bigrand(unsigned int bits, unsigned long lint[]);

void main(void)
{
   clrscr();
   randomize();
   printf("Procedure 'Sub' is to do pl <-- p1 - p2.");

   int bits;
   printf("\n\nKey-in bits of Long Number: ");
   scanf("%d", &bits);
   if (bits<=0)
      bits = BITS;
   else if (bits>MAXBITS)
      bits = MAXBITS;

   unsigned long *lint1 = new unsigned long[LENGTH+1];
   bigrand(bits, lint1);
   printf("\n\nNumber 1: ");
   hexout(lint1);

   unsigned long *lint2 = new unsigned long[LENGTH+1];
   bigrand(bits, lint2);
   printf("\nNumber 2: ");
   hexout(lint2);

   sub(lint1, lint2);
   printf("\nSub: ");
   hexout(lint1);

   getch();
}


