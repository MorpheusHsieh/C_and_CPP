// Program : GasImage.CPP
// Author  : Morpheus
// Date    : 98.09.23

#include <math.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#define RND_NUM    512

extern float gasdev(long *);
extern float ran1(long *);

void main(void)
{
  clrscr();
  printf("Generate a image that normally distributed deviate with zero mean");
  printf("\nand unit variance.\n");

  randomize();
  long seed = (rand() % 1000);

  long rndnum = RND_NUM;;
  printf("\nEnter size of image: ");
  scanf("%d", &rndnum);

  FILE *fout;
  if ((fout=fopen("GasImage.raw", "wb")) == NULL)
  {
    printf("\nOpen %s failure...");
    getch();
    exit(1);
  }

  float max = 0;
  float *Data = new float [rndnum*rndnum];
  for (int i=0; i<rndnum; i++)
  {
    for (int j=0; j<rndnum; j++)
    {
      Data[i*rndnum+j] = gasdev(&seed);
      if (fabs(Data[i*rndnum+j]) > max)
        max = fabs(Data[i*rndnum+j]);
    }
  }

  int gain = (int)(128.0 / max);
  printf("Gain is %d.\n", gain);

  for (int i=0; i<rndnum; i++)
  {
    for (int j=0; j<rndnum; j++)
      fputc(Data[i*rndnum+j] * gain + 128, fout);
  }

  delete [] Data;

  printf("\npress any key to exit this program...");
  getch();
}

