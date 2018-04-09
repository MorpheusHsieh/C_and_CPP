// Program : How much pixel value is zero?
// Author  : Avatar
// Date    : 98.07.22

#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define BLOCKSIZE   8
#define PICSIZE   512

int THRESHOLD;
unsigned char Data[512*512];

void main(void)
{
  clrscr();
  printf("How much pixel value is zero at one 8*8 block?\n");

  FILE *fin, *fout;
  char cInFile[20], cOutFile[20];
  printf("\nEnter iuput filename (RAW): ");
  scanf("%s", cInFile);
  strcat(cInFile, ".raw");
  if ((fin=fopen(cInFile, "rb")) == NULL)
  {
    printf("\nOpen %s fail...", cInFile);
    getch();
    exit(1);
  }

  printf("\nEnter output filename (TMP): ");
  scanf("%s", cOutFile);
  strcat(cOutFile, ".tmp");
  if ((fout=fopen(cOutFile, "wb")) == NULL)
  {
    printf("\nOpen %s fail...", cOutFile);
    getch();
    exit(1);
  }

  printf("\nEnter Threshold value: ");
  scanf("%d", &THRESHOLD);

  fseek(fin,0L,SEEK_END);
  long lTPixel = ftell(fin);
  fseek(fin,0L,SEEK_SET);
  fread(Data,lTPixel,1,fin);
  fclose(fin);

  printf("\nNow, Processing...\n");
  int x, y, u, v;
  long offset, Total = 0;
  static char num[64][64];
  for (v=0; v<(PICSIZE/BLOCKSIZE); v++)
  {
    for (u=0; u<(PICSIZE/BLOCKSIZE); u++)
    {
      for (y=0; y<BLOCKSIZE; y++)
      {
        for (x=0; x<BLOCKSIZE; x++)
        {
          offset = (u * 8) + ((v * 8) * 512) + x + y * 512;
          if (Data[offset] <= THRESHOLD )
            num[u][v]++;
        }
      }
      Total += num[u][v];
    }
  }
  fwrite(num,((PICSIZE/BLOCKSIZE)*(PICSIZE/BLOCKSIZE)),1,fout);
  fclose(fout);

/*  printf("\nDisplay zero number on each block.\n\n");
  for (y=0; y<(PICSIZE/BLOCKSIZE); y++)
  {
    for (x=0; x<(PICSIZE/BLOCKSIZE); x++)
    {
      printf("%2d, ", num[x+y*(PICSIZE/BLOCKSIZE)]);
      if ((x % 16) == 15)
        printf("\n");
    }
    getch();
    printf("\n");
  }*/

  printf("\n\nTotal pixel value is zero: %ld", Total);
  printf("\n\nPress any key exit this program...");
  getch();
}



