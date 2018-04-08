// Program : 'Primetest' is to test whether an integer pa is a prime or not
//                       by an error probability lower than 10^(-15)
//           if flag=0 then pa is a composite integer, else pa is a prime.
// Author  : Avatar
// Date    : 98.04.16

#define LENGTH 33
#define FULL  0xffffffff
#define RMASK 0x00000001
#define LMASK 0x80000000

//int cpass;

void bigrand(unsigned int bits, unsigned long *lint);
int compare(unsigned long *p1, unsigned long *p2);
void modexp(unsigned long *ex, unsigned long *ev, unsigned long *ep, unsigned long *ew);
void modmul(unsigned long *mx, unsigned long *my, unsigned long *mp, unsigned long *mz);
void shiftleft(unsigned long *sl);
void shiftright(unsigned long *rl);
void add(unsigned long *p1, unsigned long *p2);
void sub(unsigned long *p1, unsigned long *p2);
void mod(unsigned long *ma, unsigned long *mb);

int primetest(unsigned long *pa)
{
   if ((pa[1]==1)||(pa[1]==2))
      return 1;

   unsigned long *px = new unsigned long [LENGTH+1];
   unsigned long *py = new unsigned long [LENGTH+1];
   int flag, ptmp, ptest;
   unsigned long pcnt, pi, pk, *ppa;

   // bits of big random number count
   unsigned long *pn = new unsigned long [LENGTH+1];
   for (int i=0;i<=LENGTH; i++)
      pn[i] = pa[i];
   int bits = 0;
   while (pn[1]!=0) {
      bits++;
      shiftright(pn);
   }
//   printf("\Bits: %d", bits);
//   getch();

   ptmp = bits%32;
   if (ptmp==0)
      ptmp = (bits/32);
   else if (ptmp!=0)
      ptmp = (bits/32)+1;

   ptest = 0;   flag = 1;
   while ( (flag!=0) && (ptest<25) )
   {
      bigrand(bits, px);          // Generate a random, bits
      ppa = pa + (*pa);           // number px such that 1<px<pa & px is odd
      do {
         if ( compare(px,pa)>=0 )
         {
            pcnt = FULL;
            while ( pcnt>=(*ppa) )
               pcnt>>=1;
            px[ptmp]&=pcnt;
         }
         px[1]|=RMASK;
         if ( px[0]==0 )
            px[0] = 1;
      } while ( compare(px,pa)==0 );

      pi = 0;   ppa = pa;
      for (pcnt=0; pcnt<=LENGTH; pcnt++, ppa++)
         py[pcnt] = (*ppa);
      flag = 1;
      pk = 0;

      if ( (py[1]&RMASK)==0 )        // When the test integer is even
         flag = 0;
      else
      {
         py[1]--;                    // pa = (2^pk)*py+1
         while ( (py[1]&RMASK)==0 )
         {
            shiftright(py);   pk++;
         }
      }

      if ( flag==1 )
      {
         modexp(px,py,pa,py);
         ppa = pa;
         for (pcnt=0; pcnt<=LENGTH; pcnt++, ppa++)
            px[pcnt] = *ppa;
         px[1]--;
         if ( (compare(px,py)==0) || ((py[0]==1)&&(py[1]==1)) )
            flag = 2;
      }
      while ( (flag==1)&&(pi<pk) )
      {
         pi++;
         modmul(py, py, pa, py);
         if ( (py[0]==1) && (py[1]==1) )
            flag = 0;
         if (compare(px,py)==0)
            flag = 2;
      }
      if ( (flag==1) && ((py[0]!=1)||(py[1]!=1)) )
         flag = 0;
      ptest++;
   }
//   cpass = ptest;
   return flag;
}