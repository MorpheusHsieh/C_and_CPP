/* Program : Font00.cpp */
/* Auther  : Avatar     */
/* Date    : 94/04/19   */
/* Function: Get Chinese Fonts from Stdfont.24 & display & */
#include <io.h>
#include <conio.h>
#include <fcntl.h>
#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main(void)
{
   int i, j;
   FILE *font_fptr;
   int x_posi=10, y_posi=50;
   int GraphDriver, GraphMode;
   long file_posi=0;
   unsigned char One_char;
   unsigned char eight_bits[8]={128, 64, 32, 16, 8, 4, 2, 1};

   clrscr();
   font_fptr=fopen("d:\\et5\\stdfont.24", "r");

   GraphDriver=DETECT;
   initgraph(&GraphDriver, &GraphMode, "d:\\tc\\bgi");

   while (fseek(font_fptr, file_posi, SEEK_SET)==0) {
      One_char=getc(font_fptr);
      for (j=0; j<8; j++) {
         if ((One_char & eight_bits[j])!=0) putpixel(x_posi, y_posi, 15);
         x_posi++;
      }
      if (file_posi%3==2) {x_posi=10; y_posi++;}
      file_posi++;
      if ((file_posi%72)==71) {
         printf("%6d", file_posi/72);
         getch();
         clrscr();
         cleardevice();
         x_posi=10; y_posi=50;
      }
   }
   closegraph();
   fclose(font_fptr);
}
