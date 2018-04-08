////modexp.cpp

#include "ModExp.h"

// Procedure "compare" is to compare two integers :
// If p1>p2 then flag= 1;
// If p1=p2 then flag= 0;
// If p1<p2 then flag=-1;
int _fastcall compare(unsigned long *p1,unsigned long *p2)
{
	long j;
	int flag=0;

	if (*p1>*p2)   flag=1;
    else if (*p1<*p2)   flag=-1;
	else
	{
		j=*p1;
		p1=p1+(*p1);
		p2=p2+(*p2);
		while ((flag==0) && (j>0))
		{
		  if (*p1>*p2) flag=1;
		  else if (*p1<*p2) flag=-1;
		  j--; p1--; p2--;
		}
	}
    return flag;
}


/* Procedure "shiftleft" is to shift 1 bit left of an integer sl. */
/* This procedure is also means that sl <-- sl*2.                 */
void _fastcall shiftleft(unsigned long *sl)
{
   unsigned long b1,b2,*psl;
   long cnt;
   b1=0x0;
   psl=sl+1;
   for (cnt=*sl;cnt>0;cnt--)
   {
	 b2=(*psl)&LMASK;
	 *psl=((*psl)<<1)|b1;
	 if (b2==0x0) b1=0x0;
     else b1=RMASK;
	 psl++;
   }
   if (b1!=0x0)  { (*psl)=1; (*sl)++; }
}


/* Procedure "shiftright" is to shift 1 bit right of an integer rl. */
/* This procedure is also means that rl <-- rl/2.                   */
void _fastcall shiftright(unsigned long *rl)
{
  unsigned long b1,b2,*prl;
  long cnt;
  b1=0x0;
  prl=rl+(*rl);
  for (cnt=*rl;cnt>0;cnt--)
  {
    b2=*prl&RMASK;
    *prl=(*prl>>1)|b1;
    if (b2==0x0) b1=0x0;
    else b1=LMASK;
    prl--;
  }
  prl=rl+(*rl);
  if ((*prl)==0) (*rl)--;
}


/* Procedure "add" is to do p1 <-- p1+p2.  */
void _fastcall add(unsigned long *p1,unsigned long *p2)
{
  unsigned long len,carry,*pp1,*pp2;

  if (*p1>*p2) len=*p1;
  else len=*p2;

  carry=0;   *p1=len;
  pp1=p1+1;  pp2=p2+1;
  for (;len>0;len--)
  {
    *pp1=(*pp1)+(*pp2)+carry;
	if ((*pp1)<(*pp2)) carry=1;
	else carry=0;
	pp1++;  pp2++;
  }
  if (carry==1) { (*p1)++;  (*pp1)++; }
}


/* Procedure "sub" is to do p1<--p1-p2, where p1>p2. */
void _fastcall sub(unsigned long *p1,unsigned long *p2)
{
      unsigned long len,borrow,*pp1,*pp2;
      borrow=0;
      pp1=p1+1;  pp2=p2+1;
      for (len=*p1;len>0;len--)
	  {
	     if ((*pp1)>=(*pp2))
	     {
		    (*pp1)-= (*pp2);
		    if ((*pp1==0x0) && (borrow==0x1)) *pp1=FULL;
		    else { (*pp1)-= borrow;  borrow=0x0; }
	     }
	     else
	     {
		    (*pp1)=FULL-(*pp2)+(*pp1)+RMASK-borrow;
		    borrow=RMASK;
	     }
	     pp1++;  pp2++;
      }//for

      pp1=p1+(*p1);
      while(((*pp1)==0)&&(pp1>p1))  { (*p1)--;  pp1--; }
}


/* Procedure "mod" is to do ma<--ma (mod mb). */

void _fastcall mod(unsigned long *ma,unsigned long *mb)
{  
   LINT pmb;
   unsigned long t1,*ptr;


   if (compare(ma,mb)>0)
   {
     // Get a number pmb that is the largest multiple of mb less then ma.
	for (t1=1;t1<=LENGTH;t1++) pmb[t1]=0;
	ptr=mb;
	for (t1=0;t1<=LENGTH;t1++,ptr++) pmb[t1]=*ptr;
	while (compare(ma,pmb)>0) shiftleft(pmb);
	while (compare(ma,mb)>0)
	{
		while (compare(ma,pmb)<0) shiftright(pmb);
		sub(ma,pmb);
	  }
    }
    if (compare(ma,mb)==0)
       for (t1=0;t1<=LENGTH;t1++,ma++) *ma=0;
}


// Procedure "modmul" is to do mz<--mx*my (mod mp).

void _fastcall modmul(unsigned long *mx,unsigned long *my,unsigned long *mp,unsigned long *mz)
{   
    LINT pmx,pmy,pmz;
    unsigned long m1,m2,a1,a2,*ptr1,*ptr2;

    ptr1=mx;   ptr2=my;
    for (m1=0;m1<=LENGTH;m1++)
    {
	    pmx[m1]=*ptr1;
	    pmy[m1]=*ptr2;
	    pmz[m1]=0x0;
	    ptr1++;  ptr2++;
	}
    if (compare(pmx,mp)>0) mod(pmx,mp);
    for (m1=pmy[0];m1>0;m1--)
	{
	   a1=LMASK;
	   for (m2=32;m2>0;m2--)
	    {
		  shiftleft(pmz);
		  a2=pmy[m1]&a1;
		  if (a2!=0) add(pmz,pmx);
		  while(compare(pmz,mp)>0)  sub(pmz,mp);
		  a1>>=1;
	    }
    }
    for (m1=0;m1<=LENGTH;m1++,mz++) *mz=pmz[m1];
}


/* Procedure "modexp" is to do ew<--ex**ev (mod ep) ; */
/*  and uses right-to-left binary method for exponentiation. */

void _fastcall modexp(unsigned long *ex,unsigned long *ev,unsigned long *ep,unsigned long *ew)
{
    //int count=0;
    LINT pex,pev,pew;
    unsigned long e1,*ptm1,*ptm2;
    //count=0;
    ptm1=ev;  ptm2=ex;
    for (e1=0;e1<=LENGTH;e1++)
    {
	   pev[e1]=*ptm1;
	   pex[e1]=*ptm2;
	   pew[e1]=0x0;
	   ptm1++;  ptm2++;
	}
    pew[0]=1;  pew[1]=0x1;
    do
    { if ((pev[1]&RMASK)==RMASK)
	  { modmul(pex,pew,ep,pew);
        //count++;
      }
	  shiftright(pev);
	  if (pev[0]!=0)
      { modmul(pex,pex,ep,pex);
        //count++;
      }
	} while (pev[0]!=0);

    for (e1=0;e1<=LENGTH;e1++)
    {  *ew=pew[e1];
       ew++;
    }
}

