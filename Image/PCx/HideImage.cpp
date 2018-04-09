// Program : Hiden True color picture
// Author  : Morpheus
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
  printf("Hiden 200*160 pixel into 640*480 pixel true color picture.\n\n");

  FILE *fin1, *fin2, *fout;
  char infile1[20], infile2[20], outfile[20];
  printf("\nEnter hide information: ");
  scanf("%s", infile1);
  strcat(infile1, ".mta");
  if ((fin1=fopen(infile1, "rb")) == NULL)
  {
    printf("\nOpen %s filename fail...", infile1);
    getch();
    exit(1);
  }

  printf("\nEnter input picture: ");
  scanf("%s", infile2);
  strcat(infile2, ".mta");
  if ((fin2=fopen(infile2, "rb")) == NULL)
  {
    printf("\nOpen %s filename fail...", infile2);
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
  META_PREFIX prefix;
  fread(&prefix,sizeof(META_PREFIX),1,fin2);
  fwrite(&prefix,sizeof(META_PREFIX),1,fout);

  byte cColorMapSize;
  cColorMapSize = (byte)fgetc(fin2);
  fputc(cColorMapSize, fout);

  long lTotalBytes1, lTotalBytes2;
  fseek(fin1,0L,SEEK_END);
  lTotalBytes1 = ftell(fin1);
  fseek(fin2,0L,SEEK_END);
  lTotalBytes2 = ftell(fin2)-17;
  if ((lTotalBytes2/3) < lTotalBytes1) {
    printf("\n%s is to small for hidden.");
    getch();
    exit(1);
  }

  fseek(fin1,0L,SEEK_SET);
  fseek(fin2,17L,SEEK_SET);
  fseek(fout,17L,SEEK_SET);
  printf("\nEncyption...");

  byte cImageData1, cImageData2[4], cNewImageData[4];
  long i, lCnt1 = 0, lCnt2 = 0;
  while (lCnt2 < lTotalBytes2)
  {
    if (lCnt1 <= lTotalBytes1)
    {
      cImageData1 = (byte)fgetc(fin1); lCnt1 ++;
//      printf("\nHide data: %02x", cImageData1);
      if (lCnt1 <= 0x11)
      {
        fread(cImageData2,4,1,fin2); lCnt2 += 4;
//        printf("\n%02x, %02x, %02x, %02x",
//               cImageData2[0], cImageData2[1], cImageData2[2], cImageData2[3]);
        for (i=0; i<4; i++)
          cNewImageData[i] = (byte)((cImageData2[i] & 0xFC) |
                                   ((cImageData1 >> ((3-i)*2)) & 0x03));
        fwrite(cNewImageData,4,1,fout);
      }
      else if (lCnt1 > 0x11)
      {
        fread(cImageData2,3,1,fin2); lCnt2 += 3;
        for (i=0; i<3; i++)
          cNewImageData[i] = (byte)((cImageData2[i] & 0xFC) |
                                   ((cImageData1 >> ((3-i)*2)) & 0x03));
        fwrite(cNewImageData,3,1,fout);
      }
    }
    else
    {
      fread(cImageData2,3,1,fin2); lCnt2 += 3;
      fwrite(cImageData2,3,1,fout);
    }
  }
  fclose(fin1);
  fclose(fin2);
  fclose(fout);
  printf("\nPress any key to exit program...");
  getch();
}  

