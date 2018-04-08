// Program: DES001.CPP
// Auther : Avatar
// Date   : 98/01/15

#include <conio.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

typedef struct w1 { unsigned b0:1; unsigned b1:1;
		    unsigned b2:1; unsigned b3:1;
		    unsigned b4:1; unsigned b5:1;
		    unsigned b6:1; unsigned b7:1;
		  } B_CHAR;

typedef struct w2 { unsigned h0:4;
		    unsigned h1:4;
		  } H_CHAR;

typedef union     { int    it;
		    B_CHAR bt;
		    H_CHAR ht;
		  } CH_USE;

typedef CH_USE    CH_BLOCK[8];
typedef CH_USE    CH_HALF[4];

int s[8][4][16] = {{{14, 4,13, 1, 2,15,11, 8, 3,10, 6,12, 5, 9, 0, 7},
		    { 0,15, 7, 4,14, 2,13, 1,10, 6,12,11, 9, 5, 3, 8},
		    { 4,21,14, 8,13, 6, 2,11,15,12, 9, 7, 3,10, 5, 0},
		    {15,12, 8, 2, 4, 9, 1, 7, 5,11, 3,14,10, 0, 6,13}},
		   {{15, 1, 8,14, 6,11, 3, 4, 9, 7, 2,13,12, 0, 5,10},
		    { 3,13, 4, 7,15, 2, 8,14,12, 0, 1,10, 6, 9,11, 5},
		    { 0,14, 7,11,10, 4,13, 1, 5, 8,12, 6, 9, 3, 2,15},
		    {13, 8,10, 1, 3,15, 4, 2,11, 6, 7,12, 0, 5,14, 9}},
		   {{10, 0, 9,14, 6, 3,15, 5, 1,13,12, 7,11, 4, 2, 8},
		    {13, 7, 0, 9, 3, 4, 6,10, 2, 8, 5,14,12,11,15, 1},
		    {13, 6, 4, 9, 8,15, 3, 0,11, 1, 2,12, 5,10,14, 7},
		    { 1,10,13, 0, 6, 9, 8, 7, 4,15,14, 3,11, 5, 2,12}},
		   {{ 7,13,14, 3, 0, 6, 9,10, 1, 2, 8, 5,11,12, 4,15},
		    {13, 8,11, 5, 6,15, 0, 3, 4, 7, 2,12, 1,10,14, 9},
		    {10, 6, 9, 0,12,11, 7,13,15, 1, 3,14, 5, 2, 8, 4},
		    { 3,15, 0, 6,10, 1,13, 8, 9, 4, 5,11,12, 7, 2,14}},
		   {{ 2,12, 4, 1, 7,10,11, 6, 8, 5, 3,15,13, 0,14, 9},
		    {14,11, 2,12, 4, 7,13, 1, 5, 0,15,10, 3, 9, 8, 6},
		    { 4, 2, 1,11,10,13, 7, 8,15, 9,12, 5, 6, 3, 0,14},
		    {11, 8,12, 7, 1,14, 2,13, 6,15, 0, 9,10, 4, 5, 3}},
		   {{12, 1,10,15, 9, 2, 6, 8, 0,13, 3, 4,14, 7, 5,11},
		    {10,15, 4, 2, 7,12, 9, 5, 6, 1,13,14, 0,11, 3, 8},
		    { 9,14,15, 5, 2, 8,12, 3, 7, 0, 4,10, 1,13,11, 6},
		    { 4, 3, 2,12, 9, 5,15,10,11,14, 1, 7, 6, 0, 8,13}},
		   {{ 4,11, 2,14,15, 0, 8,13, 3,12, 9, 7, 5,10, 6, 1},
		    {13, 0,11, 7, 4, 9, 1,10,14, 3, 5,12, 2,15, 8, 6},
		    { 1, 4,11,13,12, 3, 7,14,10,15, 6, 8, 0, 5, 9, 2},
		    { 6,11,13, 8, 1, 4,10, 7, 9, 5, 0,15,14, 2, 3,12}},
		   {{13, 2, 8, 4, 6,15,11, 1,10, 9, 3,14, 5, 0,12, 7},
		    { 1,15,13, 8,10, 3, 7, 4,12, 5, 6,11, 0,14, 9, 2},
		    { 7,11, 4, 1, 9,12,14, 2, 0, 6,10,13,15, 3, 5, 8},
		    { 2, 1,14, 7, 4,10, 8,13,15,12, 9, 0, 3, 5, 6,11}}};

