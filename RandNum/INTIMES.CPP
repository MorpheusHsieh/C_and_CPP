#include <stdio.h>
#include <conio.h>

int getmins();

void main(void)
{
  int mins1, mins2;
  printf("Type first time (form 3:22): ");
  mins1 = getmins();
  printf("Type second (later) time: ");
  mins2 = getmins();
  printf("\nDifference is %d minutes.", mins2-mins1);

  printf("\nPress any key to continue...");
  getch();
}

int getmins()
{
  int hours, minutes;
  scanf("\%d:%d", &hours, &minutes);
  return (hours*60 + minutes);
}  



