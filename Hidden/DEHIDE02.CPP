// PRogram : DeHide01 -- extract watermark from JPEG-compression
// Author  : Morpheus
// Date    : 98.07.21

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <flips.h>
#include <PCX.h>
#define SRCSIZE   512
#define LOGOSIZE  160
#define BLOCKSIZE   8

void Write_Meta_Prefix(void);
void Calculator_Block_Pixel(void);

FILE *fin1, *fin2, *fout;
unsigned char RawData[512*512], DifData[512*512], NewData[512*512];
static unsigned char PixelNum[64][64];
int iThreshold;
long lTotalPixel;

void main(void)
{
  clrscr();
  printf("Extract Watermark from loss compression.\n");

  int bit_posi;
  printf("\nEnter Hiding bit position (0-7): ");
  scanf("%d", &bit_posi);

  printf("\nEnter threshold value: ");
  scanf("%d", &iThreshold);

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

  Calculator_Block_Pixel();

  char mask = (byte)(0x01 << bit_posi);
  int u, v, x, y, iCnt1 = 0, iCnt2, iThreshold2;
  int BlockNum = (SRCSIZE / BLOCKSIZE);
  int PixelPerBlock = (int)((LOGOSIZE*LOGOSIZE) / (BlockNum*BlockNum)) + 1;
  long offset;

  for (v=0; v<BlockNum; v++)
  {
    for (u=0; u<BlockNum; u++)
    {
      iCnt2 = 0;
      iThreshold2 = iThreshold;
      if (PixelNum[u][v] < PixelPerBlock)
      {
        int taggle = 0, iZeroNum;
        while (taggle != 1)
        {
          iThreshold2++; iZeroNum = 0;
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
      }

      for (y=0; y<BLOCKSIZE; y++)
      {
        for (x=0; x<BLOCKSIZE; x++)
        {
          offset = u*BLOCKSIZE + (v*BLOCKSIZE)*SRCSIZE + x + y*SRCSIZE;
          if (DifData[offset] <= iThreshold2)
          {
            if ((iCnt2 < PixelPerBlock) && (iCnt1 < LOGOSIZE*LOGOSIZE))
            {
              NewData[iCnt1++] = (char)((RawData[offset] & mask) >> bit_posi);
//            printf("%d", NewData[offset]); getch();
              iCnt2++;
            }
          }
        }
      }
    }
  }
  printf("\n\nHiding pixel: %d\n", iCnt1);

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
  memcpy(&Prefix.reserved,"",9);
  fwrite(&Prefix,sizeof(Prefix),1,fout);
}

void Calculator_Block_Pixel(void)
{
  long offset;
  int u, v, x, y;
  for (v=0; v<(SRCSIZE/BLOCKSIZE); v++)
  {
    for (u=0; u<(SRCSIZE/BLOCKSIZE); u++)
    {
      for (y=0; y<BLOCKSIZE; y++)
      {
        for (x=0; x<BLOCKSIZE; x++)
        {
          offset = (u * 8) + ((v * 8) * 512) + x + y * 512;
          if (DifData[offset] <= iThreshold )
            PixelNum[u][v]++;
        }
      }
    }
  }
}