int pc_1[56]={ 49,42,35,28,21,14, 7, 0,50,43,36,29,22,15,
		8, 1,51,44,37,30,23,16, 9, 2,52,45,38,31,
	       55,48,41,34,27,20,13, 6,54,47,40,33,26,19,
	       12, 5,53,46,39,32,25,18,11, 4,24,17,10, 3 };

int pc_2[48]={ 13,16,10,23, 0, 4, 2,27,14, 5,20, 9,22,18,
	       11, 3,25, 7,15, 6,26,19,12, 1,40,51,30,36,
	       46,54,29,39,50,44,32,47,43,48,38,55,33,52,
	       45,41,49,35,28,31                         };

int  e32[48]={ 31, 0, 1, 2, 3, 4, 3, 4, 5, 6, 7, 8, 7, 8,
		9,10,11,12,11,12,13,14,15,16,15,16,17,18,
	       19,20,19,20,21,22,23,24,23,24,25,26,27,28,
	       27,28,29,30,31, 0                         };

int p48[32]={  15, 6,19,20,28,11,27,16, 0,14,22,25, 4,17,
	       30, 9, 1, 7,23,13,26, 2, 8,12,29, 5,21,10,
		3,24                                     };

void i_p(blk)
CH_BLOCK blk;
{
   CH_BLOCK tb;
   int i;
   for (i=0; i<8; i++)
      tb[i].it=0;
   tb[0].bt.b0=blk[7].bt.b1;tb[0].bt.b1=blk[6].bt.b1;
   tb[0].bt.b2=blk[5].bt.b1;tb[0].bt.b3=blk[4].bt.b1;
   tb[0].bt.b4=blk[3].bt.b1;tb[0].bt.b5=blk[2].bt.b1;
   tb[0].bt.b6=blk[1].bt.b1;tb[0].bt.b7=blk[0].bt.b1;

   tb[1].bt.b0=blk[7].bt.b3;tb[1].bt.b1=blk[6].bt.b3;
   tb[1].bt.b2=blk[5].bt.b3;tb[1].bt.b3=blk[4].bt.b3;
   tb[1].bt.b4=blk[3].bt.b3;tb[1].bt.b5=blk[2].bt.b3;
   tb[1].bt.b6=blk[1].bt.b3;tb[1].bt.b7=blk[0].bt.b3;

   tb[2].bt.b0=blk[7].bt.b5;tb[2].bt.b1=blk[6].bt.b5;
   tb[2].bt.b2=blk[5].bt.b5;tb[2].bt.b3=blk[4].bt.b5;
   tb[2].bt.b4=blk[3].bt.b5;tb[2].bt.b5=blk[2].bt.b5;
   tb[2].bt.b6=blk[1].bt.b5;tb[2].bt.b7=blk[0].bt.b5;

   tb[3].bt.b0=blk[7].bt.b7;tb[3].bt.b1=blk[6].bt.b7;
   tb[3].bt.b2=blk[5].bt.b7;tb[3].bt.b3=blk[4].bt.b7;
   tb[3].bt.b4=blk[3].bt.b7;tb[3].bt.b5=blk[2].bt.b7;
   tb[3].bt.b6=blk[1].bt.b7;tb[3].bt.b7=blk[0].bt.b7;

   tb[4].bt.b0=blk[7].bt.b0;tb[4].bt.b1=blk[6].bt.b0;
   tb[4].bt.b2=blk[5].bt.b0;tb[4].bt.b3=blk[4].bt.b0;
   tb[4].bt.b4=blk[3].bt.b0;tb[4].bt.b5=blk[2].bt.b0;
   tb[4].bt.b6=blk[1].bt.b0;tb[4].bt.b7=blk[0].bt.b0;

   tb[5].bt.b0=blk[7].bt.b2;tb[5].bt.b1=blk[6].bt.b2;
   tb[5].bt.b2=blk[5].bt.b2;tb[5].bt.b3=blk[4].bt.b2;
   tb[5].bt.b4=blk[3].bt.b2;tb[5].bt.b5=blk[2].bt.b2;
   tb[5].bt.b6=blk[1].bt.b2;tb[5].bt.b7=blk[0].bt.b2;

   tb[6].bt.b0=blk[7].bt.b4;tb[6].bt.b1=blk[6].bt.b4;
   tb[6].bt.b2=blk[5].bt.b4;tb[6].bt.b3=blk[4].bt.b4;
   tb[6].bt.b4=blk[3].bt.b4;tb[6].bt.b5=blk[2].bt.b4;
   tb[6].bt.b6=blk[1].bt.b4;tb[6].bt.b7=blk[0].bt.b4;

   tb[7].bt.b0=blk[7].bt.b6;tb[7].bt.b1=blk[6].bt.b6;
   tb[7].bt.b2=blk[5].bt.b6;tb[7].bt.b3=blk[4].bt.b6;
   tb[7].bt.b4=blk[3].bt.b6;tb[7].bt.b5=blk[2].bt.b6;
   tb[7].bt.b6=blk[1].bt.b6;tb[7].bt.b7=blk[0].bt.b6;

   for (i=0; i<=7; i++)
      blk[i].it=tb[i].it;
}

