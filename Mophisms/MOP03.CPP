// Program : Toral Automorphism transformation v0.3
// Author  : Morpheus
// Date    : 98.07.06

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "flips.h"
#include "pcx.h"

void Encode_PCX_BW(void);
void Encode_PCX11(void);

FILE *fin, *fout, *fout2;
byte MetaData[1024*1024+128], NewData[1024*1024+128];
PCX_Header Header;
META_PREFIX Prefix;
COLOR_MAP ColorMap;
int iImageWidth, iImageHeight;
long lTotalPixel = 0, lTotalBytes = 0;

void main(void)
{
  clrscr();
  printf("Chaotic mixing by toral automorphism.\n");

  int iCount = 0;
  while ((iCount<1) || (iCount>999))
  {
    printf("\nEnter number of transformation (0-999): ");
    scanf("%d", &iCount);
  }

  int iK;
  printf("\nKey-in number of K: ");
  scanf("%d", &iK);

  char cInfile[13];
  printf("\nKey-in source image filename: ");
  scanf("%s", cInfile);  strcat(cInfile, ".pcx");
  if ((fin=fopen(cInfile, "rb")) == NULL)
  {
    printf("\nOpen %s fail, Press any key exit...", cInfile);
    getch();
    exit(1);
  }

  fread(&Header,sizeof(PCX_Header),1,fin);
  iImageWidth = Header.Xmax + 1;
  iImageHeight = Header.Ymax + 1;

  char cFn[20] = "\0";
  while ((strlen(cFn) >5) || (strlen(cFn) < 1))
  {
    printf("\nEnter dentination filename (0-5 characteristic): ");
    scanf("%s", cFn);
  }

  // Make a Duplication
  FILE *fout2;
  char cCopyFile[20];
  strcpy(cCopyFile, cFn); strcat(cCopyFile, "000.pcx");
  if ((fout2=fopen(cCopyFile, "wb")) == NULL)
  {
    printf("\nOpen %s fail...", cCopyFile);
    getch();
    exit(1);
  }
  fseek(fin,0L,SEEK_END);
  lTotalBytes = ftell(fin);
  fseek(fin,0L,SEEK_SET);
  fread(NewData,lTotalBytes,1,fin);
  fwrite(NewData,lTotalBytes,1,fout2);
  fclose(fout2);
  fseek(fin,0L,SEEK_SET);

  // Convert PCX to Metaform
  Encode_PCX_BW();
  fclose(fin);

  int i;
  char cOutFile[20], string[4];
  printf("\nNow, Run toral automorphism transformation...");
  for (i=1; i<=iCount; i++)
  {
    strcpy(cOutFile, cFn);
    itoa(i,string,10);
    if (strlen(string) < 3)
      strncat(cOutFile, "000", 3-strlen(string));
    strcat(cOutFile, string);
    strcat(cOutFile, ".pcx");
    printf("\nOutput filename: %s", cOutFile);
    if ((fout=fopen(cOutFile, "wb")) == NULL)
    {
      printf("\nOpen %s fail, Press any key exit...", cOutFile);
      getch();
      exit(1);
    }

    // Toral automorphism transformation
    int iNewX, iNewY;
    long x, y;
    for (y=0; y<iImageHeight; y++)
    {
      for (x=0; x<iImageWidth; x++)
      {
        iNewX = ((iK+1)*x+iK*y)%iImageWidth;
        iNewY = (x+y)%iImageHeight;
        NewData[iNewX+iNewY*iImageWidth] = MetaData[x+y*iImageWidth];
      }
    }

    // Convert Metaform to PCX
    Encode_PCX11();
    fclose(fout);
    fclose(fout2);

    for (x=0; x<lTotalPixel; x++)
       MetaData[x] = NewData[x];
  }
  printf("\n\nTransformation over, press any key exit this program...");
  getch();
}

void Encode_PCX_BW()
{
  fseek(fin,0L,SEEK_END);
  lTotalBytes = ftell(fin)-128;         // Calculator all bytes

  fseek(fin,128L,SEEK_SET);             // Goto Image data start byte

  int i, j, bit;
  long lCnt = 0, lRepeatCount = 0;
  char cOldChar, cNewChar;
  while (lCnt < lTotalBytes)
  {
    cOldChar = (byte)fgetc(fin); lCnt++;
    if ((byte)cOldChar > 0xC0)
    {
      lRepeatCount = ((cOldChar - 0xC0) & 0xFF);
      cNewChar = (byte)fgetc(fin); lCnt++;
    }
    else if ((byte)cOldChar <= 0xC0)
    {
      lRepeatCount = 1;
      cNewChar = cOldChar;
    }

    for (i=0; i<lRepeatCount; i++)
    {
      for (j=0; j<8; j++)
      {
        bit = (cNewChar >>(7-j)) & 0x01;
        if (bit == 0x01)
           MetaData[lTotalPixel] = 0x01;
        else if (bit == 0x0)
           MetaData[lTotalPixel] = 0x00;
        lTotalPixel++;
      }
    }
  }
}

void Encode_PCX11(void)
{
  int i;
  long lPixelCnt = 0, lRepeatCnt = 1;
  char cOld = '0', cNew = '0';

  fwrite(&Header,sizeof(PCX_Header),1,fout);

  for (i=0; i<8; i++)
    cOld = (byte)((cOld << 1) | NewData[lPixelCnt++]);

  while (lPixelCnt < lTotalPixel)
  {
    for (i=0; i<8; i++)
      cNew = (byte)((cNew << 1) | NewData[lPixelCnt++]);

    while (cNew == cOld)
    {
      lRepeatCnt++;
      for (i=0; i<8; i++)
        cNew = (byte)((cNew << 1) | NewData[lPixelCnt++]);
    }

    if (lRepeatCnt == 1)
    {
      if ((cOld & 0xFF) < 0xC0)
        fputc(cOld, fout);
      else
      {
        fputc(0xC1, fout);
        fputc(cOld, fout);
      }
    }
    else if (lRepeatCnt > 1)
    {
      if (lRepeatCnt <= 63)
      {
        fputc(0xC0+lRepeatCnt, fout);
        fputc(cOld, fout);
      }
      else if (lRepeatCnt > 63)
      {
        while (lRepeatCnt > 63)
        {
          lRepeatCnt -= 63;
          fputc(0xFF, fout);
          fputc(cOld, fout);
        }
        fputc(0xC0+lRepeatCnt, fout);
        fputc(cOld, fout);
      }
    }
    cOld = cNew; lRepeatCnt = 1;
  }
}


