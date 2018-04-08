//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Main.h"
#include "GetFileName.h"
#include "ShowDBHead.h"
#include "DBF_Head.h"

AnsiString GetFileName(AnsiString);
void ShowDBHead(DBF_HEAD,AnsiString);
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMainForm *MainForm;
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::N2Click(TObject *Sender)
{
  bResult = OpenDialog->Execute();
  if (bResult == 0)
  {
    MainForm->StatusBar->Panels->Items[0]->Text = "你選擇了取消鍵...";
    MessageBeep(0);
  }
  else if (bResult == 1)
  {
    CrntFile = GetFileName(OpenDialog->FileName);
    if (CrntFile.LastDelimiter("\.") == NULL)
      CrntFile += ".DBF";

    DBF_HEAD dbf_head;
    ShowDBHead(dbf_head,CrntFile);
  }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
