//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.h>

#include "Fast_DCT.h"
#include "Vector.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
extern void cosft2(float*, int, int);
extern float *vector(long, long);
extern void free_vector(float*, long, long);
//---------------------------------------------------------------------------
void Fast_DCT(float **data, long IS, long BS, int isign)
{
  long u, v, x, y;
  float *rawdata, value;
  rawdata = vector(1,BS);

  for (u=0; u<(IS/BS); u++)
  {
    for (v=0; v<(IS/BS); v++)
    {
      for (x=1; x<=BS; x++)  // row fast discreate cosine transform
      {
        for (y=1; y<=BS; y++)
          rawdata[y] = data[u*BS+x][v*BS+y];
        cosft2(rawdata,BS,isign);
        for (y=1; y<=BS; y++)
          data[u*BS+x][v*BS+y] = rawdata[y];
      }

      for (y=1; y<=BS; y++)    // Column fast discrete cosine transform
      {
        for (x=1; x<=BS; x++)
          rawdata[x] = data[u*BS+x][v*BS+y];
        cosft2(rawdata,BS,isign);
        for (x=1; x<=BS; x++)
        {
          data[u*BS+x][v*BS+y] = rawdata[x];
          if (isign == -1)
          {
            value = data[u*BS+x][v*BS+y] * pow(2,2) / pow(BS,2);
            data[u*BS+x][v*BS+y] = value;
          }
        }
      }
    }
  }
  free_vector(rawdata,1,BS);
}
//---------------------------------------------------------------------------

