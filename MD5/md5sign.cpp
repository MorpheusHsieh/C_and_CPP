/***************************************************************************
*                                                                          *
* This is a multisignature appilcation programe.It's based on two function *
* which is modexp function and hash function-MD5, the modexp function use  *
* key 128 bits, module n 129 bits( p=64 bits,q=65 bits) and message digest *
* 128 bit.                                                                 *
* the auther is Ki-Uan Wang studing in CCIT                                *
****************************************************************************/


#include<bios.h>
#include<dos.h>
#include<time.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<conio.h>
#include<math.h>

#define FULL 0xffffffff
#define LMASK 0x80000000   /* Mask the  leftest bit of a long integer.  */
#define RMASK 0x00000001   /* Mask the rightest bit of a long integer.  */
#define NNINE 0x3b9ac9ff   /* NNINE is 999999999(10^9-1).               */
#define LENGTH 5           /* The # of long integers in a large integer.*/
#define RIGHT 0x000000ff
#define MID   0x0000ffff
#define LEFT  0x00ffffff
#define FULL  0xffffffff
#define MSBFT 0x80000000
#define BLOCK 64
#define CLEAR 0x00000000


typedef unsigned long LINT[6];
int count,cpass;
char fn1[20],fn2[20],uname[20],index[5];
FILE *fp,*fp1;
LINT up,uq,un,ue,ud;
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
void modmul(unsigned long *mx,unsigned long *my,unsigned long *mp,\
	    unsigned long *mz)
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
	while(compare(pmz,mp)>0) sub(pmz,mp);     //?
	a1>>=1;
      }
    }
    for (m1=0;m1<=LENGTH;m1++,mz++) *mz=pmz[m1];
  }


/* Procedure "modexp" is to do ew<--ex^ev (mod ep) ; */
/*  and uses right-to-left binary method for exponentiation. */
void modexp(unsigned long *ex,unsigned long *ev,unsigned long *ep,\
	    unsigned long *ew)
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
	 if (pev[0]!=0)  // if it is last bit?
	   {  modmul(pex,pex,ep,pex);  count++; }
       }  while (pev[0]!=0);
    for (e1=0;e1<=LENGTH;e1++)
       { *ew=pew[e1];	 ew++;    }
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

/*********** the document already define 128 bit *****/


void read(unsigned long *msg)
  {
    int tmp,tmp1,tmp2,tch,buf[17];
    unsigned long i,*tmsg,esl;

    tmsg=msg;
    for(tmp=0;tmp<=LENGTH;tmp++)
      {(*tmsg)=0;     tmsg++;      }
    for(i=0;i<17;i++) buf[i]=0;
    tmp=0;
    while(!feof(fp))
     {
       buf[tmp]=fgetc(fp);
       tmp++;
     }
    tmsg=msg;
    tmp=0;  tmp1=0;
    while(tmp<16)
    {
      if(tmp1==0) { esl=buf[tmp];  tmp1++;  tmsg++;  }
	else {  esl=(esl<<8)|buf[tmp];  tmp1++;  }
      if((tmp1==4)||(tmp==buf[0])) { (*tmsg)=esl;  (*msg)++;  tmp1=0;  }
      tmp++;
    }
    if(buf[tmp]==1)
    { *(tmsg+1)=buf[tmp]; (*msg)++;}
  }



void write(unsigned long *cpt)
  {
    int tmp,tmp1,tch[4];
    cpt++;
    for (tmp=1;tmp<=LENGTH;tmp++)
    {
      tch[0]=(*cpt)>>24;
      tch[1]=((*cpt)>>16)&0xff;
      tch[2]=((*cpt)>>8)&0xff;
      tch[3]=(*cpt)&0xff;
      if(tmp<LENGTH)
	{
	  for(tmp1=0;tmp1<4;tmp1++)
	  fputc(tch[tmp1],fp1);
	}
      else
	{
	  if(tch[3]!=0)
	    fputc(tch[3],fp1);
	}
      cpt++;
    }
  }

/***************************************************************************/

//* round1()
void round1(unsigned long *word,unsigned long v,int s,unsigned long t)
 {
  unsigned long f;

  f=(word[1]&word[2])|((~word[1])&word[3]);
  word[0]=(word[0]+f+v+t);
  word[0]=word[0]<<s;
 }

