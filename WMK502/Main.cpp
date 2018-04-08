//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <stdio.h>
#include <stdlib.h>

#include "Main.h"
#include "Message.h"
#include "MDIChild.h"
#include "GetFileName.h"
#include "FuncToggle.h"
#include "Raw2BMP.h"
#include "Matrix.h"
#include "Sobel.h"
#include "Morphism.h"
#include "CycleTms.h"
#include "PriorityBk.h"
#include "WmkImg.h"
#include "SliceHelp.h"
#include "WmkHide502.h"
#include "Fast_DCT.h"
#include "ZigZag.h"
#include "Cosft2.h"
#include "Vector.h"
#include "realft.h"
#include "Nrerror.h"
#include "four1.h"
#include "Moment.h"
#include "ImgStrtPosi.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMainForm *MainForm;
//---------------------------------------------------------------------------
extern void Function_Toggle(bool);
extern void Send_Message(AnsiString);
extern void Show_Message(void);
extern AnsiString Get_FileName(AnsiString);
extern void Raw_ConvertTo_256BMP(AnsiString,Byte*,long,long);
extern void Get_Pixels(long,long);
extern void Sobel(Byte*,long, long);
extern void Morphism(Byte*,int,long);
extern long Cycle_Times(long,long);
extern void Priority_Block(Byte*,long,long,long,long);
extern double Watermarked(Byte*,Byte*,long,long,float);
extern float *vector(long, long);
extern void free_vector(float *, long, long);
extern float **matrix(long, long, long, long);
extern void free_matrix(float**, long, long, long, long);
extern void Fast_DCT(float**, long, long, int);
extern void Zig_Zag(ZigZag*, long, long);
extern void moment(float*,int,float*,float*,float*,float*,float*,float*);
extern void Img_Start_Position(Byte*,long*,long*);
extern long **Read_Position(long*,long*,long*,long*);
extern void WmkDeHde502(Byte*,Byte*,long,long,long,long,long,float);

