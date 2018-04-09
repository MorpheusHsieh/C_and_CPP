// Program : Hide04.CPP
// Author  : Avatar
// Date    : 98.11.26
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "image.h"

extern int Input_Image_Info(Image*, int);
extern void HideBlock_Position(Image*, int**);
extern int **imatrix(long, long, long, long);
extern void free_imatrix(int**, long, long, long, long);
extern float **matrix(long, long, long, long);
extern void free_matrix(float**, long, long, long, long);
extern void Sobel(Image*);
extern void Fast_DCT(float **, long, long, int);
extern void morphism(int, int, unsigned char*, unsigned char*);

void main(void)
{
  clrscr();
  printf("Watermarking into edge block.\n");

  Image img1, img2, img3;
  if (!Input_Image_Info(&img1,1)) exit(1);
  if (!Input_Image_Info(&img2,1)) exit(1);
  if (!Input_Image_Info(&img3,2)) exit(1);

  int k;
  printf("\nEnter k value: ");
  scanf("%d", &k);

  int rt;  // rotation time
  printf("\nEnter rotation time: ");
  scanf("%d", &rt);

  float alpha;
  printf("\nEnter scale value: ");
  scanf("%f", &alpha);

  // Automorphism
  long w2 = img2.Width, h2 = img2.Height;
  unsigned char *tmp1 = new unsigned char [w2*h2];
  unsigned char *tmp2 = new unsigned char [w2*h2];
  memcpy(tmp1,img2.Pixel,w2*h2);
  for (int i=0; i<rt; i++)
  {
    morphism(w2,k,tmp1,tmp2);
    memcpy(tmp1,tmp2,w2*h2);
  }
  memcpy(img2.Pixel,tmp1,w2*h2);
  delete [] tmp1;  delete [] tmp2;

  FILE *ptr;
  ptr = fopen("test.raw", "wb");
  fwrite(img2.Pixel,w2,h2,ptr);
  fclose(ptr);

  // get image important block
  int w1 = img1.Width, h1 = img1.Height;
  int ncol = w1 / BS, nrow = h1 / BS;
  memcpy(img3.Pixel,img1.Pixel,w1*h1);
  Sobel(&img3);

  int bknum = nrow * ncol;
  int **lumin;  // Hide block information
  lumin = imatrix(1,bknum,1,3);
  HideBlock_Position(&img3, lumin);

  // Hidden information
  memcpy(img3.Pixel,img1.Pixel,w1*h1);
  float **block1, **block2;
  block1 = matrix(1,BS,1,BS);
  block2 = matrix(1,BS,1,BS);

  int i, x, y, X1, Y1, X2, Y2;
//  int w2 = img2.Width, h2 = img2.Height;
  int ncol2 = w2/BS, nrow2 = h2/BS, bknum2 = ncol2*nrow2;

  for (i=0; i<bknum2; i++)
  {
    X1 = lumin[i+1][1]; Y1 = lumin[i+1][2];
    X2 = i/nrow2;  Y2 = i%nrow2;
    for (x=0; x<BS; x++)
    {
      for (y=0; y<BS; y++)
      {
        block1[x+1][y+1] = (float)img1.Pixel[X1*BS+x][Y1*BS+y] - 128.0;
        block2[x+1][y+1] = (float)img2.Pixel[X2*BS+x][Y2*BS+y] - 128.0;
      }
    }
    Fast_DCT(block1,BS,BS,1);
    Fast_DCT(block2,BS,BS,1);

    int sign;
    for (x=0; x<BS; x++)
    {
      for (y=0; y<BS; y++)
      {
        if (x*BS+y != 0)
        {
          if (block1[x+1][y+1]*block2[x+1][y+1] < 0)
            sign = -1;
          else
            sign = 1;
          block1[x+1][y+1] += block2[x+1][y+1] * alpha * sign;
        }
      }
    }

    Fast_DCT(block1,BS,BS,-1);

    for (x=0; x<BS; x++)
      for (y=0; y<BS; y++)
        img3.Pixel[X1*BS+x][Y1*BS+y]=(unsigned char)(block1[x+1][y+1]+128.0);
  }

  int w3 = img3.Width, h3 = img3.Height;
  fwrite(img3.Pixel,h3,w3,img3.fptr);
  fclose(img3.fptr);

  printf("\n\nPress any key to continue...");
  getch();
}



