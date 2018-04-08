//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "GetFilename.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
AnsiString GetFileName(AnsiString FullName)
{
  int bkPos = FullName.LastDelimiter("\\");
  int strLen = FullName.Length();
  return FullName.SubString(bkPos+1,strLen);
}
//---------------------------------------------------------------------------
