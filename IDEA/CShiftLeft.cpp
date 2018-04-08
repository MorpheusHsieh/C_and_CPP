// Program : 'CShiftLeft' is to do Circle shift n bits
// Author  : Avatar
// Date    : 98.05.01

#define LMASK 0x80000000

void CShiftLeft(unsigned int sbit, unsigned long *csl)
{
   long len = *csl;

   unsigned long tmp;
   while (sbit > 32) {
      tmp = csl[len];
      for (int i=len; i>1; i--) {
         csl[i] = csl[i-1];
      }
      csl[1] = tmp;
      sbit-=32;
   }

   unsigned long Mask = 0x0;
   for (int i=0; i<sbit; i++) {
      Mask = (Mask >> 1) | LMASK;
   }

   tmp = (csl[len]&Mask)>>(32-sbit);
   for (int i=len; i>1; i--) {
      csl[i] = (csl[i]<<sbit) | ((csl[i-1]&Mask)>>(32-sbit));
   }
   csl[1] = (csl[1]<<sbit)|tmp;
}