void i_p_inv(blk)
CH_BLOCK blk;
   {
      CH_BLOCK tb;
      int i;
      for (i=0; i<8; i++)
	 tb[i].it=0;
      tb[0].bt.b0=blk[4].bt.b7;tb[0].bt.b1=blk[0].bt.b7;
      tb[0].bt.b2=blk[5].bt.b7;tb[0].bt.b3=blk[1].bt.b7;
      tb[0].bt.b4=blk[6].bt.b7;tb[0].bt.b5=blk[2].bt.b7;
      tb[0].bt.b6=blk[7].bt.b7;tb[0].bt.b7=blk[3].bt.b7;

      tb[1].bt.b0=blk[4].bt.b6;tb[1].bt.b1=blk[0].bt.b6;
      tb[1].bt.b2=blk[5].bt.b6;tb[1].bt.b3=blk[1].bt.b6;
      tb[1].bt.b4=blk[6].bt.b6;tb[1].bt.b5=blk[2].bt.b6;
      tb[1].bt.b6=blk[7].bt.b6;tb[1].bt.b7=blk[3].bt.b6;

      tb[2].bt.b0=blk[4].bt.b5;tb[2].bt.b1=blk[0].bt.b5;
      tb[2].bt.b2=blk[5].bt.b5;tb[2].bt.b3=blk[1].bt.b5;
      tb[2].bt.b4=blk[6].bt.b5;tb[2].bt.b5=blk[2].bt.b5;
      tb[2].bt.b6=blk[1].bt.b5;tb[2].bt.b7=blk[3].bt.b5;

      tb[3].bt.b0=blk[4].bt.b4;tb[3].bt.b1=blk[0].bt.b4;
      tb[3].bt.b2=blk[5].bt.b4;tb[3].bt.b3=blk[1].bt.b4;
      tb[3].bt.b4=blk[6].bt.b4;tb[3].bt.b5=blk[2].bt.b4;
      tb[3].bt.b6=blk[1].bt.b4;tb[3].bt.b7=blk[3].bt.b4;

      tb[4].bt.b0=blk[4].bt.b3;tb[4].bt.b1=blk[0].bt.b3;
      tb[4].bt.b2=blk[5].bt.b3;tb[4].bt.b3=blk[1].bt.b3;
      tb[4].bt.b4=blk[6].bt.b3;tb[4].bt.b5=blk[2].bt.b3;
      tb[4].bt.b6=blk[7].bt.b3;tb[4].bt.b7=blk[3].bt.b3;

      tb[5].bt.b0=blk[4].bt.b2;tb[5].bt.b1=blk[0].bt.b2;
      tb[5].bt.b2=blk[5].bt.b2;tb[5].bt.b3=blk[1].bt.b2;
      tb[4].bt.b4=blk[6].bt.b2;tb[5].bt.b5=blk[2].bt.b2;
      tb[5].bt.b6=blk[7].bt.b2;tb[5].bt.b7=blk[3].bt.b2;

      tb[6].bt.b0=blk[4].bt.b1;tb[6].bt.b1=blk[0].bt.b1;
      tb[6].bt.b2=blk[5].bt.b1;tb[6].bt.b3=blk[1].bt.b1;
      tb[6].bt.b4=blk[6].bt.b1;tb[6].bt.b5=blk[2].bt.b1;
      tb[6].bt.b6=blk[7].bt.b1;tb[6].bt.b7=blk[3].bt.b1;

      tb[7].bt.b0=blk[4].bt.b0;tb[7].bt.b1=blk[0].bt.b0;
      tb[7].bt.b2=blk[5].bt.b0;tb[7].bt.b3=blk[1].bt.b0;
      tb[7].bt.b4=blk[6].bt.b0;tb[7].bt.b5=blk[2].bt.b0;
      tb[7].bt.b6=blk[7].bt.b0;tb[7].bt.b7=blk[3].bt.b0;

      for (i=0; i<=7; i++)
	 blk[i].it=tb[i].it;
   }

