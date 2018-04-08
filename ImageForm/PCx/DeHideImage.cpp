// Program : Decode hiden image
// Author  : Avatar
// Date    : 98.06.15

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <flips.h>
#include <pcx.h>

void main(void)
{
  clrscr();
  printf("Decode 200*160 pixel into 640*480 pixel true color picture.\n\n");

  FILE *fin, *fout;
  char infile[20], outfile[20];
  printf("\nEnter input picture: ");
  scanf("%s", infile);
  strcat(infile, ".mta");
  if ((fin=fopen(infile, "rb")) == NULL)
  {
    printf("\nOpen %s filename fail...", infile);
    getch();
    exit(1);
  }

  printf("\nEnter output filename: ");
  scanf("%s", outfile);
  strcat(outfile, ".mta");
  if ((fout=fopen(outfile, "wb")) == NULL)
  {
    printf("\nOpen %s filename fail...", outfile);
    getch();
    exit(1);
  }

  // write Meta prefix of output filename
  int i, j;
  byte cImagePrefix[17], cImageData, cImageTmp[4];

  fseek(fin,17L,SEEK_SET);
  for (i=0; i<17 ;i++)
  {
    cImageData = 0x0;
    fread(cImageTmp,4,1,fin);

    for (j=0; j<4; j++)
       cImageData = (byte)(cImageData | ((cImageTmp[j] & 0x03) << ((3-j)*2)));
    cImagePrefix[i] = cImageData;
  }

  fseek(fout,0L,SEEK_SET);
  fwrite(cImagePrefix,17,1,fout);

  long lImageWidth, lImageHeight, lTotalBytes;
  lImageWidth = ((cImagePrefix[4] & 0xff) << 8) | (cImagePrefix[3] & 0xff);
  lImageHeight = ((cImagePrefix[6] & 0xff) << 8) | (cImagePrefix[5] & 0xff);
  lTotalBytes = lImageHeight * lImageWidth * 3;

  fseek(fin,85L,SEEK_SET);
  fseek(fout,17L,SEEK_SET);

  long lCnt = 0;
  while (lCnt < lTotalBytes)
  {
    fread(cImageTmp,3,1,fin);
    cImageData = 0x0;
    for (i=0; i<4; i++)
      cImageData = (byte)(cImageData | ((cImageTmp[i] & 0x03) << ((3-i)*2)));
    fputc(cImageData, fout);
    lCnt++;
  }

  fclose(fin);
  fclose(fout);
  printf("\nPress any key to exit program...");
  getch();
}  

















