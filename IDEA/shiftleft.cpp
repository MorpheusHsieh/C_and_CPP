// Program : 'ShiftLeft' is to do shift 1 bit left
// Author  : Avatar
// Date    : 98.05.04

#include <conio.h>
#include <stdio.h>
#define LMASK 0x80000000
#define RMASK 0x00000001

void shiftleft(unsigned long *sl)
{
   int i;
   unsigned long tmp, carry = 0;
   for (i=1; i<=(*sl); i++)
   {
      tmp = sl[i]&LMASK;
      sl[i] = (sl[i]<<1) | carry;
      if (tmp!=0)
         carry = RMASK;
      else
         carry = 0;
   }
   if (carry!=0) {
      sl[0]++;
   }
}
