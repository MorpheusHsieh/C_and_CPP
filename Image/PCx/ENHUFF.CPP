// program: EnHuff.cpp -- Compress use Huffman Coding
// Author : Morpheus
// Date   : 98.05.27

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
        float frequency;
        int Lchild;
        int Rchild;
        int Parent;
        } NODE;

FILE *fin, *fout;
NODE table[256+255];                // Record of conversion table is 511
int bit_buf = 0, bit_count =0;

void pass1(void);
void Get_Min2(int recno, int *m0, int *m1);
void pass2(void);
void put_bit(int x);

void main(void)
{
   clrscr();
   printf("Encrypto use Huffman.\n\n");

   char infile[20], outfile[20];
   printf("\nEnter input filename: ");
   scanf("%s", infile);
   if ( (fin=fopen(infile, "rt")) == NULL ) {
      fprintf(stderr, "Can't open input file. Press any key to continue...\n");
      getch();
      exit(1);
   }

   printf("\nEnter output filename: ");
   scanf("%s", outfile);
   if ( (fout=fopen(outfile, "wt")) == NULL ) {
      fprintf(stderr, "Can't open output file. Press any key to continue...\n");
      getch();
      exit(1);
   }

   pass1();
   fseek(fin, 0L, SEEK_SET);            // Go to  the beginning
   pass2();

   fclose(fin);
   fclose(fout);

   printf("\nPress any key to exit this program...");
   getch();
}

void pass1(void)
{
   int i, m0, m1;
   long filesize = 0;

   for (i=0; i<256; i++)
      table[i].frequency = 0;

   while ( (i=fgetc(fin)) != EOF ) {
      table[i].frequency ++;
      filesize++;
   }

   fwrite(&filesize, 4, 1, fout);

   int j=0;
   for (i=0; i<256; i++) {              // if ASCII element never present then
      if ( table[i].frequency == 0 )    // frequency = 2.0
         table[i].frequency = 2.0;
      else {
         if ( (j%3) == 0 ) printf("\n");
         table[i].frequency = table[i].frequency / filesize;
         printf("[%02x] -- %f,\t", i, table[i].frequency);
         j++;
      }
      fwrite(&table[i].frequency, 4, 1, fout);
   }

   i = 256;
   Get_Min2(256, &m0, &m1);
   while (table[m1].frequency <= 1.0 )
   {
      table[i].frequency = table[m0].frequency + table[m1].frequency;
      table[i].Lchild = m0;
      table[i].Rchild = m1;
      table[m0].frequency = 2.0;
      table[m1].frequency = 2.0;
      table[m0].Parent = i;
      table[m1].Parent = i;
      i++;
      Get_Min2(i, &m0, &m1);
   }
   table[i-1].Parent = 0;
}

void Get_Min2(int recno, int *m0, int *m1)
{
   if (table[1].frequency > table[0].frequency) {
      *m0 = 0; *m1 = 1;
   }
   else {
      *m0 = 1; *m1 = 0;
   }

   int i;
   for (i=2; i<recno; i++) {
      if (table[i].frequency < table[*m0].frequency) {
         *m1 = *m0;
         *m0 = i;
      }
      else if (table[i].frequency <table[*m1].frequency)
         *m1 = i;
   }
}

void pass2(void)
{
   int i, j, ptr, old;
   int buf[256], count;

   while ((i = fgetc(fin)) != EOF )
   {
      count = 0;
      ptr = table[i].Parent;
      old = i;
      while (ptr != 0)
      {
         if (table[ptr].Lchild == old)
            buf[count++] = 0;
         else
            buf[count++] = 1;
         old =ptr;
         ptr = table[ptr].Parent;
      }
      for (j=count-1; j>=0; j--)
         put_bit(buf[j]);
   }
   if (bit_count != 0) {
      bit_buf = bit_buf << (8-bit_count);
      fputc(bit_buf, fout);
   }
}

void put_bit(int x)
{
   bit_buf = (bit_buf<<1) | (char)x;
   bit_count ++;
   if (bit_count == 8) {
      fputc(bit_buf, fout);
      bit_buf   = 0;
      bit_count = 0;
   }   
}      







