// Program : Troble Integration use Romberg
// Auther  : Avatar
// Date    : 1998/02/11
#include <math.h>
#include <conio.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#define NR_END 1
#define FREE_ARG char*

#define My_Func  2
#define X_Start  0
#define X_End    1
#define Y_Start  0
#define Y_End    1
#define Z_Start  0
#define Z_End    1

void nrerror(char error_text[])
/* Numerical Recipes standard error handler */
{
   fprintf(stderr,"Numerical Recipes run-time error...\n");
   fprintf(stderr,"%s\n",error_text);
   fprintf(stderr,"...now exiting to system...\n");
   exit(1);
}

float *vector(long nl, long nh)
/* allocate a float vector with subscript range v[nl..nh] */
{
   float *v;
   v=(float *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(float)));
   if (!v) nrerror("allocation failure in vector()");
   return v-nl+NR_END;
}

void free_vector(float *v, long nl, long nh)
/* free a float vector allocated with vector() */
{
   free((FREE_ARG) (v+nl-NR_END));
}

static float xsav, ysav;
static float (*nrfunc)(float, float, float);
float quad3d(float (*func)(float, float, float), float x1, float x2)
{
   float qgaus(float (*func)(float), float a, float b);
   float f1(float x);

   nrfunc=func;
   return qgaus(f1,x1,x2);
}

float qgaus(float (*func)(float), float a, float b)
{
	int j;
	float xr,xm,dx,s;
	static float x[]={0.0,0.1488743389,0.4333953941,
		0.6794095682,0.8650633666,0.9739065285};
	static float w[]={0.0,0.2955242247,0.2692667193,
		0.2190863625,0.1494513491,0.0666713443};

	xm=0.5*(b+a);
	xr=0.5*(b-a);
	s=0;
	for (j=1;j<=5;j++) {
		dx=xr*x[j];
		s += w[j]*((*func)(xm+dx)+(*func)(xm-dx));
	}
	return s *= xr;
}

float func(float x, float y, float z)
{
   return(My_Func);
}

float f1(float x)
{
   float qgaus(float (*func)(float), float a, float b);
   float f2(float y);
   float yy1(float), yy2(float);

   xsav=x;
   return qgaus(f2, yy1(x), yy2(x));
}

float f2(float y)
{
   float qgaus(float (*func)(float), float a, float b);
   float f3(float z);
   float z1(float, float), z2(float, float);

   ysav=y;
   return qgaus(f3, z1(xsav, y), z2(xsav, y));
}

float f3(float z)
{
   return (*nrfunc)(xsav, ysav, z);
}

float yy1(float x)
{
   return(Y_Start);
}

float yy2(float x)
{
   return(Y_End);
}

float z1(float x, float y)
{
   return(Z_Start);
}

float z2(float x, float y)
{
   return(Z_End);
}

void main(void)
{
   clrscr();
   printf("\n%f", quad3d(func, X_Start, X_End));
   getch();
}