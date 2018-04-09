// Program: 'ADD01' is to do p1 <-- p1 + p2
// Author : Morpheus
// Date   : 98.05.03

void addition(unsigned long *p1, unsigned long *p2)
{
   unsigned long len;
   if (*p1>=*p2)
      len = *p1;
   else
      len = *p2;

   int i, carry = 0;
   unsigned long tmp;
   for (i=1; i<=len; i++) {
      tmp = p1[i];
      p1[i] = p1[i] + p2[i] + carry;
      if (p1[i]<=tmp)
         carry = 1;
      else
         carry = 0;
   }
   if (carry==1) {
      p1[i]++; p1[0]++;
   }
}