//* round2()
void round2(unsigned long *word, unsigned long v ,int s,unsigned long t)
 {
  unsigned long g;

  g=(word[1]&word[3])|(word[2]&~word[3]);
  word[0]=(word[0]+g+v+t);
  word[0]=word[0]<<s;
 }

//* round3()
void round3(unsigned long *word,unsigned long v,int s,unsigned long t)
 {
  unsigned long h;

  h=word[1]^word[2]^word[3];
  word[0]=(word[0]+h+v+t);
  word[0]=word[0]<<s;
 }

//* round4()
void round4(unsigned long *word,unsigned long v,int s,unsigned long t)
 {
  unsigned long i;

  i=word[1]^(word[2]&~word[3]);
  word[0]=(word[0]+i+v+t);
  word[0]=word[0]<<s;
 }

/************************************************************************/


void append(unsigned long *buffer, int ret)
{
 unsigned long rp;
 int i,count;

 count=0;
 while(ret>=4)
   {
    ret-=4;    count++;
   }

 rp=*(buffer+count);
 switch(ret)
  {
   case 0:
	  rp=0x00000001;
	  break;
   case 1:
	  rp=(rp&RIGHT)+0x00000100;        // RIGHT=0X000000ff
	  break;
   case 2:
	  rp=(rp&MID)+0x00010000;          // MID=0X0000ffff
	  break;
   case 3:
	  rp=(rp&LEFT)+0x01000000;         // LEFT=0X00ffffff
	  break;
  }
 *(buffer+count)=rp;
 for(i=count+1;i<16;i++)
   *(buffer+i)=(*(buffer+i)&CLEAR);        // CLEAR=0X00000000
}


/**************************************************************************/
void reg(unsigned long *buffer, unsigned long *p, unsigned long *q,\
	 unsigned long *s, unsigned long *t)
{
  unsigned long word[4],x[16],tw;

  int ip[4][16]={
		  {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},
		  {1,6,11,0,5,10,15,4,9,14,3,8,13,2,7,12},
		  {5,8,11,14,1,4,7,10,13,0,3,6,9,12,15,2},
		  {0,7,14,5,12,3,10,1,8,15,6,13,4,11,2,9}
		};
  int sp[4][16]={
		  {7,12,17,22,7,12,17,22,7,12,17,22,7,12,17,22},
		  {5,9,14,20,5,9,14,20,5,9,14,20,5,9,14,20 },
		  {5,8,11,14,5,8,11,14,5,8,11,14,5,8,11,14},
		  {6,10,15,21,6,10,15,21,6,10,15,21,6,10,15,21}
		};
  unsigned long ti[4][16]={
  {0xd76aa478,0xe8c7b756,0x242070db,0xc1bdceee,0xf57c0faf,0x4787c62a,\
   0xa8304613,0xfd469501,0x698098d8,0x8b44f7af,0xffff5bb1,0x895cd7be,\
   0x6b901122,0xfd987193,0xa679438e,0x49b40821},

  {0xf61e2562,0xc040b340,0x265e5a51,0xe9b6c7aa,0xd62f105d,0x02441453,\
   0xd8a1e681,0xe7d3fbc8,0x21e1cde6,0xc33707d6,0xf4d50d87,0x455a14ed,\
   0xa9e3e905,0xfcefa3f8,0x676f02d9,0x8d2a4c8a},

  {0xfffa3942,0x8771f681,0x6d9d6122,0xfde5380c,0xa4beea44,0x4bdecfa9,\
   0xf6bb4b60,0xbebfbc70,0x289b7ec6,0xeaa127fa,0xd4ef3085,0x04881d05,\
   0xd9d4d039,0xe6db99e5,0x1fa27cf8,0xc4ac5665},

  {0xf4292244,0x411aff97,0xab9423a7,0xfc93a039,0x655b59c3,0x8f0ccc92,\
   0xffeff47d,0x85845dd1,0x6fa87e4f,0xfe2ce6e0,0xa3014314,0x4e0811a1,\
   0xf7537e82,0xbd3af235,0x2ad7d2bb,0xeb86d391}
			  };

  int i,j,k;

  for(i=0;i<16;i++)
    x[i]=*(buffer+i);
  word[0]=*p;
  word[1]=*q;
  word[2]=*s;
  word[3]=*t;
  for(i=0;i<16;i++)                    //* round1()
    {
      k=ip[0][i];
      round1(word,x[k],sp[0][i],ti[0][i]);
      tw=word[3];
      for(j=3;j>=1;j--)
	word[j]=word[j-1];
      word[0]=tw;
    }

  for(i=0;i<16;i++)                    //* round2()
    {
      k=ip[1][i];
      round2(word,x[k],sp[1][i],ti[1][i]);
      tw=word[3];
      for(j=3;j>=1;j--)
	word[j]=word[j-1];
      word[0]=tw;
    }

  for(i=0;i<16;i++)                    //* round3()
    {
      k=ip[2][i];
      round3(word,x[k],sp[2][i],ti[2][i]);
      tw=word[3];
      for(j=3;j>=1;j--)
	word[j]=word[j-1];
      word[0]=tw;
    }

  for(i=0;i<16;i++)                    //* round4()
    {
      k=ip[3][i];
      round4(word,x[k],sp[3][i],ti[3][i]);
      tw=word[3];
      for(j=3;j>=1;j--)
	word[j]=word[j-1];
      word[0]=tw;
    }
  *p=word[0]+*p;    // output value of the 64step add the initial value
  *q=word[1]+*q;
  *s=word[2]+*s;
  *t=word[3]+*t;

 }


