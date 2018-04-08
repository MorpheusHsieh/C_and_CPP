// Program : 'Primetest' is to test whether an integer pa is a prime or not
//           by an error probability lower than 10^(-15)
//           if flag=0 then pa is a composite integer, else pa is a prime.
// Author  : Avatar
// Date    : 98.04.16
// Filename: Ptest01.cpp

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LENGTH 33
#define FULL  0xffffffff
#define RMASK 0x00000001
#define LMASK 0x80000000

int cpass;

void decout(unsigned long num[]);
void hexout(unsigned long num[]);
void add(unsigned long *p1, unsigned long *p2);
void bigrand(unsigned int bits, unsigned long *lint);
int primetest(unsigned long *pa);

void main(void)
{
   clrscr();
   randomize();
   printf("  Procedure 'Primetest' is to test whether an integer pa is a prime ");
   printf("\nor not by an error probability lower than 10^(-15)");

   int bits;
   printf("\n\nKey-in bits of number: ");
   scanf("%d", &bits);

   unsigned long *pa = new unsigned long[LENGTH+1];
   bigrand(bits, pa);
   printf("\n\nPa: ");   hexout(pa);
   if (primetest(pa)==0)
      printf("It's a composite integer.");
   else
      printf("It's a Prime.");
   getch();
}






