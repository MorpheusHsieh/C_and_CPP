// Program : 'Inverse01' is to get ivb such that iva*ivb = 1 mod ivn
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
void inverse(unsigned long *iva, unsigned long *ivn, unsigned long *ivb);
void division(unsigned long *da, unsigned long *db, unsigned long *dq, unsigned long *dr);

void main(void)
{
   clrscr();
   randomize();
   printf("Procedure 'Inverse' is to get ivb such that iva*ivb = 1 mod ivn.");
   printf("\nIf Gcd(a,n)<>1 than iva /= Gcd(a,n) and ivn /= Gcd(a,n).");

   int bits;
   printf("\n\nKey-in bits of Long Number: ");
   scanf("%d", &bits);
   if (bits<=0)
      bits = BITS;
   else if (bits>MAXBITS)
      bits = MAXBITS;

   unsigned long *iva = new unsigned long[LENGTH+1];
   bigrand(bits, iva);
   printf("\n\nIva: ");
   hexout(iva);
   decout(iva);

   unsigned long *ivn = new unsigned long[LENGTH+1];
   bigrand(bits, ivn);
   printf("Ivn: ");
   hexout(ivn);
   decout(ivn);

   unsigned long *gc = new unsigned long[LENGTH+1];
   gcd(iva,ivn,gc);
   printf("Gcd(a,n): ");  hexout(gc);
   if (gc[1]!=1)
   {
      unsigned long *dq = new unsigned long[LENGTH+1];
      unsigned long *dr = new unsigned long[LENGTH+1];
      division(iva, gc, dq, dr);
      for (int i=0; i<=LENGTH; i++)
         iva[i] = dq[i];
      printf("\nNew Iva: ");
      hexout(iva);

      division(ivn, gc, dq, dr);
      for (int i=0; i<=LENGTH; i++)
         ivn[i] = dq[i];
      printf("\nNew Ivn: ");
      hexout(ivn);
   }

   unsigned long *ivb = new unsigned long [LENGTH+1];
   inverse(iva, ivn, ivb);
   printf("\nIvb = ");
   hexout(ivb);
   decout(ivb);

   getch();
}


