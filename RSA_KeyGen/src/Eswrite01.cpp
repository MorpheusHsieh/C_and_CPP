// Program : Eswrite.cpp
// Author  : Avatar
// Date    : 98.03.17

#include <stdio.h>
FILE *fp1;

void eswrite01(FILE *fp1, unsigned long *cpt)
{
   int tmp, tmp1, tch[4];
   cpt++;
   for (tmp=1; tmp<=2; tmp++)
   {
      tch[0]=(*cpt)>>24;
      tch[1]=((*cpt)>>16)&0xff;
      tch[2]=((*cpt)>>8)&0xff;
      tch[3]=(*cpt)&0xff;
      if (tmp<2)
      {
         for (tmp1=0; tmp1<4; tmp1++)
            fputc(tch[tmp1], fp1);
      }
      else
         fputc(tch[3], fp1);
      cpt++;
   }
}
