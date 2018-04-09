// Program : LSFR01.CPP
// Author  : Morpheus
// Date    : 98.09.24

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SEED_LEN    32
#define POLY_LEN    32

int LFSR(unsigned long, unsigned long*);

void main(void)
{
  clrscr();
  printf("Generate a Linear Shift Feedback Register sequence.\n");

  static char polynomial[161], order[11];
  printf("\nEnter polynomials (Ex: x^18+x^5+x^2+x^1+1): ");
  scanf("%s", polynomial);

  static int PolyCoef[32];
  int cnt = 0, len = strlen(polynomial);
  while (cnt <= len)
  {
    cnt++;
    if (polynomial[cnt] == '^')
    {
      cnt++;
      if (polynomial[cnt] > 57 || polynomial[cnt] < 48)
      {
        printf("\nError, polynomial is wrong, press any key...");
        getch();
        exit(1);
      }
      else
      {
        strcpy(order, &polynomial[cnt]);
        cnt++;
        while (polynomial[cnt] < 57 && polynomial[cnt] > 48)
          strcat(order, &polynomial[cnt++]);

        int order_num = atoi(order);
        PolyCoef[order_num] = 1;
      }
    }
  }
/*  for (int i=POLY_LEN-1; i>=0; i--)
    if (PolyCoef[i] == 1) printf("%d,\t", i);
  printf("\n");*/

  unsigned long pmask = 0;
  for (int i=31; i>0; i--)
    if (PolyCoef[i] == 1)
      pmask |= (0x01 << i);
  pmask &= 0xFFFFFFFE;    

  int i, plen;
  unsigned long *iseed = new unsigned long;
  printf("\nEnter seed value: ");
  scanf("%d", iseed);
  printf("\nEnter pseudo number bit length: ");
  scanf("%d", &plen);
  for (i=0; i<plen; i++)
    printf("%d", LFSR(pmask, iseed));
  printf("\n");

  printf("\nPress any key exit this program...");
  getch();
}

int LFSR(unsigned long pmask, unsigned long *iseed)
{
  bool toggle = true;
  unsigned long MaxOrder = 32;
  while (toggle)
  {
    MaxOrder--;
    if (((pmask & (0x01 << MaxOrder)) >> MaxOrder) == 0x01)
    {
      toggle = false;
    }
  }
  int newbit = ((*iseed & (0x01 << MaxOrder)) >> MaxOrder);

  for (int i=MaxOrder; i>0; i--)
    if (((pmask & (0x01 << i)) >> i) == 0x01)
      newbit ^= ((*iseed & (0x01 << i)) >> i);
  *iseed = (*iseed << 1) | newbit;
  return newbit;
}
