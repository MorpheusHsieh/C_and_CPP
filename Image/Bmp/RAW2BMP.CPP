// Program: Raw2BMP.CPP
// Author : Morpheus
// Date   : 98.11.20
#include <mem.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include "Flips.h"
#include "BMP.H"
#define MAXSIZE      512
#define PaletteSize  256
#define BitsPerPixel   8

typedef struct {
  FILE *fptr;
  char name[81];
  int  Width;
  int  Height;
  unsigned char pixel[MAXSIZE*MAXSIZE];
} Image;

int Input_Image_info(Image*, int);
void Put_BMP_Headers(Image*);
void Put_BMP_ColorMap(Image*);

void main(void)
{
  clrscr();
  printf("Rraw file convert to BMP image form.\n");

  Image img1, img2;
  Input_Image_info(&img1, 1);  // 1:rb, 2:wb
  Input_Image_info(&img2, 2);

  Put_BMP_Headers(&img2);
  Put_BMP_ColorMap(&img2);

  int i, j, height = img2.Height, width = img2.Width;
  for (i=0; i<height; i++)
    for (j=0; j<width; j++)
      img2.pixel[(height-i-1)*width+j] = img1.pixel[i*width+j];
  fwrite(img2.pixel,height,width,img2.fptr);

  // write bfSize BITMAPHEADER
  int eof = ftell(img2.fptr);
  fseek(img2.fptr,2,SEEK_SET);
  fwrite(&eof,4,1,img2.fptr);

  // write biSizeImage of BITMAPINFOHEADER
  fseek(img2.fptr,34,SEEK_SET);
  int bmp_size = eof - 1078;    // 0x0E + 0x28 + 0x400;
  fwrite(&bmp_size,4,1,img2.fptr);
  fclose(img2.fptr);

  printf("\nPress any key to continue...");
  getch();
}

/*************************************************************************/
int Input_Image_info(Image *img, int sign)
{
  printf("\nEnter filename: ");
  scanf("%s", img->name);
  if (sign == 1)
    strcat(img->name, ".raw");
  else if (sign == 2)
    strcat(img->name, ".bmp");

  int size;
  printf("Enter image size: ");
  scanf("%d", &size);
  img->Width = size;
  img->Height = size;

  switch(sign)
  {
    case  1: if ((img->fptr=fopen(img->name, "rb")) == NULL)
             {
               printf("\nOpen %s failure...", img->name);
               getch();
               return -1;
             }
             else
             {
               fread(img->pixel,size,size,img->fptr);
               fclose(img->fptr);
             }
             break;
    case  2: if ((img->fptr=fopen(img->name, "wb")) == NULL)
             {
               printf("\nOpen %s failure...", img->name);
               getch();
               return -1;
             }
             break;
    default: printf("\nError!!"); getch();
  }
  return 0;
}

void Put_BMP_Headers(Image *img)
{
  BITMAPFILEHEADER h1;
  BITMAPINFOHEADER h2;

  h1.bfType      = (word) 'BM';
  h1.bfSize      = 0;  /* 暫時填入 0 */
  h1.bfReserved1 = 0;
  h1.bfReserved2 = 0;
  h1.bfOffBits   = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +
                   (PaletteSize * sizeof(RGBQUAD));
  fwrite(&h1,sizeof(BITMAPFILEHEADER),1,img->fptr);

  memset(&h2,0,sizeof(BITMAPINFOHEADER));
  h2.biSize = sizeof(BITMAPINFOHEADER);
  h2.biWidth = img->Width;
  h2.biHeight = img->Height;
  h2.biPlanes = 1;  /* 固定為 1 */
  h2.biBitCount = BitsPerPixel;
  h2.biCompression = 0;
  h2.biClrUsed = PaletteSize;
  fwrite(&h2,sizeof(BITMAPINFOHEADER),1,img->fptr);
}

void Put_BMP_ColorMap(Image *img)
{
  RGBQUAD ColorMap[PaletteSize];
  int i;
  for (i=0; i<PaletteSize; i++)
  {
     ColorMap[i].rgbRed   = (byte)i;
     ColorMap[i].rgbGreen = (byte)i;
     ColorMap[i].rgbBlue  = (byte)i;
  }
  fwrite(ColorMap,sizeof(RGBQUAD),PaletteSize,img->fptr);
}

