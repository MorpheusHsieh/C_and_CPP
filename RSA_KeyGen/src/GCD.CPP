// Program : 'GCD' is to do gc <-- gcd(ga, gb)
// Author  : Avatar
// Date    : 98.04.14

#define LENGTH 33

void mod(unsigned long *ma, unsigned long *mb);

void gcd(unsigned long *ga, unsigned long *gb, unsigned long *gc)
{
   unsigned long *g0 = new unsigned long [LENGTH+1];
   unsigned long *g1 = new unsigned long [LENGTH+1];
   unsigned long *g2 = new unsigned long [LENGTH+1];
   int gcnt, grec;
   unsigned long *pga, *pgb;
   pga = ga;   pgb = gb;
   for (gcnt=0; gcnt<=LENGTH; gcnt++, pga++, pgb++) {
      g0[gcnt] = *pga;
      g1[gcnt] = *pgb;
   }
   while (g1[0]!=0)
   {
      mod(g0, g1);
      grec = g1[0];
      for (gcnt=0; gcnt<=grec; gcnt++) {
         g2[gcnt] = g1[gcnt];
         g1[gcnt] = g0[gcnt];
         g0[gcnt] = g2[gcnt];
      }
   }
   for (gcnt=0; gcnt<=LENGTH; gcnt++, gc++)
      *gc = g0[gcnt];
}