/***************************************************************************/
void md5(unsigned long length,unsigned long *msg)
 {
  unsigned long addr[16],*buffer,*tmsg;
  unsigned long carry,a[4];
  int i,j,ret,tch[16];

  tmsg=msg;
  for(i=0;i<=LENGTH;i++)   //LENGTH=5
    { (*tmsg)=0;
      tmsg++;
    }
  carry=0L;
  a[0]=0x01234567;
  a[1]=0x89abcdef;
  a[2]=0xfedcba98;
  a[3]=0x76543210;
  for(i=0;i<16;i++)
   {
    addr[i]=0L;
   }
  buffer=addr;
  do
    {
      ret=fread(buffer,sizeof(char),BLOCK,fp);
      if(ret!=BLOCK)   //BLOCK=64
	{
	   append(buffer,ret);
	   *(buffer+14)=carry;
	   *(buffer+15)=length;
	     break;
	}
      reg(buffer,&a[0],&a[1],&a[2],&a[3]);
    }while(!feof(fp));
  reg(buffer,&a[0],&a[1],&a[2],&a[3]);
  *msg=4; msg++;
  for(i=0;i<4;i++,msg++) *msg=a[3-i];
 }



/***************************************************************************/
void sign1(void)
{
  LINT msg,dtp,cpt;
  char ans,ch,fn[20];
  char fnfmt1[10]={' ','-','1','.','s','c','t'}; /*file name format*/
  FILE *fpkey;
  long utmp,ltmp;
  int  t,i,j,L;
  unsigned long length=0L;

  for(i=0;i<5;i++)
    index[i]=0;
  do{
     printf("\n\nPlease input user index :>");
     scanf(" %c",&index[0]);
     printf("\nPlease decide the next user index :>");
     scanf(" %c",&index[1]);
     printf("\nYour index is %c,and the next user index is %c!!",index[0],index[1]);
     printf("\n\nAre you sure?(press y/n!!)");
     ans=getch();
    }while(ans!='y');

  clrscr();
//key assignment
  fnfmt1[0]=index[0];
  strcpy(fn,fnfmt1);
  if((fpkey=fopen(fn,"rb"))==NULL)
   {
    printf("\n\nYour private key file isn't exist,press any key to exit!!");
    getch();  exit(1);
   }
  for(utmp=0;utmp<=LENGTH;utmp++)
    fscanf(fpkey,"%lx %lx %lx %lx",up+utmp,uq+utmp,ud+utmp,un+utmp);
  fclose(fpkey);

// filename is outside variable
  fp=fopen(fn1,"rb");   fp1=fopen(fn2,"wb");
  if((fp==NULL)||(fp1==NULL))
    {
      printf("\n\nError data filename's format. Press any key to exit!!\n");
      getch(); exit(1);
    }
  while(getc(fp)!=EOF)
     length+=8;
  fclose(fp);

  fp=fopen(fn1,"rb");    ch='>';
  fprintf(fp1,"%s %c",index,ch);
  printf("\n_______________________________________________________________");
  printf("\n---------------------------------------------------------------");
  printf("\n\nNow, process data.Waiting......!");
  md5(length,msg);    //md5:hash function
  time1=clock();
  modexp(msg,ud,un,msg);
  time2=clock();
  write(msg);
  fclose(fp);  fclose(fp1);

  printf("\n\n The time was: %f seconds.\n",(time2-time1)/CLK_TCK);
  printf("\n Please press any key back to the main menue");
  getch();

}

