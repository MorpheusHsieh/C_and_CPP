// Program : Generate a random integer with rdl*32 bits.
// Author  : Avatar
// Date    : 98.03.12
// Filename: Bigrnd02.cpp

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#define LENGTH 17

void bigrand(unsigned int rdl, unsigned long *lint);

void main(void)
{
   clrscr();
   randomize();

   unsigned long *lint = new unsigned long[LENGTH+1];

   int bits;
   printf("Get a unsigned long random integer.\n");
   printf("Key-in bits of number: ");
   scanf("%d", &bits);
   unsigned int rdl = bits/32;

   bigrand(rdl, lint);
   printf("\nNumber: ");
   for (int i=(*lint); i>0; i--)
      printf("%08lx ", lint[i]);
   getch();
}
