// Program : 'Matrix Transpose' is to do col(A) <-- row(A).
// Author  : Avatar
// Date    : 98.04.08

typedef struct {
        int col;
        int row;
        double value;
        } term;

void MatrixTranspose(term a[], term b[])
{
   int i, j, cntb;
   double n;
   n = a[0].value;
   b[0].row = a[0].col;
   b[0].col = a[0].row;
   b[0].value = n;
   if ( n>0 )
   {
      cntb = 1;
      for (i=1; i<=b[0].row; i++)
      {
         for (j=1; j<=n; j++)
         {
            if (a[j].col==i)
            {
               b[cntb].row = a[j].col;
               b[cntb].col = a[j].row;
               if ( (((a[j].col+a[j].row)%2)==0) && (a[0].row>1) )
                  b[cntb].value = -1 * a[j].value;
               else
                  b[cntb].value = a[j].value;
               cntb++;
            }
         }
      }
   }
}
