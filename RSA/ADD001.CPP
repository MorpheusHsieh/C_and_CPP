// Program : Add is to do p1 <- p1 + p2
// Author  : Avatar
// Date    : 98.03.13
// Filaname: Add001.cpp

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#define LENGTH 17

void bigrand(unsigned int rdl, unsigned long *lint);

void add(unsigned long *p1, unsigned long *p2)
{
   int len, carry;
   unsigned long *pp1;
   unsigned long *pp2;

   if ( *p1 > *p2 )
      len = *p1;
   else
      len = *p2;
   carry = 0;
   *p1 = len;
   pp1 = p1 + 1;
   pp2 = p2 + 1;
   for (; len>0; len--)
   {
      *pp1 = (*pp1) + (*pp2) + carry;
      if  ( (*pp1)<(*pp2) )
         carry = 1;
      else
         carry = 0;
      pp1++; pp2++;
   }
   if ( carry == 1 )
   {
      (*p1)++;
      (*pp1)++;
   }
}

void main(void)
{
   clrscr();
   randomize();

   int bits;
   printf("Add two integer, The result save first number.\n");
   printf("Example: p1 <- p1 + p2.\n");
   printf("\nKey-in bits of number: ");
   scanf("%d", &bits);
   unsigned int rdl = bits/32;

   unsigned long *p1 = new unsigned long[LENGTH+1];
   bigrand(rdl, p1);
   printf("\nP1:");
   for (int i=(*p1); i>0; i--)
      printf("%010lu ", p1[i]);

   unsigned long *p2 = new unsigned long[LENGTH+1];
   bigrand(rdl, p2);
   printf("\nP2:");
   for (int i=(*p2); i>0; i--)
      printf("%010lu ", p2[i]);

   printf("\n\nLength : %d", *p1);

   add(p1,p2);
   printf("\nAdd :");
   for (int i=(*p1); i>0; i--)
      printf("%010lu ", p1[i]);
   getch();
}


