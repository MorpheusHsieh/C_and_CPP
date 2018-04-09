// Program : FFT01.cpp -- Fast Fourier Transform
// Author  : Morpheus
// Date    : 98.08.05

#include <time.h>
#include <math.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define IS     512

typedef struct IMAGE {
        int  row;
        int  col;
        char name[20];
        } Image;

typedef struct ZIGZAG {
        int row;
        int col;
        } ZigZag;

FILE *fin, *fout, *fptr;
long BS, AC_Num;
unsigned char Pixel[IS][IS];

void Initial_State(Image*, Image*);
extern void Zig_Zag(ZigZag *, int);
extern float **matrix(long, long, long, long);
extern float ***f3tensor(long, long, long, long, long, long);
extern void free_matrix(float **, long, long, long, long);
extern void free_f3tensor(float ***, long, long, long, long, long, long);
extern void fourn(float*, unsigned long, int, int);
extern void rlft3(float***, float **, unsigned long, unsigned long,
                  unsigned long, int);

void main(void)
{
  clrscr();
  printf("\nTest Two-Dimension transform by address.\n");

  Image infile, outfile;
  Initial_State(&infile, &outfile);
//  printf("%s, %d, %d", infile.name, infile.row, infile.col); getch();

  float ***data, **speq;
  unsigned long row = infile.row, col = infile.col;
  data = f3tensor(1,1,1,BS,1,BS);
  speq = matrix(1,1,1,2*BS);

  time_t tstart, tstop;
  time(&tstart);

  ZigZag *zigzag = new ZigZag [row*col];
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
  unsigned char *NewPixel = new unsigned char [IS*IS];
  for (u=0; u<(row/BS); u++)
  {
    for (v=0; v<(col/BS); v++)
    {
      for (x=0; x<BS; x++)
        for (y=0; y<BS; y++)
          data[1][x+1][y+1] = (float)Pixel[u*BS+x][v*BS+y];

      //Fast discrete cocine transform
      rlft3(data,speq,1,BS,BS,1);

      for (x=0; x<BS; x++)
      {
        for (y=0; y<BS; y++)
        {
          if (x*BS+y > AC_Num)
          {
            NewX = zigzag[x*BS+y].row; NewY = zigzag[x*BS+y].col;
            data[1][NewX+1][NewY+1] = 0;
          }
        }
      }

      for (x=0; x<BS; x++)
      {
        for (y=0; y<BS; y++)
        {
          NewX = zigzag[x*BS+y].row; NewY = zigzag[x*BS+y].col;
          if (x*BS+y < AC_Num)
            if (x == 0 && y == 0)
              fprintf(fptr, "%5.1f,\n", 0.0);
            else
              fprintf(fptr, "%5.1f,\n", data[1][NewX+1][NewY+1]);
        }
      }

      rlft3(data,speq,1,BS,BS,-1);

      offset = u*BS*col + v*BS;
      for (x=0; x<BS; x++)
      {
        for (y=0; y<BS; y++)
        {
          value = (data[1][x+1][y+1] * 2.0 / ((float)(1*BS*BS)));
          NewPixel[offset+x*col+y] = (unsigned char)value;
        }
      }
    }
  }
  fwrite(NewPixel,row, col,fout);
  fclose(fptr);
  fclose(fout);
  delete [] NewPixel;
  delete [] zigzag;

  free_matrix(speq,1,1,1,2*BS);
  free_f3tensor(data,1,1,1,BS,1,BS);

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
  printf("\nEnter size of source image: ");
  scanf("%d", &size);

  infile->row = size;
  infile->col = size;

  printf("\nEnter size of block: ");
  scanf("%d", &BS);

  printf("\nEnter remain AC values (0-%d): ", BS*BS);
  scanf("%d", &AC_Num);

  printf("\nEnter output image filename: ");
  scanf("%s", &outfile->name);

  char string[10], fptrfile[20];
  itoa(BS,string,10);
  if (strlen(string) < 3)
    strncat(outfile->name, "000", 3-strlen(string));
  strcat(outfile->name, string);

  itoa(AC_Num,string,10);
  if (strlen(string) < 6)
    strncat(outfile->name, "000000", 6-strlen(string));
  strcat(outfile->name, string);
  strcpy(fptrfile, outfile->name);

  strcat(outfile->name, ".raw");
  if ((fout=fopen(outfile->name, "wb")) == NULL)
  {
    printf("\nOpen %s failure...", outfile->name);
    getch();
    exit(1);
  }
  outfile->row = infile->row;
  outfile->col = infile->col;

  strcat(fptrfile, ".out");
  if ((fptr=fopen(fptrfile, "wt")) == NULL)
  {
    printf("\nOpen %s failure...", fptrfile);
    getch();
    exit(1);
  }

  unsigned char *temp = new unsigned char [IS*IS];
  fread(temp,infile->row, infile->col,fin);
  fclose(fin);

  int i, j;
  for (i=0; i<infile->row; i++)
    for (j=0; j<infile->col; j++)
      Pixel[i][j] = temp[i*infile->col+j];

  delete((char*)temp);
}

