/* Program : Font03-2.cpp */
/* Auther  : Avatar        */
/* Date    : 94/04/22      */
/* Function: Key-in a Chinese string output file.txt */
#include <io.h>
#include <conio.h>
#include <fcntl.h>
#include <graphics.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my_def.h"
#define       Gap 3
#define     Color 15
#define X_posilst 100
#define Y_posilst 150

unsigned static int Font[72];
int Char_len=72;
int out_Handle;
int power_of_base2(int);
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
   ET_path=getenv("ET");
   if (getenv("ET")==NULL)
      ET_path="d:\\et5";

   do {
      printf("Choose [1] 15*16 Font, [2] 24*24 Font: ");
      scanf("%d", &font_type);
   }
   while ( font_type!=1 && font_type!=2 );

   if (font_type==1)
      Char_len=30;

   GraphDriver=DETECT;
   initgraph(&GraphDriver, &GraphMode, "d:\\tc\\bgi");


   if ( (out_Handle=open("c:\\garbage\\fontdata.txt", O_WRONLY|O_TRUNC|O_CREAT))==-1) {
      printf("Open Fontdata.txt is Fail!\n");
      exit(1);
   }

   if (font_type==2)
      strcat(ET_path, "\\stdfont.24");
   else
      strcat(ET_path, "\\stdfont.15");
   ET_path_len=strlen(ET_path);
   ET_path[ET_path_len+1]=NULL;
   Std_Handle=open(ET_path, O_RDONLY);

   printf("Key-in Chinese string: ");
   scanf("%s", c_string);
   i=strlen(c_string);
   c_string[i]=NULL;

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
   getch();
   closegraph();
   close(out_Handle);
   close(Std_Handle);
}

/* Show_chinese(): Diskplay Font to Monitor */
void show_chinese(int x_posi, int y_posi)
{
   static int i, j, x, y, k=2, l=0;
   unsigned int eight_bits[8]={ 128, 64, 32, 16, 8, 4, 2, 1 };
   unsigned char One_char;

   x=x_posi;
   y=y_posi;

   if (Char_len==72) { k=3; }

   for (i=0; i<Char_len; i++) {
      for (l=0; l<10000; l++);
      for (j=0; j<8; j++) {
	 if ((Font[i] & eight_bits[j])!=0) putpixel(x, y, Color);
	 x++;
      }
      if ( i%k==(k-1) ) { x=x_posi; y++; }
   }
}

void wrfile(void)
{
   int i, j;
   int H_byte, L_byte;
   unsigned int eight_bits[8]={ 128, 64, 32, 16, 8, 4, 2, 1 };
   static char H_code[3], L_code[3];
   unsigned char One_char;

   H_code[2]=NULL;
   L_code[2]=NULL;
   if (Char_len==30) {
      Font[30]=0; Font[31]=0;
   }

   for (i=0; i<8; i++) {
      H_byte=0;
      L_byte=0;
      for (j=0; j<8; j++) {
	 L_byte+=(((Font[j*2]&eight_bits[i])>>(7-i))*power_of_base2(j));
	 H_byte+=(((Font[(j+8)*2]&eight_bits[i])>>(7-i))*power_of_base2(j));
      }
      itoa(H_byte, H_code, 16);
      itoa(L_byte, L_code, 16);
      while (strlen(H_code)<2) {
	 if (strlen(H_code)>1)
	    strrev(H_code);
	 if (strlen(H_code)<2)
	    strcat(H_code, "0");
	 strrev(H_code);
      }
      while (strlen(L_code)<2) {
	 if (strlen(L_code)>1)
	    strrev(L_code);
	 if (strlen(L_code)<2)
	    strcat(L_code, "0");
	 strrev(L_code);
      }
      write(out_Handle, L_code, 2);
      write(out_Handle, H_code, 2);
      write(out_Handle, "H,", 2);
   }
   write(out_Handle, "\n", 1);
   for (i=0; i<8; i++) {
      H_byte=0;
      L_byte=0;
      for (j=0; j<8; j++) {
	 L_byte+=(((Font[j*2+1]&eight_bits[i])>>(7-i))*power_of_base2(j));
	 H_byte+=(((Font[(j+8)*2+1]&eight_bits[i])>>(7-i))*power_of_base2(j));
      }
      itoa(H_byte, H_code, 16);
      itoa(L_byte, L_code, 16);
      while (strlen(H_code)<2) {
	 if (strlen(H_code)>1)
	    strrev(H_code);
	 if (strlen(H_code)<2)
	    strcat(H_code, "0");
	 strrev(H_code);
      }
      while (strlen(L_code)<2) {
	 if (strlen(L_code)>1)
	    strrev(L_code);
	 if (strlen(L_code)<2)
	    strcat(L_code, "0");
	 strrev(L_code);
      }
      write(out_Handle, L_code, 2);
      write(out_Handle, H_code, 2);
      if (i==7)
	 write(out_Handle, "H", 1);
      else
	 write(out_Handle, "H,", 2);
   }
   write(out_Handle, "\n\n", 2);
}


int power_of_base2(int n)
{
   int i, sum=1;
   for (i=0; i<n; i++)
      sum*=2;
   return(sum);
}