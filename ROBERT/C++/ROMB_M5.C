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
#define pi 3.141592654
#define EPS     1.0e-6

/*----------------------------- user define ---------------------------*/
//Double integration -> int(int(func(y,x),y=yy1_x..yy2_x),x=X_1..X_2);
//----------------------------------------------------------------------
//#define myfunc 0.5*q(y*sqrt(aaa*Eb_N0)*cos(x))*rician_pdf(2,y)*theta_pdf(6.9897*Eb_N0,x)
  // below 3 equations are for Rayleigh pdf:
  #define myfunc 0.5*q(y*sqrt(aaa*Eb_N0))*rician_pdf(8,y)
  #define yy1_x   0
  #define yy2_x   3
  #define X_1  0
  #define X_2  1
//#define yy1_x   0
//#define yy2_x   3
//#define X_1    -pi
//#define X_2     pi
/*---------------------------------------------------------*/

static double xsav;
static double (*nrfunc)(double,double);
int step_number=5;
double Eb_N0=120.0;
double aaa=1.38;
double sum=0.0;

/*---------------------------- bessi0(x) -------------------------------*/
double bessi0(double x)
{
	double ax,ans;
	double y;
	// EPS=1.6e-7

	if ((ax=fabs(x)) < 3.75) {
		y=x/3.75;
		y*=y;
		ans=1.0+y*(3.5156229+y*(3.0899424+y*(1.2067492
			+y*(0.2659732+y*(0.360768e-1+y*0.45813e-2)))));
	} else {
		y=3.75/ax;
		ans=(exp(ax)/sqrt(ax))*(0.39894228+y*(0.1328592e-1
			+y*(0.225319e-2+y*(-0.157565e-2+y*(0.916281e-2
			+y*(-0.2057706e-1+y*(0.2635537e-1+y*(-0.1647633e-1
			+y*0.392377e-2))))))));
	}
	return ans;
}

/*---------------------------- erfcc(x) -------------------------------*/
double erfcc(double x)
{
	double t,z,ans;

	z=fabs(x);
	t=1.0/(1.0+0.5*z);
	ans=t*exp(-z*z-1.26551223+t*(1.00002368+t*(0.37409196+t*(0.09678418+
		t*(-0.18628806+t*(0.27886807+t*(-1.13520398+t*(1.48851587+
		t*(-0.82215223+t*0.17087277)))))))));
	return x >= 0.0 ? ans : 2.0-ans;
}

/*---------------------------- q(x) -------------------------------*/
double q(double x)
{
  return(.5*erfcc(x/sqrt(2)));
}

/*------------------------ rician_pdf(k,r) ---------------------------*/
double rician_pdf(double k,double r)
{
  return(2*r*(1+k)*exp(-k-r*r*(1+k))*bessi0(2*r*sqrt(k*(1+k))));
}

/*------------------------ theta_pdf(a,t) ---------------------------*/
double theta_pdf(double a,double t)
{
  double y,ans1,ans2;
  if(a>3.75)
  {
    y=3.75/a;
      ans1=(1/sqrt(a))*(0.39894228+y*(0.1328592e-1
	+y*(0.225319e-2+y*(-0.157565e-2+y*(0.916281e-2
	+y*(-0.2057706e-1+y*(0.2635537e-1+y*(-0.1647633e-1
	+y*0.392377e-2))))))));
      ans2=exp(a*cos(t)-a)/(2*pi*ans1);

    return ans2;
  }
  else
    return(exp(a*cos(t))/(2*pi*bessi0(a)));
}

/*------------------------ func(x,y,z) ---------------------------*/
double func(double x,double y)
{
 return(myfunc);
}

/*------------------------ f1(x) ---------------------------*/
double f1(double x)
{
	double trapzd(double (*func)(double), double a, double b,int n);
	double f2(double y);
	double yy1(double),yy2(double);

	xsav=x;
	return trapzd(f2,yy1(x),yy2(x),step_number);
}

/*------------------------ f2(y) ---------------------------*/
double f2(double y)
{
	return (*nrfunc)(xsav,y);
}

