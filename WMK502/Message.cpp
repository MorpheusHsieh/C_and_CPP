//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Message.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMesgForm *MesgForm;
//---------------------------------------------------------------------------
__fastcall TMesgForm::TMesgForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void Send_Message(AnsiString Mesg)
{
  if (Mesg.Length() == 0)
    MesgForm->Memo->Lines->Append("Not any message...");
  else
    MesgForm->Memo->Lines->Append(Mesg);
}
//---------------------------------------------------------------------------
void Show_Message(void)
{
  MesgForm->ShowModal();
}
//---------------------------------------------------------------------------


