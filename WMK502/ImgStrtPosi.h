//---------------------------------------------------------------------------
#ifndef ImgStrtPosiH
#define ImgStrtPosiH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TImgPosiForm : public TForm
{
__published:	// IDE-managed Components
    TGroupBox *GroupBox1;
    TLabel *Label1;
    TLabel *Label2;
    TEdit *Edit1;
    TEdit *Edit2;
    TBitBtn *BitBtn1;
private:	// User declarations
public:		// User declarations
    __fastcall TImgPosiForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TImgPosiForm *ImgPosiForm;
//---------------------------------------------------------------------------
#endif
