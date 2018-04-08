// Program : 'Array Inverse' is to get Inverse of [A].
// Author  : Avatar
// Date    : 98.04.11

#include <stdio.h>

typedef struct {
        int col;
        int row;
        double value;
        } term;

double MatrixDetermine(term da[]);
void MatrixTranspose(term ta[], term tb[]);

void MatrixInverse(term ia[], term ib[])
{
   double DetA = MatrixDetermine(ia);

   MatrixTranspose(ia, ib);
   int element = ib[0].value;

   int i;
   for (i=1; i<=element; i++)
   {
      ib[i].value = (ib[i].value/DetA);
//      printf("\nValue: %4.2f,\tDet[A]: %f",ib[i].value, DetA);
   }
}