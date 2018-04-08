// Program : 'Inverse' is to get ivb such that iva*ivb = 1 mod ivn,
//                     When Gcd(iva, ivn) = 1.
// Author  : Avatar
// Date    : 98.04.14

#define LENGTH 33

int compare(unsigned long *p1, unsigned long *p2);
void add(unsigned long *p1, unsigned long *p2);
void sub(unsigned long *p1, unsigned long *p2);
void multiply(unsigned long *mua, unsigned long *mub, unsigned long *muc);
void division(unsigned long *da, unsigned long *db, unsigned long *dq, unsigned long *dr);

void inverse(unsigned long *iva, unsigned long *ivn, unsigned long *ivb)
{
   unsigned long *y = new unsigned long [LENGTH+1];
   unsigned long *g0 = new unsigned long [LENGTH+1];
   unsigned long *g1 = new unsigned long [LENGTH+1];
   unsigned long *g2 = new unsigned long [LENGTH+1];
   unsigned long *v0 = new unsigned long [LENGTH+1];
   unsigned long *v1 = new unsigned long [LENGTH+1];
   unsigned long *v2 = new unsigned long [LENGTH+1];
   unsigned long *piv0, *piv1;
   int ivcnt, ivrec;
   piv0=ivn;   piv1=iva;
   for (ivcnt=0; ivcnt<=LENGTH; ivcnt++, piv0++, piv1++)
   {
      g0[ivcnt] = (*piv0);
      g1[ivcnt] = (*piv1);
      v0[ivcnt] = 0x0;
      v1[ivcnt] = 0x0;
   }
   v1[0]=1;   v1[1]=1;
   while (g1[0]!=0)
   {
      division(g0,g1,y,g2);
      ivrec=g1[0];
      for (ivcnt=0; ivcnt<=ivrec; ivcnt++)
      {
         g0[ivcnt]=g1[ivcnt];
         g1[ivcnt]=g2[ivcnt];
      }
      multiply(y,v1,v2);
      while (compare(v0,v2)<0) add(v0,ivn);
      sub(v0,v2);
      for (ivcnt=0; ivcnt<=LENGTH; ivcnt++)
      {
         v2[ivcnt]=v0[ivcnt];
         v0[ivcnt]=v1[ivcnt];
         v1[ivcnt]=v2[ivcnt];
      }
   }
   for (ivcnt=0; ivcnt<=LENGTH; ivcnt++, ivb++)
      *ivb = v0[ivcnt];
}