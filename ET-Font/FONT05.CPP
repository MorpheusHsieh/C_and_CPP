/* Program : Font05.cpp */
/* Auther  : Avatar     */
/* Date    : 95/02/22   */
/* Function: Key-in a Chinese string output file.txt */
#include <io.h>
#include <conio.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my_def.h"
#include <graphics.h>
#define Gap         3
#define Color      15
#define X_posilst 100
#define Y_posilst 150

int Font[72];
int Char_len=72;
int font_type;
int GraphDriver, GraphMode;
int out_Handle, Asc_Handle, Std_Handle, Spc_Handle, Spf_Handle, Usr_Handle;
char *ET_path;

void ET_PATH(void);
void wrfile(void);
void show_chinese(int, int);

void main(void)
{
   int i, j;
   char One_char;
   long Begin_posi;
   unsigned int char_posi;
   unsigned char H_byte, L_byte, *c_string;

   clrscr();
   while ( font_type!=1 && font_type!=2 ) {
      printf("Choose [1] 15*16 Font, [2] 24*24 Font: ");
      scanf("%d", &font_type);
   }
   if (font_type==1)
      Char_len=30;

   if ( etexist()==(-1) ) {
      printf("ET Chinese system is not exist.\n");
      exit(1);
   }

   ET_PATH();

   if (font_type==2) {
      Asc_Handle=open(ET_path, O_RDONLY);
      Spc_Handle=open(ET_path, O_RDONLY);
      Spf_Handle=open(ET_path, O_RDONLY);
      Std_Handle=open(ET_path, O_RDONLY);
      Usr_Handle=open(ET_path, O_RDONLY);
   }
   else {
      Asc_Handle=open(ET_path, O_RDONLY);
      Spc_Handle=open(ET_path, O_RDONLY);
      Spf_Handle=open(ET_path, O_RDONLY);
      Std_Handle=open(ET_path, O_RDONLY);
      Usr_Handle=open(ET_path, O_RDONLY);
   }
   out_Handle=open("font04.txt", O_WRONLY|O_TRUNC);

   printf("Key-in Chinese string: ");
   scanf("%s", c_string);

   for (i=0; i<(strlen(c_string)/2); i++) {
      H_byte=*(c_string+i*2);
      L_byte=*(c_string+i*2+1);
      Begin_posi=(font_posi(H_byte, L_byte)*Char_len);
      lseek(Std_Handle, Begin_posi, SEEK_SET);
      for (j=0; j<Char_len; j++) {
         _read(Std_Handle, &One_char, 1);
         Font[j]=One_char;
      }
      show_chinese(X_posilst+(i*(24+Gap)), Y_posilst);
      wrfile();
   }
   close(out_Handle);
   close(Std_Handle);
}

/* Show_chinese(): Diskplay Font to Monitor */
void show_chinese(int x_posi, int y_posi)
{
   static int i, j, x, y, k=2;
   unsigned int eight_bits[8]={ 128, 64, 32, 16, 8, 4, 2, 1 };
   unsigned char One_char;

   detectgraph(&GraphDriver, &GraphMode);
   switch(GraphDriver)
   {
      case VGA:
         GraphDriver=VGA;
         GraphMode=VGAMED;
         break;
      default:
         GraphDriver=EGA;
         GraphMode=EGAHI;
   }
   if (registerbgidriver(EGAVGA_driver)<0)
      exit(1);
   initgraph(&GraphDriver, &GraphMode, "");

   x=x_posi;
   y=y_posi;

   if (Char_len==72) { k=3; }

   for (i=0; i<Char_len; i++) {
      for (j=0; j<8; j++) {
         if ((Font[i] & eight_bits[j])!=0) putpixel(x, y, Color);
         x++;
      }
      if ( i%k==(k-1) ) { x=x_posi; y++; }
   }
   getch();
   closegraph();
}

/* wrfile(): Font data write to file.txt */
void wrfile(void)
{
   int i, char_num;
   static char Hex_code[5];
   unsigned char One_char;

   for (i=0; i<Char_len; i++) {
      if (i%2==0)
         char_num=256*Font[i];
      else {
         char_num+=Font[i];

         itoa(char_num, Hex_code, 16);
         if ( strlen(Hex_code) < 4 ) {
            if ( strlen(Hex_code)>1 )
               strrev(Hex_code);
            while( strlen(Hex_code)<4 )
               strcat(Hex_code, "0");
            strrev(Hex_code);
         }
         write(out_Handle, Hex_code, 4);
         if ((i%8)==7)
            write(out_Handle, "\n", 1);
         else
            write(out_Handle, "H, ", 3);
      }
   }
   write(out_Handle, "\n", 1);
}

void ET_PATH(void)
{
   int len;

   if (getenv("ET")==NULL) {
      ET_path="c:\et5";
      printf("\nSyntax: Set et=[d:][path]\n");
   }
   else
      ET_path=getenv("ET");
   if (font_type==1)
      strcat(ET_path, "\\stdfont.15");
   else
      strcat(ET_path, "\\stdfont.24");
   len=strlen(ET_path);
   ET_path[len]=NULL;
   if (fopen(ET_path, "rb")==NULL)
      printf("Open Stdfont Fail!\n\7");
}
