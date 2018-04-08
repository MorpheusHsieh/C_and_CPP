// Program : 'Encrypt' is encrypt a plaintext to a ciphertext
// Author  : Avatar
// Date    : 98.04.19

#include <time.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FULL  0xffffffff
#define LMASK 0x80000000
#define RMASK 0x00000001
#define NNINE 0x3b9ac9ff
#define LENGTH 33


typedef unsigned long LINT[34];
int cpass, count;
char uname[20], fmat[24];
FILE *fp, *fp1, *fopen();
LINT up, uq, un, ue, udp, udq;
clock_t time1, time2;

int esread01(FILE *fp, unsigned long *msg);
void eswrite01(FILE *fp1, unsigned long *cpt);
void hexout(unsigned long *num);
void decout(unsigned long *num);
int compare(unsigned long *p1, unsigned long *p2);
void shiftright(unsigned long *sr);
void modexp(unsigned long *ex, unsigned long *ev, unsigned long *ep, unsigned long *ew);

void encrypt(void)
{
   int flag, utmp;
   long flen;
   LINT msg, cpt;
   char fn1[30], fn2[30], ch;

   printf("\nInput the recepient's login name: ");
   scanf("%s", uname);
   strcat(uname, ".pub");
   fp = fopen(uname, "r");
   if (fp==NULL)   {
      printf("\nError login name. press any key to exit this system!!");
      getch();
      exit(1);
   }
   fscanf(fp, "%lx %lx ", ue, un);
   for (utmp=1; utmp<=(*ue); utmp++)
      fscanf(fp, "%lx %lx ", ue+utmp, un+utmp);
   fclose(fp);

   printf("\n\n e  = "); hexout(ue);
   printf(" n  = "); hexout(un);

   printf("\nPlease input the source data filename: ");
   scanf("%s", fn1);
   printf("\nPlease input the data filename to store the result: ");
   scanf("%s", fn2);
   fp=fopen(fn1, "rb");   fp1=fopen(fn2, "wb");
   if ((fp==NULL)||(fp1==NULL))   {
      printf("\n\nError data filename's format. press any key to exit!!\n");
      getch();
      exit(1);
   }
   flen=0;
   while ((utmp=fgetc(fp))!=EOF)
      flen++;
   fclose(fp);

   fp=fopen(fn1, "rb");  ch='>';
   fprintf(fp1, "%s %lx %c", fmat, flen, ch);
   printf("\n---------------------------------------------------------------");
   printf("\n\nThe file '%s' has %ld bytes.", fn1, flen);
   printf("   Now, process data. Waiting!!!");

   time1=clock();
   do {
      flag=esread01(fp, msg);
//      printf("\nMsg[%lx]: ", msg[0]); hexout(msg);
      modexp(msg, ue, un, cpt);
//      printf("\nCpt: "); hexout(cpt); getch();
      eswrite01(fp1, cpt);
   } while (flag>0);
   time2=clock();
   fclose(fp);
   fclose(fp1);
   printf("\n\nThe time was: %f seconds.\n", (time2-time1)/CLK_TCK);
   printf("\n\n\nPress any key to exit the main menu!!!");
   getch();
}

void main(void)
{
   clrscr();
   randomize();
   strcpy(fmat, "--------------------\n");
   encrypt();
}

