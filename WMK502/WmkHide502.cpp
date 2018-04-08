//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <stdio.h>
#define BS    8

#include "Main.h"
#include "ZigZag.h"
#include "WmkHide502.h"
#include "MesgBord.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
extern Byte **bmatrix(long, long, long, long);
extern void free_bmatrix(long**, long, long, long, long);
extern long **lmatrix(long, long, long, long);
extern void free_lmatrix(long**, long, long, long, long);
extern float **matrix(long, long, long, long);
extern void free_matrix(float**, long, long, long, long);
extern void Zig_Zag(ZigZag*, long, long);
extern void Fast_DCT(float**, long, long, int);
extern long **Read_Position(long*,long*,long*,long*);

extern void Send_Message(AnsiString);
extern void Raw_ConvertTo_256BMP(AnsiString,Byte*,long,long);
//---------------------------------------------------------------------------
double Watermarked(Byte *p1,Byte *p2,long w1,long w2,float alpha)
{
  // Read hide position from file
  long sx, sy, bs, bn, **Posi;
  Posi = Read_Position(&sx,&sy,&bs,&bn);
  long bs2 =bs/2;

  // set DCT coefficient sequence
  ZigZag *zigzag1 = new ZigZag [bs*bs];    //quadreple area of watermark
  ZigZag *zigzag2 = new ZigZag [bs2*bs2];
  Zig_Zag(zigzag1,bs,bs);
  Zig_Zag(zigzag2,bs2,bs2);

  // collection hideing block of source image and convert to frequency domain
  float **data2 = matrix(1,bs2,1,bs2);
  float **data3 = matrix(1,bs,1,bs);
  long i, x, y, x0, y0 , x1, y1, NewX1, NewY1, NewX2, NewY2, avg = 0;
  AnsiString Mesg;
  for (i=1; i<=bn; i++)
  {
    x0 = Posi[1][i]; y0 = Posi[2][i];
    for (y=0; y<bs; y++)
      for (x=0; x<bs; x++)
        data3[y+1][x+1] = (float)(p1[(sy+y0*bs+y)*w1+(sx+x0*bs+x)]-128);
    Fast_DCT(data3,bs,bs,1);    // 快速DCT轉換只能接受2的次方倍。
/*    if (i == 1)
    {
      FILE *fptr = fopen ("tmp001.txt", "wt");
      for (y=0; y<bs; y++)
        for (x=0; x<bs; x++)
          if (y*bs != 0)
            fprintf(fptr,"%f\n",data3[y+1][x+1]);
      fclose(fptr);
    }
*/
    // watermark convert to frequency domain
    x1 = (i-1)%bs; y1 = (i-1)/bs;
    for (y=0; y<bs2; y++)
      for (x=0; x<bs2; x++)
        data2[y+1][x+1] = (float)(p2[(y1*bs2+y)*w2+(x1*bs2+x)]-128);
    Fast_DCT(data2,bs2,bs2,1);
/*    if (i == 1)
    {
      FILE *fptr1 = fopen ("tmp002.txt", "wt");
      for (y=0; y<bs2; y++)
        for (x=0; x<bs2; x++)
          if (y*bs != 0)
            fprintf(fptr1,"%f\n",data2[y+1][x+1]);
      fclose(fptr1);
    }
*/
    // Now, watermarking...
    for (y=0; y<bs2; y++)
    {
      for (x=0; x<bs2; x++)
      {
        NewX1=zigzag1[y*bs2+x].col+1; NewY1=zigzag1[y*bs2+x].row+1;
        NewX2=zigzag2[y*bs2+x].col+1; NewY2=zigzag2[y*bs2+x].row+1;
        data3[NewY1][NewX1] += (data2[NewY2][NewX2] * alpha);
/*        if ((y*bs2+x) == 0)
          avg += data2[NewY2][NewX2];
        else if ((y*bs2+x) != 0)
          data3[NewY1][NewX1] += (data2[NewY2][NewX2]*alpha);
*/
      }
    }
    Fast_DCT(data3,bs,bs,-1);

    for (y=0; y<bs; y++)
      for (x=0; x<bs; x++)
        p1[(sy+y0*bs+y)*w1+(sx+x0*bs+x)] = (Byte)(data3[y+1][x+1]+128);
  }
  avg /= bn;
  free_matrix(data2,1,bs2,1,bs2);
  free_matrix(data3,1,bs,1,bs);
  return avg;
}
//---------------------------------------------------------------------------

