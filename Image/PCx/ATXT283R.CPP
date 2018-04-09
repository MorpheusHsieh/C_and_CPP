// Program: ATxt283R -- Add text and Pseudo random sequence generate
// Author : Morpheus
// Date   : 98.06.08

#include <math.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <flips.h>
#include <pcx.h>

void main(void)
{
   clrscr();
   randomize();
   printf("Data spread XOR with Pseudo-Random-Sequence generator.\n\n");

   FILE *fin1, *fin2, *fout1, *fout2;
   char infile1[20], infile2[20], outfile1[20], outfile2[20];

   if ((fin1=fopen("PRSG.dat", "rb")) == NULL) {
      printf("\nOpen PRSG filename error...\n");
      getch();
      exit(1);
   }

   printf("\nEnter True color picture filename: ");
   scanf("%s", infile2); strcat(infile2, ".mta");
   if ((fin2=fopen(infile2, "rb")) == NULL) {
      printf("\nOpen %s filename error...\n", infile2);
      getch();
      exit(1);
   }

   printf("\nEnter output filename: ");
   scanf("%s", outfile1); stpcpy(outfile2, outfile1);
   strcat(outfile1, ".tmp");
   strcat(outfile2, ".mta");
   if ((fout1=fopen(outfile1, "wb")) == NULL) {
      printf("\nOpen %s fail...", outfile1);
      getch();
      exit(1);
   }
   if ((fout2=fopen(outfile2, "wb")) == NULL) {
      printf("\nOpen %s fail...", outfile2);
      getch();
      exit(1);
   }

   int i, j, k, bit;
   char Text[26] = "abcdefghijklmnopqrstuvwxyz";

   printf("\nNow, Processing...\n");
   unsigned long PNCode, offset;
   for (i=0; i<sizeof(Text); i++) {
      for (j=7; j>=0; j--) {
//         printf("\n[%d, %d]\n", i, 7-j);
         fread(&PNCode,4,1,fin1);
         for (k=31; k>=0; k--) {
            bit = ((((PNCode>> k) & 0x01) ^ (Text[i]>>j)) & 0x01);
            offset = (i + (7-j)*32*sizeof(Text) + (31-k)*sizeof(Text));
//            printf("%04d ", offset);
            fseek(fout1, offset, SEEK_SET);
            fputc(bit, fout1);
         }
      }
   }
   fclose(fin1);
   fclose(fout1);

   stpcpy(infile1, outfile1);
   if ((fin1=fopen(infile1, "rb")) == NULL) {
      printf("\nOpen %s filename error...\n", infile1);
      getch();
      exit(1);
   }

   // Write Meta prefix
   META_PREFIX prefix;
   fread(&prefix, sizeof(META_PREFIX), 1, fin2);
   fwrite(&prefix, sizeof(META_PREFIX), 1, fout2);

   // Write Meta color map
   byte color_map_size;
   color_map_size = (byte)fgetc(fin2);
   fputc(color_map_size, fout2);

   unsigned long tbyte1, tbyte2;
   fseek(fin1,0L,SEEK_END);
   tbyte1 = ftell(fin1);
   fseek(fin2,0L,SEEK_END);
   tbyte2 = (ftell(fin2)-17);

   fseek(fin1, 0L,SEEK_SET);
   fseek(fin2,17L,SEEK_SET);
   byte num1, num2[3];

   printf("\nEncyption...");
   unsigned long count = 0;
   while (count < (tbyte2/3))
   {
      if (count <= tbyte1)
      {
         num1 = (char)fgetc(fin1);
         fread(num2,3,1,fin2);
//         printf("\n%02x, %02x, %02x", num2[0], num2[1], num2[2]); getch();
         if (num1 == 0x1)
           num2[0] |= 0x01;
         else
           num2[0] &= 0xFE;
      }
      else {
         fread(num2,3,1,fin2);
      }
//      printf("\n%02x,\t%02x,\%02x", num1, num2, num); getch();
      fwrite(num2,3,1,fout2);
      count++;
   }
   fclose(fin1);
   fclose(fin2);
   fclose(fout2);

   printf("\n\nPress any key to exit program...");
   getch();
}



