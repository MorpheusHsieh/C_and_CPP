// Program : Generate a ranrom integer
// Author  : Morpheus
// Date    : 98.04.03

#include <stdlib.h>

void bigrand(unsigned int bits, unsigned long bin[])
{
   unsigned int BitFlag;
   unsigned int MaxRange;
   unsigned long rp, lp, rdl;

   if ( (bits%32)!=0 )
   {
      BitFlag = 1;
      MaxRange=(1<<(bits%32));
      rdl = (bits/32)+1;
   }
   else if ( (bits%32)==0 )
   {
      BitFlag = 0;
      MaxRange = 0xffff;
      rdl = (bits/32);
   }
   *bin = rdl;

   // Clear bin[] is 0
   for (int i=1; i<=(*bin); i++)
      bin[i] = 0;

   for (int i=rdl; i>0; i--)
   {
      if ( BitFlag==1 )
      {
         bin[i] = random(MaxRange);
         MaxRange = 0xffff;
         BitFlag = 0;
      }
      else if ( BitFlag==0 )
      {
         rp = random(MaxRange);
         lp = random(MaxRange);
         bin[i] = (lp<<16)|rp;
      }
   }
}
