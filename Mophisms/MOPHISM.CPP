// Program : Mophism.cpp -- rotation by col[k+1, k][1, 1]*col[x][y];
// Auther  : Morpheus
// Date    : 98.08.17 

void mophism(int size, int k, unsigned char *Old, unsigned char *New)
{
  long x, y, NewX, NewY;
  for (y=0;  y<size; y++)
  {
    for (x=0; x<size; x++)
    {
      NewX = ((k+1)*x+k*y)%size;
      NewY = (x+y)%size;
      New[NewX+NewY*size] = Old[x+y*size];
    }
  }
}
