// Program : Esread
// Author  : Avatar
// Date    : 98.04.19

#include <stdio.h>
#include <conio.h>
#define LENGTH 33

FILE *fp;

void hexout(unsigned long *num);

int esread01(FILE *fp, unsigned long *msg)
{
   int flag, tmp, tmp1, tch, buf[5];
   unsigned long es1;
   unsigned long *tmsg = new unsigned long [LENGTH+1];

   flag=1; tmsg=msg;
   for (tmp=0; tmp<=LENGTH; tmp++)
      tmsg[tmp]=0x0;

   tmp=1;
   while ((flag!=0)&&(tmp<=4))
   {
      if ((tch=fgetc(fp))!=EOF) {
         buf[tmp]=tch;
         tmp++;
      }
      else
         flag=0;
   }
   buf[0]=tmp-1;   tmsg=msg;
   if (buf[0]==0)
      flag=0;
   else
   {
      tmp=1; tmp1=0;
      while (tmp<=buf[0])
      {
         if (tmp1==0) {
            es1=buf[tmp];
            tmp1++; tmsg++;
         }
         else {
            es1=(es1<<8)|buf[tmp];
            tmp1++;
         }
         if ((tmp1==4)||(tmp==buf[0])) {
            (*tmsg)=es1;
            (*msg)++;
            tmp1=0;
         }
         tmp++;
      }
      tmsg=msg+(*msg);
      while ( ((*tmsg)==0)&&((&msg)!=0) )
      {
         (*msg)--;   tmsg--;
      }
   }
   return flag;
}