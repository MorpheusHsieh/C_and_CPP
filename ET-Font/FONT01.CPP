/* Program : Font01.cpp */
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
   int i, j, str_num;
   int in_Handle, out_Handle;
   int x_posi=100, y_posi=150;
   int GraphDriver, GraphMode;
   FILE *out_fptr;
   static char string[5];
   static long file_posi;
   unsigned char One_char;
   unsigned char eight_bits[8]={128, 64, 32, 16, 8, 4, 2, 1};

   if ((in_Handle=open("d:\\et5\\stdfont.24", O_RDONLY))==(-1)) {
      printf("STDFONT.24 IS NOT FOUND !!\n");
      exit(0);
   }
   out_fptr=fopen("c:\\temp\\word2401.txt", "w");

   clrscr();
   GraphDriver=DETECT;
   initgraph(&GraphDriver, &GraphMode, "d:\\tc\\bgi");

   while (lseek(in_Handle, file_posi, SEEK_SET)==0) {
      printf("{ ");
      for (i=0; i<72; i++) {
         _read(in_Handle, &One_char, 1);

         if (i%2==0)
            str_num=256*One_char;
         else {
            str_num+=One_char;

            itoa(str_num, string, 16);
            if ( strlen(string)<4 ) {
               if (strlen(string)>1)
                  strrev(string);
               while ( strlen(string)<4 )
                  strcat(string, "0");
               strrev(string);
            }
            fputs(string, out_fptr);
            fputs("\n", out_fptr);
         }
         for (j=0; j<8; j++) {
            if ((One_char & eight_bits[j])!=0) putpixel(x_posi, y_posi, 15);
            x_posi++;
         }
         if (i%3==2) {x_posi=100; y_posi++;}
         printf("0x%04x", str_num);
         if (i%10==9)
            printf("\n ");
         if (i==71)
            printf(" }");
         else
            printf(",");
      }
      getch();
   }
   closegraph();
   fclose(out_fptr);
}
