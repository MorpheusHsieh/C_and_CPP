// PRogram : Hiding01 -- watermarking in difference between src and JPEG-compres
// Author  : Morpheus
// Date    : 98.07.21

#include <math.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "flips.h"
#include "PCX.h"
#define SRCSIZE   512
#define LOGOSIZE  160
#define BLOCKSIZE   8

void Write_PCX_Header(void);
void Encode_PCX81(void);
void Write_PCX_ColorPlatte(void);
void Calculator_Block_Pixel(void);

FILE *fin1, *fin2, *fin3, *fout, *fout2;
int iThreshold;
unsigned char RawData[512*512], DifData[512*512], LogoData[512*512], NewData[512*512];
long lTotalPixel, lWmkPixel;
static unsigned char PixelNum[64][64];


void main(void)
{
  clrscr();
  printf("Watermarking in DC value pixel after JPEG-compression.\n");

  int bit_posi;
  printf("\nEnter hiding bit position (0-7): ");
  scanf("%d", &bit_posi);

  char cRawFile[20], cDifFile[20], cLogoFile[20], cOutFile[20], cPCXFile[20];
  printf("\nEnter source picture filename (RAW): ");
  scanf("%s", cRawFile);
  strcat(cRawFile, ".raw");
  if ((fin1=fopen(cRawFile, "rb")) == NULL)
  {
    printf("\nOpen %s fail...", cRawFile);
    getch();
    exit(1);
  }

  printf("\nEnter difference picture filename (RAW): ");
  scanf("%s", cDifFile);
  strcat(cDifFile, ".raw");
  if ((fin2=fopen(cDifFile, "rb")) == NULL)
  {
    printf("\nOpen %s fail...", cDifFile);
    getch();
    exit(1);
  }

  printf("\nEnter logo filename (MTA): ");
  scanf("%s", cLogoFile);
  strcat(cLogoFile, ".mta");
  if ((fin3=fopen(cLogoFile, "rb")) == NULL)
  {
    printf("\nOpen %s fail...", cLogoFile);
    getch();
    exit(1);
  }

  printf("\nEnter output filename (RAW): ");
  scanf("%s", cOutFile);
  strcpy(cPCXFile, cOutFile);
  strcat(cOutFile, ".raw");
  if ((fout=fopen(cOutFile, "wb")) == NULL)
  {
    printf("\nOpen %s fail...", cOutFile);
    getch();
    exit(1);
  }

  strcat(cPCXFile, ".pcx");
  if ((fout2=fopen(cPCXFile, "wb")) == NULL)
  {
    printf("\nOpen %s fail...", cPCXFile);
    getch();
    exit(1);
  }

  printf("\nEnter threshold value (0-7): ");
  scanf("%d", &iThreshold);

  printf("\nNow, Processing...");
  // Read Raw data file 1, 2 and 3
  fread(RawData,SRCSIZE,SRCSIZE,fin1);
  lTotalPixel = ftell(fin1);
  fclose(fin1);

  fread(DifData,SRCSIZE,SRCSIZE,fin2);
  fclose(fin2);

  fseek(fin3,16L,SEEK_SET);
  fread(LogoData,LOGOSIZE,LOGOSIZE,fin3);
  lWmkPixel = ftell(fin3) - 16;
  fclose(fin3);

  // Calculator number of each 8*8 pixel block which equ 'zero'
  Calculator_Block_Pixel();

  // Hiding information
  char mask;
  int x, y, u, v, iCnt = 0, iCnt2, iThreshold2;
  int BlockNum = (SRCSIZE / BLOCKSIZE); // single edge block number
  int PixelPerBlock = (int)(pow(LOGOSIZE,2) / pow(BlockNum,2)) + 1;
  long offset;

  for (v=0; v<BlockNum; v++)
  {
    for (u=0; u<BlockNum; u++)
    {
      iThreshold2 = iThreshold;
      if (PixelNum[u][v] < PixelPerBlock)
      {
        int taggle = 0, iZeroNum;
        while (taggle == 0)
        {
          iZeroNum = 0;
          iThreshold2++;
          for (y=0; y<BLOCKSIZE; y++)
          {
            for (x=0; x<BLOCKSIZE; x++)
            {
              offset = u*BLOCKSIZE + (v*BLOCKSIZE)*SRCSIZE + x + y*SRCSIZE;
              if (DifData[offset] <= iThreshold2)
                iZeroNum++;
            }
          }
          if (iZeroNum >= PixelPerBlock)
            taggle = 1;
        }
//        if (iThreshold2 != 0)
//        {
//          printf("\n%d, %d, %d", u, v, iThreshold2); getch();
//        }
      }

      iCnt2 = 0;
      for (y=0; y<BLOCKSIZE; y++)
      {
        for (x=0; x<BLOCKSIZE; x++)
        {
          offset = u*BLOCKSIZE + (v*BLOCKSIZE)*SRCSIZE + x + y*SRCSIZE;
          if ((iCnt2 < PixelPerBlock) && (iCnt < lWmkPixel))
          {
            if (DifData[offset] <= iThreshold2)
            {
              if (LogoData[iCnt] == 0x00)
              {
                mask = (char)(0xFF - (0x01 << bit_posi));
                NewData[offset] = (char)(RawData[offset] & mask);
              }
              else if (LogoData[iCnt] == 0x01)
              {
                mask = (char)(0x01 << bit_posi);
                NewData[offset] = (char)(RawData[offset] | mask);
              }
              iCnt++; iCnt2 ++;
            }
            else if (DifData[offset] > iThreshold2)
            {
              NewData[offset] = RawData[offset];
            }
          }
          else if ((iCnt2 >= PixelPerBlock) || (iCnt >= lWmkPixel))
          {
            NewData[offset] = RawData[offset];
          }
        }
      }
/*      else if (iCnt >= lWmkPixel)
      {
        for (y=0; y<BLOCKSIZE; y++)
        {
          for (x=0; x<BLOCKSIZE; x++)
          {
            offset = u*BLOCKSIZE + (v*BLOCKSIZE)*SRCSIZE + x + y*SRCSIZE;
            NewData[offset] = RawData[offset];
          }
        }
      }*/
    }
  }

  printf("\nWatermark pixel: %ld", lWmkPixel);
  printf("\nHiding pixel: %ld", iCnt);

  fwrite(NewData,SRCSIZE,SRCSIZE,fout);
  fclose(fout);

  // Make PCX form File
  Write_PCX_Header();
  Encode_PCX81();
  Write_PCX_ColorPlatte();

  fclose(fout2);
  printf("\nPress any key exit this program...");
  getch();
}

