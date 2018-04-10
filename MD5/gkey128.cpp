#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <math.h>
#define FULL 0xffffffff
#define LMASK 0x80000000   /* Mask the  leftest bit of a long integer.  */
#define RMASK 0x00000001   /* Mask the rightest bit of a long integer.  */
#define NNINE 0x3b9ac9ff   /* NNINE is 999999999(10^9-1).               */
#define LENGTH 5           /* The # of long integers in a large integer.*/


/*   The data structure of a large integer LINT include 6 unsigned long
   integers:the first LINT[0] is the length of itself,the others show
   this large integer from LINT[1] to LINT[5](from low to high).       */

typedef unsigned long LINT[6];
int count,cpass;
char uname[20],fmat[24];
FILE *fp,*fp1,*fopen();
LINT up,uq,un,ue,ud,phi_n;
clock_t time1,time2;

/* Procedure "compare" is to compare two integers :  */
/* If p1>p2 then flag=1; */
/* If p1=p2 then flag=0; */
/* If p1<p2 then flag=-1.*/
int compare(unsigned long *p1,unsigned long *p2)
  {
    long j;
    int flag=0;
    if (*p1>*p2) flag=1;
      else if (*p1<*p2) flag=-1;
        else
        {
          j=*p1;
          p1=p1+(*p1);
          p2=p2+(*p2);
          while ((flag==0) && (j>0))
          {
            if (*p1>*p2) flag=1;
              else if (*p1<*p2) flag=-1;
            j--;p1--;p2--;
          }
        }
    return flag;
  }


/* Procedure "shiftleft" is to shift 1 bit left of an integer sl. */
/* This procedure is also means that sl <-- sl*2.                 */
void shiftleft(unsigned long *sl)
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
void shiftright(unsigned long *rl)
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
void add(unsigned long *p1,unsigned long *p2)
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
void sub(unsigned long *p1,unsigned long *p2)
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
    }
    pp1=p1+(*p1);
    while(((*pp1)==0)&&(pp1>p1))  { (*p1)--;  pp1--; }
  }


/* Procedure "mod" is to do ma<--ma (mod mb). */
void mod(unsigned long *ma,unsigned long *mb)
  {
    LINT pmb;
    unsigned long t1,t2,*ptr;
    if (compare(ma,mb)>0)
    {
      /* Get a number pmb that is the largest multiple of mb less then ma.  */
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


/* Procedure "modmul" is to do mz<--mx*my (mod mp). */
void modmul(mx,my,mp,mz)
  unsigned long *mx,*my,*mp,*mz;
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
        while(compare(pmz,mp)>0) sub(pmz,mp);
        a1>>=1;
      }
    }
    for (m1=0;m1<=LENGTH;m1++,mz++) *mz=pmz[m1];
  }


/* Procedure "modexp" is to do ew<--ex**ev (mod ep) ; */
/*  and uses right-to-left binary method for exponentiation. */
void modexp(ex,ev,ep,ew)
  unsigned long *ex,*ev,*ep,*ew;
  {
    LINT pex,pev,pew;
    unsigned long e1,*ptm1,*ptm2;
    count=0;
    ptm1=ev;  ptm2=ex;
    for (e1=0;e1<=LENGTH;e1++)
    {
      pev[e1]=*ptm1;
      pex[e1]=*ptm2;
      pew[e1]=0x0;
      ptm1++;  ptm2++;
    }
    pew[0]=1;  pew[1]=0x1;
    do { if ((pev[1]&RMASK)==RMASK)
           {  modmul(pex,pew,ep,pew);  count++; }
         shiftright(pev);
         if (pev[0]!=0)
           {  modmul(pex,pex,ep,pex);  count++; }
       }
    while (pev[0]!=0);
    for (e1=0;e1<=LENGTH;e1++,ew++) *ew=pew[e1];
  }


/* Procedure "bigrand" is to get a random integer with rdl*32 bits . */
void bigrand(unsigned int rdl,unsigned long *bin)
  {
    unsigned long lp,rp,bcnt,*bip;
    unsigned int bmsk;
    bip=bin+1;
    bmsk=0xffff;
    for (bcnt=1;bcnt<=LENGTH;bcnt++,bip++) *bip=0;
    *bin=rdl;
    bip=bin+1;
    for (bcnt=1;bcnt<=(*bin);bcnt++,bip++)
    {
      lp=random(bmsk);  rp=random(bmsk);
      *bip=(lp<<16)|rp;
    }
    bip=bin+(*bin);
    while(*bip==0) {  (*bin)--; bip--;  }
  }


