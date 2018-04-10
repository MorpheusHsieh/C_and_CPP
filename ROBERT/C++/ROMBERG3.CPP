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

#define My_Func  z
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

int nsav;
static float xsav, ysav;
static float (*nrfunc)(float, float, float);
float quad3d(float (*func)(float, float, float), float x1, float x2)
{
   float trapzd(float (*func)(float), float a, float b, int n);
   float f1(float x);

   nrfunc=func;
   return trapzd(f1,x1,x2,nsav);
}

float func(float x, float y, float z)
{
   return(My_Func);
}

float f1(float x)
{
   float trapzd(float (*func)(float), float a, float b, int n);
   float f2(float y);
   float yy1(float), yy2(float);

   xsav=x;
   return trapzd(f2, yy1(x), yy2(x), nsav);
}

float f2(float y)
{
   float trapzd(float (*func)(float), float a, float b, int n);
   float f3(float z);
   float z1(float, float), z2(float, float);

   ysav=y;
   return trapzd(f3, z1(xsav, y), z2(xsav, y), nsav);
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

#define EPS     1.0e-6
#define JMAX        20
#define JMAXP (JMAX+1)
#define K            5
float qromb(float (*func)(float, float, float), float a, float b)
{
   void polint(float xa[], float ya[], int n, float x, float *y, float *dy);
// float trapzd(float (*func)(float), float a, float b, int n);
   void nrerror(char error_text[]);
   float ss,dss;
   float s[JMAXP+1],h[JMAXP+1];
   int j;

   h[1]=1.0;
   for (j=1;j<=JMAX;j++) {
//    s[j]=trapzd(func,a,b,j);
      nsav=j;
      if (j ==1 )
	 s[j]=quad3d(func, a, b);
      else
	 s[j]=0.5*(s[j-1]+quad3d(func, a, b));
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
void polint(float xa[], float ya[], int n, float x, float *y, float *dy)
{
   int i,m,ns=1;
   float den,dif,dift,ho,hp,w;
   float *c,*d;

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

#define FUNCT(x) ((*func)(x))
float trapzd(float (*func)(float), float a, float b, int n)
{
   float x,tnm,sum,del;
// static float s;
   float s;
   int it,j;

   if (n == 1) {
      return (s=0.5*(b-a)*(FUNCT(a)+FUNCT(b)));
   } else {
      for (it=1,j=1;j<n-1;j++) it <<= 1;
      tnm=it;
      del=(b-a)/tnm;
      x=a+0.5*del;
      for (sum=0.0,j=1;j<=it;j++,x+=del) sum += FUNCT(x);
//    s=0.5*(s+(b-a)*sum/tnm);
      s=(b-a)*sum/tnm;
      return s;
   }
}
#undef FUNCT

void main(void)
{
   clrscr();
   printf("\n%f", qromb(func, X_Start, X_End));
   getch();
}