// Program : 'Hexout' is print number use Hex format
// Author  : Avatar
// Date    : 98.04.07

#include <stdio.h>
#define LENGTH 33

void hexout(unsigned long num[])
{
   unsigned long c1, c2;
   unsigned long *nptr;

   if ( *num==0 )
      printf("      0     \n");
   else if ( *num>LENGTH )
      printf("  overflow  \n");
   else
   {
      nptr = num + *num;
      c1 = *num;
      c2 = 0;

      // Search start number isn't zero
      while ( *nptr==0 ) {
         nptr--; c1--;
      }

      // Display number from high bit to low bit
      for (; c1>0; c1--)
      {
         printf(" %08lx", *nptr);
         if ( (c1%8)==7)
            printf("\n ");
         nptr--;
         c2++;
      }
      if ( (c2%8)!=0 )
         printf("\n\n");
      else
         printf("\n");
   }
}