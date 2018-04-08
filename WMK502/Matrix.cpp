//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#define NR_END 1

#include "Matrix.h"
#include "Message.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
extern void Send_Message(AnsiString);
//---------------------------------------------------------------------------
Byte **bmatrix(long nrl, long nrh, long ncl, long nch)
/* allocate a int matrix with subscript range m[nrl..nrh][ncl..nch] */
{
	long i, nrow=nrh-nrl+1, ncol=nch-ncl+1;
	Byte **m;

	/* allocate pointers to rows */
	m = new Byte* [(nrow+NR_END)*sizeof(Byte*)];
	if (!m)
      Send_Message("Allocation failure 1 in lmatrix()");
	m += NR_END;
	m -= nrl;

	/* allocate rows and set pointers to them */
	m[nrl] = new Byte[(nrow*ncol+NR_END)*sizeof(Byte)];
    if (!m[nrl])
      Send_Message("Allocation failure 2 in lmatrix()");
	m[nrl] += NR_END;
	m[nrl] -= ncl;

	for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;
	/* return pointer to array of pointers to rows */
	return m;
}
//---------------------------------------------------------------------------
float **matrix(long nrl, long nrh, long ncl, long nch)
/* allocate a int matrix with subscript range m[nrl..nrh][ncl..nch] */
{
	long i, nrow=nrh-nrl+1, ncol=nch-ncl+1;
	float **m;

	/* allocate pointers to rows */
	m = new float* [(nrow+NR_END)*sizeof(float*)];
	if (!m)
      Send_Message("Allocation failure 1 in lmatrix()");
	m += NR_END;
	m -= nrl;

	/* allocate rows and set pointers to them */
	m[nrl] = new float[(nrow*ncol+NR_END)*sizeof(float)];
    if (!m[nrl])
      Send_Message("Allocation failure 2 in lmatrix()");
	m[nrl] += NR_END;
	m[nrl] -= ncl;

	for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;
	/* return pointer to array of pointers to rows */
	return m;
}
//---------------------------------------------------------------------------
long **lmatrix(long nrl, long nrh, long ncl, long nch)
/* allocate a int matrix with subscript range m[nrl..nrh][ncl..nch] */
{
	long i, nrow=nrh-nrl+1, ncol=nch-ncl+1;
	long **m;

	/* allocate pointers to rows */
	m = new long* [(nrow+NR_END)*sizeof(long*)];
	if (!m)
      Send_Message("Allocation failure 1 in lmatrix()");
	m += NR_END;
	m -= nrl;

	/* allocate rows and set pointers to them */
	m[nrl] = new long [(nrow*ncol+NR_END)*sizeof(long)];
    if (!m[nrl])
      Send_Message("Allocation failure 2 in lmatrix()");
	m[nrl] += NR_END;
	m[nrl] -= ncl;

	for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;
	/* return pointer to array of pointers to rows */
	return m;
}
//---------------------------------------------------------------------------
void free_bmatrix(Byte **m, long nrl, long nrh, long ncl, long nch)
/* free an int matrix allocated by imatrix() */
{
  delete [] (m[nrl]+ncl-NR_END);
  delete [] (m+nrl-NR_END);
}
//---------------------------------------------------------------------------
void free_matrix(float **m, long nrl, long nrh, long ncl, long nch)
/* free an int matrix allocated by imatrix() */
{
  delete [] (m[nrl]+ncl-NR_END);
  delete [] (m+nrl-NR_END);
}
//---------------------------------------------------------------------------
void free_lmatrix(long **m, long nrl, long nrh, long ncl, long nch)
/* free an int matrix allocated by imatrix() */
{
  delete [] (m[nrl]+ncl-NR_END);
  delete [] (m+nrl-NR_END);
}
//---------------------------------------------------------------------------



