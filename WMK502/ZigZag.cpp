//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <stdio.h>

#include "ZigZag.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
void Zig_Zag(ZigZag *zz, long width, long height)
{
  int i, j;
  int x = 0, y = 0, taggle = 1; // 1:Right, 2:Left-Down, 3:Down, 4:Right-Up
  long offset;
  for (i=0; i<height; i++)
  {
    for (j=0; j<width; j++)
    {
      offset = i*width + j;
      zz[offset].row = x;
      zz[offset].col = y;
      switch(taggle)
      {
        case 1: y++;
                taggle = 2;
                break;
        case 2: x++; y--;
                (y == 0) ? taggle = 3 : taggle = 2;
                if (x == height) {
                  x--; y+=2; taggle = 4;
                }
                break;
        case 3: x++;
                if (x == height) {
                  x--; y++;
                }
                taggle = 4;
                break;
        case 4: x--; y++;
                (x == 0) ? taggle = 1 : taggle = 4;
                if (y == width) {
                  x += 2; y--; taggle = 2;
                }
                break;
        default: printf("\nUnknow operator...");
      }
    }
  }
}
//---------------------------------------------------------------------------
