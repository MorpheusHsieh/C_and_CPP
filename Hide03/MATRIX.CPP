// allocation a float matrix with subscript range m[nr1..nrh][ncl..nch]
#include <stdlib.h>
#define NR_END 1
#define FREE_ARG char*

float **matrix(long nrl, long nrh, long ncl, long nch)
{
  long i, nrow = nrh-nrl+1, ncol = nch-ncl+1;
  float **m;

  m = new float* [nrow+NR_END];
  m += NR_END;
  m -= nrl;

  m[nrl] = new float [nrow*ncol+NR_END];
  m[nrl] += NR_END;
  m[nrl] -= ncl;

  for (i=nrl+1; i<=nrh; i++)
    m[i] = m[i-1] + ncol;

  return m;
}

void free_matrix(float **m, long nrl, long nrh, long ncl, long nch)
{
	delete((FREE_ARG) (m[nrl]+ncl-NR_END));
	delete((FREE_ARG) (m+nrl-NR_END));
}

