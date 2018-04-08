// Program : 'Getkeys' is to generate a pairs key
// Author  : Avatar
// Date    : 98.04.18

#include <math.h>
#include <time.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LENGTH 33
#define MinKeyLen   32
#define MaxKeyLen 1024
#define FULL  0xffffffff
#define RMASK 0x00000001
#define LMASK 0x80000000

FILE *fptr1, *fptr2, *fopen();
typedef unsigned long LINT[34];
int count, cpass;
char uname[20];
LINT up, uq, un, ue, udp, udq;
clock_t time1, time2;

void hexout(unsigned long num[]);
void decout(unsigned long num[]);
void shiftleft(unsigned long sl[]);
int compare(unsigned long *p1, unsigned long *p2);
void bigrand(unsigned int bits, unsigned long *lint);
void add(unsigned long p1[], unsigned long p2[]);
void sub(unsigned long p1[], unsigned long p2[]);
void gcd(unsigned long *ga, unsigned long *gb, unsigned long *gc);
void multiply(unsigned long mua[], unsigned long mub[], unsigned long muc[]);
void inverse(unsigned long *iva, unsigned long *ivn, unsigned long *ivb);
void strongprime(unsigned int splen, unsigned long *spn);

void getkeys(void)
{
   char uans, fn1[20], fn2[20];
   int utmp, keylen;
   unsigned long upsn;
   unsigned long *n_one = new unsigned long [LENGTH+1];
   unsigned long *n_tmp = new unsigned long [LENGTH+1];
   unsigned long *tmp = new unsigned long [LENGTH+1];

   do {
      printf("\nKey-in your login name (1-8 character & no space):");
      scanf("%s", uname);
      printf("Key-in your password number (0-99999999):");
      do {
         upsn=0;
         scanf("%ld", &upsn);
         if ( (upsn>99999999)||(upsn<1) )
            printf("\nError, Key-in again: ");
      } while ( (upsn<1)||(upsn>99999999) );

      printf("\n\nLogin name:%s", uname);
      printf("\nPassword  :%ld", upsn);
      printf("\n\nAre you sure (y/n):");
      scanf("%s", &uans);
      clrscr();
   } while ( (uans!='y')&&(uans!='Y') );
   strcpy(fn1, uname);   strcat(fn1, ".pub");
   strcpy(fn2, uname);   strcat(fn2, ".sct");
   printf("\n\nNow generate the system keys, Wait for some minutes!!\n");

   printf("Enter length of key (32 - 1024 bits): ");
   scanf("%d", &keylen);
   if (keylen<=MinKeyLen)
      keylen = MinKeyLen;
   else if (keylen>=MaxKeyLen)
      keylen = MaxKeyLen;

   time1=clock();
   utmp=keylen/2;
   int ltmp = utmp;
   tmp[0]=1; tmp[1]=1;
   while (ltmp>0) {
      shiftleft(tmp);
      ltmp--;
   }
   int flag=0;
   while (flag==0) {
      strongprime(utmp,up);
      printf("\nUp: ");   hexout(up);
      if (compare(tmp,up)>0) flag=1;
   }
   printf("\nLast Up: ");   hexout(up);
   utmp=(keylen/2)-1;
   flag=0;
   while (flag==0) {
     strongprime(utmp, uq);
     printf("\nUq: ");   hexout(uq);
     if ((compare(tmp,uq)>0)&&(compare(up,uq)!=0)) flag=1;
   }
   printf("\nLast Uq: ");   hexout(uq);

   n_one[0]=1;   n_one[1]=1;
   for (utmp=2; utmp<=LENGTH; utmp++)
      n_one[utmp] = 0;
   sub(up,n_one);   sub(uq,n_one);
   cpass = 0;   utmp=32;
   if (keylen<32) utmp = keylen;
   strongprime(utmp, ue);
//   bigrand(utmp, ue);

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
   time2=clock();

   fptr1 = fopen(fn1, "w");
   fptr2 = fopen(fn2, "w");
   if ( (fptr1==NULL)||(fptr2==NULL) )
   {
      printf("\n\nThere are some error in disk writing, ");
      printf("Press any key to exit this system.\n");
      getch();
      exit(1);
   }
   printf("\n\n  e = "); hexout(ue);
   printf("  n = "); hexout(un);
   printf("  p = "); hexout(up);
   printf("  q = "); hexout(uq);
   printf(" dp = "); hexout(udp);
   printf(" dq = "); hexout(udq);
   printf("\n The time was: %f minutes.\n", (time2-time1)/(60.0*CLK_TCK));

   printf("\n\nPubilc Key: ");
   for (utmp=0; utmp<=(*un); utmp++)
   {
      printf("%lx %lx ", ue[utmp], un[utmp]);
      fprintf(fptr1, "%lx %lx ", ue[utmp], un[utmp]);
   }
   fclose(fptr1);

   printf("\n\nPrivacy key: ");
   upsn+=1234;
   printf("%lx %lx %lx %lx %lx ", up[0],uq[0],udp[0],udq[0],upsn);
   fprintf(fptr2, "%lx %lx %lx %lx %lx ", up[0],uq[0],udp[0],udq[0],upsn);
   for (utmp=1; utmp<=(*up); utmp++)
   {
      if (utmp<=up[0]) up[utmp]+=upsn;
      if (utmp<=uq[0]) uq[utmp]-=upsn;
      if (utmp<=udp[0]) udp[utmp]+=upsn;
      if (utmp<=udq[0]) udq[utmp]-=upsn;
      printf("%lx %lx %lx %lx ",up[utmp],uq[utmp],udp[utmp],udq[utmp]);
      fprintf(fptr2,"%lx %lx %lx %lx ",up[utmp],uq[utmp],udp[utmp],udq[utmp]);
   }
   fclose(fptr2);
   printf("\n\nPress any key to continue...");
   getch();
}

