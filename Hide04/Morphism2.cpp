// Program : Morphism2.cpp -- rotation by col[k+1, k][1, 1]*col[x][y];
// Auther  : Morpheus
// Date    : 98.12.03
#include "Hide04-2.h"

void morphism2(int size, int k, unsigned char *data)
{
  unsigned char *tmp = new unsigned char[size*size];
  long x, y, NewX, NewY;
  for (x=0;  x<size; x++)
  {
    for (y=0; y<size; y++)
    {
      NewX = ((k+1)*x+k*y)%size;
      NewY = (x+y)%size;
      tmp[NewX*size+NewY] = data[x*size+y];
    }
  }
  memcpy(data,tmp,size*size);
  delete [] tmp;
}
