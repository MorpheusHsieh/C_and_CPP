//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Stdio.h>

#include "Main.h"
#include "Morphism.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMorphismForm *MorphismForm;
//---------------------------------------------------------------------------
__fastcall TMorphismForm::TMorphismForm(TComponent* Owner)
    : TForm(Owner)
{

}
//---------------------------------------------------------------------------
void Morphism(Byte *data, int k, long size)
{
  Byte *tmp = new Byte [size*size];
  long x, y, NewX, NewY;
  for (x=0;  x<size; x++)
  {
    for (y=0; y<size; y++)
    {
      NewX = ((k+1)*x+k*y)%size;
      NewY = (x+y)%size;
      tmp[NewX*size+NewY] = data[x*size+y];
    }
  }
  memcpy(data,tmp,size*size);
  delete [] tmp;
}
//---------------------------------------------------------------------------





