// Program : Toral Automorphism transformation v0.4
// Author  : Morpheus
// Date    : 98.08.10

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "flips.h"
#include "pcx.h"

void Encode_PCX_256Color(void);
void Encode_PCX81(void);
extern void morphism(int, int, unsigned char*, unsigned char*);

FILE *fin, *fout, *fout2, *fptr;
byte MetaData[1024*1024], NewData[1024*1024];
PCX_Header Header;
META_PREFIX Prefix;
COLOR_MAP ColorMap;
int iImageWidth, iImageHeight;
long lTotalBytes = 0, lTotalPixels = 0;

void main(void)
{
  clrscr();
  printf("Chaotic mixing 256 color PCX picture by toral automorphism.\n");

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

  char cFn[20] = "\0";
  while ((strlen(cFn) > 8) || (strlen(cFn) < 1))
  {
    printf("\nEnter dentination filename (0-8 characteristic): ");
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

  // Read PCX Header information
  fseek(fin,0L,SEEK_SET);
  fread(&Header,sizeof(PCX_Header),1,fin);
  fwrite(&Header,sizeof(PCX_Header),1,fout2);
  int size = Header.Xmax + 1;

  // Read Color Map
  fseek(fin,-768L,SEEK_END);
  fread(&ColorMap,3,256,fin);
  lTotalBytes = ftell(fin) - 769 - 128;

  // Read PCX Bytes data
  fseek(fin,128L,SEEK_SET);
  fread(NewData,lTotalBytes,1,fin);
  fwrite(NewData,lTotalBytes,1,fout2);

  // Write Color Map
  fputc(0x0C, fout2);
  fwrite(&ColorMap,3,256,fout2);
  fclose(fout2);
  fclose(fin);

  // Convert PCX to Metaform
  Encode_PCX_256Color();

  long i, j;
  char cOutFile[20], string[4], fptrfile[20];
  printf("\nNow, Run toral automorphism transformation...");
  for (i=1; i<=iCount; i++)
  {
    strcpy(cOutFile, cFn);
    itoa(i,string,10);
    if (strlen(string) < 3)
      strncat(cOutFile, "000", 3-strlen(string));
    strcat(cOutFile, string);
    strcpy(fptrfile, cOutFile);

    strcat(cOutFile, ".pcx");
    printf("\nOutput filename: %s", cOutFile);
    if ((fout=fopen(cOutFile, "wb")) == NULL)
    {
      printf("\nOpen %s fail, Press any key exit...", cOutFile);
      getch();
      exit(1);
    }

    strcat(fptrfile, ".raw");
    if ((fptr=fopen(fptrfile, "wb")) == NULL)
    {
      printf("\nOpen %s fail, Press any key exit...", fptrfile);
      getch();
      exit(1);
    }

    // Toral automorphism transformation
    morphism(size,iK,MetaData,NewData);
/*    int iNewX, iNewY;
    long x, y;
    for (y=0; y<iImageHeight; y++)
    {
      for (x=0; x<iImageWidth; x++)
      {
        iNewX = ((iK+1)*x+iK*y)%iImageWidth;
        iNewY = (x+y)%iImageHeight;
        NewData[iNewX+iNewY*iImageWidth] = MetaData[x+y*iImageWidth];
      }
    }*/
//    printf("\n%d, %d", iImageWidth, iImageHeight); getch();
    fwrite(NewData,iImageWidth,iImageHeight,fptr);
    fclose(fptr);

    // Convert Metaform to PCX
    Encode_PCX81();
    fclose(fout);

    for (j=0; j<lTotalPixels; j++)
       MetaData[j] = NewData[j];
  }
  printf("\n\nTransformation over, press any key exit this program...");
  getch();
}

void Encode_PCX_256Color()
{
  byte cOld, cNew;
  long lCnt = 0, lCnt1 = 0, lRepeatCnt;
  while (lCnt < lTotalBytes)
  {
    cOld = NewData[lCnt++];
    if (cOld > 0xC0)
    {
      lRepeatCnt = (cOld & 0xFF) - 0xC0;
      cNew = NewData[lCnt++];
    }
    else if (cOld <= 0xC0)
    {
      lRepeatCnt = 1;
      cNew = cOld;
    }

    int i;
    for (i=0; i<lRepeatCnt; i++)
      MetaData[lCnt1++] = cNew;
  }
  lTotalPixels = lCnt1;
//  printf("\nlCnt1: %ld", lCnt1); getch();
}

void Encode_PCX81(void)
{
  // Write PCX Header
  fwrite(&Header, sizeof(PCX_Header),1,fout);

  // Write Pixel intensity level
  char cOld, cNew;
  long lCnt = 0, lRepeatCnt = 1;
  cOld = NewData[lCnt++];
  while (lCnt <= lTotalPixels)
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
      {
        fputc(cOld, fout);
      }
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

  // Write PCX Color Map
  fputc(0x0C,fout);
  fwrite(&ColorMap,3,256,fout);       // write color map out
}
