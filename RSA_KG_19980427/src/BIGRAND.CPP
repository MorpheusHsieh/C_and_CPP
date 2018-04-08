// Program : Generate a ranrom integer
// Author  : Avatar
// Date    : 98.04.03

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#define BMSK 0xffff
#define LENGTH 33

void bigrand(unsigned int bits, unsigned long bin[])
{
   unsigned int BitFlag;
   unsigned int MaxRange, rdl;
   unsigned long *bptr, rp, lp;

   if ( (bits%32)!=0 )
   {
      BitFlag = 1;
      MaxRange=(1<<(bits%32));
      rdl = (bits/32)+1;
   }
   else if ( (bits%32)==0 )
   {
      BitFlag = 0;
      MaxRange = BMSK;
      rdl = (bits/32);
   }
   *bin = rdl;

   // Clear bin[] is 0
   bptr = bin + 1;
   for (int i=1; i<LENGTH; i++, bptr++)
      *bptr = 0;

   bptr = *bin + bin;
   for (; rdl>0; rdl--, bptr--)
   {
      if ( BitFlag==1 )
      {
         while (*bptr==0) {
            *bptr = random(MaxRange);
         }
         MaxRange = BMSK;
         BitFlag = 0;
      }
      else if ( BitFlag==0 )
      {
         rp = random(MaxRange);
         lp = random(MaxRange);
         *bptr = (lp<<16)|rp;
      }
   }
   bptr = bin +(*bin);
   while (bptr==0){
      (*bin)--; bptr--;
   }

}