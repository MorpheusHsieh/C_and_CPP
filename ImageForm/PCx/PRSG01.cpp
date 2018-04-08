// Program: PRSG01 -- Pseudo-Random-Sequence Generators
// Author : Avatar
// Date   : 98.06.08

#include <math.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

void binout(int bits, unsigned long ch);

void main(void)
{
   clrscr();
   printf("Pseudo-Random-Sequence Generator.\n\n");

   FILE *fout;
   char outfile[20];
   printf("\nEnter output filename: ");
   scanf("%s", outfile);
   if ((fout=fopen(outfile, "wb")) == NULL) {
      printf("\nOpen %s fail...", outfile);
      getch();
      exit(1);
   }

   printf("\nNow, Pseudo random sequence generating...");
   long i, j, PNCode;
   static unsigned long sr = 0x01;
   for (i=0; i<(int)pow(2,10); i++) {
      PNCode = 0x0;
      for (j=31; j>=0; j--) {
         sr = (( ((sr >> 31)^(sr >> 6)^(sr >> 4)^(sr >> 2)^(sr >> 1)^(sr))
                & 0x00000001) << 31) | (sr >> 1);
         PNCode = (((sr & 0x01) << j) | PNCode);
      }
      fwrite(&PNCode,4,1,fout);
//      printf("\nPN Code: ");
//      binout(sizeof(PNCode)*8, PNCode); printf(", %08lx\n", PNCode); getch();
//      if (((i%9) == 0) && (i != 0))
//         printf("\n");  
   }
   fclose(fout);

   printf("\n\nPress any key to exit program...");
   getch();
}

void binout(int bits, unsigned long ch)
{
   for (int i=bits; i>0; i--)
      printf("%d", ((ch >> (i-1)) & 0x01));
}


