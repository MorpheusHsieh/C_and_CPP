// Program: 'Encypt02' is encryption a plaintext to a ciphertext.
// Author : Avatar
// Date   : 98.04.23

#include <time.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LENGTH 33

typedef unsigned long LINT[34];
char uname[24], fmat[24];

void hexout(unsigned long *num);
void modexp(unsigned long *ex, unsigned long *ev, unsigned long *ep, unsigned long *ew);

void encypt(void)
{
   unsigned long *ue = new unsigned long [LENGTH+1];
   unsigned long *un = new unsigned long [LENGTH+1];

   FILE *fptr1, *fptr2;
   printf("\nKey-in public-key filename: ");
   scanf("%s", uname);
   strcat(uname, ".pub");
   fptr1=fopen(uname, "r");
   if (fptr1==NULL) {
      printf("\nError filename, Press any key to exit this system...");
      getch();
      exit(1);
   }
   fscanf(fptr1, "%lx %lx ", ue, un);
   for (int i=1; i<=(un[0]); i++) {
      fscanf(fptr1, "%lx %lx ", ue+i, un+i);
   }
   fclose(fptr1);

   printf("\n\n  e = ");   hexout(ue);
   printf("  n = ");   hexout(un);

   char fn1[20], fn2[20];
   fptr1=NULL;
   while (fptr1==NULL) {
      printf("\nPlease enter the source data filename: ");
      scanf("%s", fn1);
      fptr1=fopen(fn1, "rb");
      if (fptr1==NULL) {
         printf("\nError source filename, Try again...");
         getch();
      }
   }
   fptr2=NULL;
   while (fptr2==NULL) {
      printf("\nPlease enter the data filename to store the result: ");
      scanf("%s", fn2);
      fptr2=fopen(fn2, "wb");
      if (fptr2==NULL) {
         printf("\nError source filename, Try again...");
         getch();
      }
   }

   int ch;
   long PlainSize=0;
   while ((ch=fgetc(fptr1))!=EOF)
      PlainSize++;
   fclose(fptr1);

   fptr1=fopen(fn1, "rb");
   fprintf(fptr2, "%s %lx %c", fmat, PlainSize, '>');
   printf("\n--------------------------------------------------------------");
   printf("\n\nThe file '%s' has %ld bytes. ", fn1, PlainSize);
   printf("Now, process data.   Waiting...");

   clock_t time1, time2;
   long EncyptLen = (*un)*4;
   long EncyptTime = PlainSize/EncyptLen;
   long EncyptR = PlainSize%EncyptLen;
   if ((EncyptR)!=0)
      EncyptTime++;
//   printf("\n%ld,\t%ld,\t%ld", EncyptLen, EncyptTime, EncyptR);  getch();

   unsigned long *msg = new unsigned long [LENGTH+1];
   unsigned long *cipher = new unsigned long [LENGTH+1];

   time1=clock();
   for (int i=1; i<=EncyptTime; i++)
   {
      (*msg)=0;
      for (int j=(*un); j>0; j--)
      {
         for (int k=0; k<4; k++) {
            if ((ch=fgetc(fptr1))==EOF)
               msg[j] = (msg[j]<<8)|0x0;
            else
               msg[j] = (msg[j]<<8)|ch;
         }
         (*msg)++;
      }
//      printf("\nMsg[%d]: ", i);   hexout(msg); getch();
      modexp(msg, ue, un, cipher);
      int buf[4];
      for (int j=(*un); j>0; j--)
      {
         buf[0]=(cipher[j])>>24;
         buf[1]=((cipher[j])>>16)&0xff;
         buf[2]=((cipher[j])>>8)&0xff;
         buf[3]=(cipher[j])&0xff;
         for (int k=0; k<4; k++) {
            fputc(buf[k], fptr2);
         }
      }
      fputc(NULL, fptr2);
   }
   time2=clock();
   fclose(fptr1); fclose(fptr2);
   printf("\n\nThe time was %f seconds.\n", (time2-time1)/CLK_TCK);
   printf("\n\nPress any key to continue...");
   getch();
}

void main(void)
{
   clrscr();
   randomize();
   strcpy(fmat, "--------------------\n");
   encypt();
}



