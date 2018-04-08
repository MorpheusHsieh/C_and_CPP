//---------------------------------------------------------------------------
#ifndef NrerrorH
#define NrerrorH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TErrorMsgForm : public TForm
{
__published:	// IDE-managed Components
    TMemo *Memo;
private:	// User declarations
public:		// User declarations
    __fastcall TErrorMsgForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TErrorMsgForm *ErrorMsgForm;
//---------------------------------------------------------------------------
#endif
