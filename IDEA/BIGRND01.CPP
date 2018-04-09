// Program : Generate a ranrom integer
// Author  : Morpheus
// Date    : 98.04.03

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#define LENGTH 33
#define MAXBITS 1024

void bigrand(unsigned int bits, unsigned long bin[])
{
   unsigned int MaxRange, rdl;
   unsigned long *bptr;

   MaxRange=(1<<bits);
   rdl = (bits/32)+1;
   *bin = rdl;
   bptr = bin + 1;

   // Clear bin[] is 0
   for (int i=1; i<LENGTH; i++, bptr++)
      *bptr = 0;

   bptr = bin + 1;
   for (int i=1; i<=rdl; i++, bptr++)
      *bptr = random(MaxRange);

   bptr = bin +(*bin);
   while (bptr==0){
      (*bin)--; bptr--;
   }

}

void main(void)
{
   clrscr();
   randomize();
   printf("Generate a unsigned long integer.\n");

   unsigned int bits, time;
   unsigned long *ULInt = new unsigned long[2];
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
      for (int i=0; i<=LENGTH; i++, ULInt++)
         printf("UNInt[%d]: %d.\t", i, *ULInt);
   }
   getch();
}

