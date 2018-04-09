#include <stdlib.h>
#define NR_END 1
#define FREE_ARG char*

/* allocate a float vector with subscript range v[nl..nh] */
float *vector(long nl, long nh)
{
	float *v;

	v = new float [nh-nl+1+NR_END];
	return v-nl+NR_END;
}

void free_vector(float *v, long nl, long nh)
/* free a float vector allocated with vector() */
{
	delete((FREE_ARG) (v+nl-NR_END));
}
