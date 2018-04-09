// Program : FDCT01.cpp -- Fast discrete cosine transform
// Author  : Morpheus
// Date    : 98.08.05

#include <math.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ImageSize   512

typedef struct IMAGE {
        int  row;
        int  col;
        char name[20];
        } Image;

FILE *fin, *fout;
unsigned char Pixel[ImageSize][ImageSize];

void Initial_State(Image*, Image*);
void Fast_DCT(float**, unsigned long, unsigned long, int);
void cosft2(float *, int, int);
void realft(float *, unsigned long, int);
void four1(float *, unsigned long, int);
extern float *vector(long nl, long nh);
extern float **matrix(long, long, long, long);
extern void free_vector(float *, long, long);
extern void free_matrix(float **, long, long, long, long);
extern void four1(float*, unsigned long, int);
extern void realft(float*, unsigned long, int);
extern void cosft2(float*, int, int);

void main(void)
{
  clrscr();
  printf("\nTest Two-Dimension transform by address.\n");

  Image infile, outfile;
  Initial_State(&infile, &outfile);
//  printf("%s, %d, %d", infile.name, infile.row, infile.col); getch();

  float **data;
  unsigned long i, j, row = infile.row, col = infile.col;
  data = matrix(1,row,1,col);
  for (i=0; i<row; i++)
    for (j=0; j<col; j++)
      data[i+1][j+1] = (float)Pixel[i][j];

  //Fast discrete cocine transform
  Fast_DCT(data, row, col, 1);
  Fast_DCT(data, row, col, -1);

  float value;
  unsigned char NewPixel[ImageSize*ImageSize];
  for (i=0; i<row; i++)
  {
    for (j=0; j<col; j++)
    {
      value = data[i+1][j+1] * pow(2,2) / pow(col,2);
      NewPixel[i*col+j] = (unsigned char)value;
    }
  }  

  fwrite(NewPixel,row, col,fout);
  fclose(fout);

  free_matrix(data,1,row,1,col);
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

  printf("\nEnter row of source image: ");
  scanf("%d", &infile->row);

  printf("\nEnter column of source image: ");
  scanf("%d", &infile->col);

  printf("\nEnter output image filename: ");
  scanf("%s", &outfile->name);
  strcat(outfile->name, ".raw");
  if ((fout=fopen(outfile->name, "wb")) == NULL)
  {
    printf("\nOpen %s failure...", outfile->name);
    getch();
    exit(1);
  }
  outfile->row = infile->row;
  outfile->col = infile->col;

  unsigned char temp[ImageSize*ImageSize];
  fread(temp,infile->row, infile->col,fin);
  fclose(fin);

  int i, j;
  for (i=0; i<infile->row; i++)
    for (j=0; j<infile->col; j++)
      Pixel[i][j] = temp[i*infile->col+j];
}

void Fast_DCT(float **data, unsigned long row, unsigned long col, int isign)
{
  long i, j;
  float *rawdata;

  // Row fast discrete cosine transform
  rawdata = vector(1,row);
  for (i=1; i<=row; i++)
  {
    for (j=1; j<=col; j++)
      rawdata[j] = data[i][j];
    cosft2(rawdata,col,isign);
    for(j=1; j<=col; j++)
      data[i][j] = rawdata[j];
  }
  free_vector(rawdata,1,row);

  // Column fast discrete cosine transform
  rawdata = vector(1,col);
  for(i=1; i<=col; i++)
  {
    for (j=1; j<=row; j++)
      rawdata[j] = data[j][i];
    cosft2(rawdata,row,isign);
    for(j=1; j<=row; j++)
      data[j][i] = rawdata[j];
  }
  free_vector(rawdata,1,col);  
}