/***************************************************************************/

void sign2(void)
 {
  LINT msg,msg1,dtp,cpt;
  char fn[10],ans,ch,u_index;
  char fnfmt3[10]={' ','-',' ','.','s','c','t'};
  long utmp,ltmps;
  int  t,i,j,L,data;
  FILE *fpkey;

  fp=fopen(fn1,"rb");
  fp1=fopen(fn2,"wb");
  if((fp==NULL)||(fp1==NULL))
   {
     printf("\n\n Error data filename's format. Press any key to exit!!\n");
     getch(); exit(1);
   }
  fscanf(fp,"%s %c",index,ch);  //read user ID
  read(msg);
  fclose(fp); fclose(fp1);
 // user input ID
  L=strlen(index);
  printf("\n Please input your index number:>");
  scanf(" %c",&u_index);
  if(u_index!=index[L-1])
   {
    printf("\n\nSorry, you aren't the assigned signer.");
    printf("\nPlease press any key to exit the system");
    getch(); exit(1);
   }
  do
   {
    printf("\n Please decide the next user index:>");
    scanf(" %c",&index[L]);
    printf("\n\n Your index is %c,and the next user index is %c!!",index[L-1],index[L]);
    printf("\n Are you sure?(press y/n!!)");
    ans=getch();
   }while(ans!='y');
  // begin assignment key
  fnfmt3[0]=index[L-1];
  fnfmt3[2]=(char)L+48;
  strcpy(fn,fnfmt3);
  fpkey=fopen(fn,"rb");
  if(fpkey==NULL)
   {printf("\n\nYour private key file isn't exist,press any key to exit!!");
    getch();  exit(1);
   }
  for(utmp=0;utmp<=LENGTH;utmp++)
    fscanf(fpkey,"%lx %lx %lx %lx",up+utmp,uq+utmp,ud+utmp,un+utmp);
  fclose(fpkey);

//begin multi_signature
  fp1=fopen(fn2,"wb");  ch='>';
  fprintf(fp1,"%s %c",index,ch);
  printf(" Now, process data.Waiting!!!");
  printf("\n________________________________________________________________");
  printf("\n----------------------------------------------------------------");
  time1=clock();
  modexp(msg,ud,un,msg); // only 128bit long msg is operating
  time2=clock();
  write(msg);
  fclose(fp1);
  printf("\n\n The time was: %f seconds.\n",(time2-time1)/CLK_TCK);
  printf("\n\n Press any key back to main menu!!!!");
  getch();
}



/**************************************************************************/
void sign(void)
{
  int i=0;
  long length=0;
  FILE *handle;
  printf("\n\nPlease input the FILENAME to SIGNATURE:>");
  scanf("%s",fn1);
  printf("\nPlease input the FILENAME to STORE:>");
  scanf("%s",fn2);

  handle=fopen(fn1, "rb");
  while(getc(handle)!=EOF)
     length+=1;
  fclose(handle);

  if(length>23)    i=1;  // the file has never been signatured
  switch(i)
    {
      case 0:
	     sign2();
	     break;
      case 1:
	     sign1();
	     break;
    }
}



