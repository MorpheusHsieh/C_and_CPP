#include "ModExp.h"
#include "KeyGen.h"

/* Procedure "bigrand" is to get a random integer with rdl*32 bits . */
// rdl is the length of *bin
void _fastcall bigrand(unsigned int rdl,unsigned long *bin)
{
    unsigned long lp,rp,bcnt,*bip;
    //unsigned int bmsk;

    if (rdl > LENGTH) abort(); // robin added

    bip=bin+1;
    //bmsk=0xffff;
    for (bcnt=1;bcnt<=LENGTH;bcnt++,bip++) *bip=0;
    *bin=rdl;
    bip=bin+1;
    for (bcnt=1;bcnt<=(*bin);bcnt++,bip++)
    {
      //lp=random(bmsk);  rp=random(bmsk);
      lp=rand();  rp=rand();
      *bip=(lp<<16)|rp;
    }
    bip=bin+(*bin);
    while(*bip==0) {  (*bin)--; bip--;  }
}

/*   Procedure "primetest" is to test whether an integer pa is a prime  */
/* or not by an error probability lower than 10**(-15).                 */
/* If flag=0 then pa is a composite integer,or pa is a prime.           */
int _fastcall primetest(unsigned long *pa)
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
    //cpass=ptest;
    return flag;
}

/* Procedure "multiply" is to do muc <-- mua*mub. */
void _fastcall multiply(unsigned long *mua,unsigned long *mub,unsigned long *muc)
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
void _fastcall division(unsigned long *da,unsigned long *db,unsigned long *dq,unsigned long *dr)
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
void _fastcall gcd(unsigned long *ga,unsigned long *gb,unsigned long *gc)
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
void _fastcall inverse(unsigned long *iva,unsigned long *ivn,unsigned long *ivb)
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

/* Procedure "getprime" is to get a prime number pn with length of plen. */
void _fastcall getprime(unsigned int plen,unsigned long *pn)
{
    LINT n210,nl;
    unsigned long *ppn,gptmp;
    int isget,gpcnt,plen1, plen2;
    int p210[48]=
        {1,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,
         83,89,97,101,103,107,109,113,121,127,131,137,139,143,149,151,157,
         163,167,169,173,179,181,187,191,193,197,199,209
        };

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
    { n210[gpcnt]=0;  nl[gpcnt]=(*ppn);  }
    n210[0]=1;  n210[1]=210;
    mod(nl,n210);
    plen1=nl[1];  gpcnt=0;
    while(plen1>p210[gpcnt]) gpcnt++;
    if(plen1<p210[gpcnt])
    { n210[1]=p210[gpcnt]-nl[1];  add(pn,n210);  }
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
}

/* Procedure "level_prime" is to get a strong prime spn. */
void _fastcall level_prime(unsigned int splen,unsigned long *spn)
{
    LINT pt,ps,pr,prs,n_tmp;
    unsigned long *psp;
    //unsigned long preg;
    int spcnt,spget,tplen,tmp1,tmp2;

    for (spcnt=2;spcnt<=LENGTH;spcnt++) n_tmp[spcnt]=0x0;
    n_tmp[0]=1;  n_tmp[1]=1;

    tplen=(int) (log((double) splen)+2);
    spcnt=(splen-tplen)/2;
    getprime(spcnt,ps);
    //fprintf(fp," %d     s1\n",count);
    //for (tmp1=0;tmp1<=LENGTH;tmp1++) fprintf(fp," %lx",ps[tmp1]);
    //fprintf(fp,"\n");

    spcnt=splen-tplen-spcnt;
    getprime(spcnt,pr);
    //fprintf(fp," %d     r1\n",count);
    //for (tmp1=0;tmp1<=LENGTH;tmp1++) fprintf(fp," %lx",pr[tmp1]);
    //fprintf(fp,"\n");

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
//    count=spcnt;
    psp=spn;
    for (spcnt=0;spcnt<=LENGTH;spcnt++,psp++) *psp=pt[spcnt];
}

