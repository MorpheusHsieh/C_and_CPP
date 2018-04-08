//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "Main.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TMainForm *MainForm;
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner)
{
}
//----------------------------------------------------------------------------
void __fastcall TMainForm::FormCreate(TObject *Sender)
{
	Application->OnHint = ShowHint;
}
//----------------------------------------------------------------------------
void __fastcall TMainForm::ShowHint(TObject *Sender)
{
	StatusLine->SimpleText = Application->Hint;
}
//----------------------------------------------------------------------------
void __fastcall TMainForm::FileNew(TObject *Sender)
{
	//--- Add code to create a new file ---
}
//----------------------------------------------------------------------------
void __fastcall TMainForm::FileOpen(TObject *Sender)
{
	if (OpenDialog->Execute())
	{
		//---- Add code to open OpenDialog->FileName ---- 
	}
}
//----------------------------------------------------------------------------
void __fastcall TMainForm::FileSave(TObject *Sender)
{
	//---- Add code to save current file under current name ----
}
//----------------------------------------------------------------------------
void __fastcall TMainForm::FileSaveAs(TObject *Sender)
{
	if (SaveDialog->Execute())
	{
		//--- Add code to save current file under SaveDialog->FileName ---
	}
}
//----------------------------------------------------------------------------
void __fastcall TMainForm::FilePrint(TObject *Sender)
{
	if (PrintDialog->Execute())
	{
		//---- Add code to print current file ----
	}
}
//----------------------------------------------------------------------------
void __fastcall TMainForm::FilePrintSetup(TObject *Sender)
{
	PrintSetupDialog->Execute();
}
//----------------------------------------------------------------------------
void __fastcall TMainForm::FileExit(TObject *Sender)
{
	Close();
}
//----------------------------------------------------------------------------
void __fastcall TMainForm::EditUndo(TObject *Sender)
{
	//---- Add code to perform Edit Undo ----
}
//----------------------------------------------------------------------------
void __fastcall TMainForm::EditCut(TObject *Sender)
{
	//---- Add code to perform Edit Cut ----
}
//----------------------------------------------------------------------------
void __fastcall TMainForm::EditCopy(TObject *Sender)
{
	//--- Add code to perform Edit Copy ----
}
//----------------------------------------------------------------------------
void __fastcall TMainForm::EditPaste(TObject *Sender)
{
	//---- Add code to perform Edit Paste ----
}
//----------------------------------------------------------------------------
void __fastcall TMainForm::HelpContents(TObject *Sender)
{
	Application->HelpCommand(HELP_CONTENTS, 0);
}
//----------------------------------------------------------------------------
void __fastcall TMainForm::HelpSearch(TObject *Sender)
{
	Application->HelpCommand(HELP_PARTIALKEY, Longint(""));
}
//----------------------------------------------------------------------------
void __fastcall TMainForm::HelpHowToUse(TObject *Sender)
{
	Application->HelpCommand(HELP_HELPONHELP, 0);
}
//----------------------------------------------------------------------------
void __fastcall TMainForm::HelpAbout(TObject *Sender)
{
	//---- Add code to show program's About Box ----
}
//----------------------------------------------------------------------------
