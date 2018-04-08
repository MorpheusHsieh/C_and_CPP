// Program : ZigZag.CPP -- AC components zig-zag order of JPEG compression
// Author  : Avatar
// Date    : 98.08.06
#include <stdio.h>

typedef struct ZIGZAG {
        int row;
        int col;
        } ZigZag;

void Zig_Zag(ZigZag *zz, int size)
{
  int i, j;
  int x = 0, y = 0, taggle = 1; // 1:Right, 2:Left-Down, 3:Down, 4:Right-Up
  long offset;
  for (i=0; i<size; i++)
  {
    for (j=0; j<size; j++)
    {
      offset = i*size + j;
      zz[offset].row = x;
      zz[offset].col = y;
      switch(taggle)
      {
        case 1: y++;
                taggle = 2;
                break;
        case 2: x++; y--;
                (y == 0) ? taggle = 3 : taggle = 2;
                if (x == size) {
                  x--; y+=2; taggle = 4;
                }
                break;
        case 3: x++;
                if (x == size) {
                  x--; y++;
                }
                taggle = 4;
                break;
        case 4: x--; y++;
                (x == 0) ? taggle = 1 : taggle = 4;
                if (y == size) {
                  x += 2; y--; taggle = 2;
                }
                break;
        default: printf("\nUnknow operator...");
      }
    }
  }
}
