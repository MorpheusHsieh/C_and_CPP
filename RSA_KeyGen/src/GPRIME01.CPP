// Program : 'Getprime' is to get a prime number pn with length of plen
// Author  : Avatar
// Date    : 98.04.15

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LENGTH 33
#define FULL  0xffffffff
#define RMASK 0x00000001
#define LMASK 0x80000000

int count;

void hexout(unsigned long num[]);
void getprime(unsigned int plen, unsigned long *pn);

void main(void)
{
   clrscr();
   randomize();

   unsigned int plen;
   printf("'GetPrime' is to get a prime number pn with length of plen.");
   printf("\n\nKey-in bits of number: ");
   scanf("%d", &plen);

   printf("\n");
   unsigned long *pn = new unsigned long[LENGTH+1];
   for (int i=0; i<10; i++)
   {
      getprime(plen, pn);
      printf("Pn[%d]:", i);
      hexout(pn);
   }
   getch();
}






