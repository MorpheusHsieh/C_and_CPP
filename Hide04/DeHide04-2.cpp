// Program : Hide04-2.CPP
// Author  : Avatar
// Date    : 98.12.02
#include "Hide04-2.h"
#include "image.h"

extern void morphism2(int, int, unsigned char*);
extern void Sobel2(unsigned char*, int);
extern void HideBlock_Position2(unsigned char*, float**, int);
extern void Fast_DCT(float **, long, long, int);

int main(void)
{
  clrscr();
  cout << "Watermark hiding v2.0.\n";

  // use constructor to cretae new objects
  char fn1[81]; int size1;
  printf("\nEnter source image name: ");
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
  printf("\nEnter watermerked image: ");
  scanf("%s", fn3);  strcat(fn3,".raw");
  printf("Enter image size: ");
  scanf("%d", &size3);
  Image img3(fn3,size3);

  char fn4[81]; int size4;
  printf("\nEnter output filename: ");
  scanf("%s", fn4);  strcat(fn4,".raw");
  printf("Enter image size: ");
  scanf("%d", &size4);
  Image img4(fn4,size4);

  int rtime, k;
  printf("\nParameter K :");
  scanf("%d", &k);

  printf("\nRotation time: ");
  scanf("%d", &rtime);

  float alpha;
  printf("\nAlpha value: ");
  scanf("%f", &alpha);

  // Get source image important block
  unsigned char *data1 = new unsigned char [size1*size1];
  img1.Read_Pixel(data1);
  Sobel2(data1,size1);

  int bknum = size1*size1/BS/BS;
  float **lumin;
  lumin = matrix(1,bknum,1,3);
  HideBlock_Position2(data1,lumin,size1);

  // Get watermarked...
  unsigned char *data2 = new unsigned char [size2*size2];
  unsigned char *data3 = new unsigned char [size3*size3];
  unsigned char *data4 = new unsigned char [size4*size4];
  img1.Read_Pixel(data1);
  img2.Read_Pixel(data2);
  img3.Read_Pixel(data3);

  float mean2 = 0;
  for (int i=0; i<size2*size2; i++)
    mean2 += (float)data2[i];
  mean2 /= (size2*size2);

  float **bk1, **bk2, **bk3;
  bk1 = matrix(1,BS,1,BS);
  bk2 = matrix(1,BS,1,BS);
  bk3 = matrix(1,BS,1,BS);

  float p;
  int i, x, y, X1, Y1, X2, Y2;
  int ncol2 = size2/BS, nrow2 = size2/BS, bknum2 = ncol2*nrow2;
  for (i=0; i<bknum2; i++)
  {
    X1 = lumin[i+1][1]; Y1 = lumin[i+1][2];                 
    X2 = i/nrow2;  Y2 = i%nrow2;
    for (x=0; x<BS; x++)
    {
      for (y=0; y<BS; y++)
      {
        bk1[x+1][y+1] = (float)data1[X1*BS*size1+Y1*BS+x*size1+y];
        bk2[x+1][y+1] = (float)data2[X2*BS*size2+Y2*BS+x*size2+y];
        bk3[x+1][y+1] = (float)data3[X1*BS*size3+Y1*BS+x*size3+y];
      }
    }
    Fast_DCT(bk1,BS,BS,1);
    Fast_DCT(bk2,BS,BS,1);
    Fast_DCT(bk3,BS,BS,1);

    int sign;
    for (x=0; x<BS; x++)
    {
      for (y=0; y<BS; y++)
      {
        if (x*BS+y != 0)
        {
          p = (bk3[x+1][y+1] - bk1[x+1][y+1]) / alpha;
          if (bk2[x+1][y+1] < 0)
            sign = -1;
          else
            sign = 1;
          bk3[x+1][y+1] = fabs(p) * sign;
        }
        else
          bk3[x+1][y+1] = 0.0;
      }
    }
    Fast_DCT(bk3,BS,BS,-1);

    for (x=0; x<BS; x++)
      for (y=0; y<BS; y++)
        data4[X2*BS*size4+Y2*BS+x*size4+y]=(unsigned char)(bk3[x+1][y+1]+mean2);
  }
  delete [] data1;
  delete [] data2;
  delete [] data3;
  free_matrix(bk1,1,BS,1,BS);
  free_matrix(bk2,1,BS,1,BS);
  free_matrix(bk3,1,BS,1,BS);

  for (int i=0; i<rtime; i++)
    morphism2(size4,k,data4);

  img4.Write_Pixel(data4);
  img4.Write_To_File();
  delete [] data4;

  cout << "\n\nPress any key to continue...";
  getch();
}







