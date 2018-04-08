/* =======================================================================
		PCX.H
		Created by Hwang Jiunn-Ming   APR-23-93
		Last modified date	      APR-23-93
   ======================================================================= */

#define PCX11	11
#define PCX14	14
#define PCX81	81
#define PCX83	83

typedef struct
{
  byte Manufacturer;
  byte VersionNo;
  byte Encoding;
  byte BitsPerPixel;
  word Xmin;
  word Ymin;
  word Xmax;
  word Ymax;
  word HResolution;
  word VResolution;
  byte HeaderPalette[48];
  byte reserved;
  byte Planes;
  word BytesPerLine;
  word PaletteType;
  word HScreenSize;
  word VScreenSize;
  byte filler[54];
} PCX_Header;

