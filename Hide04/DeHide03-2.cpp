/* Program : DeHide03-2.CPP -- Decode Hiding information
 *           First, watermark rotation by mophism,get minimum standard deviation
             , computing its frequency.
             Second, computing difference between image and compression image
             frequency value, proportion first, then watermark added image.
 *
 * Author  : Avatar
 * Date    : 98.08.23
 */

#include <math.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *fin1, *fin2, *fin3, *fout;
long IS, WS, BS, K, Period;
float alpha;

typedef struct ZIGZAG {
  int row;
  int col;
  } ZigZag;

void Init_State(void);
extern void nrerror(char *);
extern void Zig_Zag(ZigZag*, int);
extern void cosft2(float *, int, int);
extern void free_vector(float *, long, long);
extern void Fast_DCT(float **, long, long, int);
extern void free_matrix(float **, long, long, long, long);
extern float *vector(long, long);
extern float **matrix(long, long, long, long);

void main(void)
{
  clrscr();
  printf("Hinding information use mophism and Spread spectrum.\n");

  Init_State();
  unsigned char *pixel1 = new unsigned char [IS*IS];
  unsigned char *pixel2 = new unsigned char [IS*IS];

  fread(pixel1,IS,IS,fin1); fclose(fin1);
  fread(pixel2,IS,IS,fin2); fclose(fin2);

  ZigZag *zigzag1 = new ZigZag [IS*IS];
  ZigZag *zigzag2 = new ZigZag [BS*BS];
  Zig_Zag(zigzag1, IS);
  Zig_Zag(zigzag2, BS);

  long x, y;
  float **data1 = matrix(1,IS,1,IS);
  float **data2 = matrix(1,IS,1,IS);
  for (x=0; x<IS; x++)
  {
    for (y=0; y<IS; y++)
    {
      data1[x+1][y+1] = (float)pixel1[x*IS+y];
      data2[x+1][y+1] = (float)pixel2[x*IS+y];
    }
  }

  Fast_DCT(data1,IS,IS,1);
  Fast_DCT(data2,IS,IS,1);

  float value;
  long u, v, NewX1, NewY1, NewX2, NewY2, cnt1 = 0;
  float **wmkdata = matrix(1,WS,1,WS);
  for (u=0; u<(WS/BS); u++)
  {
    for (v=0; v<(WS/BS); v++)
    {
      for (x=0; x<BS; x++)
      {
        for (y=0; y<BS; y++)
        {
          NewX1 = zigzag1[cnt1].row + 1;
          NewY1 = zigzag1[cnt1].col + 1;
          NewX2 = u*BS + zigzag2[x*BS+y].row + 1;
          NewY2 = v*BS + zigzag2[x*BS+y].col + 1;
          if (x*BS+y == 0)
            value = 0;
          else
            value = (data1[NewX1][NewY1] - data2[NewX1][NewY1]) / alpha;
          wmkdata[NewX2][NewY2] = value;
          cnt1++;
        }
      }
    }
  }

  Fast_DCT(wmkdata, WS, BS, -1);
  unsigned char *WmkPixel = new unsigned char [WS*WS];
  for (x=0; x<WS; x++)
    for (y=0; y<WS; y++)
      WmkPixel[x*WS+y] = (unsigned char)(wmkdata[x+1][y+1] + 128.0);

  fwrite(WmkPixel,WS,WS,fout);
  fclose(fout);
  delete [] pixel1; delete [] pixel2; delete [] WmkPixel;
  printf("\nPress any key to exit...");
  getch();
}

void Init_State(void)
{
  char fn[40] = "\0";
  while (strlen(fn)>8 || strlen(fn)<1)
  {
    printf("\nEnter first image filename: ");
    scanf("%s", fn);
  }
  strcat(fn, ".raw");
  if ((fin1=fopen(fn, "rb")) == NULL)
  {
    printf("\nOpen %s failure...", fn);
    getch();
    exit(1);
  }

  strcpy(fn, "\0");
  while (strlen(fn)>8 || strlen(fn)<1)
  {
    printf("\nEnter second image filename: ");
    scanf("%s", fn);
  }
  strcat(fn, ".raw");
  if ((fin2=fopen(fn, "rb")) == NULL)
  {
    printf("\nOpen %s failure...", fn);
    getch();
    exit(1);
  }

  strcpy(fn, "\0");
  while (strlen(fn)>8 || strlen(fn)<1)
  {
    printf("\nEnter output filename: ");
    scanf("%s", fn);
  }
  strcat(fn, ".raw");
  if ((fout=fopen(fn, "wb")) == NULL)
  {
    printf("\nOpen %s failure...", fn);
    getch();
    exit(1);
  }

  IS = 0;
  while (IS == 0) {
    printf("\nEnter image size: ");
    scanf("%d", &IS);
  }

  WS = 0;
  while (WS == 0) {
    printf("\nEnter watermark size: ");
    scanf("%d", &WS);
  }

  BS = 0;
  while (BS == 0) {
    printf("\nEnter block size: ");
    scanf("%d", &BS);
  }

  alpha = 0;
  while (alpha == 0) {
    printf("\nEnter alpha value: ");
    scanf("%f", &alpha);
  }
}





