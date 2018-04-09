// Program : Generate a ranrom integer
// Author  : Morpheus
// Date    : 98.04.03

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#define LENGTH 33
#define MAXBITS 1024

void hexout(unsigned long *num);
void bigrand(unsigned int bits, unsigned long bin[]);

void main(void)
{
   clrscr();
   randomize();
   printf("Generate a unsigned long integer.\n");

   unsigned int bits, time;
   unsigned long *ULInt = new unsigned long[LENGTH+1];
   printf("\nEnter run times: ");
   scanf("%d", &time);
   do {
      printf("\nPlease key-in the bits of integer (1-1024): ");
      scanf("%d", &bits);
   } while ( (bits<1)||(bits>MAXBITS) );

   for (; time>0; time--)
   {
      bigrand(bits, ULInt);
      printf("\nNumber: ");  hexout(ULInt);
   }
   getch();
}

