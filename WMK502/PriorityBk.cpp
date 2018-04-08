//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Stdio.h>
#include <string.h>

#include "Main.h"
#include "Message.h"
#include "PriorityBk.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TPriorityForm *PriorityForm;
//---------------------------------------------------------------------------
extern Send_Message(AnsiString);

extern long **lmatrix(long, long, long, long);
extern void free_lmatrix(long**, long, long, long, long);
extern void Sobel(Byte*,long, long);
extern void Raw_ConvertTo_256BMP(AnsiString,Byte*,long,long);
//---------------------------------------------------------------------------
__fastcall TPriorityForm::TPriorityForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
long **Read_Position(long *sx,long *sy,long *bs,long *bn)
{
  FILE *fptr = fopen("Hide502.Pos", "rt");
  fscanf(fptr,"%4ld,%4ld,%4ld,%4ld:",sx,sy,bs,bn);
  long **posi = lmatrix(1,2,1,*bn);
  for (long i=1; i<=*bn; i++)
    fscanf(fptr,"%2d,%2d,", &posi[1][i], &posi[2][i]);
  fclose(fptr);
  return posi;
}
//---------------------------------------------------------------------------
void Priority_Block(Byte *data,long BkSize,long BkNum,long width, long height)
{
//  long width = MainForm->CurrentChild->Image->Width;
//  long height = MainForm->CurrentChild->Image->Height;
  Byte *data1 = new Byte [width*height];
  memcpy(data1,data,width*height);
  Sobel(data,width,height);

  long wbn = width/BkSize, hbn = height/BkSize;
  long **Coord = lmatrix(1,3,1,wbn*hbn);
  long row, col, x, y, p;
  for (row=0; row<hbn; row++)
  {
    for (col=0; col<wbn; col++)
    {
      Coord[1][row*wbn+col+1] = col;
      Coord[2][row*wbn+col+1] = row;
      for (y=0; y<BkSize; y++)
      {
        for (x=0; x<BkSize; x++)
        {
          p = data[(row*BkSize+y)*width+(col*BkSize+x)];
          Coord[3][row*wbn+col+1] += p*p;
        }
      }
    }
  }

  // Sort HBkPosi value by minmum
  long i, j, len = wbn*hbn, tmp;
  for (i=1; i<=len-1; i++)
  {
    for (j=i+1; j<=len; j++)
    {
      if (Coord[3][i] > Coord[3][j])
      {
        tmp = Coord[1][i]; Coord[1][i] = Coord[1][j]; Coord[1][j] = tmp;
        tmp = Coord[2][i]; Coord[2][i] = Coord[2][j]; Coord[2][j] = tmp;
        tmp = Coord[3][i]; Coord[3][i] = Coord[3][j]; Coord[3][j] = tmp;
      }
    }
  }
  memset(data,255,width*height);

  // Display the most important 17 block
  FILE *fptr1=fopen("Hide502.Pos", "wb");
  FILE *fptr2=fopen("Hide502.Pxl", "wb");
  long StrtX = MainForm->StrtPt2.x, StrtY = MainForm->StrtPt2.y;
  fprintf(fptr1,"%4ld,%4ld,%4ld,%4ld:",StrtX,StrtY,BkSize,BkNum);

  Byte *data2 = new Byte [BkSize*BkSize];
  for (i=1; i<=BkNum; i++)
  {
    col = Coord[1][i],row = Coord[2][i];
    fprintf(fptr1,"%2d,%2d,", col, row);
    for (y=0; y<BkSize; y++)
    {
      for (x=0; x<BkSize; x++)
      {
        data2[y*BkSize+x] = data1[(row*BkSize+y)*width+(col*BkSize+x)];
        data[(row*BkSize+y)*width+(col*BkSize+x)] = data2[y*BkSize+x];
      }
    }
    fwrite(data2,BkSize*BkSize,1,fptr2);
  }
  delete [] data2;
  fclose(fptr1); fclose(fptr2);
  free_lmatrix(Coord,1,3,1,wbn*hbn);
/*
  fptr1=fopen("Hide502.Pos", "rb"); fptr2=fopen("Hide502.Pxl", "rb");
  long sx, sy, bs;
  fscanf(fptr1,"%d,%d,%d:", &sx,&sy,&bs);
  Byte *tmp2=new Byte [512*512], *tmp3 = new Byte [bs*bs];
  for (i=1; i<=BkNum; i++)
  {
    fscanf(fptr1,"%d,%d,", &col, &row);
    fread(tmp3,bs,bs,fptr2);
    for (y=0; y<bs; y++)
      for (x=0; x<bs; x++)
        tmp2[(sy+row*bs+y)*512+(sx+col*bs+x)] = tmp3[y*bs+x];
  }
  fclose(fptr1); fclose(fptr2);
  Raw_ConvertTo_256BMP("T003.bmp",tmp2,512,512);
*/
}
//---------------------------------------------------------------------------


