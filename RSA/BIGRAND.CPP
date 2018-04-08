// Program : Generate a large random number
// Author  : Avatar
// Date    : 98.03.12
// Filename: Bigrand.cpp

#include <stdio.h>
#include <stdlib.h>
#define LENGTH 17

void bigrand(unsigned int rdl, unsigned long *bin)
{
   unsigned long lp, rp, bcnt, *bip;
   unsigned int bmsk;
   bip=bin+1;
   bmsk=0xffff;
   for (bcnt=1; bcnt<=LENGTH; bcnt++, bip++)
      *bip=0;
   *bin=rdl;
   bip=bin+1;
   for (bcnt=1; bcnt<=(*bin); bcnt++, bip++)
   {
      lp=random(bmsk);
      rp=random(bmsk);
      *bip=(lp<<16)|rp;
   }
   bip=bin+(*bin);
   while (*bip==0)
   {
      (*bin)--;
      bip--;
   }

/*   unsigned long MaxRange = 0xffff;
   unsigned long lp, rp;

   for (int i=LENGTH; i>0; i--)
      lint[i] = 0x0;

   *lint = rdl;
   lint = lint + (*lint);
   while ( rdl>0 )
   {
      lp = random(MaxRange);
      rp = random(MaxRange);
      *lint = (lp<<16)|rp;
      rdl --; lint --;
   }*/
}

