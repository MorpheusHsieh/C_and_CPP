//---------------------------------------------------------------------------
#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
#include "MDIChild.h"
#define BKSIZE          8
#define RemainBkNum    17
#define MaxSize      1024

class TMainForm : public TForm
{
__published:	// IDE-managed Components
    TBevel *Bevel1;
    TMainMenu *MainMenu;
    TMenuItem *File1;
    TMenuItem *ExitMenu;
    TMenuItem *N1;
    TMenuItem *SaveMenu;
    TMenuItem *OpenMenu;
    TMenuItem *N2;
    TMenuItem *CloseMenu;
    TMenuItem *CloseAllMenu;
    TMenuItem *N5;
    TMenuItem *HideMenu;
    TMenuItem *DeHideMenu;
    TMenuItem *HelpMenu;
    TMenuItem *BoardMenu;
    TPanel *SpeedBar;
    TSpeedButton *OpenSBtn;
    TSpeedButton *CloseSBtn;
    TSpeedButton *SaveSBtn;
    TSpeedButton *HideSBtn;
    TSpeedButton *DeHideSBtn;
    TSpeedButton *ExitSBtn;
    TMenuItem *N3;
    TMenuItem *AboutBox;
    TStatusBar *StatusBar;
    TOpenDialog *OpenDialog;
    TSaveDialog *SaveDialog;
    TMenuItem *Editmenu;
    TMenuItem *CopyMenu;
    TMenuItem *FilterMenu;
    TMenuItem *EgPryMenu;
    TMenuItem *SobelMenu;
    TMenuItem *CycleTmsMenu;
    TMenuItem *MorphismMenu;
    TMenuItem *N4;
    TMenuItem *PriorityMenu;
    TMenuItem *N6;
    TMenuItem *VarEstMenu;
    TMenuItem *Hde01Menu;
    TMenuItem *Hde02Menu;
    TMenuItem *DeHde01Menu;
    TMenuItem *DeHde02Menu;
    void __fastcall OpenMenuClick(TObject *Sender);
    void __fastcall BoardMenuClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall CloseAllMenuClick(TObject *Sender);
    void __fastcall CloseMenuClick(TObject *Sender);
    void __fastcall SaveMenuClick(TObject *Sender);
    void __fastcall CopyMenuClick(TObject *Sender);
    void __fastcall SobelMenuClick(TObject *Sender);
    void __fastcall MorphismMenuClick(TObject *Sender);
    void __fastcall CycleTmsMenuClick(TObject *Sender);
    void __fastcall PriorityMenuClick(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
    
    void __fastcall VarEstMenuClick(TObject *Sender);
    
    void __fastcall Hde01MenuClick(TObject *Sender);
    void __fastcall DeHde01MenuClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    bool bResult, Selected;
    Byte *Pixel;
    TPoint StrtPt2, EndPt2;
    AnsiString Mesg, CurrentFile;
    TMDIForm *CurrentChild;
    __fastcall TMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
