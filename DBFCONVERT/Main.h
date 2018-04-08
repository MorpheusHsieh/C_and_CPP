//---------------------------------------------------------------------------

#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <Db.hpp>
#include <DBCtrls.hpp>
#include <DBGrids.hpp>
#include <DBTables.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
        TMainMenu *MainMenu1;
        TMenuItem *N1;
        TMenuItem *Open;
        TMenuItem *SaveAs;
        TMenuItem *N5;
        TMenuItem *Exit;
        TMenuItem *N8;
        TMenuItem *ConvertToText;
        TMenuItem *N9;
        TOpenDialog *OpenDialog1;
        TSaveDialog *SaveDialog1;
        TPanel *Panel1;
        TPageControl *PageControl1;
        TTabSheet *TabSheet1;
        TPanel *Panel2;
        TGroupBox *GroupBox1;
        TMemo *Memo1;
        TMemo *Memo2;
        TLabel *Label1;
        TEdit *Edit1;
        TLabel *Label2;
        TEdit *Edit2;
        TLabel *Label3;
        TEdit *Edit3;
        TLabel *Label4;
        TEdit *Edit4;
        TLabel *Label5;
        TEdit *Edit5;
        TStatusBar *StatusBar1;
        TTabSheet *TabSheet3;
        TPanel *Panel4;
        TMemo *Memo3;
        TButton *Button1;
        TLabel *Label6;
        TEdit *Edit6;
        TButton *Button2;
        TTabSheet *TabSheet2;
        TDBGrid *DBGrid1;
        TDataSource *DataSource1;
        TTable *Table1;
        TQuery *Query1;
        TDatabase *Database1;
        void __fastcall OpenClick(TObject *Sender);
        void __fastcall SaveAsClick(TObject *Sender);
        void __fastcall ConvertToTextClick(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall FormActivate(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
