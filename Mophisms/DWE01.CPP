// Program : Decode Watermark Hiding
// Author  : Avatar
// Date    : 98.07.06

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <flips.h>
#include <PCX.h>
#define PX  0
#define PY  0

FILE *fin, *fout;
byte MetaData1[1024*1024*3], MetaData2[1024*1024];
COLOR_MAP ColorMap;
META_PREFIX ImagePrefix, WatermarkPrefix;
int iImageWidth, iImageHeight, iWatermarkWidth, iWatermarkHeight;
long lTotalPixel1 = 0, lTotalPixel2 = 0;

void main(void)
{
  clrscr();
  printf("Chaotic mixing of digital image and applications to Watermaerking.");

  char cInFile[20], cOutFile[20];
  printf("\n\nEnetr digital image filename: ");
  scanf("%s", cInFile); strcat(cInFile, ".mta");
  if ((fin=fopen(cInFile, "rb")) == NULL)
  {
     printf("\nOpen %s fail...", cInFile);
     getch();
     exit(1);
  }

  printf("\nEnetr output filename: ");
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
  fread(&ImagePrefix,sizeof(META_PREFIX),1,fin);
  ImagePrefix.mode = 0x01;
  ImagePrefix.id =(byte)(0xFF - ImagePrefix.version - ImagePrefix.mode);
  iImageWidth = ImagePrefix.width;
  iImageHeight = ImagePrefix.height;

  // computing number of pixels
  fseek(fin,0L,SEEK_END);
  lTotalPixel1 = ftell(fin) - sizeof(META_PREFIX) - 769;
  fseek(fin,785L,SEEK_SET);
  fread(MetaData1,lTotalPixel1,1,fin);

  // hiding information
  int x, y;
  char mask = (byte)(0x01 << iColorBit);
  long lCnt = 0;
  for (y=0; y<iImageHeight; y++)
  {
    for (x=0; x<iImageWidth; x++)
    {
      MetaData1[lCnt] = (byte)((MetaData1[lCnt] & mask) >> iColorBit);
      lCnt++;
    }
  }

  // Write meta_prefix
  fwrite(&ImagePrefix,sizeof(META_PREFIX),1,fout);

  // write Metaform pixel data
  fwrite(MetaData1,lTotalPixel1,1,fout);

  fclose(fin);
  fclose(fout);
  printf("\nPress any key exit this program...");
  getch();
}



