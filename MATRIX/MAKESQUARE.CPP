// program: 'MakeSquare" is to do NonSquarematrix trans SquareMatrix
// Author : Avatar
// Date   : 98.04.12

#include <stdio.h>

typedef struct {
        int col;
        int row;
        double value;
        } Array;

void MakeSquare(Array a[], Array SquareA[])
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
   for (i=1; i<=a[0].value; i++)
   {
      int col = a[i].col;
      int row = a[i].row;
      SquareA[(row-1)*SingleSize+col].col = col;
      SquareA[(row-1)*SingleSize+col].row = row;
      SquareA[(row-1)*SingleSize+col].value = a[i].value;
      printf("\nA[%d], Row: %d,\tCol: %d,\Value: %4.2f",
         i, SquareA[i].row, SquareA[i].col, SquareA[i].value);
   }
}