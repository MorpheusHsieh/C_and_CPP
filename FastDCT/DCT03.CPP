// Program : DCT03.CPP -- Discrete Consine Transform all image not slice
// Author  : Morpheus
// Date    : 98.08.03

#include <math.h>
#include <time.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PI          3.141592654
#define N           256

FILE *fin, *fout;
unsigned char Src[N*N], Dest[N*N];
static int Unit1[N*N], Unit3[N*N];
static float Unit2[N*N];

struct ZIGZAG {
       int row;
       int col;
       } ZigZag[N*N];

void Zig_Zag(void);
void Initial_State(void);
void DCT(int *, float *);
void IDCT(float *, int *);

void main(void)
{
  clrscr();
  printf("Ths image as a block and DCT.\n");

  Initial_State();

  printf("\nDCT running, Please wait...");
  time_t tstart, tstop;
  time(&tstart);
  DCT(Unit1, Unit2);
  time(&tstop);
  double tused = difftime(tstop, tstart);
  printf("\n\nTransform ran for %5.2f seconds\n", tused);

/*  printf("\n");
  for (int x=0; x<N; x++)
  {
    for (int y=0; y<N; y++)
      printf("%4.0f, ", Unit2[x*N+y]);
    printf("\n");
  }
  getch();*/

  printf("\nInverse DCT, Please wait...");
  IDCT(Unit2, Unit3);
/*  printf("\nInverse DCT.\n");
  fprintf(fout, "\n");
  for (int x=0; x<N; x++)
  {
    for (int y=0; y<N; y++)
    {
      fprintf(fout, "%4d, ",  Unit3[x*N+y]);
      printf("%4d, ", Unit3[x*N+y]);
    }
    fprintf(fout, "\n");
    printf("\n");
  }*/
  long i;
  for (i=0; i<sizeof(Src); i++)
    Dest[i] = (unsigned char)Unit3[i];

  fwrite(Dest,sizeof(Dest),1,fout);
  fclose(fout);

  printf("\n\nPress any key to continue...");
  getch();
}

void Zig_Zag(void)
{
  int i, j;
  int x = 0, y = 0, taggle = 1; // 1:Right, 2:Left-Down, 3:Down, 4:Right-Up
  long offset;
  for (i=0; i<N; i++)
  {
    for (j=0; j<N; j++)
    {
      offset = i*N + j;
      ZigZag[offset].row = x;
      ZigZag[offset].col = y;
      switch(taggle)
      {
        case 1: y++;
                taggle = 2;
                break;
        case 2: x++; y--;
                (y == 0) ? taggle = 3 : taggle = 2;
                if (x == N) {
                  x--; y+=2; taggle = 4;
                }  
                break;
        case 3: x++;
                if (x == N) {
                  x--; y++;
                }
                taggle = 4;
                break;
        case 4: x--; y++;
                (x == 0) ? taggle = 1 : taggle = 4;
                if (y == N) {
                  x += 2; y--; taggle = 2;
                }
                break;
        default: printf("\nUnknow operator...");
      }
    }
  }
}

void Initial_State(void)
{
  Zig_Zag();

  char infile[20], outfile[20];
  printf("\nEneter source filename: ");
  scanf("%s", infile);
  strcat(infile, ".raw");
  if ((fin=fopen(infile, "rb")) == NULL)
  {
    printf("\nOpen %s fail...", infile);
    getch();
    exit(1);
  }

  printf("\nEneter destination filename: ");
  scanf("%s", outfile);
  strcat(infile, ".raw");  
  if ((fout=fopen(infile, "wb")) == NULL)
  {
    printf("\nOpen %s fail...", outfile);
    getch();
    exit(1);
  }

  fread(Src,sizeof(Src),1,fin);
  fclose(fin);

  long i;
  for (i=0; i<sizeof(Src); i++)
    Unit1[i] = (int)Src[i];
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
      printf("\nU: %d, V: %d", u, v);
      for (x=0; x<N; x++)
      {
        for (y=0; y<N; y++)
        {
          value += ((float)(Src[x*N+y] - 128)
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
