// Program : 'Dvread'
// Author  : Avatar
// Date    : 98.03.17
// Filename: Dvread.cpp

#include <stdio.h>
#define LENGTH 17

FILE *fp;

int dvread(FILE *fp, unsigned long *msg)
{
   int flag, tmp, tmp1, tmp2, tch, buf[69];
   unsigned long *tmsg, es1;

   flag=1; tmsg=msg;
   for (tmp=0; tmp<=LENGTH; tmsg++)
      (*tmsg)=0;
   for (tmp=1; tmp<=65; tmp++)
      buf[tmp]=fgetc(fp);
   buf[0]=65;
   tmsg=msg;
   tmp=1;    tmp1=0;
   while (tmp<=buf[0])
   {
      if (tmp1==0)
      {
         es1=buf[tmp];
         tmp1++;
         tmsg++;
      }
      else
      {
         es1=(es1<<8)|buf[tmp];
         tmp1++;
      }
      if ((tmp1==4)||(tmp==buf[0]))
      {
         (*tmsg)=es1;
         (*msg)++;
         tmp1=0;
      }
      tmp++;
   }
   tmsg=msg+(*msg);
   while ( ((*msg)==0)&&((*msg)!=0) )
   {
      (*msg)--;
      tmsg--;
   }
   return flag;
}               


