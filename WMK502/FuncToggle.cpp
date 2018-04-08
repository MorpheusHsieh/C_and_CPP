//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Main.h"
#include "FuncToggle.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
void Function_Toggle(bool isign)
{
  MainForm->SaveMenu->Enabled = isign;
  MainForm->CloseMenu->Enabled = isign;
  MainForm->CloseAllMenu->Enabled = isign;
  MainForm->CopyMenu->Enabled = isign;
  MainForm->HideMenu->Enabled = isign;
  MainForm->DeHideMenu->Enabled = isign;
  MainForm->EgPryMenu->Enabled = isign;
  MainForm->PriorityMenu->Enabled = isign;
  MainForm->CycleTmsMenu->Enabled = isign;
  MainForm->MorphismMenu->Enabled = isign;
}
//---------------------------------------------------------------------------
