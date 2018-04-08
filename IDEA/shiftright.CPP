// Program : 'ShiftRight' is to do shift right 1 bit
// Author  : Avatar
// Date    : 98.05.04

#include <stdio.h>
#define RMASK 0x00000001
#define LMASK 0x80000000

void hexout(unsigned long *num);
void bigrand(unsigned int bits, unsigned long *sl);

void shiftright(unsigned long *sr)
{
   unsigned long tmp, carry = 0;

   for (int i=(*sr); i>0; i--) {
      tmp = sr[i]&RMASK;
      sr[i] = (sr[i]>>1) | carry;
      if (tmp!=0)
         carry = LMASK;
      else
         carry = 0;
   }
   if (sr[*sr]==0) sr[0]--;
}
