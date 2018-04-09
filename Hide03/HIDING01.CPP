// PRogram : Hiding01 -- watermarking in difference between src and JPEG-compres
// Author  : Morpheus
// Date    : 98.07.21

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "flips.h"
#include "PCX.h"
#define PX         0
#define PY         0
#define SRCSIZE  512
#define LOGOSIZE 160

void Write_PCX_Header(void);
void Encode_PCX81(void);
void Write_PCX_ColorPlatte();

FILE *fin1, *fin2, *fin3, *fout, *fout2;
char RawData[512*512], DifData[512*512], LogoData[512*512], NewData[512*512];
long lTotalPixel;

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

  printf("\nNow, Processing...");
  // Read Raw data file 1, 2 and 3
  fread(RawData,SRCSIZE,SRCSIZE,fin1);
  lTotalPixel = ftell(fin1);
  fclose(fin1);

  fread(DifData,SRCSIZE,SRCSIZE,fin2);
  fclose(fin2);

  fseek(fin3,16L,SEEK_SET);
  fread(LogoData,LOGOSIZE,LOGOSIZE,fin3);
  fclose(fin3);

  char mask;
  long i, lCnt = 0;
  long offset = PX + PY * SRCSIZE;
  for (i=0; i<lTotalPixel; i++)
  {
    if (i >= offset)
    {
      if (DifData[i] == 0)
      {
        if (LogoData[lCnt] == 0)
        {
          mask = (byte)(0xFF - (0x01 << bit_posi));
          NewData[i] = RawData[i] & mask;
        }
        else if (LogoData[lCnt] == 1)
        {
          mask = (byte)(0x01 << bit_posi);
          NewData[i] = RawData[i] | mask;
        }
        lCnt++;
      }
      else
        NewData[i] = RawData[i];
    }
    else
      NewData[i] = RawData[i];
  }

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

