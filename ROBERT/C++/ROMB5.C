// Program : Double Romberg Integration
// Auther  : Avatar
// Date    : 1998/02/06
#include <math.h>
#include <conio.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#define NR_END 1
#define FREE_ARG char*


/*--------------------- user define -----------------------*/
#define My_Func 2
#define yy1_x   0
#define yy2_x   1
#define z1_x_y  0
#define z2_x_y  1
/*---------------------------------------------------------*/

static double xsav,ysav;
static double (*nrfunc)(double,double,double);

//double q(double x)
//{
// return(z);
//}
/*------------------------ quad3d(func,x1,x2) ---------------------------*/
double quad3d(double (*func)(double, double, double), double x1, double x2)
{
	double qromb(double (*func)(double), double a, double b);
	double f1(double x);

	nrfunc=func;
	return qromb(f1,x1,x2);
}

/*------------------------ f1(x) ---------------------------*/
double f1(double x)
{
	double qromb(double (*func)(double), double a, double b);
	double f2(double y);
	double yy1(double),yy2(double);

	xsav=x;
	return qromb(f2,yy1(x),yy2(x));
}

/*------------------------ f2(y) ---------------------------*/
double f2(double y)
{
	double qromb(double (*func)(double), double a, double b);
	double f3(double z);
	double z1(double,double),z2(double,double);

	ysav=y;
	return qromb(f3,z1(xsav,y),z2(xsav,y));
}

/*------------------------ f3(z) ---------------------------*/
double f3(double z)
{
	return (*nrfunc)(xsav,ysav,z);
}
/* (C) Copr. 1986-92 Numerical Recipes Software 99. */

void nrerror(char error_text[])
/* Numerical Recipes standard error handler */
{
	fprintf(stderr,"Numerical Recipes run-time error...\n");
	fprintf(stderr,"%s\n",error_text);
	fprintf(stderr,"...now exiting to system...\n");
	exit(1);
}

double *vector(long nl, long nh)
/* allocate a double vector with subscript range v[nl..nh] */
{
	double *v;

	v=(double *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(double)));
	if (!v) nrerror("allocation failure in vector()");
	return v-nl+NR_END;
}

void free_vector(double *v, long nl, long nh)
/* free a double vector allocated with vector() */
{
	free((FREE_ARG) (v+nl-NR_END));
}


#define EPS     1.0e-6
#define JMAX        20
#define JMAXP (JMAX+1)
#define K            5
double qromb(double (*func)(double), double a, double b)
{
	void polint(double xa[], double ya[], int n, double x, double *y, double *dy);
	double trapzd(double (*func)(double), double a, double b, int n);
	void nrerror(char error_text[]);
	double ss,dss;
	double s[JMAXP+1],h[JMAXP+1];
	int j;

	h[1]=1.0;
	for (j=1;j<=JMAX;j++) {
		s[j]=trapzd(func,a,b,j);
		if (j >= K) {
			polint(&h[j-K],&s[j-K],K,0.0,&ss,&dss);
			if (fabs(dss) < EPS*fabs(ss)) return ss;
		}
		s[j+1]=s[j];
		h[j+1]=0.25*h[j];
	}
	nrerror("Too many steps in routine qromb");
	return 0.0;
}
#undef EPS
#undef JMAX
#undef JMAXP
#undef K

#define NRANSI
void polint(double xa[], double ya[], int n, double x, double *y, double *dy)
{
	int i,m,ns=1;
	double den,dif,dift,ho,hp,w;
	double *c,*d;

	dif=fabs(x-xa[1]);
	c=vector(1,n);
	d=vector(1,n);
	for (i=1;i<=n;i++) {
		if ( (dift=fabs(x-xa[i])) < dif) {
			ns=i;
			dif=dift;
		}
		c[i]=ya[i];
		d[i]=ya[i];
	}
	*y=ya[ns--];
	for (m=1;m<n;m++) {
		for (i=1;i<=n-m;i++) {
			ho=xa[i]-x;
			hp=xa[i+m]-x;
			w=c[i+1]-d[i];
			if ( (den=ho-hp) == 0.0) nrerror("Error in routine polint");
			den=w/den;
			d[i]=hp*den;
			c[i]=ho*den;
		}
		*y += (*dy=(2*ns < (n-m) ? c[ns+1] : d[ns--]));
	}
	free_vector(d,1,n);
	free_vector(c,1,n);
}
#undef NRANSI

#define FUNC(x) ((*func)(x))
double trapzd(double (*func)(double), double a, double b, int n)
{
	double x,tnm,sum,del;
	static double s;
	int it,j;

	if (n == 1) {
		return s=0.5*(b-a)*(FUNC(a)+FUNC(b));
	} else {
		for (it=1,j=1;j<n-1;j++) it <<= 1;
		tnm=it;
		del=(b-a)/tnm;
		x=a+0.5*del;
		for (sum=0.0,j=1;j<=it;j++,x+=del) sum += FUNC(x);
		s=0.5*(s+(b-a)*sum/tnm);
		return s;
	}
}
#undef FUNC

double func(double x,double y,double z)
{
 return(My_Func);
}
/*------------------------ yy1(x) ---------------------------*/
double yy1(double x)
{
 return(yy1_x);
}

/*------------------------ yy2(x) ---------------------------*/
double yy2(double x)
{
 return(yy2_x);
}

/*------------------------ z1(x,y) ---------------------------*/
double z1(double x,double y)
{
 return(z1_x_y);
}

/*------------------------ z2(x,y) ---------------------------*/
double z2(double x,double y)
{
 return(z2_x_y);
}


void main(void)
{
   clrscr();
   printf("\n%f", quad3d(func,0,1));
   getch();
}