// Program : 'Division' is to get dq & dr such that da/db = dq...dr
// Author  : Avatar
// Date    : 98.03.15
// Filename: Div002.cpp

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LENGTH 17

void bigrand(unsigned int rdl, unsigned long *lint);
void division(unsigned long *da, unsigned long *db, unsigned long *dq, unsigned long*dr);

void main(void)
{
   clrscr();
   randomize();

   int bits;
   printf("Procedure Division is to do da/db = dq..dr.\n");
   printf("\nKey-in bits of number: ");
   scanf("%d", &bits);
   unsigned int rdl = bits/32;

   unsigned long *da = new unsigned long[LENGTH+1];
   bigrand(rdl, da);
   printf("\nMa:");
   for (int i=(*da); i>0; i--)
      printf("%010lu ", da[i]);

   unsigned long *db = new unsigned long[LENGTH+1];
   bigrand(rdl, db);
   printf("\nMb:");
   for (int i=(*db); i>0; i--)
      printf("%010lu ", db[i]);

   unsigned long *dq = new unsigned long [LENGTH+1];
   unsigned long *dr = new unsigned long [LENGTH+1];
   division(da, db, dq, dr);

   printf("\n\nLength : %d", *da);
   printf("\n\nda    : ");
   for (int i=(*da); i>0; i--)
      printf("%010lu ", da[i]);
   printf("\ndb      : ");
   for (int i=(*db); i>0; i--)
      printf("%010lu ", db[i]);
   printf("\nQuotient: ");
   for (int i=(*dq); i>0; i--)
      printf("%010lu ", dq[i]);
   printf("\nResiduum: ");
   for (int i=(*dr); i>0; i--)
      printf("%010lu ", dr[i]);
   getch();
}






