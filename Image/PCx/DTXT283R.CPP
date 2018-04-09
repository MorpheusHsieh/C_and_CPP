// Program : DTxt283R.cpp -- Decode Text file picture from True color picture
// Author  : Morpheus
// Date    : 98.06.08

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <flips.h>
#include <pcx.h>

void main(void)
{
   clrscr();
   printf("Decode Text from Ture color picture.\n");

   FILE *fin1, *fin2, *fout1, *fout2;
   char infile[20], outfile1[20], outfile2[20];
   printf("\nEnter true colors picture filename: ");
   scanf("%s", infile); strcat(infile, ".mta");
   if ((fin1=fopen(infile, "rb")) == NULL) {
      printf("\nOpen %s fail...", infile);
      getch();
      exit(1);
   }

   if ((fin2=fopen("Prsg.dat", "rb")) == NULL) {
      printf("\nOpen %s fail...", infile);
      getch();
      exit(1);
   }

   printf("\nEnter Decryption filename: ");
   scanf("%s", outfile1);
   stpcpy(outfile2, outfile1);
   strcat(outfile1, ".tmp");
   strcat(outfile2, ".dat");
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

   fseek(fin1, 17L, SEEK_SET);
   fseek(fout1, 0L, SEEK_SET);
   byte num2[3];

   printf("\nDecyption...");
   byte value;
   long i, j, k;
   long segment, offset, bits, posi;
   unsigned long count = 0;
   while (count < 6656)
   {
      fread(num2,3,1,fin1);
      value = (char)(num2[0] & 0x01);

      segment = count % 26;
      offset = (count / 26) / 32;
      bits = (count / 26) % 32;
      posi = segment*256+offset*32+bits;
//      printf("\n[%04d]: %02d, %02d, %02d, %ld",
//         count, segment+1, offset+1, bits+1, posi);
      fseek(fout1, posi, SEEK_SET);

      if (value == 0x1)
         fputc(0x01, fout1);
      else if (value == 0x0)
         fputc(0x00, fout1);
      count ++;
   }
   fclose(fin1);
   fclose(fout1);

   if ((fin1=fopen(outfile1, "rb")) == NULL) {
      printf("\nOpen %s fail...", infile);
      getch();
      exit(1);
   }

   char ch;
   unsigned long PNCode;
   for (i=0; i<26; i++) {
      for (j=7; j>=0; j--) {
         fread(&PNCode,4,1,fin2);
         for (k=31; k>=0; k--) {
            ch = (char)fgetc(fin1);
            value = (char)(((PNCode >> k) & 0x01) ^ ch);
            if (value == 0x01)
               fputc(0x01, fout2);
            else
               fputc(0x00, fout2);
         }
      }
   }
   fclose(fin1);
   fclose(fin2);
   fclose(fout2);

   if ((fin1=fopen(outfile2, "rb")) == NULL) {
      printf("\Open %s fail...", outfile2);
      getch();
      exit(1);
   }

   int cnt1 = 0, cnt2 = 0;
   value = 0; count = 0;
   printf("\n\nText: ");
   while (count < 6656)
   {
      ch = (char)fgetc(fin1); count++;
      if (ch == 0x01)
         cnt1 ++;

      if (((count%32) == 0) && (count != 0)) {
         cnt2++;
         if (cnt1 > 16)
            value = (char)((value << 1) | 0x01);
         else
            value = (char)((value << 1) | 0x00);
         if (cnt2 == 8) {
            cnt2 = 0;
            printf("%c", value);
            value = 0;
         }
         cnt1 = 0;
      }
   }

   printf("\n\nPress any key is over...");
   getch();
}
