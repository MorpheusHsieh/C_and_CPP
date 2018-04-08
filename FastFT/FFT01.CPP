// Program : FFT01.CPP Fast Fourier Transform
// Author  : Avatar
// Date    : 98.08.05

#include <time.h>
#include <math.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define NR_END 1
#define FREE_ARG char*
#define N2        512
#define N3        512

FILE *fin, *fout;
void Initial_State(void);
void ReadPixel(float ***);
extern void nrerror(char *);
extern float **matrix(long, long, long, long);
extern float ***f3tensor(long, long, long, long, long, long);
extern void free_matrix(float **, long, long, long, long);
extern void free_f3tensor(float ***, long, long, long, long, long, long);
extern void fourn(float *, unsigned long *, int, int);
extern void rlft3(float ***, float **, unsigned long, unsigned long,
                  unsigned long, int);

void main(void)
{
  clrscr();
  printf("Fast Fourier Transform test.\n");

  time_t tstart, tstop;
  time(&tstart);
  Initial_State();

  float ***data, **speq;
  data = f3tensor(1,1,1,N2,1,N3);
  speq = matrix(1,1,1,2*N2);
  ReadPixel(data);

  rlft3(data,speq,1,N2,N3,1);
  rlft3(data,speq,1,N2,N3,-1);

  int i, j, value;
  unsigned char NewData[N2][N3];
  for (i=0; i<N2; i++)
    for (j=0; j<N3; j++)
    {
      value = (data[1][i+1][j+1] * 2.0 / ((float)(1*N2*N3)));
      NewData[i][j] = (unsigned char)value;
    }

  fwrite(NewData,N2,N3,fout);
  fclose(fout);

  free_matrix(speq,1,1,1,2*N2);
  free_f3tensor(data,1,1,1,N2,1,N3);

  time(&tstop);
  double tused = difftime(tstop, tstart);
  printf("\n\nTransform ran for %5.2f seconds\n", tused);

  printf("\n\nPress any key to continue...");
  getch();
}

void Initial_State(void)
{
  char infile[20], outfile[20];

  printf("\nEnter source image: ");
  scanf("%s", infile);
  strcat(infile, ".raw");
  if ((fin=fopen(infile,"rb")) == NULL)
  {
    printf("\nOpen %s fail...", infile);
    getch();
    exit(1);
  }

  printf("\nEnter output image fielname: ");
  scanf("%s", outfile);
  strcat(outfile, ".raw");
  if ((fout=fopen(outfile,"wb")) == NULL)
  {
    printf("\nOpen %s fail...", outfile);
    getch();
    exit(1);
  }
}

void ReadPixel(float ***data)
{
  unsigned char temp[N2][N3];
  fread(temp,N2,N3,fin);
  fclose(fin);

  int i, j;
  for (i=1; i<=N2; i++)
    for(j=1; j<=N3; j++)
      data[1][i][j] = (float)(temp[i-1][j-1]);
}

