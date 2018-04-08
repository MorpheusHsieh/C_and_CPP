// Program : 'Modmul' is to do mz <-- mx*my (mod mp)
// Author  : Avatar
// Date    : 98.04.13

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

#define LENGTH    33
#define LMASK 0x80000000

void hexout(unsigned long *num);

void shiftleft(unsigned long sl[]);
void add(unsigned long p1[], unsigned long p2[]);
void sub(unsigned long p1[], unsigned long p2[]);
void mod(unsigned long ma[], unsigned long mb[]);
int compare(unsigned long p1[], unsigned long p2[]);

void modmul(unsigned long mx[], unsigned long my[], unsigned long mp[], unsigned long mz[])
{
   unsigned long *pmx = new unsigned long [LENGTH+1];
   unsigned long *pmy = new unsigned long [LENGTH+1];
   unsigned long *pmz = new unsigned long [LENGTH+1];
   unsigned long m1, m2,a1, a2, *ptr1, *ptr2;

   ptr1 = mx;   ptr2 = my;
   for (m1=0; m1<=LENGTH; m1++)
   {
      pmx[m1] = *ptr1;
      pmy[m1] = *ptr2;
      pmz[m1] = 0x0;
      ptr1++;   ptr2++;
   }
   if (compare(pmx,mp)>0) mod(pmx,mp);
   if (compare(pmy,mp)>0) mod(pmy,mp);
   for (m1=pmy[0]; m1>0; m1--)
   {
      a1 = LMASK;
      for (m2=32; m2>0; m2--)
      {
         shiftleft(pmz);
         a2 = pmy[m1]&a1;
         if (a2!=0) add(pmz, pmx);
         while (compare(pmz,mp)>0) sub(pmz,mp);
/*         if (a2!=0) {
            printf("\nM2: %d,\tA2: %d,\t",m2, a2);
            hexout(pmz);
         }*/
         a1>>=1;
      }
      mod(pmz,mp);
   }
   for (m1=0; m1<=LENGTH; m1++, mz++) *mz=pmz[m1];
}