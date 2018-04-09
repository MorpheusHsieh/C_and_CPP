// Program : EdgeDect.CPP
// Author  : Morpheus
// Date    : 98.11.25
#include "ImgStrut.h"
#include <math.h>
#include <stdlib.h>

void Edge_Detection(Image*);
extern int Input_Image_Info(Image*, int);
extern void Sobel(Image *);

void main(void)
{
  clrscr();
  printf("Image edge detection.\n");

  Image img1, img2;
  if (!Input_Image_Info(&img1,1)) exit(1);
  if (!Input_Image_Info(&img2,2)) exit(1);

  int i, j, width = img2.Width, height = img2.Height;
  for (i=0; i<height; i++)
    for (j=0; j<width; j++)
      img2.pixel[i*width+j] = img1.pixel[i*width+j];

  Sobel(&img2);

  fwrite(img2.pixel,height,width,img2.fptr);
  fclose(img2.fptr);

  printf("\nPress ane key to conitune...");
  getch();
}












