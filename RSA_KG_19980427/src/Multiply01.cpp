// Program : 'Multiply01' is to do muc <-- mua * mub
// Author  : Avatar
// Date    : 98.04.13

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

#define BITS      32
#define MAXBITS 1024
#define LENGTH    33
#define LMASK 0x80000000

void decout(unsigned long num[]);
void hexout(unsigned long num[]);
void bigrand(unsigned int bits, unsigned long lint[]);

void multiply(unsigned long mua[], unsigned long mub[], unsigned long muc[]);

void main(void)
{
   clrscr();
   randomize();
   printf("Procedure 'Multiply' is to do muc <-- mua * mub.\n");

   int bits=0;
   while (bits==0)
   {
      printf("\nKey-in bits of Long Number (1-512): ");
      scanf("%d", &bits);
      if (bits<1||bits>512)
         bits=0;
   }

   if (bits<=0)
      bits = BITS;
   else if (bits>MAXBITS)
      bits = MAXBITS;

   unsigned long *mua = new unsigned long[LENGTH+1];
   bigrand(bits, mua);
   printf("\nMua: ");
   hexout(mua);
//   printf("     ");   decout(mua);

   unsigned long *mub = new unsigned long[LENGTH+1];
   bigrand(bits, mub);
   printf("\n\nMub: ");
   hexout(mub);
//   printf("     ");   decout(mub);

   unsigned long *muc = new unsigned long[LENGTH+1];
   multiply(mua, mub, muc);
   printf("\n\nmua * mub =");
   hexout(muc);
//   printf("\t    ");   decout(muc);

   getch();
}


