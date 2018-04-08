// Program : FDCT02.cpp -- Fast Discrete Cosine Transform
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

FILE *fin, *fout, *fptr, *fptr2;
long BS, ES, AC_Num;
unsigned char Pixel[IS][IS];

void Initial_State(Image*, Image*);
void Fast_DCT(float **, unsigned long, int);
extern void Zig_Zag(ZigZag *, int);
extern void cosft2(float *, int, int);
extern float *vector(long, long);
extern float **matrix(long, long, long, long);
extern void free_vector(float *, long, long);
extern void free_matrix(float **, long, long, long, long);
extern void four1(float*, unsigned long, int);
extern void realft(float **, unsigned long, int);

void main(void)
{
  clrscr();
  printf("\n2-Dimension Fast Discrete cosine Transform.\n");

  Image infile, outfile;
  Initial_State(&infile, &outfile);
//  printf("%s, %d, %d", infile.name, infile.row, infile.col); getch();

  float **data;
  unsigned long size = infile.size;
  data = matrix(1,BS,1,BS);

  time_t tstart, tstop;
  time(&tstart);

  ZigZag *zigzag = new ZigZag [BS*BS];
  Zig_Zag(zigzag, BS);

/*  printf("\n");
  for(int i=0; i<BS; i++)
  {
    for (int j=0; j<BS; j++)
      printf("<%d, %d>", zigzag[i*BS+j].row, zigzag[i*BS+j].col);
    printf("\n");
  }
  printf("\n"); getch();*/

  int NewX, NewY;
  long u, v, x, y, offset, value;
  unsigned char *NewPixel = new unsigned char [size*size];
  for (x=0; x<size; x++)
    for (y=0; y<size; y++)
      NewPixel[x*size+y] = Pixel[x][y];

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

//      fprintf(fptr, "\nU: %d, V: %d\n", u, v);
      for (x=0; x<BS; x++)
      {
        for (y=0; y<BS; y++)
        {
          if ((x*BS+y <= AC_Num) && (x*BS+y != 0))
          {
            NewX = zigzag[x*BS+y].row; NewY = zigzag[x*BS+y].col;
            fprintf(fptr, "%f,\n", data[NewX+1][NewY+1]);
          }
          fwrite(&data[x+1][y+1],4,1,fptr2);
        }
//        fprintf(fptr, "\n");
      }
//      fprintf(fptr, "\n");
//      getch();

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
  fclose(fptr);
  fclose(fptr2);
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
  strcat(infile->name, ".raw");
  if ((fin=fopen(infile->name, "rb")) == NULL)
  {
    printf("\nOpen %s failure...", infile->name);
    getch();
    exit(1);
  }

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

  char string[20], fptrfile[40], fptrfile2[40];
  itoa(BS,string,10);
  if (strlen(string) < 3)
    strncat(outfile->name, "000", 3-strlen(string));
  strcat(outfile->name, string);

  itoa(AC_Num,string,10);
  if (strlen(string) < 6)
    strncat(outfile->name, "000000", 6-strlen(string));
  strcat(outfile->name, string);
  strcpy(fptrfile, outfile->name);
  strcpy(fptrfile2, outfile->name);

  strcat(outfile->name, ".raw");
  if ((fout=fopen(outfile->name, "wb")) == NULL)
  {
    printf("\nOpen %s failure...", outfile->name);
    getch();
    exit(1);
  }
  outfile->size = infile->size;

  strcat(fptrfile, ".out");
  if ((fptr=fopen(fptrfile, "wt")) == NULL)
  {
    printf("\nOpen %s failure...", fptrfile);
    getch();
    exit(1);
  }

  strcat(fptrfile2, ".frq");
  if ((fptr2=fopen(fptrfile2, "wb")) == NULL)
  {
    printf("\nOpen %s failure...", fptrfile2);
    getch();
    exit(1);
  }

  unsigned char *temp = new unsigned char [size*size];
  fread(temp,size,size,fin);
  fclose(fin);

  int i, j;
  for (i=0; i<size; i++)
    for (j=0; j<size; j++)
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

