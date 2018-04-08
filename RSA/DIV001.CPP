// Program : 'Division' is to get dq & dr such that da/db = dq...dr
// Author  : Avatar
// Date    : 98.03.15
// Filename: Div001.cpp

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LENGTH 17
#define FULL  0xffffffff
#define RMASK 0x00000001

void bigrand(unsigned int rdl, unsigned long *lint);
void shiftleft(unsigned long *sl);
void shiftright(unsigned long *rl);
int compare(unsigned long *p1, unsigned long *p2);
void sub(unsigned long *p1, unsigned long *p2);

void division(unsigned long *da, unsigned long *db, unsigned long *dq, unsigned long*dr)
{
   int i;
   unsigned long d2, *dp1, *dp2, *dp3;
   unsigned long *pdb = new unsigned long [LENGTH+1];

   if ( compare(da,db)>=0 )
   {
      dp1 = da; dp2 = dq; dp3 = dr;
      for (i=0; i<=LENGTH; i++)
      {
         pdb[i] = 0;
         dq[i] = 0;
         dr[i] = da[i];                 // Set dp3 is residuum
         dp1++; dp2++; dp3++;
      }

      // get a number pdb that is the largest multiple of db less than ma.
      d2 = (*da);
      pdb[0] = (*da);
      dp1 = db + (*db);
      for (i=*db; i>0; i--, d2--, dp1--)
         pdb[d2] = (*dp1);

      while ( compare(dr, pdb)>0 )      // Comapre rediuum and divident, if rediuum largest than divident, divident shiftleft
         shiftleft(pdb);
      while ( compare(dr, pdb)<0 )
         shiftright(pdb);
      dp2 = dq + 1;

      while ( (compare(db, dr)<=0) || (compare(db, pdb)<=0) )
      {
         if ( compare(dr, pdb)>=0 )
         {
            sub(dr, pdb);
            shiftright(pdb);
            shiftleft(dq);
            (*dp2)++;
            if ( (*dq)==0 )
               (*dq) = 1;
         }
         else
         {
            shiftright(pdb);
            shiftleft(dq);
         }
      }
   }
   else if ( compare(da, db)<0 )
   {
      for (i=0; i<=LENGTH; i++)
      {
         dq[i] = 0;
         dr[i] = da[i];
      }
   }
   *dq = *da;
}

void main(void)
{
   clrscr();
   randomize();

   int bits;
   printf("Procedure Division is to do da/db = dq..dr.\n");
   printf("\nKey-in bits of number: ");
   scanf("%d", &bits);
   unsigned int rdl = bits/32;


   unsigned long *da = new unsigned long[LENGTH+1];
   bigrand(rdl, da);
   printf("\nMa:");
   for (int i=(*da); i>0; i--)
      printf("%010lu ", da[i]);

   unsigned long *db = new unsigned long[LENGTH+1];
   bigrand(rdl, db);
   printf("\nMb:");
   for (int i=(*db); i>0; i--)
      printf("%010lu ", db[i]);

   unsigned long *dq = new unsigned long [LENGTH+1];
   unsigned long *dr = new unsigned long [LENGTH+1];
   division(da, db, dq, dr);

   printf("\n\nLength : %d", *da);
   printf("\n\nda    : ");
   for (int i=(*da); i>0; i--)
      printf("%010lu ", da[i]);
   printf("\ndb      : ");
   for (int i=(*db); i>0; i--)
      printf("%010lu ", db[i]);
   printf("\nQuotient: ");
   for (int i=(*dq); i>0; i--)
      printf("%010lu ", dq[i]);
   printf("\nResiduum: ");
   for (int i=(*dr); i>0; i--)
      printf("%010lu ", dr[i]);
   getch();
}






