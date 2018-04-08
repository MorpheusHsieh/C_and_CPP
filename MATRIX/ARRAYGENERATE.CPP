#include <stdlib.h>
#define MaxRange 11

typedef struct {
        int col;
        int row;
        double value;
        } term;

void ArrayGenerate(term a[])
{
   int n;
   for (int i=1; i<=a[0].row; i++)
   {
      for (int j=1; j<=a[0].col; j++)
      {
         n = ((i-1)*a[0].col) + j;
         a[n].row = i;
         a[n].col = j;
         a[n].value = random(MaxRange);
      }
   }
}