/* Program : Font03-1.cpp */
/* Auther  : Avatar        */
/* Date    : 94/04/22      */
/* Function: Key-in a Chinese string output file.txt */
#include <io.h>
#include <conio.h>
#include <fcntl.h>
#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys\stat.h>
#include "my_def.h"
#define Gap         3
#define Color      15
#define X_posilst 100
#define Y_posilst 150

int Font[72];
int Char_len=72;
int out_Handle;
void wrfile();
void show_chinese(int, int);

void main(void)
{
   int i, j, font_type;
   int Std_Handle, ET_path_len;
   int GraphDriver, GraphMode;
   char One_char, *ET_path;
   long Begin_posi;
   unsigned int char_posi;
   unsigned char H_byte, L_byte, *c_string;

   clrscr();

   if ( etexist()==(-1) ) {
      printf("ET Chinese system is not exist.\n");
      exit(1);
   }

   do {
      printf("Choose [1] 15*16 Font, [2] 24*24 Font: ");
      scanf("%d", &font_type);
   }
   while ( font_type!=1 && font_type!=2 );

   if (font_type==1)
      Char_len=30;

   GraphDriver=DETECT;
   initgraph(&GraphDriver, &GraphMode, "d:\\tc\\bgi");

   out_Handle=creat("c:\\temp\\fontdata.txt", S_IWRITE);

   if (font_type==2)
      strcpy(ET_path, "d:\\et5\\stdfont.24");
   else
      strcpy(ET_path, "d:\\et5\\stdfont.15");
   Std_Handle=open(ET_path, O_RDONLY);

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
	 printf("%04x, ", Font[j]);
      }
      getch();
      show_chinese(X_posilst+(i*(24+Gap)), Y_posilst);
      wrfile();
   }
   closegraph();
   close(out_Handle);
   close(Std_Handle);
}

/* Show_chinese(): Diskplay Font to Monitor */
void show_chinese(int x_posi, int y_posi)
{
   static int i, j, x, y, k=2;
   unsigned int eight_bits[8]={ 128, 64, 32, 16, 8, 4, 2, 1 };
   unsigned char One_char;

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
}

/* wrfile(): Font data write to file.txt */
void wrfile(void)
{
   int i, char_num;
   static char Hex_code[4];
   unsigned char One_char;

   for (i=0; i<Char_len; i++) {
      char_num=Font[i];

      itoa(char_num, Hex_code, 16);
      if ( strlen(Hex_code) < 3 ) {
	 if ( strlen(Hex_code)>1 )
	    strrev(Hex_code);
	 while( strlen(Hex_code)<3 )
	    strcat(Hex_code, "0");
	 strrev(Hex_code);
      }
      Hex_code[4]=NULL;
      write(out_Handle, Hex_code, 3);
      write(out_Handle, "H, ", 3);
      if (i%8==7)
	 write(out_Handle, "\n", 1);
   }
   write(out_Handle, "000H, 000H\n\n", 12);

}

