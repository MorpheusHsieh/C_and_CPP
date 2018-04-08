// Program : Rnd01-1.CPP -- Uniform deviates
// Author  : Avatar
// Date    : 98.09.23

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#define RND_NUM    100

extern float ran1(long *idum);

void main(void)
{
  clrscr();
  printf("Minimal random number generator.\n");

  randomize();
  long seed = -1 * (rand() % 1000);
  
  long rndnum = RND_NUM;;
  printf("\nEnter size of random number: ");
  scanf("%d", &rndnum);

  FILE *fout;
  if ((fout=fopen("Rnd01.TXT", "wb")) == NULL)
  {
    printf("\nOpen %s failure...");
    getch();
    exit(1);
  }

  float rnd;
  printf("\nSeed: %d\n", seed);
  for (int i=0; i<rndnum; i++)
  {
    rnd = ran1(&seed);
    printf("%f,\t", rnd);
    fprintf(fout, "%f,\n", rnd);
  }

  printf("\npress any key to exit this program...");
  getch();
}




