// Program : 'Mod' is to do ma <-- ma(mod mb)
// Author  : Avatar
// Date    : 98.04.07

#define LENGTH 33

void shiftleft(unsigned long sl[]);
void shiftright(unsigned long rl[]);
void sub(unsigned long p1[], unsigned long p2[]);
int compare(unsigned long p1[], unsigned long p2[]);

void mod(unsigned long ma[], unsigned long mb[])
{
   unsigned long *pmb = new unsigned long [LENGTH+1];
   unsigned long t1, *ptr;

   if (compare(ma, mb)>0)
   {
      // Get a number pmb that is the largest multiple of mb less then ma.
      for (t1=1; t1<=LENGTH; t1++)
         pmb[t1] = 0;

      ptr=mb;
      for (t1=0; t1<=LENGTH; t1++, ptr++)
         pmb[t1] = *ptr;

      while (compare(ma, pmb)>0)
         shiftleft(pmb);
      while (compare(ma, mb)>0)
      {
         while (compare(ma, pmb)<0)
            shiftright(pmb);
         sub(ma, pmb);
      }
   }
   if (compare(ma, mb)==0)
      for (t1=0; t1<=LENGTH; t1++, ma++)
         *ma=0;
}