void Write_PCX_Header(void)
{
  PCX_Header header;
  header.Manufacturer = 0x0A;
  header.VersionNo    = 5;
  header.Encoding     = 1;
  header.BitsPerPixel = 8;
  header.Xmin         = 0;
  header.Ymin         = 0;
  header.Xmax         = 511;
  header.Ymax         = 511;
  header.HResolution  = 0x00;
  header.VResolution  = 0x00;
  memset(header.HeaderPalette,0,48);
  header.reserved     = 0x00;
  header.Planes       = 0x01;
  header.BytesPerLine = 512;
  header.PaletteType  = 2;
  header.HScreenSize  = 0;
  header.VScreenSize  = 0;
  memset(header.filler,0,54);
  fwrite(&header, sizeof(PCX_Header),1,fout2);
}

void Encode_PCX81(void)
{
  char cOld, cNew;
  long lCnt = 0, lRepeatCnt = 1;
  cOld = NewData[lCnt++];
  while (lCnt <= lTotalPixel)
  {
    cNew = NewData[lCnt++];
    while (cOld == cNew)
    {
      lRepeatCnt++;
      cNew = NewData[lCnt++];
    }

    if (lRepeatCnt == 1)
    {
      if ((cOld & 0xFF) < 0xC0)
        fputc(cOld, fout2);
      else
      {
        fputc(0xC1, fout2);
        fputc(cOld, fout2);
      }
    }
    else if (lRepeatCnt > 1)
    {
      if (lRepeatCnt <= 63)
      {
        fputc(0xC0+lRepeatCnt, fout2);
        fputc(cOld, fout2);
      }
      else if (lRepeatCnt > 63)
      {
        while (lRepeatCnt > 63)
        {
          lRepeatCnt -= 63;
          fputc(0xFF, fout2);
          fputc(cOld, fout2);
        }
        fputc(0xC0+lRepeatCnt, fout2);
        fputc(cOld, fout2);
      }
    }
    cOld = cNew; lRepeatCnt = 1;
  }
}

void Write_PCX_ColorPlatte(void)
{
  COLOR_MAP ColorMap;
  for (int i=0; i<256; i++)
  {
    ColorMap.color[i].R = (byte)i;
    ColorMap.color[i].G = (byte)i;
    ColorMap.color[i].B = (byte)i;
  }

  fputc(0x0C, fout2);
  fwrite(ColorMap.color,3,256,fout2);
}

void Calculator_Block_Pixel(void)
{
  long lOffset;
  int u, v, x, y;
  printf("\n");
  for (v=0; v<(SRCSIZE/BLOCKSIZE); v++)
  {
    for (u=0; u<(SRCSIZE/BLOCKSIZE); u++)
    {
      for (y=0; y<BLOCKSIZE; y++)
      {
        for (x=0; x<BLOCKSIZE; x++)
        {
          lOffset = (u * 8) + ((v * 8) * 512) + x + y * 512;
          if (DifData[lOffset] <= iThreshold )
            PixelNum[u][v]++;
        }
      }
//      printf("%2d, ", PixelNum[u][v]);
//      if ((u % 16) == 15)
//        printf("\n");
    }
//    printf("\n");getch();
  }
}
