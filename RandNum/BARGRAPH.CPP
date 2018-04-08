#include <conio.h>
#include <stdio.h>

void bar(int);

void main(void)
{
  printf("Terry\t");
  bar(27);
  printf("Chris\t");
  bar(41);
  printf("Reggie\t");
  bar(34);
  printf("Cindy\t");
  bar(22);

  printf("\nPress any key to continue...");
  getch();
}

void bar(int score)
{
  int i;
  for (i=0; i<score; i++)
    printf("=");
  printf("\n");
}

