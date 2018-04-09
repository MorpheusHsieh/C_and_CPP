// Program: Raw2BMP.CPP
// Author : Morpheus
// Date   : 98.11.24
#include <conio.h>
#include <stdio.h>
#include <string.h>
#define MAXSIZE      512

typedef struct {
  FILE *fptr;
  char name[81];
  int  Width;
  int  Height;
  unsigned char pixel[MAXSIZE*MAXSIZE];
} Image;

void main(void)
{
  clrscr();
  printf("BMP image format convert to RAW file.\n");

  Image img1, img2;
  printf("\nEnter filename: ");
  scanf("%s", img1.name);
  strcat(img1.name, ".BMP");

  printf("\nEnter filename: ");
  scanf("%s", img2.name);
  strcat(img2.name, ".RAW");

  if ((img1.fptr=fopen(img1.name, "rb")) == NULL)
  {
    printf("\nOpen %s failure...", img1.name);
    getch();
  }

  if ((img2.fptr=fopen(img2.name, "wb")) == NULL)
  {
    printf("\nOpen %s failure...", img2.name);
    getch();
  }

  int height, width;
  fseek(img1.fptr,18L,SEEK_SET);
  fread(&width,4,1,img1.fptr);
  fread(&height,4,1,img1.fptr);

  fseek(img1.fptr,1078L,SEEK_SET);
  fread(img1.pixel,height,width,img1.fptr);
  fclose(img1.fptr);

  int i, j;
  for (i=0; i<height; i++)
    for (j=0; j<width; j++)
      img2.pixel[(height-i-1)*height+j] = img1.pixel[i*height+j];
  fwrite(img2.pixel,height,width,img2.fptr);
  fclose(img2.fptr);

  printf("\nPress any key to continue...");
  getch();
}


