// Program : 'mod' is to do ma <-- ma mod mb
// Author  : Morpheus
// Date    : 98.05.07

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#define LENGTH 5

void shiftleft(unsigned long *sl);
void shiftright(unsigned long *sr);
void sub(unsigned long *p1, unsigned long *p2);
int compare(unsigned long *p1, unsigned long *p2);

void Mod(unsigned long *ma, unsigned long *mb)
{
   unsigned long *pmb = new unsigned long [LENGTH+1];
   for (int i=0; i<=LENGTH; i++) pmb[i]=mb[i];

   if (compare(ma,mb)>0)
   {
      while (compare(ma,pmb)>0)
         shiftleft(pmb);

      while (compare(ma,mb)>0)
      {
         while(compare(ma,pmb)<0)
            shiftright(pmb);
         sub(ma,pmb);
      }
   }
   else if (compare(ma,mb)==0)
   {
      for (int i=1; i<=LENGTH; i++)
         ma[i]=0;
      ma[0] = 1;
   }
}