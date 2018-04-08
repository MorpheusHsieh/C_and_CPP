// Program : 'Getkeys' is to generate a pairs key
// Author  : Avatar
// Date    : 98.04.24

#include <math.h>
#include <time.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LENGTH 33
#define MinKeyLen   32
#define MaxKeyLen 1024
#define RMASK 0x00000001
#define LMASK 0x80000000

void getkeys(void)
{
   char UserName[8];
   long upwd=0;
   do {
      clrscr();
      printf("\nKey-in your login name (1-8 character & no space): ");
      scanf("%s", UserName);
      do {
         printf("Key-in your password number (0-99999999): ");
         scanf("%d", &upwd);
         if ((upwd>99999999)||(upwd<1))
            printf("\Error, Out of Range. Key-in again...");
      } while ( (upwd<1)||(upwd>99999999) );

      printf("\n\nLogin name:%s", UserName);
      printf("\nPassword  :%ld", upwd);
      printf("\n\nAre you sure (y/n):");
   } while ( (getch()!='y')&&(getch()!='Y') );
   getch();

   char fn1[20], fn2[20];
   strcpy(fn1, UserName);   strcat(fn1, ".pub");
   strcpy(fn2, UserName);   strcat(fn2, ".sct");
   printf("\n\nNow generate the system keys, Wait for some minutes!!\n");

   unsigned int  key1, key2, key3, key2r, key2l, key3r, key3l, tkeyl;
   printf("Enter length of key (32 - 512 bits): ");
   scanf("%d", &key1);
   if (key1<=MinKeyLen)
      key1 = MinKeyLen;
   else if (key1>=MaxKeyLen)
      key1 = MaxKeyLen;

   key2=(key1/2);
   tklen = (int)(log((double)key2)/log(2.0));
   key2r = (key2-tklen)/2;
   key2l = (key2-tklen-key2r);




   key2l=(key1/2)+1;



}




void main(void)
{
   clrscr();
   randomize();
   getkeys();
}   
