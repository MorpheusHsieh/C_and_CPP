// Program : DCT01.CPP Discrete Consine Transform
// Author  : Morpheus
// Date    : 98.07.21

#include <math.h>
#include <conio.h>
#include <stdio.h>
#define PI          3.141592654
#define N           8

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
  printf("Raw file convert to JPEG file. (only 512*512 pixels)\n");

  FILE *fout;
  fout=fopen("DCT01.out", "wt");
  Initial_State();
  printf("\n");
  for (int x=0; x<N; x++)
  {
    for (int y=0; y<N; y++)
    {
      fprintf(fout, "%4d, ", Unit1[x*N+y]);
      printf("%4d, ", Unit1[x*N+y]);
    }
    fprintf(fout, "\n");
    printf("\n");
  }

  DCT(Unit1, Unit2);
  printf("\n");
  for (int x=0; x<N; x++)
  {
    for (int y=0; y<N; y++)
      printf("%5.1f, ", Unit2[x*N+y]);
    printf("\n");
  }
  getch();

/*  Quantizer(Unit2, Unit3);

  printf("\nQuantizer.\n");
  for (int x=0; x<N; x++)
  {
    for (int y=0; y<N; y++)
      printf("%4.0f, ", Unit3[x*N+y]);
    printf("\n");
  }
  getch();

  IQuantizer(Unit3, Unit4);

  printf("\nInverse Quantizer.\n");
  for (int x=0; x<N; x++)
  {
    for (int y=0; y<N; y++)
      printf("%4.0f, ", Unit4[x+y*N]);
    printf("\n");
  }
  getch();*/


  IDCT(Unit4, Unit5);
  printf("\nInverse DCT.\n");
  fprintf(fout, "\n");
  for (int x=0; x<N; x++)
  {
    for (int y=0; y<N; y++)
    {
      fprintf(fout, "%4d, ",  Unit5[x*N+y]);
      printf("%4d, ", Unit5[x*N+y]);
    }
    fprintf(fout, "\n");
    printf("\n");
  }

  fclose(fout);

  printf("\n");
  for (int x=0; x<N; x++)
  {
    for (int y=0; y<N; y++)
    {
      printf("%4d, ", (Unit5[x*N+y] - Unit1[x*N+y]));
    }
    printf("\n");
  }

  printf("\n\nPress any key to continue...");
  getch();
}

void Initial_State(void)
{
/*  int Data1[N][N] = { {139, 144, 149, 153, 155, 155, 155, 155},
                      {144, 151, 153, 156, 159, 156, 156, 156},
                      {150, 155, 160, 163, 158, 156, 156, 156},
                      {159, 161, 162, 160, 160, 159, 159, 159},
                      {159, 160, 161, 162, 162, 155, 155, 155},
                      {161, 161, 161, 161, 160, 157, 157, 157},
                      {162, 162, 161, 163, 162, 157, 157, 157},
                      {162, 162, 161, 161, 163, 158, 158, 158} };*/

  int Data1[N][N] = { {1, 1, 1, 1, 1, 1, 1, 1},
                      {1, 1, 1, 1, 1, 1, 1, 1},
                      {1, 1, 1, 1, 1, 1, 1, 1},
                      {1, 1, 1, 1, 1, 1, 1, 1},
                      {1, 1, 1, 1, 1, 1, 1, 1},
                      {1, 1, 1, 1, 1, 1, 1, 1},
                      {1, 1, 1, 1, 1, 1, 1, 1},
                      {1, 1, 1, 1, 1, 1, 1, 1} };                       


  int x, y;
  for (x=0; x<N; x++)
    for (y=0; y<N; y++)
      Unit1[x*N+y] = Data1[x][y];
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
