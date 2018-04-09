#include <stdlib.h>
#define NR_END 1

extern void nrerror(char error_text[]);

int **imatrix(long nrl, long nrh, long ncl, long nch)
/* allocate a int matrix with subscript range m[nrl..nrh][ncl..nch] */
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
/* free an int matrix allocated by imatrix() */
{
	delete [] (m[nrl]+ncl-NR_END);
	delete [] (m+nrl-NR_END);
}
