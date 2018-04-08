// Program : FDCT02-1.cpp -- Fast Discrete Cosine Transform
// Author  : Avatar
// Date    : 98.08.10

#include <time.h>
#include <math.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define IS     512

typedef struct IMAGE {
        int  size;
        char name[40];
        } Image;

typedef struct ZIGZAG {
        int row;
        int col;
        } ZigZag;

FILE *fin, *fout, *fout2, *fptr;
long BS, ES, AC_Num;
char outfile2[40];
unsigned char Pixel[IS][IS];

void Initial_State(Image*, Image*);
void Decide_filename(int, Image*, Image*);
void Fast_DCT(float **, unsigned long, int);
extern void Zig_Zag(ZigZag *, int);
extern cosft2(float *, int, int);
extern float *vector(long, long);
extern float **matrix(long, long, long, long);
extern void free_vector(float *, long, long);
extern void free_matrix(float **, long, long, long, long);
extern void four1(float*, unsigned long, int);
extern void realft(float **, unsigned long, int);
extern cosft2(float *, int, int);

void main(void)
{
  clrscr();
  printf("\n2-Dimension Fast Discrete cosine Transform.\n");

  Image infile, outfile;
  Initial_State(&infile, &outfile);
  long size = infile.size;

  float **data;
  data = matrix(1,BS,1,BS);

  time_t tstart, tstop;
  time(&tstart);

  ZigZag *zigzag = new ZigZag [BS*BS];
  Zig_Zag(zigzag, BS);

  unsigned char *NewPixel = new unsigned char [size*size];
  for (int i=9; i>=0; i--)
  {
    Decide_filename(i, &infile, &outfile);

    for (long i=0; i<size; i++)
    {
      for (long j=0; j<size; j++)
        NewPixel[i*size+j] = Pixel[i][j];
    }

    int NewX, NewY;
    float freq;
    long u, v, x, y, offset, value;
    for (u=0; u<(ES/BS); u++)
    {
      for (v=0; v<(ES/BS); v++)
      {
        for (x=0; x<BS; x++)
          for (y=0; y<BS; y++)
            data[x+1][y+1] = (float)Pixel[u*BS+x][v*BS+y];

        //Fast discrete cocine transform
        Fast_DCT(data,BS,1);

        for (x=0; x<BS; x++)
        {
          for (y=0; y<BS; y++)
          {
            if (x*BS+y > AC_Num)
            {
              NewX = zigzag[x*BS+y].row; NewY = zigzag[x*BS+y].col;
              data[NewX+1][NewY+1] = 0;
            }
          }
        }

        fout2=fopen(outfile2, "a+");
        for (x=0; x<BS; x++)
        {
          for (y=0; y<BS; y++)
          {
            NewX = zigzag[x*BS+y].row; NewY = zigzag[x*BS+y].col;
            if ((x*BS+y <= AC_Num) && (x*BS+y != 0))
            {

              freq = (int)(data[NewX+1][NewY+1] * 10) / 10;
              fprintf(fout2, "%11.1f ", freq);
            }
          }
        }
        fclose(fout2);
//        printf("\nU: %d, V: %d,\n", u, v); getch();

        Fast_DCT(data,BS,-1);

        offset = u*BS*size + v*BS;
        for (x=0; x<BS; x++)
        {
          for (y=0; y<BS; y++)
          {
            value = (data[x+1][y+1] * pow(2,2) / pow(BS, 2));
            NewPixel[offset+x*size+y] = (unsigned char)value;
          }
        }
      }
    }
    fwrite(NewPixel,size,size,fout);
    fclose(fout);
  }
  delete [] NewPixel;
  delete [] zigzag;

  free_matrix(data,1,BS,1,BS);

  time(&tstop);
  double tused = difftime(tstop, tstart);
  printf("\n\nTransform ran for %5.2f seconds\n", tused);

  printf("\nPress any key to continue...");
  getch();
}

void Initial_State(Image *infile, Image *outfile)
{
  printf("\nEnter source image filename: ");
  scanf("%s", &infile->name);

  long size;
  printf("\nEnter source image size: ");
  scanf("%d", &size);
  infile->size = size;

  printf("\nEnter watermark size (0-%d): ", size);
  scanf("%d", &ES);

  printf("\nEnter size of block: ");
  scanf("%d", &BS);

  printf("\nEnter remain AC values (0-%d): ", BS*BS-1);
  scanf("%d", &AC_Num);

  printf("\nEnter output image filename: ");
  scanf("%s", &outfile->name);
  outfile->size = infile->size;
  strcpy(outfile2, outfile->name);  

  // frequency data filename
  char string[20];
  itoa(BS,string,10);
  if (strlen(string) < 3)
    strncat(outfile2, "000", 3-strlen(string));
  strcat(outfile2, string);

  itoa(AC_Num,string,10);
  if (strlen(string) < 6)
    strncat(outfile2, "000000", 6-strlen(string));
  strcat(outfile2, string);

  strcat(outfile2, ".out");
  if ((fptr=fopen(outfile2, "wt")) == NULL)
  {
    printf("\nOpen %s fail...", outfile2);
    getch();
    exit(1);
  }
  fclose(fptr);
}

void Decide_filename(int i, Image *in, Image *out)
{
  char string[20], runfile[40], outname[40];

  strcpy(runfile, in->name);
  strcpy(outname, out->name);

  // decide input filename
  itoa(i,string,10);
  if (strlen(string) < 3)
  {
    strncat(runfile, "000", 3-strlen(string));
    strncat(outname, "000", 3-strlen(string));
  }
  strcat(runfile, string);
  strcat(outname, string);

  strcat(runfile, ".raw");
  if ((fin=fopen(runfile, "rb")) == NULL)
  {
    printf("\nOpen %s & %s fail...", runfile);
    getch();
    exit(1);
  }
  printf("\n%s running...", runfile);

  // decide output filenam
  itoa(BS,string,10);
  if (strlen(string) < 3)
    strncat(outname, "000", 3-strlen(string));
  strcat(outname, string);

  itoa(AC_Num,string,10);
  if (strlen(string) < 6)
    strncat(outname, "000000", 6-strlen(string));
  strcat(outname, string);

  strcat(outname, ".raw");
  if ((fout=fopen(outname, "wb")) == NULL)
  {
    printf("\nOpen %s failure...", outname);
    getch();
    exit(1);
  }

  long size = in->size;
  unsigned char *temp = new unsigned char [size*size];
  fread(temp,size,size,fin);
  fclose(fin);

  for (long i=0; i<size; i++)
    for (long j=0; j<size; j++)
      Pixel[i][j] = temp[i*size+j];

  delete((char*)temp);
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



