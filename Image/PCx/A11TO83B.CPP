// Program : A11to81.cpp -- Add B/W picture to 256 color picture
// Author  : Morpheus
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

   printf("\nEnter true colors picture filename: ");
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
   byte color_map_size;
   color_map_size = fgetc(fin2);
   fputc(color_map_size, fout);

   unsigned long total_bytes1, total_bytes2;
   fseek(fin1, 0L, SEEK_END);
   total_bytes1 = ftell(fin1) - 16;
   fseek(fin2, 0L, SEEK_END);
   total_bytes2 = (ftell(fin2) - 17);
   if (total_bytes1 > (total_bytes2/3)) {
      printf("\nB/W picture large than 24-bit colors picture, Hiding fail...");
      getch();
      exit(1);
   }

   fseek(fin1, 16L, SEEK_SET);
   fseek(fin2, 17L, SEEK_SET);
   byte num1, num2[3];

   printf("\nEncyption...");
   unsigned long count = 0;
   while (count < (total_bytes2/3))
   {
      if (count <= total_bytes1)
      {
         num1 = fgetc(fin1);
         fread(num2,3,1,fin2);
         if (num1 == 0x1)
           num2[2] = num2[2] | 0x01;
         else
           num2[2] = num2[2] & 0xFE;
      }
      else {
         fread(num2,3,1,fin2);
      }
//      printf("\n%02x,\t%02x,\%02x", num1, num2, num); getch();
      fwrite(num2,3,1,fout);
      count++;
   }

   fclose(fin1);
   fclose(fin2);
   fclose(fout);
   printf("\nPress any key is over...");
   getch();
}
