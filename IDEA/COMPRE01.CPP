// Program : 'Compare' is to compare two integer
// Author  : Avatar
// Date    : 98.05.05

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#define LENGTH 5

void hexout(unsigned long num[]);
void bigrand(unsigned int bits, unsigned long num[]);
int compare(unsigned long p1[], unsigned long p2[]);

void main(void)
{
   clrscr();
   randomize();
   printf("\nProcedure 'Compare' is compare two integers.");

   unsigned long *p1 = new unsigned long[LENGTH+1];
   bigrand(128, p1);
   printf("\n\nNumber 1: ");
   hexout(p1);

   unsigned long *p2 = new unsigned long[LENGTH+1];
   bigrand(128, p2);
   printf("Number 2: ");
   hexout(p2);

   int ans;
   ans = compare(p1, p2);
   if ( ans==1 )
      printf("\nP1 > P2 ");
   else if ( ans==0 )
      printf("\nP1 = P2 ");
   else if ( ans<1 )
      printf("\nP1 < P2 ");

   getch();
}


