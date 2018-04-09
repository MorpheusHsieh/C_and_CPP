// Program : Neural.CPP
// Author  : Morpheus
// Date    : 98.12.14
#include <time.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#define NR_END 1
#define PATTERN_LEN  20
#define PATTERN_NUM   4

int **imatrix(long, long, long, long);
void free_imatrix(int **, long, long, long, long);
void nrerror(char*);
void pattern_generate(int**, int, int);
void display_pattern(int **, int, int);
void Compute_weight_matrix(int**, int**, int, int);
int  Hopfield(int*, int**, int);

void main(void)
{
  clrscr();
  printf("Hopfield network to solved the problem of associative memory.\n");

  // Design prototype and input pattern
  // pat[1]...pat[n-1] --> prototype pattern
  // pat[n]            --> input pattern
  int num = PATTERN_NUM, len = PATTERN_LEN;
  int **pat = imatrix(1,num,1,len);
  pattern_generate(pat,len,num);
  display_pattern(pat,20,4);
  printf("\n");

  // find weight matrix with target patterns
  int **wm=imatrix(1,len,1,len);
  Compute_weight_matrix(wm,pat,len,num-1);

  // Compare neural network
  int *a = new int [len];
  for (int i=0; i<len; i++)
    a[i] = pat[4][i+1];

  int v1, v2, toggle = 1, cnt1 = 0, cnt2 = 0;
  while (toggle)
  {
    v1 = Hopfield(a, wm, len);

    a[cnt1] *= -1;
    v2 = Hopfield(a, wm, len);
    if (v1 < v2)
    {
      a[cnt1] *= -1;
      cnt2++;
      if (cnt2 == len)
        toggle = 0;
    }
    else if (v1 >= v2)
      cnt2 = 0;
    if (cnt1++ == len)
      cnt1 = 0;
  }
  for(int i=1; i<=len; i++)
    pat[4][i] = a[i-1];
  display_pattern(pat,20,4);

  int i, h1 = 0, h2 = 0, h3 = 0;
  for (i=1; i<=len; i++)
  {
    if (pat[1][i] != a[i-1])
      h1++;
    if (pat[2][i] != a[i-1])
      h2++;
    if (pat[3][i] != a[i-1])
      h3++;
  }
  printf("\n\nHammin distance 1: %d", h1);
  printf("\nHammin distance 2: %d", h2);
  printf("\nHammin distance 3: %d", h3);

  delete [] a;
  free_imatrix(wm,1,len,1,len);
  free_imatrix(pat,1,num,1,len);
  printf("\n\nPress any key to continue...");
  getch();
}

void pattern_generate(int **pat, int len, int num)
{
  randomize();
  int i, j, rnd, seed = 10;
  for (i=1; i<=num; i++)
  {
    for (j=1; j<=len; j++)
    {
      rnd = random(seed);
      if (rnd >= (seed/2))
        pat[i][j] = -1;
      else
        pat[i][j] = 1;
    }
  }
}

void Compute_weight_matrix(int **wm, int **pat, int len, int num)
{
  int **t = imatrix(1,len,1,num);
  int **p = imatrix(1,num,1,len);

  int i, j, k;
  for (i=1; i<=num; i++)
  {
    for (j=1; j<=len; j++)
    {
      t[j][i] = pat[i][j];
      p[i][j] = pat[i][j];
    }
  }

  for (i=1; i<=len; i++)
  {
    for (j=1; j<=len; j++)
    {
      wm[i][j] = 0;
      for (k=1; k<=3; k++) {
        wm[i][j] += t[i][k]*p[k][j];
      }
    }
  }

//  printf("\n");
//  display_pattern(wm,len,len);
  free_imatrix(t,1,len,1,num);
  free_imatrix(p,1,num,1,len);
}

int Hopfield(int *a, int **wm, int len)
{
  int i, j, k;
  int *tmp1 = new int [len];
  for (i=1; i<=1; i++)
  {
    for (j=1; j<=len; j++)
    {
      tmp1[j-1] = 0;
      for (k=1; k<=len; k++)
        tmp1[j-1] += a[k-1]*wm[k][j];
    }
  }

  int v = 0;
  for (i=0; i<len; i++)
    v += tmp1[i] * a[i];
  return (v*-0.5);
}

void display_pattern(int **pat, int len, int num)
{
  int i, j;
  for (i=1; i<=num; i++)
  {
    printf("\nPattern[%2d]: ", i);
    for (j=1; j<=len; j++)
      if (pat[i][j] == 1)
        printf("1");
      else
        printf("0");
//        printf("%2d ", pat[i][j]);
  }
}

int **imatrix(long nrl, long nrh, long ncl, long nch)
{
	long i, nrow=nrh-nrl+1, ncol=nch-ncl+1;
	int **m;

	/* allocate pointers to rows */
	m = new int* [(nrow+NR_END)*sizeof(int*)];
	if (!m) nrerror("allocation failure 1 in matrix()");
	m += NR_END;
	m -= nrl;


	/* allocate rows and set pointers to them */
	m[nrl] = new int [(nrow*ncol+NR_END)*sizeof(int)];
   if (!m[nrl]) nrerror("allocation failure 2 in matrix()");
	m[nrl] += NR_END;
	m[nrl] -= ncl;

	for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;

	/* return pointer to array of pointers to rows */
	return m;
}

void free_imatrix(int **m, long nrl, long nrh, long ncl, long nch)
{
	delete [] (m[nrl]+ncl-NR_END);
	delete [] (m+nrl-NR_END);
}

void nrerror(char error_text[])
{
	fprintf(stderr,"Numerical Recipes run-time error...\n");
	fprintf(stderr,"%s\n",error_text);
	fprintf(stderr,"...now exiting to system...\n");
	exit(1);
}
