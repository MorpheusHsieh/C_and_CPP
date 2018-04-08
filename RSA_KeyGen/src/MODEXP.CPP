// Program : 'Modex' is to do ew <-- ex**ev (mod ep) and uses
//                   right-to-left binary method for exponentiation
// Author  : Avatar
// Date    : 98.04.13

#define LENGTH    33
#define LMASK 0x80000000
#define RMASK 0x00000001

int count;

void shiftright(unsigned long sr[]);
void modmul(unsigned long *mx, unsigned long *my, unsigned long *mp, unsigned long *mz);

void modexp(unsigned long *ex, unsigned long *ev, unsigned long *ep, unsigned long *ew)
{
   unsigned long *pex = new unsigned long [LENGTH+1];
   unsigned long *pev = new unsigned long [LENGTH+1];
   unsigned long *pew = new unsigned long [LENGTH+1];
   unsigned long e1, *ptm1, *ptm2;

   count = 0;
   ptm1 = ev; ptm2 =ex;
   for (e1=0; e1<=LENGTH; e1++)
   {
      pev[e1] = *ptm1;
      pex[e1] = *ptm2;
      pew[e1] = 0x0;
      ptm1++; ptm2 ++;
   }
   pew[0]=1; pew[1] = 0x1;
   do {
      if (( pev[1]&RMASK)==RMASK) {
         modmul(pex, pew, ep, pew);
         count++;
      }
      shiftright(pev);
      if (pev[0]!=0) {
         modmul(pex, pex, ep, pex);
         count++;
      }
   } while (pev[0]!=0);
   for (e1=0; e1<=LENGTH; e1++, ew++)
      *ew = pew[e1];
}