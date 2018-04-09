// Program: 'Radar01' is calculator nonlinear least squares
// Author : Morpheus
// Date   : 98.04.12

#include <math.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#define RAD 360/(2*3.1415926)
#define MaxRadarNum 4
#define MaxArraySize 101
#define TargetX 141
#define TargetY 141
#define InitEstimateX 135
#define InitEstimateY 135

typedef struct {
        double x;
        double y;
        } Radar;

typedef struct {
        int col;
        int row;
        double value;
        } Array;

Array tmp1[MaxArraySize], tmp2[MaxArraySize], tmp3[MaxArraySize];

void OutArray(Array a[]);
void InputSrData(Radar sr[]);
double Determine(Array det[]);
void Inverse(Array a[], Array ia[]);
void Transpose(Array a[], Array ta[]);
void MakeSquare(Array a[], Array SquareA[]);
void Multiply(Array a[], Array b[], Array d[]);

void Estimate(int RadarNo, Radar sr[], Array DeltaX[])
{
   double Beta0, BetaK, Z;
   BetaK = atan( (TargetY-sr[RadarNo].y)/(TargetX-sr[RadarNo].x) ) * RAD;
   Beta0 = atan( (sr[0].y-sr[RadarNo].y)/(sr[0].x-sr[RadarNo].x) ) * RAD;
   Z = Beta0 - BetaK;
   printf("\n\nZ: %e", Z);
//   printf("\n\nBetaK: %f\tBeta0: %f\tZ: %f", BetaK, Beta0, Z);
//   getch();

   Array H[MaxArraySize];
   H[0].row = 1;
   H[0].col = 2;
   H[0].value = 2;

   double U;
   U = (sr[0].y-sr[RadarNo].y)/(sr[0].x-sr[RadarNo].x);
   for (int i=1; i<=H[0].value; i++)
   {
      H[i].row = ((i-1)/H[0].col)+1;
      H[i].col = ((i-1)%H[0].col)+1;
      if (i==1)
         H[i].value =( (-1*U)/((1+U*U)*(sr[0].x-sr[RadarNo].x)) );
      else if (i==2)
         H[i].value =( 1/((1+U*U)*(sr[0].x-sr[RadarNo].x)) );
   }
   printf("\n\nH: ");
   OutArray(H);

   Transpose(H, tmp1);
   printf("\nH^T:");
   OutArray(tmp1);
/*   for (int i=0; i<=tmp1[0].value; i++)
   {
      printf("\n%d Row: %d,\t Col: %d,\tValue: %f",
             i, tmp1[i].row, tmp1[i].col, tmp1[i].value);
   }*/

   Multiply(tmp1, H, tmp2);
   printf("\nH^T * H:\n");
   OutArray(tmp2);
/*   for (int i=0; i<=tmp2[0].value; i++)
   {
      printf("\n%d Row: %d,\t Col: %d,\tValue: %f",
             i, tmp2[i].row, tmp2[i].col, tmp2[i].value);
   }
   getch();*/

   Inverse(tmp2, tmp3);
   printf("\nTmp3:");
   OutArray(tmp3);

   Multiply(tmp3, tmp1, DeltaX);
   printf("\nOld DeltaX: ");
   OutArray(DeltaX);

   for (int i=1; i<=DeltaX[0].value; i++)
      DeltaX[i].value *= Z;
}

void main(void)
{
   clrscr();
   randomize();
   printf("Procedure 'Radar message' is calculator target position");

   Radar sr[MaxRadarNum];
   InputSrData(sr);
//   printf("\n");
//   for (int i=0; i<MaxRadarNum; i++)
//   {
//      printf("\nRadar[%d].X: %4.2f\t", i, sr[i].x);
//      printf("Radar[%d].Y: %4.2f", i, sr[i].y);
//   }
   Array DeltaX[MaxArraySize];
   DeltaX[0].row = 1;
   DeltaX[0].col = 2;
   DeltaX[0].value = 2;

//   for (int i=1; i<MaxRadarNum; i++)
   int i = 1;
   Estimate(i, sr, DeltaX);

   printf("\nDeltaX: ");
   OutArray(DeltaX);

   getch();
}

void InputSrData(Radar sr[])
{
   sr[0].x = InitEstimateX;
   sr[0].y = InitEstimateY;
   sr[1].x = 130;
   sr[1].y = 11;
   sr[2].x = 100;
   sr[2].y = 41;
   sr[3].x = 70;
   sr[3].y = 71;
}

void OutArray(Array a[])
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
         printf("%e ", a[cnt].value);
      }
      printf("]\n");
   }
}

void Transpose(Array a[], Array b[])
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

double Determine(Array det[])
{
   Array tmp[MaxArraySize];

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
         DetMatrix = DetMatrix + sign * det[i].value * Determine(tmp);
      }
   }
   else if (element==1)
      DetMatrix = det[1].value;
   return DetMatrix;
}

void Inverse(Array ia[], Array ib[])
{
   double DetA = Determine(ia);
   printf("\nDetA: %e", DetA);

   Transpose(ia, ib);
   int element = ib[0].value;

   int i;
   for (i=1; i<=element; i++)
   {
      ib[i].value = (ib[i].value/DetA);
//      printf("\nValue: %4.2f,\tDet[A]: %f",ib[i].value, DetA);
   }
}

void Multiply(Array a[], Array b[], Array d[])
{
   if (a[0].col!=b[0].row)
   {
      fprintf(stderr,"Error, row of [A] not equare column of [B].");
      exit(1);
   }

   int i, j;

   Array squ_a[MaxArraySize];
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

   Array squ_b[MaxArraySize];
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

   Array new_b[MaxArraySize];
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

