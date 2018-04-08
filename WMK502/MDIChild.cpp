//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Main.h"
#include "Message.h"
#include "MDIChild.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMDIForm *MDIForm;
//---------------------------------------------------------------------------
#include <stdio.h>

extern Send_Message(AnsiString);
extern Function_Toggle(bool);
//---------------------------------------------------------------------------
__fastcall TMDIForm::TMDIForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TMDIForm::FormClose(TObject *Sender, TCloseAction &Action)
{
  Action = caFree;
}
//---------------------------------------------------------------------------
void Get_Pixels(long col, long row)
{
  FILE *fptr = fopen(MainForm->CurrentFile.c_str(),"rb");

  long w1 = MainForm->CurrentChild->Image->Width;
  long h1 = MainForm->CurrentChild->Image->Height;
  long w2; ((w1/4)*4 == w1) ? w2=w1 : w2=((w1/4)+1)*4;
  long x1 = MainForm->CurrentChild->StrtPt.x;
  long y1 = MainForm->CurrentChild->StrtPt.y;
  Byte *tmp = new Byte [w1];
  for (long y=0; y<row; y++)
  {
    if ((col == w1) && (row == h1))
    {
      fseek(fptr,1078+(h1-1-y)*w2,SEEK_SET);
      fread(tmp,1,w1,fptr);
      memcpy(&MainForm->Pixel[y*w1],tmp,w1);
    }
    else
    {
      fseek(fptr,1078+(h1-y1-y)*w2,SEEK_SET);
      fread(tmp,1,w1,fptr);
      memcpy(&MainForm->Pixel[y*col],&tmp[x1],col);
    }
  }
  fclose(fptr);
  delete [] tmp;
}
//---------------------------------------------------------------------------
void __fastcall TMDIForm::FormCreate(TObject *Sender)
{
  Drawing = false;
  Graphics::TBitmap *Bitmap = new Graphics::TBitmap();
  Bitmap->LoadFromFile(MainForm->CurrentFile);

  Image->Width = Bitmap->Width; Image->Height = Bitmap->Height;
  ClientWidth = Bitmap->Width;  ClientHeight = Bitmap->Height;
  Image->Canvas->Draw(0,0,Bitmap);
  Image->Stretch = true;
  Image->AutoSize = true;
  Caption = MainForm->CurrentFile;
}
//---------------------------------------------------------------------------

void __fastcall TMDIForm::ImageMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if (Button == mbLeft)
  {
    Drawing = true;
    StrtPt = MovePt = Point(X,Y);
    Image->Canvas->MoveTo(X,Y);
  }
  else if (Button == mbRight)
  {
    MainForm->Selected = false;
    Image->Canvas->Pen->Style = psDot;
    Image->Canvas->Pen->Mode = pmNotXor;
    Image->Canvas->Rectangle(StrtPt.x,StrtPt.y,MovePt.x,MovePt.y);
  }

  TVarRec MousePosi[2] = {X,Y}; // TVarRec is a new class.
  MainForm->StatusBar->Panels->Items[0]->Text = Format("Origin: (%d, %d)", MousePosi,2);
}
//---------------------------------------------------------------------------

void __fastcall TMDIForm::ImageMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
  if (Drawing)
  {
    Image->Canvas->Pen->Style = psDot;
    Image->Canvas->Pen->Mode = pmNotXor;
    Image->Canvas->Rectangle(StrtPt.x,StrtPt.y,MovePt.x,MovePt.y);
    MovePt = Point(X,Y);
    Image->Canvas->Rectangle(StrtPt.x,StrtPt.y,MovePt.x,MovePt.y);
  }

  TVarRec MousePosi[2] = {X,Y};
  MainForm->StatusBar->Panels->Items[2]->Text = Format("Current: (%d, %d)", MousePosi,2);
}
//---------------------------------------------------------------------------
void __fastcall TMDIForm::ImageMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if (Drawing)
  {
    Image->Canvas->Pen->Mode = pmNop;
    EndPt = MovePt;
    Image->Canvas->Rectangle(StrtPt.x,StrtPt.y,MovePt.x,MovePt.y);
    MainForm->Selected = true;
    Drawing = False;
  }
  TVarRec MousePosi[2] = {(int)EndPt.x,(int)EndPt.y};
  MainForm->StatusBar->Panels->Items[1]->Text = Format("End: (%d, %d)", MousePosi,2);
}
//---------------------------------------------------------------------------
void __fastcall TMDIForm::FormDestroy(TObject *Sender)
{
  MainForm->CurrentChild = (TMDIForm*)MainForm->ActiveMDIChild;
  MainForm->CurrentFile = MainForm->CurrentChild->Caption;
  if (MainForm->MDIChildCount-1 == 0)
    Function_Toggle(false);
//  AnsiString Mesg="Destory "; Mesg+=MainForm->CurrentFile;
//  Send_Message(Mesg);
}
//---------------------------------------------------------------------------
void __fastcall TMDIForm::FormActivate(TObject *Sender)
{
  MainForm->CurrentChild = (TMDIForm*)MainForm->ActiveMDIChild;
  MainForm->CurrentFile = MainForm->CurrentChild->Caption;
  Get_Pixels(Image->Width,Image->Height);
//  AnsiString Mesg="Activate: "; Mesg+=MainForm->CurrentFile;
//  Send_Message(Mesg);
}
//---------------------------------------------------------------------------

