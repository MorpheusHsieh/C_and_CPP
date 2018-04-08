// Program : 'ShiftRight' is to shift 1 bit Right of an integer rl
// Author  : Avatar
// Date    : 98.04.07

#define LMASK 0x80000000
#define RMASK 0x00000001

void shiftright(unsigned long rl[])
{
   unsigned long b1, b2, *prl;
   long cnt;

   b1 = 0x0;
   prl = rl + (*rl);
   for (cnt=*rl; cnt>0; cnt--)
   {
      b2=(*prl)&RMASK;             // RMASK is 0x00000001
      *prl=((*prl)>>1)|b1;
      if ( b2==0x0 )
         b1 = 0x0;
      else
         b1 = LMASK;
      prl--;
   }
   prl = rl + (*rl);
   if ((*prl)==0 )
      (*rl)--;
}