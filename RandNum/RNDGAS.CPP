// Program : RndGas.CPP
// Author  : Avatar
// Date    : 98.09.23

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#define RND_NUM    100

extern float gasdev(long*);
extern float ran1(long *);

void main(void)
{
  clrscr();
  printf("Generate a normally distributed deviate with zero mean and ");
  printf("\nunit variance.\n");

  randomize();
  long seed = (rand() % 1000);

  long rndnum = RND_NUM;;
  printf("\nEnter size of random number: ");
  scanf("%d", &rndnum);

  int gain = 0;
  while (gain == 0)
  {
    printf("\nEnter gain of random number: ");
    scanf("%d", &gain);
  }

  FILE *fout;
  if ((fout=fopen("RndGas.TXT", "wt")) == NULL)
  {
    printf("\nOpen %s failure...");
    getch();
    exit(1);
  }

  float rnd;
//  printf("\nSeed: %d\n", seed);
  for (int i=0; i<rndnum; i++)
  {
    rnd = gasdev(&seed) * (float)gain;
//    printf("%f,\t", rnd);
    fprintf(fout, "%f,\n", rnd);
  }

  printf("\npress any key to exit this program...");
  getch();
}

