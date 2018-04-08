// roomplot.CPP
#include <conio.h>
#include <stdio.h>

void rectang(int, int);

void main(void)
{
  printf("\nLiving room\n");
  rectang(22,12);
  printf("\nkitchen\n");
  rectang(16,16);
  printf("\nBathroom\n");
  rectang(6,8);
  printf("\nBedroom\n");
  rectang(12,4);

  printf("\nPress any key to continue...");
  getch();
}

void rectang(int length, int width)
{
  int i, j;
  length /= 2;
  width /= 2;
  for (i=0; i<width; i++)
  {
    printf("\t\t");
    for (j=0; j<length; j++)
      printf("\x2a");
    printf("\n");
  }
}      