/* Procedure "strongprime" is to get a strong prime spn. */
void _fastcall strongprime(unsigned int splen,unsigned long *spn)
{
    LINT pt,ps,pr,prs,n_tmp;
    unsigned long *psp;
    //unsigned long *psp,preg;
    int spcnt,spget,tplen,tmp1,tmp2;

    for (spcnt=2;spcnt<=LENGTH;spcnt++) n_tmp[spcnt]=0x0;
    n_tmp[0]=1;  n_tmp[1]=1;

    tplen=(int) (log((double) splen)+3);   /* log: nature-log(ln) */
    spcnt=(splen-tplen)/2;
    level_prime(spcnt,ps);
    //fprintf(fp," %d     p1\n",count);
    //for (tmp1=0;tmp1<=LENGTH;tmp1++) fprintf(fp," %lx",ps[tmp1]);
    //fprintf(fp,"\n");


    spcnt=splen-tplen-spcnt;
    level_prime(spcnt,pr);
    //fprintf(fp," %d     p2\n",count);
    //for (tmp1=0;tmp1<=LENGTH;tmp1++) fprintf(fp," %lx",pr[tmp1]);
    //fprintf(fp,"\n");


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
//    count=spcnt;
    psp=spn;
    for (spcnt=0;spcnt<=LENGTH;spcnt++,psp++) *psp=pt[spcnt];
}

/* Procedure "hexout" is to print an large integer with the size   */
/* from 0 to 544 bits.                                             */
/*
void _fastcall hexout(unsigned long *num)
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
*/

/*
void _fastcall _genkeys128b(char* pkey,char *mkey)
{
    //char uans;
    int  i,j,utmp,cpass;
    //long upsn;
    LINT n_one,n_tmp;

    //randomize();
    srand( (unsigned)time( NULL ) );

    //for (i=0;i<=LENGTH;i++) up[i]=0;
    //for (i=0;i<=LENGTH;i++) uq[i]=0;
    //for (i=0;i<=LENGTH;i++) un[i]=0;
    //for (i=0;i<=LENGTH;i++) ue[i]=0;
    //for (i=0;i<=LENGTH;i++) ud[i]=0;
    //for (i=0;i<=LENGTH;i++) phi_n[i]=0;
    //for (i=0;i<=LENGTH;i++) n_one[i]=0;
    //for (i=0;i<=LENGTH;i++) n_tmp[i]=0;

    for (i=0;i<=5;i++)
    { up[i]=0;
      uq[i]=0;
      un[i]=0;
      ue[i]=0;
      ud[i]=0;
      phi_n[i]=0;
      n_one[i]=0;
      n_tmp[i]=0;
    }

    //time1=clock();

    utmp=64;  strongprime(utmp,up);
    utmp=65;  strongprime(utmp,uq);
    n_one[0]=1;  n_one[1]=1;
    for (utmp=2;utmp<=5;utmp++)  n_one[utmp]=0;

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
    multiply(up,uq,phi_n);      //phi_n=(p-1)(q-1)

    inverse(ue,phi_n,ud);

    add(up,n_one);

    add(uq,n_one);

    multiply(up,uq,un);

    //time2=clock();

    j=0;
    for (utmp=0;utmp<=5;utmp++)
        j+=sprintf(pkey+j,"%08.8x %08.8x ",ue[utmp],un[utmp]);

    j=0;
    for (utmp=0;utmp<=5;utmp++)
        j+=sprintf(mkey+j,"%08.8x %08.8x %08.8x %08.8x ",
                   up[utmp],uq[utmp],ud[utmp],un[utmp]);

}
*/

// Return the Key Size in Bytes
int _fastcall _genkeys(int keylen, char* pkey,char *mkey)
{
    LINT up,uq,un,ue,ud,phi_n;

    //char uans;
    int  i,j,utmp,cpass;
    //long upsn;
    LINT n_one,n_tmp;

    //randomize();
    srand( (unsigned)time( NULL ) );

    for (i=0;i<=LENGTH;i++) up[i]=0;
    for (i=0;i<=LENGTH;i++) uq[i]=0;
    for (i=0;i<=LENGTH;i++) un[i]=0;
    for (i=0;i<=LENGTH;i++) ue[i]=0;
    for (i=0;i<=LENGTH;i++) ud[i]=0;
    for (i=0;i<=LENGTH;i++) phi_n[i]=0;
    for (i=0;i<=LENGTH;i++) n_one[i]=0;
    for (i=0;i<=LENGTH;i++) n_tmp[i]=0;

    keylen = keylen - (keylen%32);
    if (keylen<32) keylen = 32;
    else if (keylen>1024) keylen = 1024;
    //utmp=64 ;  strongprime(utmp,up);
    //utmp=65;  strongprime(utmp,uq);
    utmp=(keylen>>1);   strongprime(utmp, up);
    utmp=(keylen>>1)+1; strongprime(utmp, uq);

    n_one[0]=1;  n_one[1]=1;

    for (utmp=2;utmp<=LENGTH;utmp++)  n_one[utmp]=0;
    sub(up,n_one);  sub(uq,n_one);
    cpass=0;
    utmp=1;  bigrand(utmp,ue);
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
    // Gen OK!

    j=0;
    for (utmp=0;utmp<=LENGTH;utmp++)
        j+=sprintf(pkey+j,"%08.8x %08.8x ",ue[utmp],un[utmp]);

    j=0;
    for (utmp=0;utmp<=LENGTH;utmp++)
        j+=sprintf(mkey+j,"%08.8x %08.8x %08.8x %08.8x ",
                   up[utmp],uq[utmp],ud[utmp],un[utmp]);

}

