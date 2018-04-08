// Program : Generate a random integer with rdl*32 bits.
// Author  : Avatar
// Date    : 98.03.12
// Filename: Bigrnd01.cpp

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#define LENGTH 17

void bigrand(unsigned int rdl, unsigned long *lint)
{
   unsigned long MaxRange = 0xffff;
   unsigned long lp, rp;

   *lint = rdl;
   lint = lint + (*lint);
   while ( rdl>0 )
   {
      lp = random(MaxRange);
      rp = random(MaxRange);
      *lint = (lp<<16)|rp;
      rdl --; lint --;
   }
}

void main(void)
{
   clrscr();
   randomize();

//   unsigned long *lint = new unsigned long[LENGTH+1];

   int bits;
   printf("Get a unsigned long random integer.\n");
   printf("Key-in bits of number: ");
   scanf("%d", &bits);
   unsigned int rdl = bits/32;

   unsigned long *lint = new unsigned long[LENGTH+1];
   bigrand(rdl, lint);

   printf("\nNumber: ");
   printf("Len : %d\n", *lint);
   for (int i=(*lint); i>0; i--)
      printf("%08lx ", lint[i]);
   getch();
}
