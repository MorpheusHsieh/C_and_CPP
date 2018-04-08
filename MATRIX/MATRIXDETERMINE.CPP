// Program : 'Array Determine' is calculator dte(A), .
// Author  : Avatar
// Date    : 98.04.08

#include <stdio.h>
#include <stdlib.h>
#define MAX_TERMS 101

typedef struct {
        int col;
        int row;
        double value;
        } term;

double MatrixDetermine(term det[])
{
   term tmp[MAX_TERMS];

   int col, row, element;
   double DetMatrix = 0;
   col = det[0].col;
   row = det[0].row;
   element = det[0].value;

   int i, j, sign = -1;
   if ( (col!=row) )
   {
      printf("Array isn't a Square Matrix.");
      exit(1);
   }
   else if ( (col==row)&&(element!=1) )
   {
      tmp[0].col = col - 1;
      tmp[0].row = row - 1;

      for (i=1; i<=col; i++)
      {
         int cnt = 0;
         for (j=1; j<=element; j++)
         {
            if ( (det[j].row>1)&&(det[j].col!=i) )
            {
               cnt++;
               if (det[j].col>i)
                  tmp[cnt].col = det[j].col-1;
               else
                  tmp[cnt].col = det[j].col;
               tmp[cnt].row = det[j].row-1;
               tmp[cnt].value = det[j].value;
            }
         }
         tmp[0].value = cnt;
         sign *= -1;
         DetMatrix = DetMatrix + sign * det[i].value * MatrixDetermine(tmp);
      }
   }
   else if (element==1)
      DetMatrix = det[1].value;
   return DetMatrix;
}