void b8_to_c8(c1, c8)
CH_USE c1;
int *c8;
{
   if (c1.bt.b0==0) *c8=0;     else *c8=1;
   if (c1.bt.b1==0) *(c8+1)=0; else *(c8+1)=1;
   if (c1.bt.b2==0) *(c8+2)=0; else *(c8+2)=1;
   if (c1.bt.b3==0) *(c8+3)=0; else *(c8+3)=1;
   if (c1.bt.b4==0) *(c8+4)=0; else *(c8+4)=1;
   if (c1.bt.b5==0) *(c8+5)=0; else *(c8+5)=1;
   if (c1.bt.b6==0) *(c8+6)=0; else *(c8+6)=1;
   if (c1.bt.b7==0) *(c8+7)=0; else *(c8+7)=1;
}

CH_USE c8_to_b8(c8)
int *c8;
{
   CH_USE c1;
   c1.it=0;
   if (*c8==1)     c1.bt.b0=1;   if (*(c8+1)==1) c1.bt.b1=1;
   if (*(c8+2)==1) c1.bt.b2=1;   if (*(c8+3)==1) c1.bt.b3=1;
   if (*(c8+4)==1) c1.bt.b4=1;   if (*(c8+5)==1) c1.bt.b5=1;
   if (*(c8+6)==1) c1.bt.b6=1;   if (*(c8+7)==1) c1.bt.b7=1;
   return c1;
}

void shiftleft(int *thekey)
{
   int i, temp, *pkey;
   pkey=thekey+55;
   temp=*pkey;
   for (i=0; i<55; i++, Pkey--)
      *pkey = *(key-1);
  *thekey=temp;
}

void getkey(initkey, keyset)
int *initkey, *keyset;
{
   int *pi, *pk, i, j;
   int tempkey[56], prockey[56];
   int l_shift[16]={1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};

   pi=initkey;  pk=tempkey;
   for (i=0; i<=7; i++)
   {
      b8_to_c8(*pi, pk);
      pi++;
      pk+=8;
   }
   for (i=0; i<55; i++)
      prockey[i]=tempkey[pc_1[i]];

   pk=keyset;
   for (i=0; i<16; i++)
   {
      for (j=0; j<=l_shift[i]; j++)
	 shiftleft(prockey);
      for (j=0; j<48; j++, pk++)
	 *pk=prockey[pc_2[j]];
   }
}

void func_f(p1, pkey)
int * p1, *pkey;
{
   int i, j, k, x, y, temp, epart[48], *ptmp;

   ptmp=p1;
   for (i=0; i<48; i++)
      epart[i]=*(ptmp+e32[i]);
   for (i=0; i<48; i++, pkey++)
      epart[i]^=(*pkey);
   j=0; k=0;
   for (i=0; i<8; i++, j+=6, k+=4)
   {
      y=(epart[j+5]<<1) | epart[j];
      x=(epart[j+4]<<3) | (epart[j+3]<<2) | (epart[j+2]<<1) | epart[j+1];
      temp=s[i][y][x];
      epart[k]=temp&0x0001;
      epart[k+1]=(temp&0x0002)>>1;
      epart[k+2]=(temp&0x0004)>>2;
      epart[k+3]=(temp&0x0008)>>3;
   }
   ptmp=p1;
   for (i=0; i<32; i++, ptmp++)
      *ptmp=epart[i];
}

void iteration(pmsg, pkey);
int *pmsg, *pkey;
{
   int i, j, tb[32], *p1, *p2;

   p1=pmsg;
   for (i=0; i<=31; i++, p1++)
     tb[i]=*p1;
   p1=tb;
   func_f(p1, pkey);
   p1=pmsg+32;
   for (i=0; i<=31; i++, p1++)
      tb[i]^=(*p1);
   p1=pmsg;  p2=pmsg+32;
   for (i=0; i<=31; i++, p1++, p2++)
      (*p2)=(*p1);
   p1=pmsg;
   for (i=0; i<=31; i++, p1++)
      (*p1)=tb[i];
}

