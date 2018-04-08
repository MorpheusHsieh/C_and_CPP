// Program : 'Compare' is to compare two integer
// Author  : Avatar
// Date    : 98.04.07

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#define BITS   1024
#define LENGTH 33

void hexout(unsigned long num[]);
void bigrand(unsigned int bits, unsigned long lint[]);
int compare(unsigned long lint1[], unsigned long lint2[]);

void main(void)
{
   clrscr();
   randomize();
   printf("\nProcedure 'Compare' is compare two integers.");

   unsigned long *lint1 = new unsigned long[LENGTH+1];
   bigrand(BITS, lint1);
   printf("\n\nNumber 1: ");
   hexout(lint1);

   unsigned long *lint2 = new unsigned long[LENGTH+1];
   bigrand(BITS, lint2);
   printf("Number 2: ");
   hexout(lint2);

   int ans;
   ans = compare(lint1, lint2);
   if ( ans==1 )
      printf("\nP1 > P2 ");
   else if ( ans==0 )
      printf("\nP1 = P2 ");
   else if ( ans<1 )
      printf("\nP1 < P2 ");

   getch();
}