//ÁÂ¿³§»
/*
int _fastcall _genkeys(int keylen, char* pkey,char *mkey)
{
   char uans, fn1[20], fn2[20];
   int utmp, keylen, uu, uu1,i,j;
   unsigned long upsn;
   LINT n_one, n_tmp;

   keylen = keylen - (keylen%32);
   if (keylen<32)
      keylen = 32;
   else if (keylen>1024)
      keylen = 1024;

   //time1=clock();
   utmp=(keylen>>1);   strongprime(utmp, up);
   utmp=(keylen>>1)+1; strongprime(utmp, uq);
   n_one[0]=1;   n_one[1]=1;
   for (utmp=2; utmp<=LENGTH; utmp++)
      n_one[utmp] = 0;
   sub(up, n_one);   sub(uq, n_one);
   cpass = 0;
   utmp = 1;
   bigrand(utmp, ue);
   while ( cpass==0 )
   {
      ue[1]|=0x1;
      gcd(up, ue, n_tmp);
      if ((n_tmp[0]==1)&&(n_tmp[1]==1))
         cpass = 1;
      if (cpass==1)
      {
         gcd(uq, ue, n_tmp);
         if ((n_tmp[0]==1)&&(n_tmp[1]==1))
            cpass = 1;
         else
            cpass = 0;
      }
      if (cpass==0)
         add(ue, n_one);
   }
   inverse(ue, up, udp);
   inverse(ue, uq, udq);
   add(up, n_one);
   add(uq, n_one);
   multiply(up, uq, un);
   //time2=clock();

   j=0;
   for (utmp=0;utmp<=LENGTH;utmp++)
       j+=sprintf(pkey+j,"%08.8x %08.8x ",ue[utmp],un[utmp]);

   j=0;
   for (utmp=0;utmp<=LENGTH;utmp++)
        j+=sprintf(mkey+j,"%08.8x %08.8x %08.8x %08.8x ",
                   up[utmp],uq[utmp],udp[utmp],udq[utmp]);
   //printf("\n\n  e = "); hexout(ue);
   //printf("  n = "); hexout(un);
   //printf("  p = "); hexout(up);
   //printf("  q = "); hexout(uq);
   //printf(" dp = "); hexout(udp);
   //printf(" dq = "); hexout(udq);
   //printf("\n The time was: %f minutes.\n", (time2-time1)/(60.0*CLK_TCK));

   //for (utmp=0; utmp<=LENGTH; utmp++)
   //{
   // printf("%lx %lx ", ue[utmp], un[utmp]);
   //   fprintf(fp, "%lx %lx ", ue[utmp], un[utmp]);
   //}
   //fclose(fp);

   //upsn+=1234;
   // printf("%lx %lx %lx %lx %lx ", up[0],uq[0],udp[0],udq[0],upsn);
   //fprintf(fp1, "%lx %lx %lx %lx %lx ", up[0],uq[0],udp[0],udq[0],upsn);
   //for (utmp=1; utmp<=LENGTH; utmp++)
   //{
   //   if (utmp<=up[0]) up[utmp]+=upsn;
   //   if (utmp<=uq[0]) uq[utmp]-=upsn;
   //   if (utmp<=udp[0]) udp[utmp]+=upsn;
   //   if (utmp<=udq[0]) udq[utmp]-=upsn;
   //   printf("%lx %lx %lx %lx ",up[utmp],uq[utmp],udp[utmp],udq[utmp]);
   //   fprintf(fp1,"%lx %lx %lx %lx ",up[utmp],uq[utmp],udp[utmp],udq[utmp]);
   //}
   //fclose(fp1);
   //printf("\n\nPress any key to continue...");
   //getch();
}
*/

