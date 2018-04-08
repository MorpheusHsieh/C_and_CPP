// Program : 'ShiftLeft' is to shift 1 bit left of an integer sl
// Author  : Avatar
// Date    : 98.04.07

#define LMASK 0x80000000
#define RMASK 0x00000001

void shiftleft(unsigned long sl[])
{
   unsigned long b1, b2, *psl;
   long cnt;

   b1 = 0x0;
   psl = sl + 1;
   for (cnt=*sl; cnt>0; cnt--)
   {
      b2=(*psl)&LMASK;                // LMASK equ 0x80000000
      *psl = ((*psl)<<1)|b1;
      if (b2==0x0)
         b1 = 0x0;
      else
         b1 = RMASK;
      psl++;
   }
   if ( b1!=0x0)
   {
      (*psl)=1;
      (*sl)++;
   }
}