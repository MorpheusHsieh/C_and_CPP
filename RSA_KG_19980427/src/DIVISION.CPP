// Program : 'Division' is to get dq &dr such that d1/db=dq...dr
// Author  : Avatar
// Date    : 98.04.07

#define LENGTH    33

void shiftleft(unsigned long sl[]);
void shiftright(unsigned long rl[]);
void sub(unsigned long p1[], unsigned long p2[]);
int compare(unsigned long p1[], unsigned long p2[]);

void division(unsigned long da[], unsigned long db[], unsigned long dq[], unsigned long dr[])
{
   unsigned long *pdb = new unsigned long [LENGTH+1];
   unsigned long d1, d2, *dp1, *dp2, *dp3;

   if (compare(da,db)>=0)
   {
      dp1=da; dp2=dq; dp3=dr;
      for (d1=0; d1<=LENGTH; d1++)
      {
         pdb[d1]=0;
         *dp2=0;
         *dp3=(*dp1);
         dp1++; dp2++; dp3++;
      }

      // Get a number pdb that is the largest multiple of db less than ma.
      d2=(*da);
      pdb[0]=(*da);
      dp1=db+(*db);
      for (d1=*db; d1>0; d1--, d2--, dp1--)
         pdb[d2]=(*dp1);
      while (compare(dr,pdb)>0)
         shiftleft(pdb);
      while (compare(dr,pdb)<0)
         shiftright(pdb);
      dp2=dq+1;

      while ((compare(db,dr)<=0)||(compare(db,pdb)<=0))
      {
         if (compare(dr,pdb)>=0)
         {
            sub(dr,pdb);
            shiftright(pdb);
            shiftleft(dq);
            (*dp2)++;
            if ((*dq)==0)
               (*dq)=1;
         }
         else
         {
            shiftright(pdb);
            shiftleft(dq);
         }
      }
   }
   else if (compare(da,db)<0)
   {
      dp1=da; dp2=dq; dp3=dr;
      for (d1=0; d1<=LENGTH; d1++)
      {
         (*dp2)=0;
         (*dp3)=(*dp1);
         dp1++; dp2++; dp3++;
      }
   }
}