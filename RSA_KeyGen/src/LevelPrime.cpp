// Program : 'Level prime' is to get a strong prime spn
// Author  : Avatar
// Date    : 98.04.17

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define LENGTH 33
#define FULL  0xffffffff
#define RMASK 0x00000001
#define LMASK 0x80000000

int count;

void hexout(unsigned long *num);
void getprime(unsigned int plen, unsigned long *pn);
void inverse(unsigned long *iva, unsigned long *ivn, unsigned long *ivb);
void shiftleft(unsigned long *sl);
void multiply(unsigned long *mua, unsigned long *mub, unsigned long *muc);
void sub(unsigned long *p1, unsigned long *p2);
void division(unsigned long *da, unsigned long *db, unsigned long *dq, unsigned long*dr);
int compare(unsigned long *p1, unsigned long *p2);
void add(unsigned long *p1, unsigned long *p2);
int primetest(unsigned long *pa);

void levelprime(unsigned int splen, unsigned long *spn)
{
   unsigned long *pt = new unsigned long [LENGTH+1];
   unsigned long *ps = new unsigned long [LENGTH+1];
   unsigned long *pr = new unsigned long [LENGTH+1];
   unsigned long *prs = new unsigned long [LENGTH+1];
   unsigned long *n_tmp = new unsigned long [LENGTH+1];
   unsigned long *psp;
   int spcnt, tplen, spget, tmp1, tmp2;
   int SmallPrime[4] = { 2, 3, 5, 7};

   for (spcnt=2; spcnt<=LENGTH; spcnt++)
      n_tmp[spcnt] = 0x0;
   n_tmp[0] = 1;   n_tmp[1] = 1;

   tplen = (int)(log((double)(0.693*splen))/log(2.0) );
   spcnt = (splen-tplen)/2;
   if (spcnt<=3) {
      int rnd = random(4);
      for (int i=0; i<=LENGTH; i++)
         ps[i] = 0x0;
      ps[0]=1; ps[1] = SmallPrime[rnd];
   }
   else
      getprime(spcnt, ps);
//   printf("\nS1: ");   hexout(ps);

   spcnt = (splen-tplen-spcnt)-1;
   do {
      if (spcnt<=3) {
         int rnd = random(4);
         for (int i=0; i<=LENGTH; i++)
            pr[i] = 0x0;
         pr[0]=1; pr[1] = SmallPrime[rnd];
      }
      else
         getprime(spcnt, pr);
   } while (compare(pr,ps)==0);
//   printf("\nr1: ");   hexout(pr);

   inverse(ps, pr, pt);                   // pt = ps^-1
   shiftleft(pt);                         // pt = 2(ps^-1)
   multiply(ps, pt, pt);                  // pt = 2(ps^-1)ps
   sub(pt, n_tmp);                        // pt = 2ps*(ps^1)-1
//   printf("\n2ss^(-1)-1: ");   decout(pt);
   multiply(pr, ps, prs);                 // prs = pr*ps
   shiftleft(prs);                        // prs = 2pr*ps
//   printf("\n2rs: ");   decout(prs);

   for (spcnt=1; spcnt<=LENGTH; spcnt++)
      pr[spcnt] = 0;
   tmp1 = splen%32;
   if (tmp1==0) {                        // pr[] = 2^(L-1)
      pr[0] = splen/32;
      tmp2 = pr[0];
      pr[tmp2] = LMASK;
   }
   else {
      pr[0] = splen/32+1;
      tmp2 = pr[0];
      pr[tmp2] = RMASK;
      pr[tmp2] <<=(tmp1-1);
   }

   division(pr, prs, ps, n_tmp);        // (2^(L-1)/2pr*ps)=Q..R, Q=ps, R=n_tmp
   multiply(ps, prs, pr);               // pr = 2Qpr*ps
   if ( compare(pt, n_tmp)<0 ) {
      add(pt, pr);   add(pt, prs);      // p = 2ps(ps^-1)+(Q+1)pr*ps
   }
   else
      add(pt, pr);                      // p = 2ps(ps^-1)+Q*pr*ps

   spget = 0;
   while ( spget==0 ) {
      spget = primetest(pt);
      if (spget==0) add(pt, prs);
   }
   psp = spn;
   for (spcnt=0; spcnt<=LENGTH; spcnt++, psp++)
      *psp = pt[spcnt];
}


