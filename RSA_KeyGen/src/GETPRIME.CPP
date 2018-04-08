// Program : 'Getprime' is to get a prime number pn with length of plen
// Author  : Avatar
// Date    : 98.04.16

#define LENGTH 33
#define FULL  0xffffffff
#define RMASK 0x00000001
#define LMASK 0x80000000

int count;

void bigrand(unsigned int bits, unsigned long *lint);
int compare(unsigned long *p1, unsigned long *p2);
void modexp(unsigned long *ex, unsigned long *ev, unsigned long *ep, unsigned long *ew);
void modmul(unsigned long *mx, unsigned long *my, unsigned long *mp, unsigned long *mz);
void shiftleft(unsigned long *sl);
void shiftright(unsigned long *rl);
void add(unsigned long *p1, unsigned long *p2);
void sub(unsigned long *p1, unsigned long *p2);
void mod(unsigned long *ma, unsigned long *mb);
int primetest(unsigned long *pa);

void getprime(unsigned int plen, unsigned long *pn)
{
   unsigned long *n1 = new unsigned long [LENGTH+1];
   unsigned long *n210 = new unsigned long [LENGTH+1];
   unsigned long *ppn, gptmp;
   unsigned int isget, gpcnt, plen1, plen2;
   int p210[48] = {   1,  11,  13,  17,  19,  23,  29,  31,  37,  41,
                     43,  47,  53,  59,  61,  67,  71,  73,  79,  83,
                     89,  97, 101, 103, 107, 109, 113, 121, 127, 131,
                    137, 139, 143, 149, 151, 157, 163, 167, 169, 173,
                    179, 181, 187, 191, 193, 197, 199, 209 };
   plen1 = plen/32;   plen2 = plen%32;
   if ( plen2==0 ) {
      plen1--;      plen2 = 32;
   }
   plen1++;
   bigrand(plen, pn);
   ppn = pn + plen1;
   plen2 = 32 - plen2;
   gptmp = FULL >> plen2;  (*ppn)&=gptmp;
   gptmp = LMASK >> plen2; (*ppn)|=gptmp;
   ppn = pn + 1;           (*ppn)|=0x1;

   ppn = pn;
   for (gpcnt=0; gpcnt<=LENGTH; gpcnt++, ppn++)
   {
      n210[gpcnt] = 0;
      n1[gpcnt] = (*ppn);
   }
   n210[0] = 1;   n210[1] = 210;
   mod(n1, n210);
   plen1 = n1[1];   gpcnt = 0;
   while ( plen1>p210[gpcnt] )
      gpcnt++;
   if ( plen1<p210[gpcnt] )
   {
      n210[1] = p210[gpcnt] - n1[1];
      add(pn, n210);
   }
   n1[1] = p210[gpcnt];

   isget = 0;   plen2 = 0;
   while ( isget==0 )
   {
      isget = primetest(pn);
      plen2++;
      if ( isget==0 )
      {
         gptmp = p210[gpcnt];
         gpcnt++;
         if ( gpcnt==48 )
         {
            n1[1] = 2;    gpcnt = 0;
         }
         else
         {
            n1[1] = p210[gpcnt] - gptmp;
         }
         add(pn, n1);
      }
   }
   count = plen2;
}


