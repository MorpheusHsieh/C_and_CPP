// Program : 'GCD' is to do gc <-- gcd(ga, gb)
// Author  : Avatar
// Date    : 98.04.14

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

#define BITS      32
#define MAXBITS 1024
#define LENGTH    33

void decout(unsigned long num[]);
void hexout(unsigned long num[]);
void bigrand(unsigned int bits, unsigned long lint[]);
void gcd(unsigned long *ga, unsigned long *gb, unsigned long *gc);

void main(void)
{
   clrscr();
   randomize();
   printf("Procedure 'GCD' is to do gc <-- gcd(ga, gb).");

   int bits;
   printf("\n\nKey-in bits of Long Number: ");
   scanf("%d", &bits);
   if (bits<=0)
      bits = BITS;
   else if (bits>MAXBITS)
      bits = MAXBITS;

   unsigned long *ga = new unsigned long[LENGTH+1];
   bigrand(bits, ga);
   printf("\n\nGa: ");
   hexout(ga);
   decout(ga);

   unsigned long *gb = new unsigned long[LENGTH+1];
   bigrand(bits, gb);
   printf("Gb: ");
   hexout(gb);
   decout(gb);

   unsigned long *gc = new unsigned long [LENGTH];
   gcd(ga, gb, gc);
   printf("Gc = ");
   hexout(gc);
   decout(gc);

   getch();
}


