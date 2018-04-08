// Program : A11to81.cpp -- Add B/W picture to 256 color picture
// Author  : Avatar
// Date    : 98.06.02

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <flips.h>
#include <pcx.h>

void main(void)
{
   clrscr();
   printf("Add BlackWhite picture into 256 color picture.\n");

   FILE *fin1, *fin2, *fout;
   char fn[20], infile1[20], infile2[20], outfile[20];

   printf("\nEnter BlackWhite picture filename: ");
   scanf("%s", fn);
   stpcpy(infile1, fn); strcat(infile1, ".mta");
   if ((fin1=fopen(infile1, "rb")) == NULL) {
      printf("\nOpen %s fail...", infile1);
      getch();
      exit(1);
   }

   printf("\nEnter 256 colors picture filename: ");
   scanf("%s", fn);
   stpcpy(infile2, fn); strcat(infile2, ".mta");
   if ((fin2=fopen(infile2, "rb")) == NULL) {
      printf("\nOpen %s fail...", infile2);
      getch();
      exit(1);
   }

   printf("\nEnter encrypt filename: ");
   scanf("%s", fn);
   stpcpy(outfile, fn); strcat(outfile, ".mta");
   if ((fout=fopen(outfile, "wb")) == NULL) {
      printf("\nOpen %s fail...", outfile);
      getch();
      exit(1);
   }

   // Write Meta prefix
   META_PREFIX prefix;

   fread(&prefix, sizeof(META_PREFIX), 1, fin2);
   fwrite(&prefix, sizeof(META_PREFIX), 1, fout);

   // Write Meta color map
   byte buf[769];
   fread(buf, 769, 1, fin2);
   fwrite(buf, 769, 1, fout);

   unsigned long total_bytes1, total_bytes2;
   fseek(fin1, 0L, SEEK_END);
   total_bytes1 = ftell(fin1) - 16;
   fseek(fin2, 0L, SEEK_END);
   total_bytes2 = ftell(fin2) - 128 - 768;
   if (total_bytes1 > total_bytes2) {
      printf("\nB/W picture large than 256 colors picture, Hiding fail...");
      getch();
      exit(1);
   }

   fseek(fin1, 16L, SEEK_SET);
   fseek(fin2, 785L, SEEK_SET);
   byte num, num1, num2;

   printf("\nEncyption...");
   unsigned long count = 0;
   while (count < total_bytes2)
   {
      if (count <= total_bytes1)
      {
         num1 = fgetc(fin1);
         num2 = fgetc(fin2);
         if (num1 == 0x1)
           num = num2 | 0x01;
         else
           num = num2 & 0xFE;
      }
      else {
         num = fgetc(fin2);
      }
//      printf("\n%02x,\t%02x,\%02x", num1, num2, num); getch();
      fputc(num, fout);
      count++;
   }

   fclose(fin1);
   fclose(fin2);
   fclose(fout);
   printf("\nPress any key is over...");
   getch();
}
