// Program: 'Decypt01.cpp' is cryptolography ciphertext to plaintext
// Auther : Avatar
// Date   : 98.04.20

#include <time.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FULL 0xffffffff
#define RMASK 0x00000001
#define LMASK 0x80000000
#define LENGTH 33

typedef unsigned long LINT[34];
char uname[20], fmat[24];
FILE *fptr1, *fptr2;
LINT up, uq, udp, udq, un, ue;
clock_t time1, time2;

void hexout(unsigned long *num);
void mod(unsigned long ma[], unsigned long mb[]);
void modexp(unsigned long *ex, unsigned long *ev, unsigned long *ep, unsigned long *ew);
int compare(unsigned long lint1[], unsigned long lint2[]);
void add(unsigned long p1[], unsigned long p2[]);
void sub(unsigned long p1[], unsigned long p2[]);
void inverse(unsigned long *iva, unsigned long *ivn, unsigned long *ivb);
void modmul(unsigned long mx[], unsigned long my[], unsigned long mp[], unsigned long mz[]);
void multiply(unsigned long mua[], unsigned long mub[], unsigned long muc[]);


void decypt(void)
{
   long upsn;
   char PublicKeyFile[20];
   while (fptr1==NULL)
   {
      printf("\nKey-in your secret file name: ");
      scanf("%s", uname);
      if (*uname==0x1b)
         exit(1);
      strcpy(PublicKeyFile, uname);
      strcat(uname, ".sct");
      strcat(PublicKeyFile, ".pub");
      fptr1 = fopen(uname, "r");
      if (fptr1==NULL)
      {
         printf("\nError login name, Press any key to continue...");
         getch();
      }
   }
   fscanf(fptr1, "%lx %lx %lx %lx %lx ", up, uq, udp, udq, &upsn);
   for (int i=1; i<=(*up); i++)
      fscanf(fptr1, "%lx %lx %lx %lx ", up+i, uq+i, udp+i, udq+i);
   fclose(fptr1);

   int KeyLen;
   fptr2=fopen(PublicKeyFile, "r");
   fscanf(fptr2, "%lx %lx ", ue, un);
   fclose(fptr2);
   KeyLen = (*un);

   int flag = 3;
   long passnum;
   do {
      printf("\nKey-in your password number: ");
      scanf("%d", &passnum);
      flag--;
      passnum+=1234;
      if (passnum!=upsn)
         printf("\nError password!!!");
   } while ( (flag>0)&&(passnum!=upsn));

   if (flag<=0) {
      printf("\nPasswrod error, get out here...");
      getch();
      exit(1);
   }

   char fn1[20], fn2[20], str[24];
   fptr1 = NULL; fptr2 = NULL;
   while (fptr1==NULL)
   {
      printf("\nPlease key-in ciphertext filename: ");
      scanf("%s", fn1);
      if (*fn1==0x1b) exit(1);
      fptr1 = fopen(fn1, "rb");
      if (fptr1==NULL) {
         printf("\nError filename, try again...");
         getch();
      }
   }
   while (fptr2==NULL)
   {
      printf("\nPlease key-in plaintext filename: ");
      scanf("%s", fn2);
      if (*fn2==0x1b) exit(1);
      fptr2 = fopen(fn2, "wb");
      if (fptr2==NULL) {
         printf("\nError filename, try again...");
         getch();
      }
   }
   for (int i=1; i<=(*up); i++)
   {
      if (i<=up[0])   up[i]-=upsn;
      if (i<=uq[0])   uq[i]+=upsn;
      if (i<=udp[0])  udp[i]-=upsn;
      if (i<=udq[0])  udq[i]+=upsn;
   }
   long CipherSize;
   fscanf(fptr1, "%s %lx", str, &CipherSize);
   strcat(str, "\n");
   flag = strcmp(str, fmat);
   if (flag!=0) {
      printf("\nThe file hasn't encrypted, press any key to exit.");
      getch();
      exit(1);
   }

   char ch;
   do {
     ch=fgetc(fptr1);
   } while (ch!='>');
   printf("\nPress any key to continue...");
   ch = getch();
   if (ch==0x1b) {
      printf("\n\n  P = "); hexout(up);
      printf("  q = ");   hexout(uq);
      printf(" dP = ");   hexout(udp);
      printf(" dq = ");   hexout(udq);
   }
   printf("\n------------------------------------------------------------");
   printf("\n\nNow, process data. Waiting...");

   int DecyptLen = KeyLen*4;
   long DecyptTime = CipherSize/DecyptLen;
   long DecyptR = CipherSize%DecyptLen;
//   printf("\n%lx %lx %lx", DecyptLen, DecyptTime, DecyptR); getch();
   if ((DecyptR)!=0)
      DecyptTime++;

   LINT msg, msg1, dtp;
   time1=clock();
   for (int i=1; i<=DecyptTime; i++)
   {
      (*msg)=0;
      for (int j=KeyLen; j>0; j--)
      {
         for (int k=0; k<4; k++)
         {
            msg[j]=(msg[j]<<8)|fgetc(fptr1);
         }
         (*msg)++;
      }
      fgetc(fptr1);
//      printf("\nmsg[%d]: ", i);   hexout(msg);   getch();

      for (int j=0; j<=(*msg); j++)
         msg1[j] = msg[j];

      mod(msg, up);              mod(msg1, uq);
      modexp(msg, udp, up, msg); modexp(msg1, udq, uq, msg1);
      while (compare(msg,msg1)>0)
         add(msg1,uq);
      sub(msg1, msg);
      inverse(up, uq, dtp);
      modmul(msg1, dtp, uq, msg1);
      multiply(msg1, up, msg1);
      add(msg1, msg);
//      printf("\nMsg1[%d]: ", i); hexout(msg1); getch();

      char buf[4];
      for (int j=KeyLen; j>0; j--)
      {
         buf[0] = (msg1[j])>>24;
         buf[1] = (msg1[j]>>16)&0xff;
         buf[2] = (msg1[j]>>8)&0xff;
         buf[3] = (msg1[j])&0xff;
         for (int k=0; k<4; k++)
            fputc(buf[k], fptr2);
      }
   }
   time2=clock();
   fclose(fptr1);   fclose(fptr2);
   printf("\n\nThe time was : %f seconds.\n", (time2-time1)/CLK_TCK);
   printf("\n\n\nPress any key to continue...");
   getch();
}



void main(void)
{
   clrscr();
   strcpy(fmat, "--------------------\n");
//   for (int i=0; i<24; i++) printf("%d", fmat[i]);
   decypt();
}
