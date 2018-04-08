// Program : 'Modmul02' is to do mz <-- mx*my (mod mp)
// Author  : Avatar
// Date    : 98.04.013

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

#define BITS      32
#define MAXBITS 1024
#define LENGTH    34
#define LMASK 0x80000000

void hexout(unsigned long num[]);
void bigrand(unsigned int bits, unsigned long lint[]);
void modmul(unsigned long mx[], unsigned long my[], unsigned long mp[], unsigned long mz[]);

void main(void)
{
   clrscr();
   randomize();
   printf("Procedure 'Modmul' is to do mz <-- mx*my (mod mp).");

   int bits;
   printf("\n\nKey-in bits of Long Number: ");
   scanf("%d", &bits);
   if (bits<=0)
      bits = BITS;
   else if (bits>MAXBITS)
      bits = MAXBITS;

   unsigned long *mx = new unsigned long[LENGTH];
   bigrand(bits, mx);
   printf("\n\nMx: ");
   hexout(mx);

   unsigned long *my = new unsigned long[LENGTH];
   bigrand(bits, my);
   printf("My: ");
   hexout(my);

   unsigned long *mp = new unsigned long [LENGTH];
   bigrand(bits, mp);
   printf("Mp: ");
   hexout(mp);

   unsigned long *mz = new unsigned long [LENGTH];
   modmul(mx, my, mp, mz);
   printf("Mz = ");
   hexout(mz);

   getch();
}


