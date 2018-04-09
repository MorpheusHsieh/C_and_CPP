// PRogram : DeHide01 -- extract watermark from JPEG-compression
// Author  : Morpheus
// Date    : 98.07.21

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <flips.h>
#include <PCX.h>
#define PX         0
#define PY         0
#define SRCSIZE  512
#define LOGOSIZE 160

void Write_Meta_Prefix(void);

FILE *fin1, *fin2, *fout;
char RawData[512*512], DifData[512*512], NewData[512*512];
long lTotalPixel;

void main(void)
{
  clrscr();
  printf("Extract Watermark from loss compression.\n");

  int bit_posi;
  printf("\nEnter Hiding bit position (0-7): ");
  scanf("%d", &bit_posi);

  char cRawFile[20],  cDifFile[20], cOutFile[20];
  printf("\nEnter source picture filename (RAW): ");
  scanf("%s", cRawFile);
  strcat(cRawFile, ".raw");
  if ((fin1=fopen(cRawFile, "rb")) == NULL)
  {
    printf("\nOpen %s fail...", cRawFile);
    getch();
    exit(1);
  }

  printf("\nEnter Difference picture filename (RAW): ");
  scanf("%s", cDifFile);
  strcat(cDifFile, ".raw");
  if ((fin2=fopen(cDifFile, "rb")) == NULL)
  {
    printf("\nOpen %s fail...", cDifFile);
    getch();
    exit(1);
  }

  printf("\nEnter output filename (MTA): ");
  scanf("%s", cOutFile);
  strcat(cOutFile, ".mta");
  if ((fout=fopen(cOutFile, "wb")) == NULL)
  {
    printf("\nOpen %s fail...", cOutFile);
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


  char mask = (byte)(0x1 << bit_posi);
  long i, lCnt = 0;
  long offset = PX + PY * SRCSIZE;
  for (i=0; i<lTotalPixel; i++)
  {
    if ((i >= offset) && (lCnt < (LOGOSIZE * LOGOSIZE)))
    {
      if (DifData[i] == 0)
      {
        NewData[lCnt++] = (byte)((RawData[i] & mask) >> bit_posi);
      }
    }
  }

  // Make Metaform File
  Write_Meta_Prefix();
  fwrite(NewData,LOGOSIZE,LOGOSIZE,fout);
  fclose(fout);

  printf("\nPress any key exit this program...");
  getch();
}

void Write_Meta_Prefix(void)
{
  META_PREFIX Prefix;
  
  Prefix.version = (byte)0x10;
  Prefix.mode    = (byte)0x01;
  Prefix.id      = (byte)(0xFF - Prefix.version - Prefix.mode);
  Prefix.width   = (word)LOGOSIZE;
  Prefix.height  = (word)LOGOSIZE;
  fwrite(&Prefix,sizeof(Prefix),1,fout);
}