/*   Procedure "primetest" is to test whether an integer pa is a prime  */
/* or not by an error probability lower than 10**(-15).                 */
/* If flag=0 then pa is a composite integer,or pa is a prime.           */
int primetest(unsigned long *pa)
  {
    LINT px,py;
    int flag,ptmp,ptest;
    unsigned long pcnt,pi,pk,*ppa;
    ptmp=*pa;
    ptest=0;
    flag=1;
    while ((flag!=0) && (ptest<25))
    {
      bigrand(ptmp,px);                      /* Generate a random   */
      ppa=pa+(*pa);                          /* number px such that */
      do {  if (compare(px,pa)>=0)           /* 1<px<pa & px is odd.*/
            {
              pcnt=FULL;
              while(pcnt>=(*ppa)) pcnt>>= 1;
              px[ptmp]&= pcnt;
            }
            px[1]|= RMASK;
            if(px[0]==0) px[0]=1;
         }  while (compare(px,pa)==0);
      pi=0;
      ppa=pa;
      for (pcnt=0;pcnt<=LENGTH;pcnt++,ppa++) py[pcnt]=(*ppa);
      flag=1;
      pk=0;
      if((py[1]&RMASK)==0) flag=0;   /* When the test integer is even. */
        else
        {
          py[1]--;    /*  pa=(2^pk)*py+1  */
          while((py[1]&RMASK)==0) {  shiftright(py);  pk++;  }
        }
      if (flag==1)
      {
        modexp(px,py,pa,py);
        ppa=pa;
        for (pcnt=0;pcnt<=LENGTH;pcnt++,ppa++) px[pcnt]=*ppa;
        px[1]--;
        if ((compare(px,py)==0)||((py[0]==1)&&(py[1]==1))) flag=2;
      }
      while ((flag==1) && (pi<pk))
      {
        pi++;
        modmul(py,py,pa,py);
        if ((py[0]==1) && (py[1]==1)) flag=0;
        if (compare(px,py)==0) flag=2;
      }
      if ((flag==1) && ((py[0]!=1) || (py[1]!=1))) flag=0;
      ptest++;
    }
    cpass=ptest;
    return flag;
  }


/* Procedure "multiply" is to do muc <-- mua*mub. */
void multiply(mua,mub,muc)
  unsigned long *mua,*mub,*muc;
  {
    LINT mucc,muaa,mubb;
    unsigned long mu1,mu2,ma1,ma2,*mptr1,*mptr2;
    mptr1=mua;  mptr2=mub;
    for (mu1=0;mu1<=LENGTH;mu1++)
    {
      muaa[mu1]=*mptr1;
      mubb[mu1]=*mptr2;
      mucc[mu1]=0x0;
      mptr1++;  mptr2++;
    }
    for (mu1=mubb[0];mu1>0;mu1--)
    {
      ma1=LMASK;
      for (mu2=32;mu2>0;mu2--)
      {
        shiftleft(mucc);
        ma2=mubb[mu1]&ma1;
        if (ma2!=0) add(mucc,muaa);
        ma1>>= 1;
      }
    }
    for (mu1=0;mu1<=LENGTH;mu1++,muc++) *muc=mucc[mu1];
  }


/* Procedure "division" is to get dq & dr such that da/db=dq ... dr. */
void division(da,db,dq,dr)
  unsigned long *da,*db,*dq,*dr;
  {
    LINT pdb;
    unsigned long d1,d2,*dp1,*dp2,*dp3;
    if (compare(da,db)>=0)
    {
      dp1=da;  dp2=dq;  dp3=dr;
      for (d1=0;d1<=LENGTH;d1++)
      {
        pdb[d1]=0;
        *dp2=0;
        *dp3=(*dp1);
        dp1++;  dp2++;  dp3++;
      }
      /* Get a number pdb that is the largest multiple of db less than ma. */
      d2=(*da);
      pdb[0]=(*da);
      dp1=db+(*db);
      for (d1=*db;d1>0;d1--,d2--,dp1--) pdb[d2]=(*dp1);
      while (compare(dr,pdb)>0) shiftleft(pdb);
      while (compare(dr,pdb)<0) shiftright(pdb);
      dp2=dq+1;
      while ((compare(db,dr)<=0)||(compare(db,pdb)<=0))
      {
        if (compare(dr,pdb)>=0)
        {
          sub(dr,pdb);
          shiftright(pdb);
          shiftleft(dq);
          (*dp2)++;
          if ((*dq)==0) (*dq)=1;
        }
        else
        {
          shiftright(pdb);
          shiftleft(dq);
        }
      }
    }
    else
    {
      dp1=da;  dp2=dq;   dp3=dr;
      for (d1=0;d1<=LENGTH;d1++)
      {
        (*dp2)=0;
        (*dp3)=(*dp1);
        dp1++;  dp2++;  dp3++;
      }
    }
  }


