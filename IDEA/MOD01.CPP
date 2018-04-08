// Program : 'mod01' is to do ma <-- ma mod mb
// Author  : Avatar
// Date    : 98.05.05

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#define LENGTH 5
#define FULL  0xffffffff

void hexout(unsigned long *num);
void bigrand(unsigned int bits, unsigned long *sl);

void Mod(unsigned long *ma, unsigned long *mb);

void main(void)
{
   clrscr();
   randomize();
   printf("Procedure 'Mod' is to do ma <-- ma mod mb.\n\n");

   unsigned long *ma = new unsigned long [LENGTH+1];
   printf("\nMa: ");   bigrand(30, ma);   hexout(ma);

   unsigned long *mb = new unsigned long [LENGTH+1];
   printf("\nMb: ");   bigrand(16, mb);   hexout(mb);

   Mod(ma, mb);
   printf("Ma mod Mb: ");   hexout(ma);
   getch();
}