/**************************************************************************/
void verify(void)  /* not the first signer; it must be the receiver*/
{
  LINT msg1,msg2;
  char fn[10], ch,u_index;
  char fnfmt2[10]={' ','-',' ','.','p','u','b'};
  unsigned long length=0;
  long utmp,ltmp;
  int  t,i,L,ans,data;
  FILE *fpkey;

  printf("\n\nPlease input the SIGNATURED FILENAME :>");
  scanf("%s",fn1);
  printf("\nPlease input the MESSAGE FILENAME :>");
  scanf("%s",fn2);
  if((fp=fopen(fn1,"rb"))==NULL)
   {
    printf("\n\n Error data filename's format. Press any key to exit!!\n");
    getch(); exit(1);
   }
  fscanf(fp,"%s ",index); /* read the index data  */
  do{ ch=fgetc(fp); } while(ch!='>');
  read(msg1); /* copy the signed data */
  fclose(fp);

/* Begin to allocate the public-key to verified the signature. */
  L=strlen(index);
  printf("The docutment have been signed for %d times.",L-1);
  t=1;
  for(i=L-1;i>0;i--)
    {
      fnfmt2[0]=index[i-1];  /* which user's index to be used */
      fnfmt2[2]=(char)(i+48);/* which key to be used          */
      strcpy(fn,fnfmt2);
      if((fpkey=fopen(fn,"rb"))==NULL)
	{
	  printf("\n\n Press any key to exit!!!");
	  getch();exit(1);
	}
      printf("\n\n this is %dth time to compute.\n",t);
      t=t+1;
      for(utmp=0;utmp<=LENGTH;utmp++)
	fscanf(fp,"%lx %lx",ue+utmp,un+utmp);
      printf(" e = "); hexout(ue);
      printf(" n = "); hexout(un);
      fclose(fpkey);
      modexp(msg1,ue,un,msg1);
    }

  if((fp=fopen(fn2,"rb"))==NULL)
   {
    printf("\n\n Error data filename's format. Press any key to exit!!\n");
    getch(); exit(1);
   }
  while(getc(fp)!=EOF)
     length+=8;
  fclose(fp);
  fp=fopen(fn2,"rb");
  md5(length,msg2);    //md5:hash function
  fclose(fp);

 ans=compare(msg1,msg2);
 if(ans==0)
   { printf("\n\nthe file Pass to the verify!!..OK...");}
 else
   { printf("\n\nthe file Fail to the verify!!...SORRY...");
     printf("\n press any to exit..");
     getch();  exit(1);
   }
 printf("\n\nPlease press any key back to main menu!");
 getch();
}

/***************************************************************************/




void main(void)
{
  int cc,ans;
  union {  int  i;
	   char ch[2];
	} pans;

  window(1,1,80,25);
  do
  {

   clrscr();
   textcolor(YELLOW); textbackground(BLUE);gotoxy(10,4);
   cprintf("---< An Integrated System of Doctument Signature >---");

   textcolor(LIGHTCYAN); textbackground(BLACK);gotoxy(20,7);
   cprintf("Choose one of the following functions");
   ans='a';  pans.i=0;
   while( pans.i!=0x1c0d)   /* No press ENTER */
   {
     for(cc='a';cc<='c';cc++)
     {
       if(cc!=ans) { textbackground(BLACK); textcolor(LIGHTGRAY); }
	  else { textbackground(WHITE); textcolor(RED);  }
       switch(cc)
       {
	 case 'a': gotoxy(18,9);  cprintf("  a: Signature  ;  "); break;
	 case 'b': gotoxy(18,11); cprintf("  b: Verify     ;  "); break;
	 case 'c': gotoxy(18,13); cprintf("  c: Quit & Exit;  "); break;
       }
       gotoxy(61,7);
     }
     while(!bioskey(1));
     pans.i=bioskey(0);
     if ((pans.ch[1]==0x48)&&(ans>'a')) ans--;
     if ((pans.ch[1]==0x50)&&(ans<'c')) ans++;
     if ((pans.ch[0]>='a')&&(pans.ch[0]<='c')) ans=pans.ch[0];
   }


   textbackground(BLACK);
   clrscr();
   switch(ans)
   {
     case 'a':textcolor(YELLOW); textbackground(BLUE);gotoxy(15,2);
	      cprintf("  -----<  SIGNATURE  >-----  ");
	      textcolor(LIGHTGRAY); textbackground(BLACK);gotoxy(1,5);
	      sign();
	      break;
     case 'b':textcolor(YELLOW); textbackground(BLUE);gotoxy(15,2);
	      cprintf("  ------<   VERIFY   >-----  ");
	      textcolor(LIGHTGRAY); textbackground(BLACK);gotoxy(1,5);
	      verify();
	      break;
   }
  }  while(ans!='c');

  printf("\a");
  nosound();
}

