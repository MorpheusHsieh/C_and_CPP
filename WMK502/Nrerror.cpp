//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <stdio.h>
#include <stdlib.h>

#include "Nrerror.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TErrorMsgForm *ErrorMsgForm;
//---------------------------------------------------------------------------
__fastcall TErrorMsgForm::TErrorMsgForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void Show_ErrorMessage(AnsiString error_text)
{
  ErrorMsgForm->Memo->Lines->Append("Numerical Recipes run-time error...\n");
  ErrorMsgForm->Memo->Lines->Append(error_text);
  ErrorMsgForm->Memo->Lines->Append("...now exiting to system...\n");
  exit(1);
}
//---------------------------------------------------------------------------

