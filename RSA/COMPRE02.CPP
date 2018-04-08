// Program : Compare two integer, If p1 > p2 then flag =  1;
//                                If p1 = p2 then flag =  0;
//                                If p1 < p2 then flag = -1;
// Author  : Avatar
// Date    : 98.03.12
// Filename: Compre02.cpp

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#define LENGTH 17

void bigrand(unsigned int rdl, unsigned long *lint);
int compare(unsigned long *p1, unsigned long *p2);

void main(void)
{
   clrscr();
   randomize();

   int bits;
   printf("Compare two long random number.\n");
   printf("Key-in bits of number: ");
   scanf("%d", &bits);
   unsigned int rdl = bits/32;

   unsigned long *p1 = new unsigned long[LENGTH+2];
   bigrand(rdl, p1);
   printf("\nNumber 1:");
   for (int i=(*p1); i>0; i--)
      printf("%08lx ", p1[i]);

   unsigned long *p2 = new unsigned long[LENGTH+2];
   bigrand(rdl, p2);
   printf("\nNumber 2:");
   for (int i=(*p2); i>0; i--)
      printf("%08lx ", p2[i]);

   int flag = compare(p1,p2);
   printf("\n\nThe result is ");
   if (  flag == 1 )
      printf("P1 > P2.");
   else if ( flag == 0 )
      printf("P1 = P2.");
   else if ( flag == -1 )
      printf("P1 < P2.");
   getch();
}

