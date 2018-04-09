// Edge Detection
#include "Hide04-2.h"

void Sobel2(unsigned char *src, int size)
{
  int x, y, X, Y, z1, z2, z3, z4, z5, z6, z7, z8, z9;
  X = Y = size;

  float **data, temp;
  data = matrix(1,size,1,size);
  for (x=0; x<size; x++)
    for (y=0; y<size; y++)
      data[x+1][y+1] = (float)src[x*size+y];

  for (x=1; x<=size; x++)
  {
    for (y=1; y<=size; y++)
    {
      ((x-1 == 0) || (y-1 == 0)) ? z1 = 0 : z1 = -1 * data[x-1][y-1];
      ((x-1 == 0) || (y   == 0)) ? z2 = 0 : z2 = -2 * data[x-1][y  ];
      ((x-1 == 0) || (y+1 >  Y)) ? z3 = 0 : z3 = -1 * data[x-1][y+1];
      z4 = z5 = z6 = 0;
      ((x+1 >  X) || (y-1 == 0)) ? z7 = 0 : z7 =  1 * data[x+1][y-1];
      ((x+1 >  X) || (y   == 0)) ? z8 = 0 : z8 =  2 * data[x+1][y  ];
      ((x+1 >  X) || (y+1 >  Y)) ? z9 = 0 : z9 =  1 * data[x+1][y+1];
      temp = (z1+z2+z3+z4+z5+z6+z7+z8+z9);

      ((x-1 == 0) || (y-1 == 0)) ? z1 = 0 : z1 = -1 * data[x-1][y-1];
      z2 = 0;
      ((x-1 == 0) || (y+1 >  Y)) ? z3 = 0 : z3 =  1 * data[x-1][y+1];
      ((x   == 0) || (y-1 == 0)) ? z4 = 0 : z4 = -2 * data[x  ][y-1];
      z5 = 0;
      ((x   == 0) || (y+1 >  Y)) ? z6 = 0 : z6 =  2 * data[x  ][y+1];
      ((x+1 >  X) || (y-1 == 0)) ? z7 = 0 : z7 = -1 * data[x+1][y-1];
      z8 = 0;
      ((x+1 >  X) || (y+1 >  Y)) ? z9 = 0 : z9 =  1 * data[x+1][y+1];
      temp += (z1+z2+z3+z4+z5+z6+z7+z8+z9);
      if (temp <=  T)
        src[(x-1)*size+(y-1)] = 0;
      else
        src[(x-1)*size+(y-1)] = 255;
    }
  }
  free_matrix(data,1,size,1,size);
}