/* Procedure "gcd" is to do gc<--gcd(ga,gb). */
void gcd(unsigned long *ga,unsigned long *gb,unsigned long *gc)
  {
    LINT g0,g1,g2;
    int gcnt,grec;
    unsigned long *pga,*pgb;
    pga=ga;  pgb=gb;
    for (gcnt=0;gcnt<=LENGTH;gcnt++,pga++,pgb++)
      {  g0[gcnt]=*pga;  g1[gcnt]=*pgb;  }
    while (g1[0]!=0)
    {
      mod(g0,g1);
      grec=g1[0];
      for(gcnt=0;gcnt<=grec;gcnt++)
      {
        g2[gcnt]=g1[gcnt];
        g1[gcnt]=g0[gcnt];
        g0[gcnt]=g2[gcnt];
      }
    }
    for (gcnt=0;gcnt<=LENGTH;gcnt++,gc++) *gc=g0[gcnt];
  }


/* Procedure "inverse" is to get ivb such that iva*ivb=1 mod ivn. */
void inverse(unsigned long *iva,unsigned long *ivn,unsigned long *ivb)
  {
    LINT y,g0,g1,g2,v0,v1,v2;
    unsigned long *piv0,*piv1;
    int ivcnt,ivrec;
    piv0=ivn;  piv1=iva;
    for (ivcnt=0;ivcnt<=LENGTH;ivcnt++,piv0++,piv1++)
    {
      g0[ivcnt]=(*piv0);
      g1[ivcnt]=(*piv1);
      v0[ivcnt]=0x0;
      v1[ivcnt]=0x0;
    }
    v1[0]=1;   v1[1]=1;
    while(g1[0]!=0)
    {
      division(g0,g1,y,g2);
      ivrec=g1[0];
      for (ivcnt=0;ivcnt<=ivrec;ivcnt++)
      {
        g0[ivcnt]=g1[ivcnt];
        g1[ivcnt]=g2[ivcnt];
      }
      multiply(y,v1,v2);
      while(compare(v0,v2)<0) add(v0,ivn);
      sub(v0,v2);
      for (ivcnt=0;ivcnt<=LENGTH;ivcnt++)
      {
        v2[ivcnt]=v0[ivcnt];
        v0[ivcnt]=v1[ivcnt];
        v1[ivcnt]=v2[ivcnt];
      }
    }
    for (ivcnt=0;ivcnt<=LENGTH;ivcnt++,ivb++) *ivb=v0[ivcnt];
  }


/* Procedure "hexout" is to print an large integer with the size   */
/* from 0 to 544 bits.                                             */
void hexout(unsigned long *num)
  {
    unsigned long c1,c2,*nptr;
    if (*num==0) printf("    0    \n");
    else if (*num>LENGTH) printf("  overflow   \n");
    else
    {
      nptr=num+(*num);
      c1=*num;
      c2=0;
      while (*nptr==0)
      { nptr--;  c1--; }
      for (;c1>0;c1--)
      {
        printf(" %08lx",*nptr);
        if ((c2%8)==7) printf("\n     ");
        nptr--;
        c2++;
      }
      if ((c2%8)!=0) printf("\n\n");
        else printf("\n");
    }
  }


