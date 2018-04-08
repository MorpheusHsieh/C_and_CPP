// Program : Toral Automorphism transformation v0.4
// Author  : Avatar
// Date    : 98.08.25

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "flips.h"
#include "pcx.h"

void Encode_PCX_256Color(void);
void Encode_PCX81(void);
extern void morphism(int, int, unsigned char*, unsigned char*);

FILE *fin, *fout;
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

  char cInfile[30], outfile[40], string[10];
  printf("\nKey-in source image filename: ");
  scanf("%s", cInfile);
  strcpy(outfile, cInfile);
  strcat(cInfile, ".pcx");
  if ((fin=fopen(cInfile, "rb")) == NULL)
  {
    printf("\nOpen %s fail, Press any key exit...", cInfile);
    getch();
    exit(1);
  }

  int iK;
  printf("\nKey-in number of K: ");
  scanf("%d", &iK);

  int iCount = 0;
  while ((iCount<1) || (iCount>999))
  {
    printf("\nEnter number of transformation (0-999): ");
    scanf("%d", &iCount);
  }

  itoa(iCount,string,10);
  if (strlen(string) < 3)
    strncat(outfile, "000", 3-strlen(string));
  strcat(outfile, string);
  strcat(outfile, ".pcx");
  printf("\nOutput filename: %s", outfile);
  if ((fout=fopen(outfile, "wb")) == NULL)
  {
    printf("\nOpen %s fail, Press any key exit...", outfile);
    getch();
    exit(1);
  }

  // Read PCX Header information
  fseek(fin,0L,SEEK_SET);
  fread(&Header,sizeof(PCX_Header),1,fin);
  int size = Header.Xmax + 1;

  // Read Color Map
  fseek(fin,-768L,SEEK_END);
  fread(&ColorMap,3,256,fin);
  lTotalBytes = ftell(fin) - 769 - 128;

  // Read PCX Bytes data
  fseek(fin,128L,SEEK_SET);
  fread(NewData,lTotalBytes,1,fin);
  fclose(fin);

  // Convert PCX to Metaform
  Encode_PCX_256Color();

  long i, j;
  printf("\nNow, Run toral automorphism transformation...");
  for (i=1; i<=iCount; i++)
  {
    // Toral automorphism transformation
    morphism(size,iK,MetaData,NewData);

    for (j=0; j<lTotalPixels; j++)
       MetaData[j] = NewData[j];
  }
  Encode_PCX81();
  fclose(fout);

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
