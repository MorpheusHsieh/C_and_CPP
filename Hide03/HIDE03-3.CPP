/* Program : Hide03-3.CPP -- Hiding information
 *           First, watermark rotation by mophism,get minimum standard deviation
             , computing its frequency.
             Second, computing difference between image and compression image
             frequency value, proportion first, then watermark added image.
 *
 * Author  : Morpheus
 * Date    : 98.08.23
 */

#include <math.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *fin1, *fin2, *fin3, *fout;
long IS, WS, BS, K, Period;

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
extern void mophism(int, int, unsigned char*, unsigned char*);
extern void moment(float*, int, float*, float*, float*, float*, float*, float*);
extern float *vector(long, long);
extern float **matrix(long, long, long, long);

void main(void)
{
  clrscr();
  printf("Hinding information use mophism and Spread spectrum.\n");

  Init_State();
  unsigned char *pixel1 = new unsigned char [IS*IS];
  unsigned char *pixel2 = new unsigned char [IS*IS];
  unsigned char *pixel3 = new unsigned char [WS*WS];

  fread(pixel1,IS,IS,fin1); fclose(fin1);
  fread(pixel2,IS,IS,fin2); fclose(fin2);
  fread(pixel3,WS,WS,fin3); fclose(fin3);

  ZigZag *zigzag1 = new ZigZag [IS*IS];
  ZigZag *zigzag2 = new ZigZag [BS*BS];
  Zig_Zag(zigzag1, IS);
  Zig_Zag(zigzag2, BS);

  // rotation watermark period times by mophism
  long i, j;
  static unsigned char *PixelTmp = new unsigned char [WS*WS];
  for (i=0; i<Period; i++)
  {
    mophism(WS,K,pixel3,PixelTmp);
    for (j=0; j<WS*WS; j++)
      pixel3[j] = PixelTmp[j];
  }
  delete [] PixelTmp;

  long x, y;
  float **data1 = matrix(1,IS,1,IS);
  float **data2 = matrix(1,IS,1,IS);
  float **data3 = matrix(1,WS,1,WS);
  for (x=0; x<IS; x++)
  {
    for (y=0; y<IS; y++)
    {
      data1[x+1][y+1] = (float)pixel1[x*IS+y];
      data2[x+1][y+1] = (float)pixel2[x*IS+y];
      if (x<WS && y<WS)
        data3[x+1][y+1] = (float)pixel3[x*WS+y];
    }
  }

  Fast_DCT(data1,IS,IS,1);
  Fast_DCT(data2,IS,IS,1);
//  Fast_DCT(data3,WS,BS,1);

  float *rawdata3 = vector(1,WS*WS);
  float *diff = vector(1,WS*WS);
  long u, v, cnt1 = 0;
  for (u=0; u<(WS/BS); u++)
  {
    for (v=0; v<(WS/BS); v++)
    {
      for (x=0; x<BS; x++)
      {
        for (y=0; y<BS; y++)
        {
          diff[cnt1] = data1[u*BS+x+1][v*BS+y+1]
                     - data2[u*BS+x+1][v*BS+y+1];
          rawdata3[cnt1] = data3[u*BS+x+1][v*BS+y+1];
          cnt1++;
        }
      }
    }
  }
  float ave1, adev1, sdev1, vrnce1, skew1, curt1;
  float ave2, adev2, sdev2, vrnce2, skew2, curt2;
  moment(diff,cnt1,&ave1,&adev1,&sdev1,&vrnce1,&skew1,&curt1);
  moment(rawdata3,cnt1,&ave2,&adev2,&sdev2,&vrnce2,&skew2,&curt2);
//  printf("\n%d, %f, %f, %f, %f, %f, %f", cnt1, ave1, adev1, sdev1, vrnce1, skew1, curt1);
//  printf("\n%d, %f, %f, %f, %f, %f, %f", cnt2, ave2, adev2, sdev2, vrnce2, skew2, curt2);
  printf("\nSuggest Alpha = %f", sdev1 / sdev2);
  float alpha = 0;
  while (alpha == 0)
  {
    printf("\nEnter alpha value: ");
    scanf("%f", &alpha);
  }
  free_vector(diff,1,WS*WS);

  cnt1 = 0;
  long NewX1, NewY1, NewX2, NewY2;
  for (long u=0; u<(WS/BS); u++)
  {
    for (long v=0; v<(WS/BS); v++)
    {
      for (x=0; x<BS; x++)
      {
        for (y=0; y<BS; y++)
        {
          NewX1 = zigzag1[cnt1].row + 1;
          NewY1 = zigzag1[cnt1].col + 1;
          cnt1++;
          NewX2 = u*BS + zigzag2[x*BS+y].row + 1;
          NewY2 = v*BS + zigzag2[x*BS+y].col + 1;
//          printf("\n%f, ", data1[u*BS+NewX+1][v*BS+NewY+1]);
//          printf("%f, ", rawdata3[cnt1]);
          data1[NewX1][NewY1] += data3[NewX2][NewY2] * alpha;
//          printf("%f", data1[u*BS+NewX+1][v*BS+NewY+1]); getch();
        }
      }
    }
  }

  Fast_DCT(data1, IS, IS, -1);
  for (x=0; x<IS; x++)
    for (y=0; y<IS; y++)
      pixel1[x*IS+y] = (unsigned char)data1[x+1][y+1];

  fwrite(pixel1,IS,IS,fout);
  fclose(fout);
  free_vector(rawdata3,1,WS*WS);
  free_matrix(data1,1,IS,1,IS);
  free_matrix(data2,1,IS,1,IS);
  free_matrix(data3,1,WS,1,WS);
  delete [] pixel1; delete [] pixel2; delete [] pixel3;
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
    printf("\nEnter watermark filename: ");
    scanf("%s", fn);
  }
  strcat(fn, ".raw");
  if ((fin3=fopen(fn, "rb")) == NULL)
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

  K = 0;
  while (K == 0) {
    printf("\nEnter K of mophisms: ");
    scanf("%d", &K);
  }

  Period = -1;
  while (Period < 0) {
    printf("\nEnter mophisms period: ");
    scanf("%d", &Period);
  }
}





