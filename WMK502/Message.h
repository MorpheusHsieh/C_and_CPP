//---------------------------------------------------------------------------
#ifndef MessageH
#define MessageH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TMesgForm : public TForm
{
__published:	// IDE-managed Components
    TMemo *Memo;
private:	// User declarations
public:		// User declarations
    __fastcall TMesgForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMesgForm *MesgForm;
//---------------------------------------------------------------------------
#endif
