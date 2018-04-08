// program : 'Printdec'
// Author  : Avatar
// Date    : 98.04.13

#include <stdio.h>

void printdec(unsigned long num[])
{
   unsigned long c1, c2, *nptr;
   if (*num==0)
      printf("0    \n");
   else if (*num>20)
      printf("  overflow  \n");
   else
   {
      nptr = num + (*num);
      c1 = *num;
      c2 = 0;
      while (*nptr==0) {
         nptr--; c1--;
      }
      for (; c1>0; c1--)
      {
         printf("%09ld ", *nptr);
         if ((c2%7)==6)
            printf("\n    ");
         nptr--;
         c2++;
      }
      if ((c2%7)!=0)
         printf("\n");
      else
         printf(" ");
   }
}                     




