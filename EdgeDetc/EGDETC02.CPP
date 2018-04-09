// Program : EgDetc02.CPP
// Author  : Morpheus
// Date    : 98.11.25
#include "ImgStrut.h"
#include <math.h>
#include <stdlib.h>
#define BKSIZE  8

void Edge_Detection(Image*);
extern int Input_Image_Info(Image*, int);
extern void Sobel(Image *);
extern float **matrix(long, long, long, long);
extern void free_matrix(float **, long, long, long, long);
void Compute_Important_Block(Image*, float **);

void main(void)
{
  clrscr();
  printf("Image edge detection and compute important block .\n");

  Image img1, img2;
  if (!Input_Image_Info(&img1,1)) exit(1);
  if (!Input_Image_Info(&img2,2)) exit(1);

  int br;
  printf("\nHow much block remain ?");
  scanf("%d", &br);

  // duplicable image pixel data to img2
  int i, j, width = img2.Width, height = img2.Height;
  for (i=0; i<height; i++)
    for (j=0; j<width; j++)
      img2.pixel[i*width+j] = img1.pixel[i*width+j];

  // search edge and save to raw file format
  Sobel(&img2);
  fwrite(img2.pixel,height,width,img2.fptr);
  fclose(img2.fptr);

  //Compute important block and save to "EgDetc02.TXT"
  int TBkNum = height / BKSIZE * width / BKSIZE;
  float **lumin;
  lumin = matrix(1,TBkNum,1,3);
  Compute_Important_Block(&img2, lumin);

  FILE *fptr;
  fptr = fopen("EgDetc02.TXT", "wt");
  for (i=0; i<br; i++)
    fprintf(fptr,"%2d %2d %2d\n",
            (int)lumin[i+1][1], (int)lumin[i+1][2], (int)lumin[i+1][3]);
  fclose(fptr);
  free_matrix(lumin,1,TBkNum,1,3);


  printf("\nPress ane key to conitune...");
  getch();
}

void Compute_Important_Block(Image *img, float **lumin)
{
  int u, v, x, y, offs;
  int W = img->Width, H = img->Height;
  int HB = H/BKSIZE, WB = W / BKSIZE;;
  for (u=0; u<HB; u++)
  {
    for (v=0; v<WB; v++)
    {
      lumin[u*WB+v+1][1] = (float)u;
      lumin[u*WB+v+1][2] = (float)v;
      lumin[u*WB+v+1][3] = 0.0;
      for (x=0; x<BKSIZE; x++)
      {
        for (y=0; y<BKSIZE; y++)
        {
          offs = u*BKSIZE*W + v*BKSIZE + x*W + y;
          if (img->pixel[offs] == 255)
            lumin[u*WB+v+1][3] ++;
        }
      }
    }
  }

  int tbn = H/BKSIZE*W/BKSIZE;
  float tmp;
  for (u=0; u<tbn-1; u++)
  {
    for (v=u+1; v<tbn; v++)
    {
      if (lumin[u+1][3] < lumin[v+1][3])
      {
        tmp=lumin[u+1][1]; lumin[u+1][1]=lumin[v+1][1]; lumin[v+1][1]=tmp;
        tmp=lumin[u+1][2]; lumin[u+1][2]=lumin[v+1][2]; lumin[v+1][2]=tmp;
        tmp=lumin[u+1][3]; lumin[u+1][3]=lumin[v+1][3]; lumin[v+1][3]=tmp;
      }
    }
  }
}







