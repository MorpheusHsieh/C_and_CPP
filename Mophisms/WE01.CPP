// Program : Watermark Hiding
// Author  : Morpheus
// Date    : 98.07.06

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <flips.h>
#include <PCX.h>
#define PX  0
#define PY  0

FILE *fin1, *fin2, *fout;
byte MetaData1[1024*1024*3], MetaData2[1024*1024];
COLOR_MAP ColorMap;
META_PREFIX ImagePrefix, WatermarkPrefix;
int iImageWidth, iImageHeight, iWatermarkWidth, iWatermarkHeight;
long lTotalPixel1 = 0, lTotalPixel2 = 0;

void main(void)
{
  clrscr();
  printf("Chaotic mixing of digital image and applications to Watermaerking.");

  char cInFile1[20], cInFile2[20], cOutFile[20];
  printf("\n\nEnetr digital image filename: ");
  scanf("%s", cInFile1); strcat(cInFile1, ".mta");
  if ((fin1=fopen(cInFile1, "rb")) == NULL)
  {
     printf("\nOpen %s fail...", cInFile1);
     getch();
     exit(1);
  }

  printf("\n\nEnetr watermark filename: ");
  scanf("%s", cInFile2); strcat(cInFile2, ".mta");
  if ((fin2=fopen(cInFile2, "rb")) == NULL)
  {
     printf("\nOpen %s fail...", cInFile2);
     getch();
     exit(1);
  }

  printf("\n\nEnetr output filename: ");
  scanf("%s", cOutFile); strcat(cOutFile, ".mta");
  if ((fout=fopen(cOutFile, "wb")) == NULL)
  {
     printf("\nOpen %s fail...", cOutFile);
     getch();
     exit(1);
  }

  int iColorBit = -1;
  while (iColorBit < 0 || iColorBit > 7)
  {
    printf("\nEnter bit of color level (0-7): ");
    scanf("%d", &iColorBit);
    if (iColorBit < 0 || iColorBit > 7)
      iColorBit == -1;
  }

  // Get Header
  fread(&ImagePrefix,sizeof(META_PREFIX),1,fin1);
  iImageWidth = ImagePrefix.width;
  iImageHeight = ImagePrefix.height;

  // Write meta_prefix
  fwrite(&ImagePrefix,sizeof(META_PREFIX),1,fout);

  // Read Source Image Color Map
  char cPaletteSize = (byte)fgetc(fin1);
  fread(&ColorMap,3,256,fin1);

  // write Color Map
  fputc(cPaletteSize, fout);
  fwrite(&ColorMap,3,256,fout);

  fread(&WatermarkPrefix,sizeof(META_PREFIX),1,fin2);
  iWatermarkWidth = WatermarkPrefix.width;
  iWatermarkHeight = WatermarkPrefix.height;

  // computing number of pixels
  fseek(fin1,0L,SEEK_END);
  lTotalPixel1 = ftell(fin1) - sizeof(META_PREFIX) - 769;
  fseek(fin1,785L,SEEK_SET);
  fread(MetaData1,lTotalPixel1,1,fin1);

  fseek(fin2,0L,SEEK_END);
  lTotalPixel2 = ftell(fin2) - 16;
  fseek(fin2,16L,SEEK_SET);
  fread(MetaData2,lTotalPixel2,1,fin2);

  // hiding information
  char mask1, mask2;
  int x, y, LSB2;
  long lOffset1, lOffset2;
  mask1 = (byte)(0xFF - (0x01 << iColorBit));
  mask2 = (byte)(0x01 << iColorBit);
  for (y=0; y<iWatermarkHeight; y++)
  {
    for (x=0; x<iWatermarkWidth; x++)
    {
      lOffset1 = (PX + PY * iImageWidth) + (x + y * iImageWidth);
      lOffset2 = x + y * iWatermarkWidth;
      LSB2 = MetaData2[lOffset2];

      if (LSB2 == 0)
        MetaData1[lOffset1] = (byte)(MetaData1[lOffset1] & mask1);
      else if (LSB2 == 1)
        MetaData1[lOffset1] = byte(MetaData1[lOffset1] | mask2);
    }
  }

  // write Metaform pixel data
  fwrite(MetaData1,lTotalPixel1,1,fout);

  fclose(fin1);
  fclose(fin2);
  fclose(fout);
  printf("\nPress any key exit this program...");
  getch();
}  



