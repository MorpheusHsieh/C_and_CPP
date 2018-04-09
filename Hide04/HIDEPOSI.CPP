#include "image.h"

void HideBlock_Position(Image *img, int **lumin)
{
  int u, v, x, y;
  int width = img->Width, height = img->Height;
  int ncol = width/BS, nrow = height/BS;
  for (u=0; u<nrow; u++)
  {
    for (v=0; v<ncol; v++)
    {
      lumin[u*ncol+v+1][1] = u;
      lumin[u*ncol+v+1][2] = v;
      lumin[u*ncol+v+1][3] = 0;
      for (x=0; x<BS; x++)
      {
        for (y=0; y<BS; y++)
        {
          if (img->Pixel[u*BS+x][v*BS+y] == 255)
            lumin[u*ncol+v+1][3] ++;
        }
      }
    }
  }

  int bknum = height/BS*width/BS;
  int i, j, tmp;
  for (i=0; i<bknum-1; i++)
  {
    for (j=i+1; j<bknum; j++)
    {
      if (lumin[i][3] < lumin[j][3])
      {
        tmp=lumin[i][1]; lumin[i][1]=lumin[j][1]; lumin[j][1]=tmp;
        tmp=lumin[i][2]; lumin[i][2]=lumin[j][2]; lumin[j][2]=tmp;
        tmp=lumin[i][3]; lumin[i][3]=lumin[j][3]; lumin[j][3]=tmp;
      }
    }
  }
}

