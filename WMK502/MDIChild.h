//---------------------------------------------------------------------------
#ifndef MDIChildH
#define MDIChildH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TMDIForm : public TForm
{
__published:	// IDE-managed Components
    TImage *Image;
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall ImageMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall ImageMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
    void __fastcall ImageMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall FormDestroy(TObject *Sender);

    void __fastcall FormActivate(TObject *Sender);
private:	// User declarations
public:		// User declarations
    bool Drawing;
    TPoint StrtPt, MovePt, EndPt;
    __fastcall TMDIForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMDIForm *MDIForm;
//---------------------------------------------------------------------------
#endif
