// Program : Generate a ranrom integer
// Author  : Avatar
// Date    : 98.04.03

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#define LENGTH 34
#define MAXBITS 1024

void bigrand(unsigned int bits, unsigned long bin[]);

void main(void)
{
   clrscr();
   randomize();
   printf("Generate a unsigned long integer.\n");

   unsigned int bits, time;
   unsigned long *cnt;
   unsigned long *ULInt = new unsigned long[LENGTH];
   printf("\nEnter run times: ");
   scanf("%d", &time);
   do {
      printf("\nPlease key-in the bits of integer (0-1024): ");
      scanf("%d", &bits);
   } while ( (bits<1)||(bits>MAXBITS) );

   for (; time>0; time--)
   {
      bigrand(bits, ULInt);
      printf("\n");
      printf("Length: %d", *ULInt);
      printf("\nNumber: ");
      cnt = *ULInt + ULInt;
      for (; cnt>ULInt; cnt--)
         printf("%08lx ", *cnt);
   }
   getch();
}

