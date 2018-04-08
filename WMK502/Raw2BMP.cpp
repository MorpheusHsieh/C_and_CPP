//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Raw2BMP.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
#include <Math.h>
#include <stdio.h>
#define PaletteSize   256
#define BitsPerPixel    8

void Put_BMP_Headers(FILE *, long, long);
void Put_BMP_ColorMap(FILE *);
//---------------------------------------------------------------------------
void Raw_ConvertTo_256BMP(AnsiString Name,Byte *data,long width,long height)
{
  FILE *fout;
  fout=fopen(Name.c_str(),"wb");
  Put_BMP_Headers(fout, width, height);
  Put_BMP_ColorMap(fout);

  long width2;
  ((width/4)*4 == width) ? width2=width : width2=(ceil(width/4)+1)*4;
  Byte *data2 = new Byte [height*width2];
  for (long y=0; y<height; y++)
  {
    for (long x=0; x<width2; x++)
    {
      if (x<width)
        data2[y*width2+x] = data[(height-y-1)*width+x];
      else if (x>=width)
        data2[y*width2+x] = 0;
    }
  }
  fwrite(data2,width2,height,fout);

  // write bfSize BITMAPHEADER
  long eof = ftell(fout);
  fseek(fout,2,SEEK_SET);
  fwrite(&eof,4,1,fout);

  // write biSizeImage of BITMAPINFOHEADER
  fseek(fout,34,SEEK_SET);
  long bmp_size = eof - 1078;    // 0x0E + 0x28 + 0x400;
  fwrite(&bmp_size,4,1,fout);
  fclose(fout);
}
//---------------------------------------------------------------------------
void Put_BMP_Headers(FILE *fptr, long width, long height)
{
  BITMAPFILEHEADER h1;
  BITMAPINFOHEADER h2;

  h1.bfType      = (unsigned short) 'BM';
  h1.bfSize      = 0;  /* set zero */
  h1.bfReserved1 = 0;
  h1.bfReserved2 = 0;
  h1.bfOffBits   = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +
                   (PaletteSize * sizeof(RGBQUAD));
  fwrite(&h1,sizeof(BITMAPFILEHEADER),1,fptr);

  memset(&h2,0,sizeof(BITMAPINFOHEADER));
  h2.biSize = sizeof(BITMAPINFOHEADER);
  h2.biWidth = width;
  h2.biHeight = height;
  h2.biPlanes = 1;  /* Fixed 1 */
  h2.biBitCount = BitsPerPixel;
  h2.biCompression = 0;
  h2.biClrUsed = PaletteSize;
  fwrite(&h2,sizeof(BITMAPINFOHEADER),1,fptr);
}
//---------------------------------------------------------------------------
void Put_BMP_ColorMap(FILE *fptr)
{
  RGBQUAD ColorMap[PaletteSize];
  int i;
  for (i=0; i<PaletteSize; i++)
  {
     ColorMap[i].rgbRed   = (byte)i;
     ColorMap[i].rgbGreen = (byte)i;
     ColorMap[i].rgbBlue  = (byte)i;
  }
  fwrite(ColorMap,sizeof(RGBQUAD),PaletteSize,fptr);
}
//---------------------------------------------------------------------------

