// Program: DeHuff.cpp -- Decode Huffman Compress
// Author : Morpheus
// Date   : 98.05.28

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
NODE table[256+255];                    // Record of conversion table is 511
long total_bytes;
int tail;

void pass1(void);
void Get_Min2(int recno, int *m0, int *m1);
void pass2(void);
int get_bit(void);

void main(void)
{
   clrscr();
   char infile[20], outfile[20];
   printf("Decryption Huffman Code.\n\n");

   printf("\nEnter input filename: ");
   scanf("%s", infile);
   if ( (fin=fopen(infile, "rt")) == NULL )
   {
      fprintf(stderr, "Can't open input file.\n");
      getch();
      exit(1);
   }

   printf("\nEnter output filename: ");
   scanf("%s", outfile);
   if ( (fout=fopen(outfile, "wt")) == NULL )
   {
      fprintf(stderr, "Can't ope output file.\n");
      getch();
      exit(1);
   }

   pass1();
   pass2();

   fclose(fin);
   fclose(fout);
   printf("\nPress any key to exit this program...");
   getch();
}

void pass1(void)
{
   int i, m0, m1;

   fread(&total_bytes, 4, 1, fin);
   for (i=0; i<256; i++)
      fread(&table[i].frequency, 4, 1, fin);

   i = 256;
   Get_Min2(256,&m0,&m1);
   while (table[m1].frequency <= 1.0)
   {
      table[i].frequency = table[m0].frequency + table[m1].frequency;
      table[i].Lchild = m0;
      table[i].Rchild = m1;
      table[m0].frequency = 2.0;
      table[m1].frequency = 2.0;
      table[m0].Parent = i;
      table[m1].Parent = i;
      i++;
      Get_Min2(i,&m0,&m1);
   }
   table[i-1].Parent = 0;
   tail = i-1;
}

void Get_Min2(int recno, int *m0, int *m1)
{
   if (table[1].frequency > table[0].frequency) {
      *m0 = 0;
      *m1 = 1;
   }
   else {
      *m0 = 1;
      *m1 = 0;
   }

   int i;
   for (i=2; i<recno; i++)
   {
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
   int ptr;
   long count = 0L;

   while (count <= total_bytes)
   {
      ptr = tail;                       // from ROOT NODE down to traverse
      while ((table[ptr].Lchild != 0) || (table[ptr].Rchild != 0))
      {
         if ( get_bit() == 0)
            ptr = table[ptr].Lchild;
         else
            ptr = table[ptr].Rchild;
      }
      fputc(ptr, fout);
      count++;
   }
}

int get_bit(void)
{
   static bit_count = 0;
   static bit_buf;
   int x;

   if (bit_count == 0)
   {
      bit_buf   = fgetc(fin);
      bit_count = 8;
   }
   x       = bit_buf &0x80;
   bit_buf = bit_buf << 1;
   bit_count--;

   return x;
}

