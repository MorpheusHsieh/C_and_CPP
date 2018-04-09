// Program : 'IDEASubKey' is generate subKey of IDEA
// Author  : Morpheus
// Date    : 98.05.01

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LENGTH 33
#define SUBKEY 56
#define LMASK 0x80000000

void hexout(unsigned long num[]);
void bigrand(unsigned int plen, unsigned long *pn);
void MakeSubKey(unsigned long *key, unsigned int *SKey);

void main(void)
{
   clrscr();
   randomize();

   unsigned int keylen = 128;
   printf("'GetPrime' is to get a prime number pn with length of plen.\n");

   unsigned long *ideakey = new unsigned long[LENGTH+1];
   bigrand(keylen, ideakey);
   printf("IDEA Key: ");   hexout(ideakey);

   unsigned int *SubKey = new unsigned int [SUBKEY+1];
   MakeSubKey(ideakey, SubKey);

   for (int i=1; i<=SubKey[0]; i++) {
      printf("Skey[%02d]: %04x,\t", i, SubKey[i]);
      if ((i%4)==0)
         printf("\n");
   }
   getch();
}






