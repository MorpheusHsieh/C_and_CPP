//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <stdio.h>

#include "MesgBord.h"
#include "Main.h"
#include "MDIChild.h"
#include "ImgStrtPosi.h"
#include "lMatrix.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TImgPosiForm *ImgPosiForm;
//---------------------------------------------------------------------------
__fastcall TImgPosiForm::TImgPosiForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
extern void Send_Message(AnsiString);
extern long **lmatrix(long, long, long, long);
extern void free_lmatrix(long**, long, long, long, long);
extern long **Read_Position(long*,long*,long*,long*);
//---------------------------------------------------------------------------
void Img_Start_Position(Byte *pixel,long *NewX,long *NewY)
{
  AnsiString Mesg;
  long bknum = RemainBkNum;
  long w1 = MainForm->CurrentChild->Image->Width;
  long h1 = MainForm->CurrentChild->Image->Height;

  // Read Hideing block position
  long sx, sy, bs, bn, **Posi;
  Posi = Read_Position(&sx,&sy,&bs,&bn);
  long bs2 = bs/2;

  // Read pixels of block
  FILE *fptr;
  if ((fptr=fopen("Hide502.Pxl", "rb")) == NULL)
  {
    MainForm->StatusBar->Panels->Items[2]->Text="Open Hide502.Pxl failure...";
    MessageBeep(0);
    return;
  }

  Byte *data = new Byte [bs*bs];
  Byte *data2 = new Byte [bs2*bs2];
  long **block  = lmatrix(1,2,1,bknum);
  long i, j, row, col, x, y, p, min, sum;
  for (i=1; i<=bknum; i++)
  {
    fread(data,bs,bs,fptr);
    for (y=0; y<bs2; y++)
      for (x=0; x<bs2; x++)
        data2[y*bs2+x] = data[y*bs+x];
    min=255.0*255.0*bs2*bs2;

    for (row=0; row<h1-bs2; row++)
    {
      for (col=0; col<w1-bs2; col++)
      {
        sum = 0;
        for (y=0; y<bs2; y++)
        {
          for (x=0; x<bs2; x++)
          {
            p = data2[y*bs2+x]-pixel[(row+y)*w1+(col+x)];
            sum += p*p;
          }
        }
        if (sum < min)
        {
          min = sum;
          block[1][i] = (sx + Posi[1][i]*bs) - col;
          block[2][i] = (sy + Posi[2][i]*bs) - row;
        }
      }
    }
//    Mesg=i; Mesg+=" --> ("; Mesg+=block[1][i]; Mesg+=", ";
//    Mesg+=block[2][i]; Mesg+=")";
//    Send_Message(Mesg);
  }
  fclose(fptr);

  // computing repeat start point
  int cnt, x1, y1, x2, y2;
  static int N[2][RemainBkNum];
  for (i=1; i<=bknum; i++)
  {
    x1 = block[1][i]; y1 = block[2][i];
    N[0][i-1] = i; cnt = 1;
    for (j=0; j<bknum; j++)
    {
      if (i != j)
      {
        x2 = block[1][j]; y2 = block[2][j];
        if ((x2 == x1) && (y2 == y1))
          cnt++;
      }
    }
    N[1][i-1] = cnt;
  }

  // search the most start position by sorting
  long tmp1;
  for (i=0; i<bknum-1; i++)
  {
    for (j=i+1; j<bknum; j++)
    {
      if (N[1][i] < N[1][j])
      {
        tmp1 = N[0][i]; N[0][i] = N[0][j]; N[0][j] = tmp1;
        tmp1 = N[1][i]; N[1][i] = N[1][j]; N[1][j] = tmp1;
      }
    }
  }
  int k = N[0][0];
  *NewX = block[1][k]; *NewY = block[2][k];
  ImgPosiForm->Edit1->Text = *NewY;
  ImgPosiForm->Edit1->Enabled = false;
  ImgPosiForm->Edit2->Text = *NewX;
  ImgPosiForm->Edit2->Enabled = false;

}
//---------------------------------------------------------------------------
