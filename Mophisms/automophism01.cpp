// Program : Automorphism
// Author  : Avatar
// Date    : 98.07.02

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#define  N  16

void main(void)
{
  clrscr();
  printf("Torus automorphism on integer lattices.");

  FILE *fout;
  if ((fout=fopen("mophism.tmp", "w")) == NULL)
  {
    printf("\nOpen %s erroe, Press any key to exit...");
    getch();
    exit(1);
  }

  int x, y;
  static int data[N][N];
  printf("\nNow, Toral automorphism calculator...\n");
  for (y=0; y<16; y++)
  {
    for (x=0; x<16; x++)
    {
      if (((x%3) == 0) && (x != 0))
      {
        printf("\n(%02d,%02d)->(%02d,%02d),\t", x, y, (2*x+y)%N, (x+y)%N);
        fprintf(fout, "\n(%02d,%02d)->(%02d,%02d),\t", x, y, (2*x+y)%N, (x+y)%N);
      }
      else
      {
        printf("(%02d,%02d)->(%02d,%02d),\t", x, y, (2*x+y)%N, (x+y)%N);
        fprintf(fout, "(%02d,%02d)->(%02d,%02d),\t", x, y, (2*x+y)%N, (x+y)%N);
      }
      data[(2*x+y)%N][(x+y)%N]++;
    }
    printf("\n");
    fprintf(fout, "\n");
//    getch();
  }
  fclose(fout);

  printf("\n");
  for (x=0; x<N; x++)
  {
    for (y=0; y<N; y++)
    {
      if (((y%4) == 0) && (y != 0))
         printf("\n");
      printf("(%02d,%02d): %01d,\t", x, y, data[x][y]);
    }
    printf("\n");
    getch();
  }

  getch();
}



