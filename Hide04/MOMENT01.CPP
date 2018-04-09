/* Program : Moment01.CPP
 *           Given a raw file, rotation by mophisms, and  computeing  its  mean
 *           ave, average deviation adev, standard deviation sdev, variance var,
 *           skewness skew, and kuytosis. return less one.
 *
 * Author  : Morpheus
 * Date    : 98.08.17
 */

#include <math.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct OPENFILE {
  char name[40];
  int  size;
  int  NewSize;
  int  BlockSize;
  int  k;
  long period;
  } File;

typedef struct ZIGZAG {
  int row;
  int col;
  } ZigZag;

void Get_Filename(char *, File *);
void Fast_DCT(float **, unsigned long, int);
extern void morphism(int , int, unsigned char*, unsigned char*);
extern void nrerror(char *);
extern void Zig_Zag(ZigZag*, int);
extern void cosft2(float*, int, int);
extern void moment(float*, int, float*, float*, float*, float*, float*, float*);
extern float *vector(long, long);
extern float **matrix(long, long, long, long);
extern void free_vector(float *, long, long);
extern void free_matrix(float **, long, long, long, long);

void main(void)
{
  clrscr();
  printf("Rotation by mophisms, and computing mean, deviation.\n");

  FILE *fin, *fout1, *fout2;
  File infile;
  Get_Filename("Enter input filename (0-8 char): ", &infile);
  long K = infile.k;
  long size = infile.size;
  long ES = infile.NewSize;
  long BS = infile.BlockSize;
  long period = infile.period;

  char runfile[40], outfile[40];
  static unsigned char *Pixel = new unsigned char [ES*ES];
  static unsigned char *NewPixel = new unsigned char [ES*ES];
  strcpy(runfile, infile.name);
  strcpy(outfile, infile.name);
  strcat(runfile, ".raw");
  if ((fin=fopen(runfile, "rb")) == NULL)
  {
    printf("\nOpen %s fail...", runfile);
    getch();
    exit(1);
  }
  fread(NewPixel,size,size,fin);
  fclose(fin);
  for (long x=0; x<ES; x++)
  {
    for (long y=0; y<ES; y++)
    {
      if (x<size && y<size)
        Pixel[x*ES+y] = NewPixel[x*size+y];
      else
        Pixel[x*ES+y] = 0xff;
    }
  }

  strcat(outfile, ".dat");
  if ((fout2=fopen(outfile, "wt")) == NULL)
  {
    printf("\nOpen %s fail...", outfile);
    getch();
    exit(1);
  }

  ZigZag *zigzag = new ZigZag [BS*BS];
  Zig_Zag(zigzag, BS);

  float **data, *rawdata, adev, ave, curt, sdev, skew, vrnce;
  data = matrix(1,BS,1,BS);
  rawdata = vector(1,ES*ES);

  char string[10];
  long i, u, v, x, y, NewX, NewY;
  fprintf(fout2,"  K      Mean ave,         ADEV,         SDEV,");
  fprintf(fout2,"    Max Value,    Min Value,      Max-Min\n");
  fprintf(fout2,"----------------------------------------------------");
  fprintf(fout2,"-----------------------------------\n");
  for (i=0; i<=period; i++)
  {
    strcpy(runfile, "\0");
    strcpy(runfile, infile.name);
    itoa(i,string,10);
    if (strlen(string) < 3)
      strncat(runfile, "000", 3-strlen(string));
    strcat(runfile, string);
    strcat(runfile, ".raw");
    if ((fout1=fopen(runfile, "wb")) == NULL)
    {
      printf("\nOpen %s fail...", runfile);
      getch();
      exit(1);
    }
    printf("\n%s running...", runfile);

    for (x=0; x<ES; x++)
      for (y=0; y<ES; y++)
        NewPixel[x*ES+y] = Pixel[x*ES+y];
    fwrite(NewPixel,ES,ES,fout1);
    fclose(fout1);

    float max = 0, min = 0;
    for (u=0; u<(ES/BS); u++)
    {
      for (v=0; v<(ES/BS); v++)
      {
        for (x=0; x<BS; x++)
        {
          for (y=0; y<BS; y++)
          {
            long offset = u*BS*ES+v*BS+x*ES+y;
            data[x+1][y+1] = (float)Pixel[offset];
          }
        }
        Fast_DCT(data,BS,1);
        for (x=0; x<BS; x++)
        {
          for (y=0; y<BS; y++)
          {
            long offset = u*BS*ES+v*BS+x*ES+y;
            NewX = zigzag[x*BS+y].row; NewY = zigzag[x*BS+y].col;
            if (x*BS+y == 0)
              rawdata[offset] = 0;
            else
            {
              if (data[NewX+1][NewY+1] > max)
                max = data[NewX+1][NewY+1];
              else if (data[NewX+1][NewY+1] < min)
                min = data[NewX+1][NewY+1];
              rawdata[offset] = data[NewX+1][NewY+1];
            }
          }
        }
      }
    }
    int len = ES*ES - ES*ES/BS/BS;
    moment(rawdata,len,&ave,&adev,&sdev,&vrnce,&skew,&curt);
    fprintf(fout2, "%3d, %12.6f, %12.6f, %12.6f, %12.6f, %12.6f, %12.6f\n",
            i, ave, adev, sdev, max, min, max-min);
    morphism(ES, K, NewPixel, Pixel);
  }
  fclose(fout2);
  delete [] Pixel;
  delete [] NewPixel;
  free_matrix(data,1,BS,1,BS);
  free_vector(rawdata,1,ES*ES);

  printf("\n\nPress any key to continue...");
  getch();
}

void Get_Filename(char *readme, File *file)
{
  char fn[40];
  while ((strlen(fn) > 8) || (strlen(fn) < 1))
  {
    printf("\n%s", readme);
    scanf("%s", fn);
  }
  strcpy(file->name, fn);

  file->size = 0;
  while (file->size == 0)
  {
    printf("\nEnter size of %s (1-512): ", file->name);
    scanf("%d", &file->size);
  }

  file->NewSize = 0;
  while (file->NewSize == 0)
  {
    printf("\nEnter range of %s (1-512): ", file->name);
    scanf("%d", &file->NewSize);
  }

  file->BlockSize = 0;
  while (file->BlockSize == 0)
  {
    printf("\nEnter block size of %s (1-512): ", file->name);
    scanf("%d", &file->BlockSize);
  }

  file->k = 0;
  while (file->k == 0)
  {
    printf("\nEnter k of mophisms constant: ");
    scanf("%d", &file->k);
  }

  file->period = 0;
  while (file->period == 0)
  {
    printf("\nEnter period of %s (0-999): ", file->name);
    scanf("%d", &file->period);
  }
}

void Fast_DCT(float **data, unsigned long size, int isign)
{
  long i, j;
  float *rawdata;
  // row fast discreate cosine transform
  rawdata = vector(1,size);
  for (i=1; i<=size; i++)
  {
    for (j=1; j<=size; j++)
      rawdata[j] = data[i][j];
    cosft2(rawdata,size,isign);
    for (j=1; j<=size; j++)
      data[i][j] = rawdata[j];
  }

  // Column fast discrete cosine transform
  for (i=1; i<=size; i++)
  {
    for (j=1; j<=size; j++)
      rawdata[j] = data[j][i];
    cosft2(rawdata, size, isign);
    for (j=1; j<=size; j++)
      data[j][i] = rawdata[j];
  }
  free_vector(rawdata,1,size);
}

