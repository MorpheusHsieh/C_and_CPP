// Program : Read Bitmap Header
// Author  : Avatar
// Date    : 98.07.03

#include <math.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <flips.h>
#include <bmp.h>

FILE *fin;

void main(void)
{
  clrscr();
  printf("Display header of bitmap image.\n");

  char cInFile[20];
  printf("\nEnter input filename: ");
  scanf("%s", cInFile); strcat(cInFile, ".bmp");
  if ((fin=fopen(cInFile, "rb")) == NULL)
  {
    printf("\nOpen %s fail, exit this program...", cInFile);
    getch();
    exit(1);
  }

  long lTotalBytes;
  fseek(fin,0L,SEEK_END);
  lTotalBytes = ftell(fin);

  fseek(fin,0L,SEEK_SET);
  Bitmap_File_Header BmpFileHeader;
  fread(&BmpFileHeader,sizeof(Bitmap_File_Header),1,fin);
  if ((memcmp(&BmpFileHeader.bfType, "BM", 2) != 0) &&
      (BmpFileHeader.bfSize != lTotalBytes))
  {
    printf("\nNot a BMP file.");
    getch();
    exit(1);
  }

  // Display File Header
  printf("\n\nFile Header---");
  printf("\nFile size             : %ld bytes", BmpFileHeader.bfSize);
  printf("\nBitmap Offset         : %ld(%lxh) bytes"
         , BmpFileHeader.bfOffBits, BmpFileHeader.bfOffBits);

  Bitmap_Info_Header BmpInfoHeader;
  fread(&BmpInfoHeader,sizeof(Bitmap_Info_Header),1,fin);

  // Display Information Header
  printf("\n\nBitmap infomation Header---");
  printf("\nImage Info Header Size: %lu", BmpInfoHeader.biSize);
  printf("\nImage Dimension       : %d x %d",
         BmpInfoHeader.biWidth, BmpInfoHeader.biHeight);
  printf("\nNumber of Planes      : %d", BmpInfoHeader.biPlanes);
  printf("\nBits per Pixel        : %d", BmpInfoHeader.biBitCount);
  printf("\nCompression Scheme    : ");
  if (BmpInfoHeader.biCompression == 0)
     printf("BI_RGB(no compression)");
  else if (BmpInfoHeader.biCompression == 1)
    printf("BI_RLE8");
  else if (BmpInfoHeader.biCompression == 2)
    printf("BI_RLE4");
  printf("\nBitmap size           : %d", BmpInfoHeader.biSizeImage);
  printf("\nX Pixels/Meter        : %d", BmpInfoHeader.biXPerMeter);
  printf("\nY Pixels/Meter        : %d", BmpInfoHeader.biYPerMeter);
  printf("\nColor Used            : %d", BmpInfoHeader.biClrUsed
         ? BmpInfoHeader.biClrUsed : 1 << BmpInfoHeader.biBitCount);
  printf("\nColor Important       : %d", BmpInfoHeader.biClrImportant);

  // Display Color Map
  printf("\n\nPress any key display color index..."); getch();
  if (BmpInfoHeader.biBitCount == 24)
    printf("\n\nTure color mode hasn't color index.");
  else
  {
    RGBQUAD ColorPalette[256];
    int iColorSize = pow(2,BmpInfoHeader.biBitCount);
    fread(&ColorPalette,4,iColorSize,fin);

    printf("\n\nColor Index    B   G   R");
    printf("\n--------------------------");
    for (int i=0; i<pow(2,BmpInfoHeader.biBitCount); i++)
    {
       printf("\n        %3d, %3d %3d %3d ", i, ColorPalette[i].rgbBlue,
               ColorPalette[i].rgbGreen,  ColorPalette[i].rgbRed);
    }
  }
  printf("\n\nPress any key exit this program...");
  getch();
  fclose(fin);
}


