// Program : DCT02.CPP Discrete Consine Transform all image by 8*8 block
// Author  : Morpheus
// Date    : 98.07.21

#include <time.h>
#include <math.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PI          3.141592654
#define IS        256                  // IS: Image Size
#define N           8                  // BS: Block Size

FILE *fin1, *fin2, *fout;
char infile1[20], outfile[20];
unsigned char PixelData[IS*IS];
static float Unit2[N*N], Unit3[N*N], Unit4[N*N];
static int Unit1[N*N], Unit5[N*N];

void Initial_State(void);
void DCT(int *, float *);
void Quantizer(float *, float *);
void IQuantizer(float *, float *);
void IDCT(float *, int *);

void main(void)
{
  clrscr();
  printf("Discrete Cosine Transform.\n");

  Initial_State();

  int u, v, x, y;
  printf("\nNow, Porgram is running, Please waiting...");
  time_t tstart, tstop;
  time(&tstart);
  for (u=0; u<(IS/N); u++)
  {
    for (v=0; v<(IS/N); v++)
    {
      for (x=0; x<N; x++)
      {
        for (y=0; y<N; y++)
          Unit1[x*N+y] = (int)PixelData[u*N*IS+v*N+x*IS+y];
      }

      DCT(Unit1, Unit2);
      Quantizer(Unit2, Unit3);
      IQuantizer(Unit3, Unit4);
      IDCT(Unit4, Unit5);

      for (x=0; x<N; x++)
      {
        for (y=0; y<N; y++)
          PixelData[u*N*IS+v*N+x*IS+y] = (unsigned char)(Unit5[x*N+y] & 0xff);
      }
    }
  }
  fwrite(PixelData,sizeof(PixelData),1,fout);

  time(&tstop);
  double tused = difftime(tstop, tstart);
  printf("\n\nTransform ran for %5.2f seconds\n", tused);
  
  fclose(fout);
  printf("\n\nPress any key to continue...");
  getch();
}

void Initial_State(void)
{
  printf("\nEnter input filename: ");
  scanf("%s", infile1);
  strcat(infile1, ".raw");
  if ((fin1=fopen(infile1, "rb")) == NULL)
  {
    printf("\nOpen %s fail...", infile1);
    getch();
    exit(1);
  }

  printf("\nEnter output filename: ");
  scanf("%s", outfile);
  strcat(outfile, ".raw");
  if ((fout=fopen(outfile, "wb")) == NULL)
  {
    printf("\nOpen %s fail...", outfile);
    getch();
    exit(1);
  }

  fread(PixelData,sizeof(PixelData),1,fin1);
  fclose(fin1);
}

void DCT(int *Src, float *Dest)
{
  int x, y, u, v;
  float Cu, Cv, value;

  for (u=0; u<N; u++)
  {
    for (v=0; v<N; v++)
    {
      value = 0;
      for (x=0; x<N; x++)
      {
        for (y=0; y<N; y++)
        {
          value += ((Src[x*N+y] - 128)
            * cos(((2.0 * (float)x + 1.0) * (float)u * PI) / 16.0)
            * cos(((2.0 * (float)y + 1.0) * (float)v * PI) / 16.0));
        }
      }
      (u == 0) ? Cu = ( 1 / sqrt(2.0)) : Cu = 1;
      (v == 0) ? Cv = ( 1 / sqrt(2.0)) : Cv = 1;
      Dest[u*N+v] = (value * Cu * Cv / 4.0 );
    }
  }
}

void Quantizer(float *Src, float *Dest)
{
  int Ql[N][N] = { {16, 11, 10, 16,  24,  40,  51,  61},
                   {12, 12, 14, 19,  26,  58,  60,  55},
                   {14, 13, 16, 24,  40,  57,  69,  56},
                   {14, 17, 22, 29,  51,  87,  80,  62},
                   {18, 22, 37, 56,  68, 109, 103,  77},
                   {24, 35, 55, 64,  81, 104, 113,  92},
                   {49, 64, 78, 87, 103, 121, 120, 101},
                   {72, 92, 95, 98, 112, 100, 103,  99} };

  int x, y;
  for (x=0; x<N; x++)
    for (y=0; y<N; y++)
      Dest[x*N+y] = Src[x*N+y] / Ql[x][y];
}

void IQuantizer(float *Src, float * Dest)
{
  int Ql[N][N] = { {16, 11, 10, 16,  24,  40,  51,  61},
                   {12, 12, 14, 19,  26,  58,  60,  55},
                   {14, 13, 16, 24,  40,  57,  69,  56},
                   {14, 17, 22, 29,  51,  87,  80,  62},
                   {18, 22, 37, 56,  68, 109, 103,  77},
                   {24, 35, 55, 64,  81, 104, 113,  92},
                   {49, 64, 78, 87, 103, 121, 120, 101},
                   {72, 92, 95, 98, 112, 100, 103,  99} };

  int x, y;
  for (x=0; x<N; x++)
    for (y=0; y<N; y++)
      Dest[x*N+y] = Src[x*N+y] * Ql[x][y];
}

void IDCT(float *Src, int *Dest)
{
  int x, y, u, v;
  float Cu, Cv, value;

  for (x=0; x<N; x++)
  {
    for (y=0; y<N; y++)
    {
      value = 0;
      for (u=0; u<N; u++)
      {
        for (v=0; v<N; v++)
        {
          (u == 0) ? Cu = ( 1 / sqrt(2.0)) : Cu = 1;
          (v == 0) ? Cv = ( 1 / sqrt(2.0)) : Cv = 1;
          value += (Src[u*N+v] * Cu * Cv
            * cos(((2.0 * (float)x + 1.0) * (float)u * PI) / 16.0)
            * cos(((2.0 * (float)y + 1.0) * (float)v * PI) / 16.0));
        }
      }
      Dest[x*N+y] = (int)(value / 4.0) + 128;
    }
  }
}

