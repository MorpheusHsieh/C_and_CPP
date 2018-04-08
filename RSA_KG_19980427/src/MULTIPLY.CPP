// Program : 'Multiply' is to do muc <-- mua * mub
// Author  : Avatar
// Date    : 98.04.13

#define LENGTH 33
#define LMASK 0x80000000

void shiftleft(unsigned long sl[]);
void add(unsigned long p1[], unsigned long p2[]);

void multiply(unsigned long mua[], unsigned long mub[], unsigned long muc[])
{
   unsigned long *mucc = new unsigned long [LENGTH+1];
   unsigned long *muaa = new unsigned long [LENGTH+1];
   unsigned long *mubb = new unsigned long [LENGTH+1];
   unsigned long mu1, mu2, ma1, ma2, *mptr1, *mptr2;
   
   mptr1=mua;   mptr2=mub;
   for (mu1=0; mu1<=LENGTH; mu1++)
   {
      muaa[mu1] = *mptr1;
      mubb[mu1] = *mptr2;
      mucc[mu1] = 0x0;
      mptr1++;   mptr2++;
   }
   for (mu1=mubb[0]; mu1>0; mu1--)
   {
      ma1 = LMASK;
      for (mu2=32; mu2>0; mu2--)
      {
         shiftleft(mucc);
         ma2=mubb[mu1]&ma1;
         if (ma2!=0) add(mucc, muaa);
         ma1>>=1;
      }
   }
   for (mu1=0; mu1<=LENGTH; mu1++, muc++)
      *muc=mucc[mu1];
}