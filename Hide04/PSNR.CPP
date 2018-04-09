// Program : PSNR.CPP
// Author  : Morpheus
// Date    : 1998.12.14
#include <math.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include "image.h"

void main(void)
{
  clrscr();
  printf("Compute image PSNR.\n");

  char fn1[81];  int size1;
  printf("\nEnter source image filename: ");
  scanf("%s", fn1);  strcat(fn1, ".raw");
  printf("Enter size: ");
  scanf("%d", &size1);
  Image img1(fn1,size1);
  img1.Read_From_File();

  char fn2[81];  int size2;
  printf("\nEnter source image filename: ");
  scanf("%s", fn2);  strcat(fn2, ".raw");
  printf("Enter size: ");
  scanf("%d", &size2);
  Image img2(fn2,size2);
  img2.Read_From_File();

  unsigned char *p1 = new unsigned char [size1*size1];
  img1.Read_Pixel(p1);
  unsigned char *p2 = new unsigned char [size2*size2];
  img2.Read_Pixel(p2);

  int i;
  double pw1 = 0.0, pw2 = 0.0;
  for (i=0; i<size1*size1; i++)
  {
    pw1 += pow((double)p1[i],2.0);
    pw2 += pow((double)(p1[i]-p2[i]),2.0);
  }
  
  pw1 /= (size1*size1);
  pw2 /= (size2*size2);
  printf("\nPower 1: %d,\tPower 2: %d", pw1, pw2);
  printf("\nPSNR = %f dB", 20.0*log(pw1/pw2));

  delete [] p1;
  delete [] p2;
  printf("\nPress any key to continue...");
  getch();
}



