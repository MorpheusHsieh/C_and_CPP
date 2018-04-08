// Program : 'Mod01' is to do ma <-- ma(mod mb)
// Author  : Avatar
// Date    : 98.04.07

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

#define BITS      32
#define MAXBITS 1024
#define LENGTH    33

void hexout(unsigned long num[]);
void mod(unsigned long ma[], unsigned long mb[]);
void bigrand(unsigned int bits, unsigned long lint[]);


void main(void)
{
   clrscr();
   randomize();
   printf("Procedure 'MOD' is to do ma <-- ma(mod mb).");

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

   mod(lint1, lint2);
   printf("\nma mod mb = ");
   hexout(lint1);

   getch();
}


