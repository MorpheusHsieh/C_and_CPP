// Program : 'Sub' is to do ma <-- ma - mb
// Author  : Avatar
// Date    : 98.05.05

#define FULL  0xffffffff

void sub(unsigned long *p1, unsigned long *p2)
{
   int i, len, carry = 0;
   if (*p1>=*p2)
      len = (*p1);
   else
      len = (*p2);

   for (i=1; i<=len; i++) {
      if (p1[i]>=p2[i]) {
         p1[i] = p1[i] - p2[i];
         if ( (p1[i]==0x0)&&(carry==1) ) {
            p1[i]=FULL;
            carry = 1;
         }
         else {
            p1[i] -= carry;
            carry = 0;
         }
      }
      else {
         p1[i] = p1[i] + (FULL - p2[i]) - carry;
         carry = 1;
      }
   }

   if (carry==1) {
      p1[i] = FULL - p2[i];
      (*p1) = len;
   }

   while (p1[len]==0) {
      len--; p1[0]--;
   }
}
