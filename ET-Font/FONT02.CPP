/* Program : Font02.cpp */
/* Auther  : Avatar     */
/* Date    : 94/04/20   */
/* Function: Key-in a Chinese string output file.txt */
#include <io.h>
#include <conio.h>
#include <fcntl.h>
#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my_def.h"
#define Gap         3
#define Color      15
#define X_posilst 100
#define Y_posilst 150

FILE *Std24_Handle, *out_fptr;
long file_posi;
void wrfile(void);
void show_chinese(int, int);
long chinese_addr(unsigned int, unsigned int);

void main(void)
{
   int i;
   int GraphDriver, GraphMode;
   unsigned int char_posi;
   unsigned char H_char, L_char, *c_string;

   clrscr();
   if ( etexist()==(-1) ) {
      printf("ET Chinese system is not exist.\n");
      exit(1);
   }

   detectgraph(&GraphDriver, &GraphMode);
   switch(GraphDriver)
   {
      case VGA:
         GraphDriver=VGA;
         GraphMode=VGAMED;
         break;
      case EGA:
         GraphDriver=EGA;
         GraphMode=EGAHI;
   }
   initgraph(&GraphDriver, &GraphMode, "d:\\tc\\bgi");

   Std24_Handle=fopen("d:\\et5\\stdfont.24k", "r");
   out_fptr=fopen("c:\\temp\\word2402.txt", "w");

   printf("Key-in Chinese string: ");
   scanf("%s", c_string);

   for (i=0; i<(strlen(c_string)/2); i++) {
      H_char=*(c_string+i*2);
      L_char=*(c_string+i*2+1);
      file_posi=chinese_addr(H_char, L_char);
      show_chinese(X_posilst+(i*(24+Gap)), Y_posilst);
      wrfile();
   }
   getch();
   closegraph();
   fclose(out_fptr);
   fclose(Std24_Handle);
}

/* chinese_addr():  */
long chinese_addr(unsigned int H_byte, unsigned int L_byte)
{
   int i, L_byte_offset;
   long Seri_no, Begin_posi;
   unsigned int Int_code;

   Int_code=H_byte*256+L_byte;

   L_byte_offset=L_byte-0x40+1;
   if (L_byte_offset>63) L_byte_offset-=34;

   Seri_no=(H_byte-0xA1)*157+L_byte_offset;
   if (Int_code>=0xA440)
      Seri_no-=30;
   if (Int_code>=0xC940)
      Seri_no-=408;
   Seri_no-=441;
   Begin_posi=(Seri_no-1)*72;

   return(Begin_posi);
}

/* Show_chinese(): Diskplay Font to Monitor */
void show_chinese(int x_posi, int y_posi)
{
   int i, j, k, x, y;
   unsigned int eight_bits[8]={ 128, 64, 32, 16, 8, 4, 2, 1 };
   unsigned char One_char;

   x=x_posi;
   y=y_posi;

   if ( fseek(Std24_Handle, file_posi, SEEK_SET) != 0 ) {
      printf("Fseek failed!");
      getch();
   }
   else {
      for (i=0; i<72; i++) {
         One_char=fgetc(Std24_Handle);
         for (j=0; j<8; j++) {
            if ((One_char & eight_bits[j])!=0) putpixel(x, y, Color);
            x++;
            y=y_posi;
         }
         if (i%3==2) { x=x_posi; y++; }
      }
   }
}

/* wrfile(): Font data write to file.txt */
void wrfile(void)
{
   int i, char_num;
   static char Hex_code[5];
   unsigned char One_char;

   if ( fseek(Std24_Handle, file_posi, SEEK_SET) != 0 )
      printf("Fseek failed !\n");
   else {
      for (i=0; i<72; i++) {
         One_char=fgetc(Std24_Handle);
         if (i%2==0)
            char_num=256*One_char;
         else {
            char_num+=One_char;

            itoa(char_num, Hex_code, 16);
            if ( strlen(Hex_code) < 4 ) {
               if ( strlen(Hex_code)>1 )
                  strrev(Hex_code);
               while( strlen(Hex_code)<4 )
                  strcat(Hex_code, "0");
               strrev(Hex_code);
            }
            fputs(Hex_code, out_fptr);
            fputs("\n", out_fptr);
         }
      }
   }
}

