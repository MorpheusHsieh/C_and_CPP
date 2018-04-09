// Progeam: DContent
// Author : Morpheus
// Date   : 98.06.09

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

void main(void)
{
   clrscr();
   printf("\nDisplay content.");
   FILE *fin;
   if ((fin=fopen("hdata3.dat", "rb")) == NULL) {
      printf("\nOpen Hdata3.dat fail...");
      getch();
      exit(1);
   }

   printf("\nDisplay Context..."); getch();

   fseek(fin,0L,SEEK_SET);

   char ch = 0x0;
   static int cnt1, cnt2, cnt3;
   while ((ch=fgetc(fin)) != EOF)
   {
      cnt1++;
      if (ch == 0x01)
         cnt2++;
      if ((cnt1%32) == 0) {
         if (cnt2 > 16)
            ch = (ch << 1) | 0x01;
         else
            ch = (ch << 1);
         cnt3 ++;
         if ((cnt3%8) == 0) {
            printf("%x", ch);
            ch = 0x0;
         }
      }
   }

   printf("\nPress any key exit...");
   getch();
}   