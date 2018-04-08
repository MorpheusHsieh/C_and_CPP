//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.h>

#include "Main.h"
#include "Sobel.h"
#include "Matrix.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
extern long **lmatrix(long, long, long, long);
extern void free_lmatrix(long**, long, long, long, long);
//---------------------------------------------------------------------------
void Sobel(Byte *pixels,long col, long row)
{
  long x, y, z1, z2, z3, z4, z5, z6, z7, z8, z9;

  long **data, temp1, temp2, temp3;
  data = lmatrix(1,row,1,col);
  for (y=0; y<row; y++)
    for (x=0; x<col; x++)
      data[y+1][x+1] = pixels[y*col+x];

  for (y=1; y<=row; y++)
  {
    for (x=1; x<=col; x++)
    {
      ((x-1 ==  0) || (y-1 ==  0)) ? z1 = 0 : z1 = -1*data[y-1][x-1];
      ((x-1 ==  0) || (y   ==  0)) ? z2 = 0 : z2 = -2*data[y  ][x-1];
      ((x-1 ==  0) || (y+1 > row)) ? z3 = 0 : z3 = -1*data[y+1][x-1];
      z4 = z5 = z6 = 0;
      ((x+1 > col) || (y-1 ==  0)) ? z7 = 0 : z7 =  1*data[y-1][x+1];
      ((x+1 > col) || (y   ==  0)) ? z8 = 0 : z8 =  2*data[y  ][x+1];
      ((x+1 > col) || (y+1 > row)) ? z9 = 0 : z9 =  1*data[y+1][x+1];
      temp1 = (z1+z2+z3+z4+z5+z6+z7+z8+z9);

      ((x-1 ==  0) || (y-1 ==  0)) ? z1 = 0 : z1 = -1*data[y-1][x-1];
      z2 = 0;
      ((x-1 ==  0) || (y+1 > row)) ? z3 = 0 : z3 =  1*data[y+1][x-1];
      ((x   ==  0) || (y-1 ==  0)) ? z4 = 0 : z4 = -2*data[y-1][x  ];
      z5 = 0;
      ((x   ==  0) || (y+1 > row)) ? z6 = 0 : z6 =  2*data[y+1][x  ];
      ((x+1 > col) || (y-1 ==  0)) ? z7 = 0 : z7 = -1*data[y-1][x+1];
      z8 = 0;
      ((x+1 > col) || (y+1 > row)) ? z9 = 0 : z9 =  1*data[y+1][x+1];
      temp2 = (z1+z2+z3+z4+z5+z6+z7+z8+z9);
      temp3 = 255 - pow(temp1*temp1+temp2*temp2,0.5);
      if (temp3 <= 0)
        pixels[(y-1)*col+(x-1)] = 0;
      else if ((temp3 > 0) && (temp3 <= 255))
        pixels[(y-1)*col+(x-1)] = (Byte)temp3;
      else if (temp3 > 255)
        pixels[(y-1)*col+(x-1)] = 255;
    }
  }
  free_lmatrix(data,1,row,1,col);
}
//---------------------------------------------------------------------------
