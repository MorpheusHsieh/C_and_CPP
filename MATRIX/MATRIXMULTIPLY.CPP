// Program : 'Array multiply' is to do ma <-- ma * mb.
// Author  : Avatar
// Date    : 98.04.08

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_TERMS 101

typedef struct {
        int col;
        int row;
        double value;
        } term;

void OutArray(term a[]);
void MakeSquare(term a[], term squ_a[]);

void MatrixMultiply(term a[], term b[], term d[])
{
   if (a[0].col!=b[0].row)
   {
      fprintf(stderr,"Error, row of [A] not equare column of [B].");
      exit(1);
   }

   int i, j;

   term squ_a[MAX_TERMS];
   if (a[0].row!=a[0].col)
   {
      MakeSquare(a, squ_a);
   }
   else if (a[0].row==a[0].col)
   {
      for (i=0; i<=a[0].value; i++)
      {
         squ_a[i].row = a[i].row;
         squ_a[i].col = a[i].col;
         squ_a[i].value = a[i].value;
      }
   }

   term squ_b[MAX_TERMS];
   if (b[0].row!=b[0].col)
   {
      MakeSquare(b, squ_b);
   }
   else if (b[0].row==b[0].col)
   {
      for (i=0; i<=b[0].value; i++)
      {
         squ_b[i].row = b[i].row;
         squ_b[i].col = b[i].col;
         squ_b[i].value = b[i].value;
      }
   }
/*   for (i=0; i<=squ_b[0].value; i++)
   {
      printf("\n%d, Row: %d,\tCol: %d,\tValue: %4.2f",
             i, squ_b[i].row, squ_b[i].col, squ_b[i].value);
   }*/

   term new_b[MAX_TERMS];
   double n = squ_b[0].value;
   new_b[0].row = squ_b[0].col;
   new_b[0].col = squ_b[0].row;
   new_b[0].value = n;
   if ( n>0 )
   {
      int col = squ_b[0].col;
      int row = squ_b[0].row;
      for (i=1; i<=row; i++)
      {
         for (j=1; j<=col; j++)
         {
            new_b[(i-1)*col+j].row = squ_b[(j-1)*col+i].row;
            new_b[(i-1)*col+j].col = squ_b[(j-1)*col+i].col;
            new_b[(i-1)*col+j].value = squ_b[(j-1)*col+i].value;
         }
      }
   }
   printf("\nTNB: ");
   OutArray(new_b);
   getch();
/*   for (i=0; i<=new_b[0].value; i++)
   {
      printf("\n%d, Row: %d,\tCol: %d,\tValue: %4.2f",
             i, new_b[i].row, new_b[i].col, new_b[i].value);
   }*/

   d[0].col = a[0].row;
   d[0].row = b[0].col;
   d[0].value = a[0].row * b[0].col;

   int cnt = 0;
   double sum;
   for (i=1; i<=a[0].row; i++)
   {
      int row = 1;
      while (row<=b[0].col)
      {
         sum = 0; cnt++;
         d[cnt].row = i;
         d[cnt].col = row;
         for (j=1; j<=b[0].col; j++)
         {
            int cnt_a = (i-1)*new_b[0].col + j;
            int cnt_b = (row-1)*new_b[0].col + j;
            sum += squ_a[cnt_a].value * new_b[cnt_b].value;
//            printf("\nA[%d]: %4.2f,\tB[%d]: %4.2f,\tSum: %4.2f",
//                   cnt_a, a[cnt_a].value, cnt_b, new_b[cnt_b].value, sum);
         }
         d[cnt].row = i;
         d[cnt].col = row;
         d[cnt].value = sum;
//         printf("\n%d Row: %d,\tCol: %d,\tValue: %f",
//                cnt, d[cnt].row, d[cnt].col, d[cnt].value);
         row++;
      }
   }
}

void MakeSquare(term a[], term SquareA[])
{
   int i;
   int col_a = a[0].col;
   int row_a = a[0].row;

   int SingleSize = col_a;
   if (row_a>col_a)
      SingleSize = row_a;

   SquareA[0].col = SingleSize;
   SquareA[0].row = SingleSize;
   SquareA[0].value = SingleSize * SingleSize;
   for (i=1; i<=SquareA[0].value; i++)
      SquareA[i].value = 0;

   int cnt = 1;
   for (int row=1; row<=SingleSize; row++)
   {
      for (int col=1; col<=SingleSize; col++)
      {
         SquareA[cnt].row = row;
         SquareA[cnt].col = col;
         cnt++;
      }
   }

   for (i=1; i<=a[0].value; i++)
   {
      int col = a[i].col;
      int row = a[i].row;
      SquareA[(row-1)*SingleSize+col].col = col;
      SquareA[(row-1)*SingleSize+col].row = row;
      SquareA[(row-1)*SingleSize+col].value = a[i].value;
   }
}