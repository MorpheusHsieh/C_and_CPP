// Program : 'Array Determine' is to do dte(A), .
// Author  : Morpheus
// Date    : 98.04.08

#include <math.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_TERMS 101
#define MaxRange   10

typedef struct {
        int col;
        int row;
        double value;
        } term;

void OutArray(term a[]);
void ArrayGenerate(term a[]);
double MatrixDetermine(term det[]);

void main(void)
{
   clrscr();
   randomize();
   printf("Procedure 'Determine' is compute det[A].\n");

   int row, column;
   term a[MAX_TERMS];

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

   ArrayGenerate(a);
   OutArray(a);
   double DetValue = MatrixDetermine(a);
   printf("\nDet[A] = %04.2f", DetValue);

   getch();
}

void ArrayGenerate(term a[])
{
   int i, j, col, row;
   col = a[0].col;
   row = a[0].row;
   a[0].value = col * row;
   for (i=0; i<a[0].row; i++)
   {
      for (j=1; j<=a[0].col; j++)
      {
         a[i*col+j].row = i+1;
         a[i*col+j].col = j;
         a[i*col+j].value = random(MaxRange);
      }
   }
}

void OutArray(term a[])
{
   int i, col, cnt;
   col = a[0].col;
   cnt = a[0].value;

   printf("\nMatrix = ");
   for (i=1; i<=cnt; i++)
   {
      if ( (i%col)==1 || (cnt==1))
         printf("[ ");
      printf("%04.2f ", a[i].value);
//      printf("%d %d %02.1f ", a[i].row, a[i].col, a[i].value);
      if ( (i%col)==0 )
         printf("]\n         ");
   }
}
