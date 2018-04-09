// Program : PixelAvg.cpp
// Author  : Morpheus
// Date    : 98.12.10
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main(void)
{
  clrscr();
  printf("Calculate pixel average of image.\n");

  FILE *ptr;
  char fn[81];
  printf("\nEnter image name (Raw File): ");
  scanf("%s", fn);
  strcat(fn, ".raw");

  if ((ptr=fopen(fn, "rb")) == NULL)
  {
    printf("\nOpen %s failure...", fn);
    getch();
    exit(1);
  }

  long len;
  fseek(ptr,0L,SEEK_END);
  len = ftell(ptr);
  fseek(ptr,0L,SEEK_SET);

  unsigned char *pixel = new unsigned char [len];
  fread(pixel,len,1,ptr);
  fclose(ptr);

  long sum = 0;
  for (int i=0; i<=len; i++)
    sum += pixel[i];
  sum /= len;

  printf("\nAverage pixel value: %d", sum);

  printf("\nPress any key to continue...");
  getch();
}
