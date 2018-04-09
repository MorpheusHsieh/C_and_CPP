#include <conio.h>
#include <stdio.h>
#include <string.h>

float **matrix(long, long, long, long);
void free_matrix(float**, long, long, long, long);
void Fast_DCT(float**, long, long, int);

void main(void)
{
  FILE *ptr1, *ptr2;

  char fn1[81];
  printf("Enter image name: ");
  scanf("%s", fn1);
  strcat(fn1, ".raw");

  int s1;
  printf("Enter size: ");
  scanf("%d", &s1);

  unsigned char *p1 = new unsigned char[s1*s1];
  ptr1=fopen(fn1, "rb");
  fread(p1,s1,s1,ptr1);
  fclose(ptr1);

  int i, j;
  float **m = matrix(1,s1,1,s1);
  for (i=0; i<s1; i++)
    for (j=0; j<s1; j++)
      m[i+1][j+1] = (float)(p1[i*s1+j]-128);

  Fast_DCT(m,s1,8,1);
  Fast_DCT(m,s1,8,-1);

  for (i=0; i<s1; i++)
    for (j=0; j<s1; j++)
      p1[i*s1+j] = (unsigned char)(m[i+1][j+1]+128.0);

  ptr2=fopen("test.raw", "wb");
  fwrite(p1,s1,s1,ptr2);
  fclose(ptr2);
  delete [] p1;

  printf("\nPress any key to continue...");
  getch();
}




