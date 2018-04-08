// Program : 'Sub' is to do p1 <-- p1 - p2
// Author  : Avatar
// Date    : 98.04.07

#define LENGTH 33
#define FULL  0xffffffff
#define RMASK 0x00000001

void sub(unsigned long p1[], unsigned long p2[])
{
   unsigned long len, carry, *pp1, *pp2;

   carry = 0;
   pp1=p1+1;   pp2=p2+1;
   for (len=*p1; len>0; len--)
   {
      if ( (*pp1)>=(*pp2) )
      {
         (*pp1)-=(*pp2);
         if ( (*pp1==0x0)&&(carry==0x1))
            *pp1=FULL;
         else
         {
            (*pp1)-=carry;   carry=0x0;
         }
      }
      else
      {
         (*pp1)=FULL-(*pp2)+(*pp1)+RMASK-carry;
         carry = RMASK;
      }
      pp1++;   pp2++;
   }
   pp1=p1+(*p1);
   while (((*pp1)==0)&&(pp1>p1))
   {
      (*p1)--;   pp1--;
   }
}