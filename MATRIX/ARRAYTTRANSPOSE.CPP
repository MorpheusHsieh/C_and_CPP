// Program : 'Array Transpose' is to do col(A) <-- row(A).
// Author  : Avatar
// Date    : 98.04.08

#include <math.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_TERMS 101
#define MaxRange  100

typedef struct {
        int col;
        int row;
        double value;
        } term;

term a[MAX_TERMS], b[MAX_TERMS];

void MatrixTranspose(term a[], term b[]);

void ArrayGenerate(term a[])
{
   int n;
   for (int i=0; i<a[0].row; i++)
   {
      for (int j=1; j<=a[0].col; j++)
      {
         n = (i*a[0].col) + j;
         a[n].row = i;
         a[n].col = j;
         a[n].value = random(MaxRange);
      }
   }
}

void OutArray(term a[])
{
   int col, row, cnt;
   col = a[0].col;
   row = a[0].row;
   printf("\nMatrix = ");
   for (int i=0; i<row; i++)
   {
      printf("[ ");
      for (int j=1; j<=col; j++)
      {
         cnt = (i*col)+j;
         printf("%+04.1f ", a[cnt].value);
      }
      printf("]\n         ");
   }
}


void main(void)
{
   clrscr();
   randomize();
   printf("Procedure 'Transpose' is to do col[a1, a2] --> row[a1, a2].\n");

   int row, column;

   do {
      printf("\nKey-in Row of array: ");
      scanf("%d", &row);
   } while ( (row<=0)||(row>10) );

   do {
      printf("Key-in Column of array: ");
      scanf("%d", &column);
   } while ( (column<=0)||(column>10) );

   a[0].row = row;
   a[0].col = column;
   a[0].value = row * column;

   ArrayGenerate(a);
   OutArray(a);
   MatrixTranspose(a, b);
   OutArray(b);

   getch();
}