/*------------------------ f2(y) ---------------------------*/
//double f2(double y)
//{
//	double trapzd(double (*func)(double), double a, double b,int n);
//	double f3(double z);
//	double z1(double,double),z2(double,double);
//
//	ysav=y;
//	return trapzd(f3,z1(xsav,y),z2(xsav,y),step_number);
//}

/*------------------------ f3(z) ---------------------------*/
//double f3(double z)
//{
//	return (*nrfunc)(xsav,ysav,z);
//}
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



#define JMAX        20
#define JMAXP (JMAX+1)
#define K            5
double qromb(double (*func)(double,double), double a, double b)
{
	void polint(double xa[], double ya[], int n, double x, double *y, double *dy);
	//double trapzd(double (*func)(double), double a, double b, int n);
	double quad3d(double (*func)(double, double), double x1, double x2);
	void nrerror(char error_text[]);
	double ss,dss;
	double s[JMAXP+1],h[JMAXP+1];
	int j;

	h[1]=1.0;

	for (j=1;j<=JMAX;j++) {
		step_number=j;
		//printf("%d  ",step_number);
		//s[j]=quad3d(func,a,b);
		if (j==1)
		   s[j]=quad3d(func,a,b);
		else
		   s[j]=0.5*(s[j-1]+quad3d(func,a,b));
		//s[j]=trapzd(func,a,b,j);
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

//#define FUNC(x) ((*func)(x))
double trapzd(double (*func)(double), double a, double b, int n)
{
	double x,tnm,sum,del;
	//static double s;
	double s;
	int it,j;

	if (n == 1) {
		return s=0.5*(b-a)*((*func)(a)+(*func)(b));
	} else {
		for (it=1,j=1;j<n-1;j++) it <<= 1;
		tnm=it;
		del=(b-a)/tnm;
		x=a+0.5*del;
		for (sum=0.0,j=1;j<=it;j++,x+=del) sum += (*func)(x);
		//s=0.5*(s+(b-a)*sum/tnm);
		s=(b-a)*sum/tnm;
		return s;
	}
}
//#undef FUNC

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

/*------------------------ quad3d(func,x1,x2) ---------------------------*/
double quad3d(double (*func)(double, double), double x1, double x2)
{
	double trapzd(double (*func)(double), double a, double b,int n);
	double f1(double x);

	nrfunc=func;
	return trapzd(f1,x1,x2,step_number);
}


void main()
{
 int i;
 FILE *out0,*out1;
 clrscr();

 if ((out0 = fopen("c:\\trash\\co50.dat", "a+t")) == NULL)
 {
   fprintf(stderr, "Cannot open input file.\n");
   return 1;
 }
 if ((out1 = fopen("c:\\trash\\co51.dat", "a+t")) == NULL)
 {
   fprintf(stderr, "Cannot open input file.\n");
   return 1;
 }
 //printf("exp power :%lf,%lf",exp(800),bessi0(800));
 //printf("bessi0: %lf",bessi0(800));
 //getch();
 fprintf(out0,"\n\n --- Rician pdf for K=8  ---");
 fprintf(out1,"\n\n --- Rician pdf for K=8  ---");
 printf("\n\n --- Rician pdf for K=8  ---");
 for(i=0;i<=40;i=i+1)    /** Eb_N0 = 0..10 dB **/
 {
   sum=0.0;
   aaa=1.38;
   Eb_N0=pow(10,(double)(i/(4.0*10)));
   sum=qromb( func, X_1, X_2  );   // cos(x) in Q(.) should be positive!
   aaa=2.12;                      // if x-> 0..2*pi then Q(.) is negative!
   sum+=qromb( func, X_1, X_2  );   // cos(x) in Q(.) should be positive!
   fprintf(out0,"\nEb_N0=  %5.3f dB        result=%12.7lf ",i/10.0,log10(sum));
   fprintf(out1,"\n%12.7lf ",log10(sum));
   printf("\nEb_N0=  %5.3f dB       result=%12.7lf ",i/4.0,log10(sum));

 }
 getch();
}