//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormCreate(TObject *Sender)
{
  Pixel = new Byte [MaxSize*MaxSize];
  Function_Toggle(false);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::OpenMenuClick(TObject *Sender)
{
  bResult = OpenDialog->Execute();
  if (bResult == 0)
  {
    MainForm->StatusBar->Panels->Items[2]->Text = "你選擇了取消鍵...";
    MessageBeep(0);
  }
  else if (bResult == 1)
  {
    CurrentFile = Get_FileName(OpenDialog->FileName);
    if (CurrentFile.LastDelimiter("\.") == NULL)
      CurrentFile += ".bmp";
    CurrentChild = new TMDIForm(Application);
    Get_Pixels(CurrentChild->Image->Width,CurrentChild->Image->Height);
    Function_Toggle(true);
  }
//  AnsiString Mesg="Open "; Mesg+=CurrentFile;
//  Send_Message(Mesg);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SaveMenuClick(TObject *Sender)
{
  CurrentChild = (TMDIForm*)MainForm->ActiveMDIChild;
  if (!CurrentChild)
  {
    StatusBar->Panels->Items[2]->Text = "沒有開啟任何影像檔...";
    MessageBeep(0);
    return;
  }

  bResult = SaveDialog->Execute();
  if (bResult == 0)
  {
    MainForm->StatusBar->Panels->Items[2]->Text = "你選擇了取消鍵...";
    MessageBeep(0);
  }
  else if (bResult == 1)
  {
    AnsiString CurrentFile = Get_FileName(SaveDialog->FileName);
    if (CurrentFile.LastDelimiter("\.") == NULL)
      CurrentFile += ".bmp";
    long width = CurrentChild->Image->Width;
    long height = CurrentChild->Image->Height;
    Byte *data = new Byte[width*height];
    for (long y=0; y<height; y++)
      for (long x=0; x<width; x++)
        data[y*width+x] = Pixel[y*width+x];
    Raw_ConvertTo_256BMP(CurrentFile,data,width,height);
  }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::CloseMenuClick(TObject *Sender)
{
  MainForm->ActiveMDIChild->Close();
  CurrentChild = (TMDIForm*)MainForm->ActiveMDIChild;
  CurrentFile = CurrentChild->Caption;
  if (MDIChildCount - 1 == 0)
    Function_Toggle(0);
//  AnsiString Mesg="Close "; Mesg+=CurrentFile;
//  Send_Message(Mesg);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::CloseAllMenuClick(TObject *Sender)
{
  for (int i=0; i<MDIChildCount; i++)
    MDIChildren[i]->Close();
  Function_Toggle(0);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::BoardMenuClick(TObject *Sender)
{
  Show_Message();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::CopyMenuClick(TObject *Sender)
{
  if (!Selected)
  {
    StatusBar->Panels->Items[2]->Text = "尚未選取任何區域...";
    MessageBeep(0);
    return;
  }
  else
  {
    CurrentChild = (TMDIForm*)MainForm->ActiveMDIChild;
    long x1=CurrentChild->StrtPt.x, y1=CurrentChild->StrtPt.y;
    long x2=CurrentChild->EndPt.x,  y2=CurrentChild->EndPt.y;
    long width = x2-x1-1, height = y2-y1-1;
    StrtPt2 = CurrentChild->StrtPt;
    Get_Pixels(width,height);

    CurrentFile = "Noname" + IntToStr(MDIChildCount+1) + ".bmp";
    Raw_ConvertTo_256BMP(CurrentFile,Pixel,width,height);
    CurrentChild = new TMDIForm(Application);
  }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SobelMenuClick(TObject *Sender)
{
  long width = CurrentChild->Image->Width;
  long height = CurrentChild->Image->Height;
  Byte *data = new Byte [width*height];
  memcpy(data,Pixel,width*height);
  Sobel(data,width,height);
  CurrentFile = "Noname" + IntToStr(MDIChildCount+1) + ".bmp";
  Raw_ConvertTo_256BMP(CurrentFile,data,width,height);
  CurrentChild = new TMDIForm(Application);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::MorphismMenuClick(TObject *Sender)
{
  MorphismForm->ImageName->Caption = CurrentFile;
  long width = CurrentChild->Image->Width;
  long height = CurrentChild->Image->Height;
  long size; (width>=height) ? size=width : size=height;
  MorphismForm->Edit1->Text = size;
  MorphismForm->Edit1->Enabled = false;

  int result = MorphismForm->ShowModal();
  if (result == mrCancel) return;
  long k = atol(MorphismForm->Edit2->Text.c_str());
  long rtime = atol(MorphismForm->Edit3->Text.c_str());

  Byte *data = new Byte [size*size];
  memset(data,255,size*size);
  for (long y=0; y<height; y++)
    memcpy(&data[y*size],&Pixel[y*width],width);

  for (long i=0; i<rtime; i++)
    Morphism(data,k,size);
  CurrentFile = "Noname" + IntToStr(MDIChildCount+1) + ".bmp";
  Raw_ConvertTo_256BMP(CurrentFile,data,size,size);
  CurrentChild = new TMDIForm(Application);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::CycleTmsMenuClick(TObject *Sender)
{
  long width = CurrentChild->Image->Width;
  long height = CurrentChild->Image->Height;
  long size; (width >= height) ? size = width : size = height;
  CycleTmsForm->Edit1->Text = size;
  CycleTmsForm->Edit3->Enabled = false;

  int result = CycleTmsForm->ShowModal();
  if (result == mrCancel) return;
  long K = atol(CycleTmsForm->Edit2->Text.c_str());
  long RTime = Cycle_Times(size,K);
  Send_Message(RTime);
  CycleTmsForm->Edit1->Enabled = false;
  CycleTmsForm->Edit2->Enabled = false;
  CycleTmsForm->Edit3->Enabled = true;
  CycleTmsForm->Edit3->Text = RTime;
  CycleTmsForm->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::PriorityMenuClick(TObject *Sender)
{
  long width = CurrentChild->Image->Width;
  long height = CurrentChild->Image->Height;
  Byte *data = new Byte [width*height];
  memcpy(data,Pixel,width*height);

  PriorityForm->Edit1->Text = 8;
  int result = PriorityForm->ShowModal();
  if (result == mrCancel) return;
  long BkSize = atol(PriorityForm->Edit1->Text.c_str());
  long BkNum = atol(PriorityForm->Edit2->Text.c_str());
  Priority_Block(data,BkSize,BkNum,width,height);

  CurrentFile = "Noname" + IntToStr(MDIChildCount+1) + ".bmp";
  Raw_ConvertTo_256BMP(CurrentFile,data,width,height);
  CurrentChild = new TMDIForm(Application);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormActivate(TObject *Sender)
{
  Selected = false;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::VarEstMenuClick(TObject *Sender)
{
  long width = CurrentChild->Image->Width;
  long height = CurrentChild->Image->Height;
  long size; (width >= height) ? size = width : size = height;
  MomentForm->Edit1->Text = width;
  MomentForm->Edit2->Text = height;
  MomentForm->Edit1->Enabled = false;
  MomentForm->Edit2->Enabled = false;
  int result = MomentForm->ShowModal();
  if (result == mrCancel) return;
  long BS = atol(MomentForm->Edit3->Text.c_str());
  long K = atol(MomentForm->Edit4->Text.c_str());

  Byte *pixel = new Byte [size*size];
  memset(pixel,255,size*size);
  for (long y=0; y<height; y++)
    memcpy(&pixel[y*size],&Pixel[y*width],width);

  long rtime = Cycle_Times(size,K), i, NewX, NewY;
  float **data, *rawdata, ave, adev, sdev, vrnce, skew, curt, max, min;
  data = matrix(1,size,1,size);
  rawdata = vector(1,size*size);
  ZigZag *zigzag = new ZigZag [BS*BS];
  Zig_Zag(zigzag,BS,BS);

  Mesg="  K    Mean ave,       SDEV,  Max Value,  Min Value,    Max-Min";
  Send_Message(Mesg);
  Mesg = "---------------------------------------------------------------";
  Send_Message(Mesg);
  StatusBar->Panels->Items[2]->Text = "計算中，請等待...";
  for (i=0; i<rtime; i++)
  {
    Morphism(pixel,K,size);
    for (long y=0; y<size; y++)
      for (long x=0; x<size; x++)
        data[y+1][x+1] = (float)pixel[y*size+x];
    Fast_DCT(data,size,BS,1);

    max = 0.0; min = 0.0;
    for (long row=0; row<(size/BS); row++)
    {
     for (long col=0; col<(size/BS); col++)
     {
      for (long y=0; y<BS; y++)
      {
       for (long x=0; x<BS; x++)
       {
        NewX = col*BS + zigzag[y*BS+x].col;
        NewY = row*BS + zigzag[y*BS+x].row;
        if ((y*BS+x) == 0)
          rawdata[NewY*size+NewX+1] = 0;
        else
        {
          rawdata[NewY*size+NewX+1] = data[NewY+1][NewX+1];
          if (data[NewY+1][NewX+1] > max)
            max = data[NewY+1][NewX+1];
          else if (data[NewY+1][NewX+1] < min)
            min = data[NewY+1][NewX+1];
        }
       }
      }
     }
    }
    moment(rawdata,size*size,&ave,&adev,&sdev,&vrnce,&skew,&curt);

    TVarRec tmpvar2[7] = { (float)i, ave, sdev, max, min, max-min};
    Mesg = Format("%3.0f, %10.4f, %10.4f, %10.4f, %10.4f, %10.4f",tmpvar2,7);
    Send_Message(Mesg);
  }
  StatusBar->Panels->Items[2]->Text = "計算完畢.";
  MessageBeep(0);
  Mesg = "---------------------------------------------------------------";
  Send_Message(Mesg);
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::Hde01MenuClick(TObject *Sender)
{
  // selected watermark area or not ?
  if (!Selected)
  {
    StatusBar->Panels->Items[2]->Text = "未圈選任何隱藏浮水印的區域";
    MessageBeep(0);
    return;
  }
  else
  {
    StrtPt2 = CurrentChild->StrtPt;
    EndPt2 = CurrentChild->EndPt;
  }

  long w1 = CurrentChild->Image->Width;
  long h1 = CurrentChild->Image->Height;
  AnsiString SrcName = CurrentFile;
  TMDIForm *SrcChild = CurrentChild;
  Byte *pixel1 = new Byte [w1*h1];
  Get_Pixels(w1,h1);
  memcpy(pixel1,Pixel,w1*h1);
//  CurrentFile = "Noname" + IntToStr(MDIChildCount+1) + ".bmp";
//  Raw_ConvertTo_256BMP(CurrentFile,pixel1,w1,h1);
//  CurrentChild = new TMDIForm(Application);

  // Load watermark
  OpenDialog->Title = "選擇要隱藏的浮水印";
  bResult = OpenDialog->Execute();
  if (bResult == 0)
  {
    MainForm->StatusBar->Panels->Items[2]->Text = "你選擇了取消鍵...";
    MessageBeep(0);
  }
  else if (bResult == 1)
  {
    CurrentFile = Get_FileName(OpenDialog->FileName);
    if (CurrentFile.LastDelimiter("\.") == NULL)
      CurrentFile += ".bmp";
    CurrentChild = new TMDIForm(Application);
    Get_Pixels(CurrentChild->Image->Width,CurrentChild->Image->Height);
  }
  AnsiString WmkName = CurrentFile;
//  TMDIForm *WmkChild = CurrentChild;
  long w2 = CurrentChild->Image->Width;
  long h2 = CurrentChild->Image->Height;
  long size2; (w2 >= h2) ? size2 = w2 : size2 = h2;
  Byte *pixel2 = new Byte [size2*size2];
  memset(pixel2,255,size2*size2);
  Get_Pixels(w2,h2);
  for (long y=0; y<h2; y++)
    memcpy(&pixel2[y*size2],&Pixel[y*w2],w2);
//  CurrentFile = "Noname" + IntToStr(MDIChildCount+1) + ".bmp";
//  Raw_ConvertTo_256BMP(CurrentFile,pixel2,size2,size2);
//  CurrentChild = new TMDIForm(Application);

  // Estimation hiding area is enought or not
  CurrentFile = SrcName; CurrentChild = SrcChild;
  long w3 = EndPt2.x-StrtPt2.x-1, h3 = EndPt2.y-StrtPt2.y-1;
  if ((w3*h3) < (w2*h2*4))
  {
    StatusBar->Panels->Items[2]->Text = "隱藏浮水印的區域太小...";
    MessageBeep(0);
    CloseAllMenuClick(Sender);
    Function_Toggle(1);
    return;
  }
  Byte *pixel3 = new Byte [w3*h3];
  Get_Pixels(w3,h3);
  memcpy(pixel3,Pixel,w3*h3);
//  CurrentFile = "Noname" + IntToStr(MDIChildCount+1) + ".bmp";
//  Raw_ConvertTo_256BMP(CurrentFile,pixel3,w3,h3);
//  CurrentChild = new TMDIForm(Application);

  // Watermark Process
  WmkImgForm->GroupBox5->Visible = false;
  WmkImgForm->Caption = "設定隱藏浮水印參數";
  WmkImgForm->SrcImgName->Caption = SrcName;
  WmkImgForm->WmkImgName->Caption = WmkName;
  WmkImgForm->Edit1->Text = w1;
  WmkImgForm->Edit2->Text = h1;
  WmkImgForm->Edit1->Enabled = false;
  WmkImgForm->Edit2->Enabled = false;
  WmkImgForm->Edit3->Text = w2;
  WmkImgForm->Edit4->Text = h2;
  WmkImgForm->Edit3->Enabled = false;
  WmkImgForm->Edit4->Enabled = false;
  int result = WmkImgForm->ShowModal();
  if (result == mrCancel) return;
  long K  = atol(WmkImgForm->Edit5->Text.c_str());
  long rtime = atol(WmkImgForm->Edit6->Text.c_str());
  float alpha = atof(WmkImgForm->Edit7->Text.c_str());

  // Compute important block
//  SliceHelpForm->ShowModal();
  long BkSize = 8;
  long BkNum = size2/BkSize*size2/BkSize;
  Priority_Block(pixel3,BkSize*2,BkNum,w3,h3);
//  CurrentFile = "Noname" + IntToStr(MDIChildCount+1) + ".bmp";
//  Raw_ConvertTo_256BMP(CurrentFile,pixel3,w3,h3);
//  CurrentChild = new TMDIForm(Application);

  // Rotation watermark
  for (long i=0; i<rtime; i++)
     Morphism(pixel2,K,size2);
//  CurrentFile = "Noname" + IntToStr(MDIChildCount+1) + ".bmp";
//  Raw_ConvertTo_256BMP(CurrentFile,pixel2,size2,size2);
//  CurrentChild = new TMDIForm(Application);


  // Watermark hideing
  double DCAvg = Watermarked(pixel1,pixel2,w1,size2,alpha);
  CurrentFile = "Noname" + IntToStr(MDIChildCount+1) + ".bmp";
  Raw_ConvertTo_256BMP(CurrentFile,pixel1,w1,h1);
  CurrentChild = new TMDIForm(Application);

  Mesg = "Key={";
  Mesg += w1;                 Mesg += ", ";
  Mesg += h1;                 Mesg += ", ";
  Mesg += w2;                 Mesg += ", ";
  Mesg += h2;                 Mesg += ", ";
  Mesg += K;                  Mesg += ", ";
  Mesg += rtime;              Mesg += ", ";
  Mesg += alpha;              Mesg += ", ";
  Mesg += FloatToStr(DCAvg);  Mesg += "}";
  Send_Message(Mesg);
  WmkImgForm->GroupBox5->Visible = true;
  WmkImgForm->Edit8->Text = DCAvg;
  WmkImgForm->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::DeHde01MenuClick(TObject *Sender)
{
  if (!CurrentChild)
  {
    StatusBar->Panels->Items[2]->Text = "尚未開啟任何影像...";
    MessageBeep(0);
    return;
  }
  // get source image information
  AnsiString SrcName = CurrentFile;
  long w1 = CurrentChild->Image->Width;
  long h1 = CurrentChild->Image->Height;
  Byte *pixel1 = new Byte [w1*h1];
  Get_Pixels(w1,h1);
  memcpy(pixel1,Pixel,w1*h1);
//  CurrentFile = "Noname" + IntToStr(MDIChildCount+1) + ".bmp";
//  Raw_ConvertTo_256BMP(CurrentFile,pixel1,w1,h1);
//  CurrentChild = new TMDIForm(Application);

  long mean = 0;
  for (long y=0; y<h1; y++)
    for (long x=0; x<w1; x++)
      mean += pixel1[y*w1+x];
  mean /= (w1*h1);

  // set watermarking parameter
  AnsiString WmkName = "Noname" + IntToStr(MDIChildCount+2) + ".bmp";
  WmkImgForm->Caption = "設定隱藏浮水印參數";
  WmkImgForm->SrcImgName->Caption = SrcName;
  WmkImgForm->WmkImgName->Caption = WmkName;
  int result = WmkImgForm->ShowModal();
  if (result == 0) return;
  long w2 = atol(WmkImgForm->Edit1->Text.c_str());
  long h2 = atol(WmkImgForm->Edit2->Text.c_str());
  long w3 = atol(WmkImgForm->Edit3->Text.c_str());
  long h3 = atol(WmkImgForm->Edit4->Text.c_str());
  long K  = atol(WmkImgForm->Edit5->Text.c_str());
  long rtime = atol(WmkImgForm->Edit6->Text.c_str());
  float alpha = atof(WmkImgForm->Edit7->Text.c_str());
  long DCAvg = atol(WmkImgForm->Edit8->Text.c_str());

  // Alignment image for get Left-Up point coordinate
  StatusBar->Panels->Items[2]->Text = "現在正在計算影像的起始位置，請等待...";
  long StrtX = 0, StrtY = 0;
  Img_Start_Position(pixel1,&StrtX,&StrtY);
  Mesg = "Start Position: "; Mesg+=StrtX; Mesg+=", "; Mesg+=StrtY;
  Send_Message(Mesg);
  StatusBar->Panels->Items[2]->Text = "計算完畢.";
  MessageBeep(0);

  // rebuild source image
  Byte *pixel2 = new Byte [w2*h2];
  Byte *pixel3 = new Byte [w3*h3];
  memset(pixel2,255,w2*h2);
  for (long y=0; y<h1; y++)
    for (long x=0; x<w1; x++)
      pixel2[(StrtY+y)*w2+(StrtX+x)] = pixel1[y*w1+x];
//  CurrentFile = "Noname" + IntToStr(MDIChildCount+1) + ".bmp";
//  Raw_ConvertTo_256BMP(CurrentFile,pixel2,w2,h2);
//  CurrentChild = new TMDIForm(Application);

  // Read block position
  WmkDeHde502(pixel2,pixel3,w2,h2,w3,h3,DCAvg,alpha);
  long size; (w3 >= h3) ? size = w3 : size = h3;
  long ctime = Cycle_Times(size,K);
  for (long i=0; i<(ctime-rtime); i++)
     Morphism(pixel3,K,size);

  CurrentFile = "Noname" + IntToStr(MDIChildCount+1) + ".bmp";
  Raw_ConvertTo_256BMP(CurrentFile,pixel3,w3,h3);
  CurrentChild = new TMDIForm(Application);
}
//---------------------------------------------------------------------------

