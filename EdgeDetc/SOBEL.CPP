// Edge Detection
#include "ImgStrut.h"
#include <math.h>
#define T    128

extern int **imatrix(long, long, long, long);
extern void free_imatrix(int **, long, long, long, long);

void Sobel(Image *img)
{
  int x, y, X, Y, z1, z2, z3, z4, z5, z6, z7, z8, z9;
  int width = Y = img->Width, height = X = img->Height;

  int **data, temp1, temp2;
  data = imatrix(1,height,1,width);
  for (x=0; x<height; x++)
    for (y=0; y<width; y++)
      data[x+1][y+1] = (double)img->pixel[x*width+y];

  for (x=1; x<=height; x++)
  {
    for (y=1; y<=width; y++)
    {
      ((x-1 == 0) || (y-1 == 0)) ? z1 = 0 : z1 =  1 * data[x-1][y-1];
      ((x-1 == 0) || (y   == 0)) ? z2 = 0 : z2 =  2 * data[x-1][y  ];
      ((x-1 == 0) || (y+1 >  Y)) ? z3 = 0 : z3 =  1 * data[x-1][y+1];
      z4 = z5 = z6 = 0;
      ((x+1 >  X) || (y-1 == 0)) ? z7 = 0 : z7 = -1 * data[x+1][y-1];
      ((x+1 >  X) || (y   == 0)) ? z8 = 0 : z8 = -2 * data[x+1][y  ];
      ((x+1 >  X) || (y+1 >  Y)) ? z9 = 0 : z9 = -1 * data[x+1][y+1];
      temp1 = (z1+z2+z3+z4+z5+z6+z7+z8+z9);

      ((x-1 == 0) || (y-1 == 0)) ? z1 = 0 : z1 = -1 * data[x-1][y-1];
      z2 = 0;
      ((x-1 == 0) || (y+1 >  Y)) ? z3 = 0 : z3 =  1 * data[x-1][y+1];
      ((x   == 0) || (y-1 == 0)) ? z4 = 0 : z4 = -2 * data[x  ][y-1];
      z5 = 0;
      ((x   == 0) || (y+1 >  Y)) ? z6 = 0 : z6 =  2 * data[x  ][y+1];
      ((x+1 >  X) || (y-1 == 0)) ? z7 = 0 : z7 = -1 * data[x+1][y-1];
      z8 = 0;
      ((x+1 >  X) || (y+1 >  Y)) ? z9 = 0 : z9 =  1 * data[x+1][y+1];
      temp2 = (z1+z2+z3+z4+z5+z6+z7+z8+z9);
      img->pixel[(x-1)*width+(y-1)] = 255 - pow(temp1*temp1+temp2*temp2, 0.5);
    }
  }
  free_imatrix(data,1,height,1,width);
}