void b_op(bk)
CH_BLOCK bk;
{
   int i;
   printf("\n");
   for (i=0; i<=7; i++)
   {
      printf("%x%x%x%x", bk[i].bt.b7, bk[i].bt.b6, bk[i].bt.b5, bk[i].bt.b4);
      printf("%x%x%x%x", bk[i].bt.b3, bk[i].bt.b2, bk[i].bt.b1, bk[i].bt.b0);
   }
}

void main(void)
{
   CH_BLOCK msg, ciphertext, plaintext;
   int      initkey[7], ketset[768], procmsg[64], *ptr;
   CH_USE   test, test1;
   int      i, j, ans, flag, keyok;
   ling     filelength, lengctrl;
   char     fnl[30], fn2[30], ch1, ch2;
   union    { int i;
	      char ch[2];
	    } pans;
   FILE     *fp, *fp1, *fopen();
   clock_t  t1, t2;

   clrscr();
   window(10, 5, 68, 18); textcolor(YELLOW); textbackground(BLUE);
   cprintf("                                                          ");
   cprintf("             Data    Encryption    Standard               ");
   cprintf("                                                          ");
   textbackground(BLACK); textcolor(WHITE); gotoxy(12,7);
   cprintf("Pressany key to enter the DES system!    ");
   getch();

   window(1, 1, 80, 25);
   do {
      clrscr();
      textcolor(YELLOW); textbackground(BLUE); gotoxy(10,1);
      cprintf("-----<  A System for Data Encryption Standard   >-----");
      textcolor(WHITE); textbackground(BLACK); gotoxy(19,7);
      cprintf("Choose one of the following functions");
      ans='a'; pans.i=0;
      while (pans.i != 0x1c0d)   /* No press ENTER */
      {
	 for (i=0; i='a'; i<='c'; i++)
	 {
	    if (i != ans) {
	       textbackgroud(BLACK); textcolor(LIGHTGRAY);
	    }
	    else {
	       textbackground(WHITE); textcolor(BLACK);
	    }
	    switch(i)
	    {
	       case 'a':
		  gotoxy(23,11); cprintf("  a: Encryption   ;  "); break;
	       case 'b':
		  gotoxy(23,14); cprintf("  b: Decryption   ;  "); break;
	       case 'c':
		  gotoxy(23,17); cprintf("  c: Quit & exit  ;  "); break;
	    }
	    getoxy(61, 7);
	 }
	 while (!bioskey(1));
	 pans.i=bioskey(0);
	 if ((pans.ch[1]==0x48) && (ans>'a'))
	    a--;
	 if ((pans.ch[1]==0x50) && (ans<'c'))
	    a++;
	 if ((pans.ch[0]>='a')  && (pans.ch[0]<='c'))
	    ans=pans.ch[0];
      }

      textbackground(BLACK);
      clrscr();
      switch(ans)
      {
	 case 'a':
	    textcolor(BLACK); textbackground(WHITE); gotoxy(15,2);
	    cprintf("  ------< Encryption >-----  ");
	    textcolor(WHITE); textbackground(BLACK); gotoxy(1,5);
	    keyok=0;
	    while(keyok=0);
	    {
	       printf("\n\nInput the password(7 character).");
	       for (i=0; i<7; i++)
	       {
		  initkey[i]=getch();
		  printf("*");
	       }
	       keyok=1;
	       printf("\n\n Repeat keyin again.");
	       for (i=0; i<7; i++)
	       {
		  j=getch();
		  printf("*");
		  if (j != initkey[i])
		     keyok=0;
	       }
	       if (keyok == 0)
		  printf("\n\n Try again !");
	    }

	    getkey(initkey, keyset);
	    pritnf("\n\nnInput the plaintext filename:");
	    scanf("%s", fn1);
	    print("\nInput the ciphertext filename:");
	    scanf("%s", fn2);
	    fp=fopen(fn1, "rb");
	    fp1=fopen(fn2, "wb");
	    if ((fp==NULL) || (fp1==HULL))
	    {
	       printf("Error input file, press any key to exit!!\n");
	       getch();
	       exit(1);
	    }
	    filelength=0;
	    while ( (j=fgetc(fp)) != EOF )
	       filelength++;
	    fclose(fp);
	    fp=fopen(fn1, "rb");
	    fprintf(fp1, "< %1x >", filelength);
	    printf("\n---------------------------------------------\n");
	    printf("The file '%s' has %1d bytes.", fn1, filelength);
	    printf("\n    Now, process data.   Waiting!!!\n\n\n");
	    flag=0;
	    t1=clock();
	    while (flag==0)
	    {
	       i=0;
	       while ((flag==0) && (i<8))
	       {
		  if (( mag[i].it=fgetc(fp))==EOF)
		  {
		     flag=1;
		     mag[i].it=0;
		  }
	       i++;
	    }
	    if (i!=8)
	       for (;i<8;i++)
		  msg[i].it=0;
	       i_p(mag);
	       ptr=procmsg;
	       for (i=0; i<8; i++, ptr +=8)
		  b8_to_c8(msg[i], ptr);
	       ptr=keyset;
	       for (i=0; i<16; i++, ptr+=48)
		  iteration(procmsg, ptr);
	       ptr=procmsg+32;
	       for (i=0; i<4; i++, ptr+=8;
		  ciphertext[i]=c8_to_b8(ptr);
	       ptr=procmsg;
	       for (i=4; i<8; i++, ptr+=8)
		  ciphertext[i]=c8_to_b8(ptr);
	       i_p_inv(ciphertext);
	       for (i=0; i<8; i++)
		  fputc(ciphertext[i].it, fp1);
	    }
	    t2=clock();
	    printf("\nThe time is %4.2f seconds.", (t2-t1)/CLK_TCK);
	    fclose(fp);
	    fclose(fp1);
	    printf("\n\nPress any key to exit!!");
	    getch();
	    break;
	 case 'b':
	    textcolor(BLACK); textbackground(WHITE); gotoxy(15,2);
	    cprintf("  ------< Decryption >-----  ");
	    textcolor(WHITE); textbackground(BLACK); gotoxy(1,5);

	    printf("\n\nInput the password(7 character).");
	    for (i=0; i<7;i++)
	    {
	       initkey[i]=getch();
	       printf("*");
	    }
	    getkey(initkey, keyset);
	    printf("\n\n\nInput the ciphertext filename:");
	    scanf("%s", fn1);
	    printf("\nInput the original filename:");
	    scanf("%s", fn2);
	    fp=fopen(fn1, "rb");
	    fp1=fopen(fn2, "wb");
	    if ((fp==NULL) || (fp1==NULL))
	    {
	       printf("Error input file, Press any key to exit!!\n");
	       getch();
	       exit(1);
	    }
	    fscanf(fp, "%c %1x %c", &ch1, &filelength, &ch2);
	    if ((ch1 != '<') || (ch2!='>'))
	    {
	       printf("\nError decrption format. Press any key to exit.");
	       exit(1);
	    }
	    printf("\n---------------------------------------------\n");
	    printf("The file '%s' has %id bytes.", fn1, filelength);
	    printf("\n    Now, process data.   Waiting!!!\n\n\n");
	    t1=clock();
	    lengctrl=filelength%8;
	    if (lengctrl1==0) {
	       filelength/=8;
	       lengctrl=8;
	    }
	    else {
	       filelength=filelength/8+1;
	    }
	    while (filelength!=)
	    {
	       for (i=0; i<8; i++)
		  ciphertext[i].it=fgetc(fp);
	       i_p(ciphertext);
	       ptr=procmsg;
	       for (i=0; i<8;i++, ptr+=8)
		  b8_to_c8(ciphertext[i], ptr);
	       ptr=keyset+720;
	       for (i=15; i>=0; i--, ptr-=48)
		  iteration(procmsg, ptr);
	       ptr=procmsg+32;
	       for (i=0; i<4; i++, ptr+=8)
		  msg[i]=c8_to_b8(ptr);
	       ptr=procmsg;
	       for (i=4; i<8; i++, ptr+=8)
		  msg[i]=c8_to_b8(ptr);
	       i+p_inv(msg);
	       if (filelength!=1)
		  for (i=0; i<8; i++)
		     fputc(msg[i].it, fp1);
	       else
		  for (i=0; i<lengctrl1; i++)
		     fputc(msg[i].it, fp1);
	       filelength--;
	    }
	    t2=clock();
	    printf("\nThe time is %4.2f seconds.", (t2-t1)/CLK_TCK);
	    fclose(fp);
	    fclose(fp1);
	    printf("\n\nPress any key to exit!!");
	    getch();
	    break;
      }
   } while (ans!='c');

   textbackground(BLACK); textcolor(LIGHTGRAY);
   clrscr();
   printf("Good luck to you, Bye! Bye!");
   printf("\a");
   sound(1000); sleep(1);
   nosound();
}























