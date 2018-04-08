// Program : Toral Automorphism transformation
// Author  : Avatar
// Date    : 98.07.02

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <flips.h>
#include <pcx.h>

FILE *fin, *fout;
PCX_Header Header;
META_PREFIX Prefix;
COLOR_MAP ColorMap;
int iImageWidth, iImageHeight;

void main(void)
{
  clrscr();
  printf("Chaotic mixing by toral automorphism.\n");

  char cInfile[20], cOutfile[20];
  printf("\nEnter image filename: ");
  scanf("%s", cInfile); strcat(cInfile, ".mta");
  if ((fin=fopen(cInfile, "rb")) == NULL)
  {
    printf("\nOpen %s fail, Press any key exit...", cInfile);
    getch();
    exit(1);
  }

  printf("\nEnter output filename: ");
  scanf("%s", cOutfile); strcat(cOutfile, ".mta");
  if ((fout=fopen(cOutfile, "wb")) == NULL)
  {
    printf("\nOpen %s fail, Press any key exit...", cOutfile);
    getch();
    exit(1);
  }

  // Write Meta Prefix
  fread(&Prefix,sizeof(Prefix),1,fin);
  fwrite(&Prefix,sizeof(Prefix),1,fout);

  iImageWidth = Prefix.width + 1;
  iImageHeight = Prefix.height + 1;

  printf("\nNow, Run toral automorphism transformation...");
  byte cColorLevel, ImageData[640*640];
  int iNewX, iNewY;
  long x, y;
  for (y=0; y<iImageHeight; y++)
  {
    for (x=0; x<iImageWidth; x++)
    {
      cColorLevel = (byte)fgetc(fin);
      iNewX = (2*x+y)%iImageWidth;
      iNewY = (x+y)%iImageHeight;
      ImageData[iNewX+iNewY*iImageWidth] = cColorLevel;
//      printf("\n%02d, %02d", iNewX, iNewY); getch();
    }
  }
  fwrite(ImageData,iImageWidth*iImageHeight,1,fout);
  fclose(fin);
  fclose(fout);
  printf("\n\nTransformation over, press any key exit this program...");
  getch();
}

