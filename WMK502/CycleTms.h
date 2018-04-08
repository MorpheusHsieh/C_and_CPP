//---------------------------------------------------------------------------
#ifndef CycleTmsH
#define CycleTmsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TCycleTmsForm : public TForm
{
__published:	// IDE-managed Components
    TGroupBox *GroupBox2;
    TLabel *Label2;
    TEdit *Edit2;
    TEdit *Edit3;
    TBitBtn *BitBtn1;
    TBitBtn *BitBtn2;
    TLabel *Label1;
    TGroupBox *GroupBox1;
    TEdit *Edit1;
    TUpDown *UpDown1;
private:	// User declarations
public:		// User declarations
    __fastcall TCycleTmsForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TCycleTmsForm *CycleTmsForm;
//---------------------------------------------------------------------------
#endif
