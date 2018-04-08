#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

void main(void)
{
   clrscr();
   printf("\nEnter input filename: ");

   FILE *fin;
   char infile[20];
   scanf("%s", infile);
   if ((fin=fopen(infile, "rb")) == NULL) {
      printf("\nError, can't open input file.");
      getch();
      exit(1);
   }

   int ch;
   unsigned long count = 0;
   while ((ch=fgetc(fin)) != EOF)
      count++;
   printf("\nCount1: %ld", count);

   fseek(fin, 0L, SEEK_SET);
   fseek(fin, 0L, SEEK_END);
   count = ftell(fin);
   printf("\nCount2: %ld", count);
   fclose(fin);
   getch();
}      
