#include <stdio.h>

typedef struct {
        int col;
        int row;
        double value;
        } term;

void OutArray(term a[])
{
   int col, row, cnt;
   col = a[0].col;
   row = a[0].row;
   for (int i=0; i<row; i++)
   {
      printf("\t[ ");
      for (int j=1; j<=col; j++)
      {
         cnt = (i*col)+j;
         printf("%05.1f ", a[cnt].value);
      }
      printf("]\n");
   }
}