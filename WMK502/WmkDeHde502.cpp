//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <stdio.h>

#include "Main.h"
#include "Matrix.h"
#include "ZigZag.h"
#include "Message.h"
#include "WmkDeHde502.h"
//#include "PriorityBk.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
extern void Send_Message(AnsiString);
extern long **Read_Position(long*,long*,long*,long*);
extern void Zig_Zag(ZigZag*, long, long);
extern float **matrix(long, long, long, long);
extern void free_matrix(float**, long, long, long, long);
extern void Fast_DCT(float**, long, long, int);
//---------------------------------------------------------------------------
void WmkDeHde502(Byte *p2,Byte *p3,long w2,long h2,long w3,long h3,long DCAvg,float alpha)
{
  long sx, sy, bs, bn, **Posi;
  Posi = Read_Position(&sx,&sy,&bs,&bn);
  long bs2 =bs/2;

  // set DCT coefficient sequence
  ZigZag *zigzag1 = new ZigZag [bs*bs];    //quadreple area of watermark
  ZigZag *zigzag2 = new ZigZag [bs2*bs2];
  Zig_Zag(zigzag1,bs,bs);
  Zig_Zag(zigzag2,bs2,bs2);

  long i, x, y, x1, y1, x2, y2;
  Byte *p1 = new Byte [bs*bs];
  float **data0 = matrix(1,bs2,1,bs2);
  float **data1 = matrix(1,bs,1,bs);
  float **data2 = matrix(1,bs,1,bs);
  FILE *fptr = fopen("Hide502.Pxl","rb");
  for (i=1; i<=bn; i++)
  {
    x2 = Posi[1][i]; y2 = Posi[2][i];
    fread(p1,bs,bs,fptr);
    for (y=0; y<bs; y++)
    {
      for (x=0; x<bs; x++)
      {
        data1[y+1][x+1] = p1[y*bs+x] - 128;
        data2[y+1][x+1] = p2[(sy+y2*bs+y)*w2+(sx+x2*bs+x)] - 128;
      }
    }
    Fast_DCT(data1,bs,bs,1);
    Fast_DCT(data2,bs,bs,1);

    for (y=0; y<bs2; y++)
    {
      for (x=0; x<bs2; x++)
      {
        x1 = zigzag1[y*bs2+x].col+1; y1 = zigzag1[y*bs2+x].row+1;
        x2 = zigzag2[y*bs2+x].col+1; y2 = zigzag2[y*bs2+x].row+1;
        data0[y2][x2] = data2[y1][x1] - data1[y1][x1];
        data0[y2][x2] /= alpha;
/*        if (y*bs2+x == 0)
          data0[y2][x2] = DCAvg;
        else
        {
          data0[y2][x2] = data1[y1][x1] - data2[y1][x1];
          data0[y2][x2] /= alpha;
        }
*/
      }
    }
    Fast_DCT(data0,bs2,bs2,-1);

    x1 = (i-1)%bs; y1 = (i-1)/bs;
    for (y=0; y<bs2; y++)
      for (x=0; x<bs2; x++)
        p3[(y1*bs2+y)*w3+(x1*bs2+x)] = data0[y+1][x+1] + 128;
  }
  fclose(fptr);

}
//---------------------------------------------------------------------------
