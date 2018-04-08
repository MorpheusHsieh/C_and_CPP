// Program : 'DIV01' is to get dq &dr such that d1/db=dq...dr
// Author  : Avatar
// Date    : 98.04.07

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

#define BITS      32
#define MAXBITS 1024
#define LENGTH    33

void hexout(unsigned long num[]);
void shiftleft(unsigned long sl[]);
void shiftright(unsigned long rl[]);
void sub(unsigned long p1[], unsigned long p2[]);
int compare(unsigned long p1[], unsigned long p2[]);
void bigrand(unsigned int bits, unsigned long lint[]);
void division(unsigned long da[], unsigned long db[], unsigned long dq[], unsigned long dr[]);

void main(void)
{
   clrscr();
   randomize();
   printf("Procedure 'Division' is to do da/db = dq..dr.");

   int bits;
   printf("\n\nKey-in bits of Long Number: ");
   scanf("%d", &bits);
   if (bits<=0)
      bits = BITS;
   else if (bits>MAXBITS)
      bits = MAXBITS;

   unsigned long *da = new unsigned long[LENGTH+1];
   bigrand(bits, da);
   printf("\n\nNumber 1: ");
   hexout(da);

   unsigned long *db = new unsigned long[LENGTH+1];
   bigrand(bits, db);
   printf("\nNumber 2: ");
   hexout(db);

   unsigned long *dq = new unsigned long [LENGTH+1];
   unsigned long *dr = new unsigned long [LENGTH+1];
   division(da, db, dq, dr);
   printf("\nda/db = ");
   hexout(dq);
   printf("...");
   hexout(dr);

   getch();
}