/* Procedure "getprime" is to get a prime number pn with length of plen. */
void getprime(unsigned int plen,unsigned long *pn)
  {
    LINT n210,nl;
    unsigned long *ppn,gptmp;
    int isget,gpcnt,plen1,plen2;
    int p210[48]={1,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,
         83,89,97,101,103,107,109,113,121,127,131,137,139,143,149,151,157,
         163,167,169,173,179,181,187,191,193,197,199,209};

    plen1=plen/32;
    plen2=plen%32;
    if (plen2==0) { plen1--;  plen2=32; }
    plen1++;
    bigrand(plen1,pn);
    ppn=pn+plen1;
    plen2=32-plen2;
    gptmp=FULL >> plen2;  (*ppn)&= gptmp;
    gptmp=LMASK >> plen2; (*ppn)|= gptmp;
    ppn=pn+1;             (*ppn)|= 0x1;

    ppn=pn;
    for (gpcnt=0;gpcnt<=LENGTH;gpcnt++,ppn++)
      {  n210[gpcnt]=0;  nl[gpcnt]=(*ppn);  }
    n210[0]=1;  n210[1]=210;
    mod(nl,n210);
    plen1=nl[1];  gpcnt=0;
    while(plen1>p210[gpcnt]) gpcnt++;
    if(plen1<p210[gpcnt])
      {  n210[1]=p210[gpcnt]-nl[1];  add(pn,n210);  }
    nl[1]=p210[gpcnt];

    isget=0;  plen2=0;
    while (isget==0)
    {
      isget=primetest(pn);
      plen2++;
      if (isget==0)
      {
        gptmp=p210[gpcnt];
        gpcnt++;
        if (gpcnt==48) {  nl[1]=2; gpcnt=0; }
          else { nl[1]=p210[gpcnt]-gptmp; }
        add(pn,nl);
      }
    }
    count=plen2;
  }



/* Procedure "level_prime" is to get a strong prime spn. */
void level_prime(unsigned int splen,unsigned long *spn)
  {
    LINT pt,ps,pr,prs,n_tmp;
    unsigned long *psp,preg;
    int spcnt,spget,tplen,tmp1,tmp2;

    for (spcnt=2;spcnt<=LENGTH;spcnt++) n_tmp[spcnt]=0x0;
    n_tmp[0]=1;  n_tmp[1]=1;

    tplen=(int) (log((double) splen)+2);
    spcnt=(splen-tplen)/2;
    getprime(spcnt,ps);
    fprintf(fp," %d     s1\n",count);
    for (tmp1=0;tmp1<=LENGTH;tmp1++) fprintf(fp," %lx",ps[tmp1]);
    fprintf(fp,"\n");

    spcnt=splen-tplen-spcnt;
    getprime(spcnt,pr);
    fprintf(fp," %d     r1\n",count);
    for (tmp1=0;tmp1<=LENGTH;tmp1++) fprintf(fp," %lx",pr[tmp1]);
    fprintf(fp,"\n");

    inverse(ps,pr,pt);      /* pt x ps= 1 ( mod pr) */
    shiftleft(pt);
    multiply(ps,pt,pt);
    sub(pt,n_tmp);          /* pt=(2ps x ps^-1)-1  */

    multiply(pr,ps,prs);
    shiftleft(prs);
    for(spcnt=1;spcnt<=LENGTH;spcnt++) pr[spcnt]=0;
    tmp1=splen%32;
    if(tmp1==0) { pr[0]=splen/32; tmp2=pr[0]; pr[tmp2]=LMASK; }
      else {  pr[0]=splen/32+1; tmp2=pr[0];
              pr[tmp2]=RMASK; pr[tmp2]<<= (tmp1-1); }
    division(pr,prs,ps,n_tmp);
    multiply(ps,prs,pr);
    if (compare(pt,n_tmp)<0) { add(pt,pr);  add(pt,prs); }
      else add(pt,pr);

    spcnt=0;  spget=0;
    while(spget==0)
    {
      spget=primetest(pt);
      spcnt++;
      if (spget==0) add(pt,prs);
    }
    count=spcnt;
    psp=spn;
    for (spcnt=0;spcnt<=LENGTH;spcnt++,psp++) *psp=pt[spcnt];
  }





