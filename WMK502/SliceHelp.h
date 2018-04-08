//---------------------------------------------------------------------------
#ifndef SliceHelpH
#define SliceHelpH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TSliceHelpForm : public TForm
{
__published:	// IDE-managed Components
    TMemo *Memo1;
    TBitBtn *BitBtn1;
private:	// User declarations
public:		// User declarations
    __fastcall TSliceHelpForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSliceHelpForm *SliceHelpForm;
//---------------------------------------------------------------------------
#endif
