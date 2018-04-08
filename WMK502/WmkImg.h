//---------------------------------------------------------------------------
#ifndef WmkImgH
#define WmkImgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TWmkImgForm : public TForm
{
__published:	// IDE-managed Components
    TGroupBox *GroupBox1;
    TLabel *Label2;
    TEdit *Edit1;
    TGroupBox *GroupBox3;
    TLabel *Label4;
    TLabel *Label5;
    TEdit *Edit5;
    TEdit *Edit6;
    TUpDown *UpDown1;
    TUpDown *UpDown2;
    TBitBtn *BitBtn1;
    TBitBtn *BitBtn2;
    TLabel *Label7;
    TEdit *Edit2;
    TGroupBox *GroupBox5;
    TEdit *Edit8;
    TGroupBox *GroupBox2;
    TLabel *Label8;
    TLabel *Label9;
    TEdit *Edit3;
    TEdit *Edit4;
    TLabel *Label1;
    TLabel *SrcImgName;
    TLabel *Label10;
    TLabel *WmkImgName;
    TGroupBox *GroupBox4;
    TEdit *Edit7;
private:	// User declarations
public:		// User declarations
    __fastcall TWmkImgForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TWmkImgForm *WmkImgForm;
//---------------------------------------------------------------------------
#endif