/* Procedure "strongprime" is to get a strong prime spn. */
void strongprime(unsigned int splen,unsigned long *spn)
  {
    LINT pt,ps,pr,prs,n_tmp;
    unsigned long *psp,preg;
    int spcnt,spget,tplen,tmp1,tmp2;

    for (spcnt=2;spcnt<=LENGTH;spcnt++) n_tmp[spcnt]=0x0;
    n_tmp[0]=1;  n_tmp[1]=1;

    tplen=(int) (log((double) splen)+3);   /* log: nature-log(ln) */
    spcnt=(splen-tplen)/2;
    level_prime(spcnt,ps);
    fprintf(fp," %d     p1\n",count);
    for (tmp1=0;tmp1<=LENGTH;tmp1++) fprintf(fp," %lx",ps[tmp1]);
    fprintf(fp,"\n");


    spcnt=splen-tplen-spcnt;
    level_prime(spcnt,pr);
    fprintf(fp," %d     p2\n",count);
    for (tmp1=0;tmp1<=LENGTH;tmp1++) fprintf(fp," %lx",pr[tmp1]);
    fprintf(fp,"\n");


    inverse(ps,pr,pt);      /* pt x ps= 1 ( mod pr) */
    shiftleft(pt);
    multiply(ps,pt,pt);
    sub(pt,n_tmp);          /* pt=(2ps x ps^-1)-1  */

    multiply(pr,ps,prs);
    shiftleft(prs);
    for(spcnt=1;spcnt<=LENGTH;spcnt++) pr[spcnt]=0;
    tmp1=splen%32;
    if(tmp1==0) { pr[0]=splen/32; tmp2=pr[0]; pr[tmp2]=LMASK; }
      else {  pr[0]=splen/32+1; tmp2=pr[0];
              pr[tmp2]=RMASK; pr[tmp2]<<= (tmp1-1); }
    division(pr,prs,ps,n_tmp);
    multiply(ps,prs,pr);
    if (compare(pt,n_tmp)<0) { add(pt,pr);  add(pt,prs); }
      else add(pt,pr);

    spcnt=0;  spget=0;
    while(spget==0)
    {
      spget=primetest(pt);
      spcnt++;
      if (spget==0) add(pt,prs);
    }
    count=spcnt;
    psp=spn;
    for (spcnt=0;spcnt<=LENGTH;spcnt++,psp++) *psp=pt[spcnt];
  }


void getkeys(void)
  {
    char uans,fn1[20],fn2[20];
    int utmp;
    long upsn;
    LINT n_one,n_tmp;

    printf("Please input your login name(1-8 characters & no space):");
    scanf("%s",uname);

    strcpy(fn1,uname);    strcat(fn1,".pub");
    strcpy(fn2,uname);    strcat(fn2,".sct");

    printf("\n\nNow generate the system keys, wait for about 15 minutes!!\n");

    time1=clock();
    utmp=64;  strongprime(utmp,up);
    utmp=65;  strongprime(utmp,uq);
    n_one[0]=1;  n_one[1]=1;
    for (utmp=2;utmp<=LENGTH;utmp++)  n_one[utmp]=0;

 sub(up,n_one);  sub(uq,n_one);
     cpass=0;
     utmp=1;    bigrand(utmp,ue);

     while(cpass==0)
      {
       ue[1]|=0x1;

       gcd(up,ue,n_tmp);

       if((n_tmp[0]==1) && (n_tmp[1]==1)) cpass=1;
       if (cpass==1)
       {
	 gcd(uq,ue,n_tmp);
	 if((n_tmp[0]==1) && (n_tmp[1]==1)) cpass=1;
	   else cpass=0;
       }
       if (cpass==0) add(ue,n_one);
      }
     multiply(up,uq,phi_n);      /* phi_n=(p-1)(q-1) */

     inverse(ue,phi_n,ud);

     add(up,n_one);

     add(uq,n_one);

     multiply(up,uq,un);

     time2=clock();

     fp=fopen(fn1,"w");  fp1=fopen(fn2,"w");

     if((fp==NULL) || (fp1==NULL))
     {
       printf("\n\n There are some error in disk writing, ");
       printf(" press any key to exit this system.\n");
       getch();
       exit(1);
     }
     printf(" e = ");  hexout(ue);
     printf(" n = ");  hexout(un);
     printf(" p = ");  hexout(up);
     printf(" q = ");  hexout(uq);
     printf(" d = ");  hexout(ud);
     printf("\n  The time was: %f minutes.\n",(time2-time1)/(60.0*CLK_TCK));

     for (utmp=0;utmp<=LENGTH;utmp++)
       fprintf(fp,"%lx %lx ",ue[utmp],un[utmp]);
     fclose(fp);

     for (utmp=0;utmp<=LENGTH;utmp++)
       fprintf(fp1,"%lx %lx %lx %lx ",up[utmp],uq[utmp],ud[utmp],un[utmp]);
     fclose(fp1);

     printf("\nPress any key to continue!\n");
     getch();
  }



void main(void)
{
  clrscr();
  randomize();
  getkeys();
}

