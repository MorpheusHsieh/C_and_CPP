#include "Hide04-2.h"

void HideBlock_Position(unsigned char *src, float **lumin, int size)
{
  int u, v, x, y;
  int ncol = size/BS, nrow = size/BS;
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
          if (src[u*BS*size+v*BS+x*size+y] == 255)
            lumin[u*ncol+v+1][3] ++;
        }
      }
    }
  }

  int bknum = size/BS*size/BS;
  int i, j, tmp;
  for (i=1; i<bknum; i++)
  {
    for (j=i+1; j<bknum+1; j++)
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

