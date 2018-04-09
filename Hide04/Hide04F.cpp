// Program : Hide04F.CPP
// Author  : Morpheus
// Date    : 98.12.04
#include "Hide04-2.h"
#include "image.h"

typedef struct ZIGZAG {
  int row;
  int col;
} ZigZag;

void Compute_momemt(float **, int, int);
extern void morphism2(int, int, unsigned char*);
extern void Sobel2(unsigned char*, int);
extern void HideBlock_Position(unsigned char*, float**, int);
extern void Zig_Zag(ZigZag*, int);
extern void moment(float*,int,float*,float*,float*,float*,float*,float*);
extern float *vector(long, long);
extern void free_vector(float*, long, long);
extern float ***f3tensor(long, long, long, long, long, long);
extern void free_f3tensor(float***, long, long, long, long,long, long);
extern void rlft3(float***,float**,unsigned long,unsigned long,unsigned long,int);

void main(void)
{
  clrscr();
  cout << "Watermark hiding v2.0.\n";

  // use constructor to cretae new objects
  char fn1[81]; int size1;
  printf("\nEnter input image name: ");
  scanf("%s", fn1);  strcat(fn1,".raw");
  printf("Enter size: ");
  scanf("%d", &size1);
  Image img1(fn1,size1);
  img1.Read_From_File();

  char fn2[81]; int size2;
  printf("\nEnter watermark name: ");
  scanf("%s", fn2);  strcat(fn2,".raw");
  printf("Enter size: ");
  scanf("%d", &size2);
  Image img2(fn2,size2);
  img2.Read_From_File();

  char fn3[81]; int size3;
  printf("\nEnter output image name: ");
  scanf("%s", fn3);  strcat(fn3,".raw");
  printf("Enter image size: ");
  scanf("%d", &size3);
  Image img3(fn3,size3);

  // Automorphism
  int k;
  printf("\nParameter K: ");
  scanf("%d", &k);

  int rtime;
  printf("\nRotation time: ");
  scanf("%d", &rtime);

  ZigZag *zigzag1 = new ZigZag [size2*size2*4];
  ZigZag *zigzag2 = new ZigZag [BS*BS];
  Zig_Zag(zigzag1,size2*2);
  Zig_Zag(zigzag2,BS);

  // Get source image important block
  unsigned char *data1 = new unsigned char [size1*size1];
  img1.Read_Pixel(data1);
  Sobel2(data1,size1);

  int bknum = size3/BS * size3/BS;
  float **lumin;
  lumin = matrix(1,bknum,1,3);
  HideBlock_Position(data1,lumin,size1);
  delete [] data1;

  unsigned char *data2 = new unsigned char [size2*size2];
  img2.Read_Pixel(data2);
  for (int i=0; i<rtime; i++)
    morphism2(size2,k,data2);

  // Watermarked...
  data1 = new unsigned char [size1*size1];
  unsigned char *data3 = new unsigned char [size3*size3];
  img1.Read_Pixel(data1);
  img1.Read_Pixel(data3);

  int u, v, x, y, X1, Y1, X2, Y2;
  int bksize = size2*2;
  bknum = bksize/BS * bksize/BS;

  float ***data = f3tensor(1,1,1,bksize,1,bksize);
  float **speq = matrix(1,1,1,2*bksize);
  float **bk1 = matrix(1,bksize,1,bksize);
  int row = bksize/BS,  col = bksize/BS;

  for (int i=0; i<bknum; i++)
  {
    X1 = lumin[i+1][1]; Y1 = lumin[i+1][2];
    X2 = i/row;     Y2 = i%col;
    for (x=0; x<BS; x++)
    {
      for (y=0; y<BS; y++)
      {
        data[1][X2*BS+x+1][Y2*BS+y+1] =
          (float)(data1[X1*BS*size1+Y1*BS+x*size1+y] - 128);
      }
    }
  }
  rlft3(data,speq,1,bksize,bksize,1);
  for (x=0; x<bksize; x++)
    for (y=0; y<bksize; y++)
      bk1[x+1][y+1] = data[1][x+1][y+1];
  free_matrix(speq,1,1,1,2*bksize);
  free_f3tensor(data,1,1,1,bksize,1,bksize);

  data = f3tensor(1,1,1,BS,1,BS);
  speq = matrix(1,1,1,2*BS);
  float **bk2 = matrix(1,size2,1,size2);
  for (u=0; u<(size2/BS); u++)
  {
    for (v=0; v<(size2/BS); v++)
    {
      for (x=0; x<BS; x++)
        for (y=0; y<BS; y++)
          data[1][x+1][y+1] = (data2[u*BS*size2+v*BS+x*size2+y] - 128);

      rlft3(data,speq,1,BS,BS,1);

      for (x=0; x<BS; x++)
        for (y=0; y<BS; y++)
          bk2[u*BS+x+1][v*BS+y+1] = data[1][x+1][y+1];
    }
  }
  free_matrix(speq,1,1,1,2*BS);
  free_f3tensor(data,1,1,1,BS,1,BS);

  Compute_momemt(bk1,bksize,bksize);
  Compute_momemt(bk2,size2,BS);

  float alpha;
  printf("\n\nAlpha value: ");
  scanf("%f", &alpha);

  data = f3tensor(1,1,1,bksize,1,bksize);
  speq = matrix(1,1,1,2*bksize);
  for (x=0; x<bksize; x++)
    for (y=0; y<bksize; y++)
      data[1][x+1][y+1] = bk1[x+1][y+1];

  float value, avg = 0.0;
  long NewX1, NewY1, NewX2, NewY2, cnt = 0;
  for (u=0; u<(size2/BS); u++)
  {
    for (v=0; v<(size2/BS); v++)
    {
      for (x=0; x<BS; x++)
      {
        for (y=0; y<BS; y++)
        {
          NewX1 = zigzag1[cnt].row + 1;
          NewY1 = zigzag1[cnt].col + 1;
          NewX2 = u*BS + zigzag2[x*BS+y].row + 1;
          NewY2 = v*BS + zigzag2[x*BS+y].col + 1;
          if (x*BS+y == 0) {
            avg += bk2[NewX2][NewY2];
            value = 0.0;
          }
          else if (x*BS+y != 0)
            value = bk2[NewX2][NewY2] * alpha;
          data[1][NewX1][NewY1] = bk1[NewX1][NewY1] + value;
          cnt++;
        }
      }
    }
  }
  rlft3(data,speq,1,bksize,bksize,-1);
  printf("\nDC value: %f", avg / (size2/BS*size2/BS));

  for (int i=0; i<bknum; i++)
  {
    X1 = lumin[i+1][1]; Y1 = lumin[i+1][2];
    X2 = i/row;     Y2 = i%col;
    for (x=0; x<BS; x++)
    {
      for (y=0; y<BS; y++)
      {
        value = (data[1][X2*BS+x+1][Y2*BS+y+1]*2.0/((float)(1*bksize*bksize)));
        data3[X1*BS*size1+Y1*BS+x*size1+y] = (unsigned char)(value + 128.0);
      }
    }
  }
  delete [] data1;
  delete [] data2;
  free_matrix(bk1,1,bksize,1,bksize);
  free_matrix(bk2,1,BS,1,BS);
  free_matrix(speq,1,1,1,2*bksize);
  free_f3tensor(data,1,1,1,bksize,1,bksize);

  img3.Write_Pixel(data3);
  img3.Write_To_File();
  delete [] data3;

  cout << "\n\nPress any key to continue...";
  getch();
}

void Compute_momemt(float **bk, int bksize, int bs)
{
  int u, v, x, y, cnt = 0;
  float *m = vector(1,bksize*bksize);
  for (u=0; u<(bksize/bs); u++)
  {
    for (v=0; v<(bksize/bs); v++)
    {
      for (x=0; x<bs; x++)
      {
        for (y=0; y<bs; y++)
        {
          if (x*bs+y == 0)
            m[cnt] = 0.0;
          else
            m[cnt] = bk[u*bs+x+1][v*bs+y+1];
          cnt++;
        }  
      }
    }
  }
  float ave, adev, sdev, vrnce, skew, curt;
  moment(m,bksize*bksize,&ave,&adev,&sdev,&vrnce,&skew,&curt);
  printf("\nMean1: %.2f,\tSDev1: %.2f,\tVrnce1: %.2f", ave, sdev, vrnce);
  free_vector(m,1,bksize*bksize);
}

