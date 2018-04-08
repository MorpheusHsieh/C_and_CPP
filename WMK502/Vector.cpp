//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <stdlib.h>
#define NR_END 1
#define FREE_ARG char*

#include "Vector.h"
#include "Nrerror.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
void Show_ErrorMessage(AnsiString);
//---------------------------------------------------------------------------
/* allocate a float vector with subscript range v[nl..nh] */
float *vector(long nl, long nh)
{
	float *v;

	v = new float [nh-nl+1+NR_END];
	if (!v)
      Show_ErrorMessage("allocation failure 1 in matrix()");
	return v-nl+NR_END;
}
//---------------------------------------------------------------------------
void free_vector(float *v, long nl, long nh)
/* free a float vector allocated with vector() */
{
	delete((FREE_ARG) (v+nl-NR_END));
}
//---------------------------------------------------------------------------
