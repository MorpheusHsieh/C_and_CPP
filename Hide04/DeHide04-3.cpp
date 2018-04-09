// Program : DeHide04-3.CPP
// Author  : Avatar
// Date    : 98.12.07
#include "Hide04-2.h"
#include "image.h"

typedef struct ZIGZAG {
  int row;
  int col;
} ZigZag;

extern void morphism2(int, int, unsigned char*);
extern void Sobel2(unsigned char*, int);
extern void HideBlock_Position(unsigned char*, float**, int);
extern void Fast_DCT(float **, long, long, int);
extern void Zig_Zag(ZigZag*, int);

int main(void)
{
  clrscr();
  cout << "Watermark Dehiding v2.0.\n";

  // use constructor to cretae new objects
  char fn1[81]; int size1;
  printf("\nEnter source image name: ");
  scanf("%s", fn1);  strcat(fn1,".raw");
  printf("Enter size: ");
  scanf("%d", &size1);
  Image img1(fn1,size1);
  img1.Read_From_File();

  char fn2[81]; int size2;
  printf("\nEnter watermerked image: ");
  scanf("%s", fn2);  strcat(fn2,".raw");
  printf("Enter size: ");
  scanf("%d", &size2);
  Image img2(fn2,size2);
  img2.Read_From_File();

  char fn3[81]; int size3;
  printf("\nEnter output filename: ");
  scanf("%s", fn3);  strcat(fn3,".raw");
  printf("Enter size: ");
  scanf("%d", &size3);
  Image img3(fn3,size3);

  int rtime, k;
  printf("\nParameter K: ");
  scanf("%d", &k);

  printf("\nRotation time: ");
  scanf("%d", &rtime);

  float alpha;
  printf("\nAlpha value: ");
  scanf("%f", &alpha);

  float dc;
  printf("\nDC value: ");
  scanf("%f", &dc);

  ZigZag *zigzag1 = new ZigZag [size2*size2*4];
  ZigZag *zigzag2 = new ZigZag [BS*BS];
  Zig_Zag(zigzag1,size2*2);
  Zig_Zag(zigzag2,BS);

  // Get source image important block
  unsigned char *data1 = new unsigned char [size1*size1];
  img1.Read_Pixel(data1);
  Sobel2(data1,size1);

  int bknum1 = size1/BS*size1/BS;
  float **lumin = matrix(1,bknum1,1,3);
  HideBlock_Position(data1,lumin,size1);
  delete [] data1;

  // Get pixel data...
  data1 = new unsigned char [size1*size1];
  unsigned char *data2 = new unsigned char [size2*size2];
  img1.Read_Pixel(data1);
  img2.Read_Pixel(data2);

  // Get hide block pixel and transform to frequency
  int i, x, y, X1, Y1, X2, Y2;
  int bksize=size3*2, row=bksize/BS,  col=bksize/BS, bknum=row*col;
  float **bk1 = matrix(1,bksize,1,bksize);
  float **bk2 = matrix(1,bksize,1,bksize);
  for (i=0; i<bknum; i++)
  {
    X1 = lumin[i+1][1]; Y1 = lumin[i+1][2];
    X2 = i/row        ; Y2 = i%row;
    for (x=0; x<BS; x++)
    {
      for (y=0; y<BS; y++)
      {
        bk1[X2*BS+x+1][Y2*BS+y+1]=(float)(data1[X1*BS*size1+Y1*BS+x*size1+y]-128);
        bk2[X2*BS+x+1][Y2*BS+y+1]=(float)(data2[X1*BS*size2+Y1*BS+x*size2+y]-128);
      }
    }
  }
  Fast_DCT(bk1,bksize,bksize,1);
  Fast_DCT(bk2,bksize,bksize,1);

  // Get watermark frequency
  int u, v;
  float value;
  float **bk3 = matrix(1,size3,1,size3);
  long NewX1, NewY1, NewX2, NewY2, cnt = 0;
  for (u=0; u<(size3/BS); u++)
  {
    for (v=0; v<(size3/BS); v++)
    {
      for (x=0; x<BS; x++)
      {
        for (y=0; y<BS; y++)
        {
          NewX1 = zigzag1[cnt].row + 1;
          NewY1 = zigzag1[cnt].col + 1;
          NewX2 = u*BS + zigzag2[x*BS+y].row + 1;
          NewY2 = v*BS + zigzag2[x*BS+y].col + 1;
          if (x*BS+y == 0.0)
            bk3[NewX2][NewY2] = dc;
          else
          {
            value = (bk2[NewX1][NewY1]-bk1[NewX1][NewY1]);
            bk3[NewX2][NewY2] = value/alpha;
          }    
          cnt++;
        }
      }
    }
  }
  Fast_DCT(bk3,size3,BS,-1);

  unsigned char *data3 = new unsigned char [size3*size3];
  for (x=0; x<size3; x++)
    for (y=0; y<size3; y++)
      data3[x*size3+y]=(unsigned char)(bk3[x+1][y+1]+128.0);

  // Automorphism
  for (int i=0; i<rtime; i++)
    morphism2(size3,k,data3);

  delete [] data1;
  delete [] data2;
  free_matrix(bk1,1,bksize,1,bksize);
  free_matrix(bk2,1,bksize,1,bksize);
  free_matrix(bk3,1,size3,1,size3);

  img3.Write_Pixel(data3);
  img3.Write_To_File();
  delete [] data3;

  cout << "\n\nPress any key to continue...";
  getch();
}







