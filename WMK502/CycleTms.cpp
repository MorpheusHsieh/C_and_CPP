//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Main.h"
#include "CycleTms.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TCycleTmsForm *CycleTmsForm;
//---------------------------------------------------------------------------
__fastcall TCycleTmsForm::TCycleTmsForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
long Cycle_Times(long size, long k)
{
  bool toggle;
  long RTimes = 1, cnt, x, y, OldX, OldY, NewX, NewY;
  for (y=0; y<size; y++)
  {
    for (x=0; x<size; x++)
    {
      cnt = 0; OldY = y; OldX = x, toggle = true;
      while (toggle)
      {
        NewX = ((k+1)*OldX + k*OldY)%size;
        NewY = (OldX + OldY) % size;
        if (NewX == x && NewY == y)
          toggle = false;
        OldX = NewX; OldY = NewY;
        cnt++;
      }
      if (cnt > RTimes)
        RTimes = cnt;
    }
  }
  return RTimes;
}
//---------------------------------------------------------------------------
