//---------------------------------------------------------------------------
#ifndef MomentH
#define MomentH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TMomentForm : public TForm
{
__published:	// IDE-managed Components
    TGroupBox *GroupBox1;
    TLabel *Label1;
    TLabel *Label2;
    TEdit *Edit1;
    TEdit *Edit2;
    TGroupBox *GroupBox2;
    TLabel *Label3;
    TEdit *Edit4;
    TBitBtn *BitBtn1;
    TBitBtn *BitBtn2;
    TLabel *Label4;
    TEdit *Edit3;
private:	// User declarations
public:		// User declarations
    __fastcall TMomentForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMomentForm *MomentForm;
//---------------------------------------------------------------------------
#endif
