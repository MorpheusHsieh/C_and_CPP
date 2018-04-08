// Program : BMP.H
// Author  : Avatar
// Date    : 98.07.03

// define header
typedef struct {
  word  bfType;
  dword bfSize;
  word  bfReserved1;
  word  bfReserved2;
  dword  bfOffBits;
} Bitmap_File_Header;

// define bmp information
typedef struct {
  dword biSize;
  dword biWidth;
  dword biHeight;
  word  biPlanes;
  word  biBitCount;
  dword biCompression;
  dword biSizeImage;
  dword biXPerMeter;
  dword biYPerMeter;
  dword biClrUsed;
  dword biClrImportant;
} Bitmap_Info_Header;

// constant value of biCompression
#define BI_RGB  0L
#define BI_RLE8 1L
#define BI_RLE4 2L

// Color table entry
typedef struct {
  byte rgbBlue;
  byte rgbGreen;
  byte rgbRed;
  byte rgbReserved;
} RGBQUAD;

