// Program : 'Modexp01' is to do ew <-- ex**ev (mod ep) and uses
//                      right-to-left binary method for exponentiation
// Author  : Avatar
// Date    : 98.04.13

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

#define BITS      32
#define MAXBITS 1024
#define LENGTH    33
#define LMASK 0x80000000
#define RMASK 0x00000001

int count;

void decout(unsigned long num[]);
void hexout(unsigned long num[]);
void bigrand(unsigned int bits, unsigned long lint[]);
void modexp(unsigned long *ex, unsigned long *ev, unsigned long *ep, unsigned long *ew);

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

   unsigned long *ex = new unsigned long[LENGTH+1];
   bigrand(bits, ex);
   printf("\n\nEx: ");
   hexout(ex);
   decout(ex);

   unsigned long *ev = new unsigned long[LENGTH+1];
   bigrand(bits, ev);
   printf("Ev: ");
   hexout(ev);
   decout(ev);

   unsigned long *ep = new unsigned long [LENGTH+1];
   bigrand(bits, ep);
   printf("Ep: ");
   hexout(ep);
   decout(ep);

   unsigned long *ew = new unsigned long [LENGTH+1];
   modexp(ex, ev, ep, ew);
   printf("Ew = ");
   hexout(ew);
   decout(ew);
   getch();
}


