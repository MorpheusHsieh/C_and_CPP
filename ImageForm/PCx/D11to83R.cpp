// Program : D11to83.cpp -- Add B/W picture to 256 color picture
// Author  : Avatar
// Date    : 98.06.03

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <flips.h>
#include <pcx.h>

void main(void)
{
   clrscr();
   printf("Decode BlackWhite picture from Ture color picture.\n");

   FILE *fin, *fout;
   char fn[20], infile[20], outfile[20];

   printf("\nEnter true colors picture filename: ");
   scanf("%s", fn);
   stpcpy(infile, fn); strcat(infile, ".mta");
   if ((fin=fopen(infile, "rb")) == NULL) {
      printf("\nOpen %s fail...", infile);
      getch();
      exit(1);
   }

   printf("\nEnter Decryption filename: ");
   scanf("%s", fn);
   stpcpy(outfile, fn); strcat(outfile, ".mta");
   if ((fout=fopen(outfile, "wb")) == NULL) {
      printf("\nOpen %s fail...", outfile);
      getch();
      exit(1);
   }

   // Read Meta prefix of Ture color picture
   META_PREFIX prefix, prefixBW;
   fread(&prefix, sizeof(META_PREFIX), 1, fin);
   
   long image_width, image_height;
   image_width = prefix.width;
   image_height = prefix.height;

   prefixBW.version = 0x10;
   prefixBW.mode = 0x01;
   prefixBW.id = 0xEE;
   prefixBW.width = image_width;
   prefixBW.height = image_height;
   for(int i=0; i<9; i++) prefixBW.reserved[i] = 0x0;
   fwrite(&prefixBW, sizeof(META_PREFIX), 1, fout);

   unsigned long total;
   total = image_width * image_height;

   fseek(fin, 17L, SEEK_SET);
   fseek(fout, 16L, SEEK_SET);
   byte num1, num2[3];

   printf("\nDecyption...");
   byte value;
   unsigned long count = 0;
   while (count < total)
   {
      fread(num2,3,1,fin);
      value = num2[0]&0x01;
//      printf("\n%02x, %02x, %02x, %02d", num2[0], num2[1], num2[2], value); getch();
      if (value == 0x1)
         fputc(0x01, fout);
      else if (value == 0x0)
         fputc(0x00, fout);
      count++;
   }

   fclose(fin);
   fclose(fout);
   printf("\nPress any key is over...");
   getch();
}
