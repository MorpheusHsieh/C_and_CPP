//---------------------------------------------------------------------------
#ifndef PriorityBkH
#define PriorityBkH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TPriorityForm : public TForm
{
__published:	// IDE-managed Components
    TGroupBox *GroupBox1;
    TEdit *Edit1;
    TLabel *Label1;
    TLabel *Label2;
    TEdit *Edit2;
    TBitBtn *BitBtn1;
    TBitBtn *BitBtn2;
private:	// User declarations
public:		// User declarations
    __fastcall TPriorityForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TPriorityForm *PriorityForm;
//---------------------------------------------------------------------------
#endif
