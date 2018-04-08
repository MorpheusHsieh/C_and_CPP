// Program : 'Add' is to do p1 <-- p1 + p2
// Author  : Avatar
// Date    : 98.04.07

void add(unsigned long p1[], unsigned long p2[])
{
   unsigned long len, carry, *pp1, *pp2;

   if ( (*p1)>=(*p2) )
      len = *p1;
   else
      len = *p2;
      
   carry=0;  *p1=len;
   pp1=p1+1; pp2=p2+1;
   for (; len>0; len--)
   {
      *pp1=(*pp1)+(*pp2)+carry;
      if ( (*pp1)<(*pp2) )
         carry = 1;
      else
         carry = 0;
      pp1++; pp2++;
   }
   if (carry==1) {
      (*p1)++;   (*pp1)++;
   }
}