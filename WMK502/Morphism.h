//---------------------------------------------------------------------------
#ifndef MorphismH
#define MorphismH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TMorphismForm : public TForm
{
__published:	// IDE-managed Components
    TGroupBox *GroupBox2;
    TLabel *Label1;
    TLabel *ImageName;
    TLabel *Label2;
    TLabel *Label3;
    TEdit *Edit1;
    TEdit *Edit2;
    TEdit *Edit3;
    TUpDown *UpDown2;
    TUpDown *UpDown3;
    TBitBtn *BitBtn1;
    TBitBtn *BitBtn2;
    TGroupBox *GroupBox1;
    TLabel *Label4;
private:	// User declarations
public:		// User declarations
    __fastcall TMorphismForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMorphismForm *MorphismForm;
//---------------------------------------------------------------------------
#endif
