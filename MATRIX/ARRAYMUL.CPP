// Program : 'Array multiply' is to do ma <-- ma *mb.
// Author  : Morpheus
// Date    : 98.04.08

#include <math.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_TERMS 101
#define MaxRange   11

typedef struct {
        int col;
        int row;
        double value;
        } term;

term a[MAX_TERMS], b[MAX_TERMS], d[MAX_TERMS];

void OutArray(term a[]);
void ArrayGenerate(term a[]);
void MatrixTranspose(term a[], term b[]);
void MatrixMultiply(term a[], term b[], term d[]);

void main(void)
{
   clrscr();
   randomize();
   printf("Procedure 'Matrix multiply' is to do ma[] <-- ma[] * mb.\n");

   int row1, col1, row2, col2;

   do {
      printf("\nKey-in row of Array[A]: ");
      scanf("%d", &row1);
   } while ( (row1<=0)||(row1>10) );

   do {
      printf("Key-in column of Array[A]: ");
      scanf("%d", &col1);
   } while ( (col1<=0)||(col1>10) );
   a[0].row = row1;
   a[0].col = col1;
   a[0].value = row1 * col1;

   do {
      printf("\nKey-in row of Array[B]: ");
      scanf("%d", &row2);
   } while ( (row2<=0)||(row2>10) );

   do {
      printf("Key-in column of Array[B]: ");
      scanf("%d", &col2);
   } while ( (col2<=0)||(col2>10) );
   b[0].row = row2;
   b[0].col = col2;
   b[0].value = row2 * col2;

   ArrayGenerate(a);
   printf("\n[A] :");
   OutArray(a);

   ArrayGenerate(b);
   printf("\n[B] :");
   OutArray(b);
   printf("\nPress any key to continue...\n");
   getch();

   MatrixMultiply(a, b, d);
   printf("\n\n[A*B]: ");
   OutArray(d);
   for (int i=0; i<=d[0].value; i++)
   {
      printf("\n%d, Row: %d,\tCol: %d,\tValue: %4.2f",
             i, d[i].row, d[i].col, d[i].value);
   }

   getch();
}



