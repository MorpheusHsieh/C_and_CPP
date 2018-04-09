// Program : EnPCX83.cpp -- Convert Meta format into true color PCX format
// Author  : Morpheus
// Date    : 98.06.04

#include <m2pcx01.h>

void encode_PCX83(FILE *fin, FILE *fout)
{
  PCX_Header header;
  int i,j,k;
  byte buf[2048*3],t[3];

  header.Manufacturer = 0x0A;
  header.VersionNo = 5;
  header.Encoding = 1;
  header.BitsPerPixel = 8;
  header.Xmin = 0;
  header.Ymin = 0;
  header.Xmax = (word)(image_width - 1);
  header.Ymax = (word)(image_height - 1);
  header.HResolution = 0;
  header.VResolution = 0;
  memset(header.HeaderPalette,0,48);
  header.reserved = 0;
  header.Planes = 3;
  header.BytesPerLine = (word)image_width;
  header.PaletteType = 1;
  header.HScreenSize = 0;
  header.VScreenSize = 0;
  memset(header.filler,0,54);
  header.filler[0] = prefix.reserved[0];
  header.filler[1] = prefix.reserved[1];
  header.filler[2] = prefix.reserved[2];
  header.filler[3] = prefix.reserved[3];
  fwrite(&header,sizeof(PCX_Header),1,fout);

  fseek(fin, 17L, SEEK_SET);
  for (i=0; i<image_height; i++)
  {
    for (j=0; j<image_width; j++)
    {
      fread(t,3,1,fin);
      for (k=0; k<3; k++)
        buf[k*image_width+j] = t[k];
    }
    put_a_line(buf,image_width*3);
  }
}