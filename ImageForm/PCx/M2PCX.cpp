// Program: M2PCX,C
// Author : Avatar
// Date   : 98.05.27

#include <stdio.h>
#include <stdlib.h>
#include <mem.h>
#include <io.h>
#include <fcntl.h>
#include <d:\trash\image\flips.h>       // ?
#include <color.h>       // ?
#include <pcx.h>         // ?

void main(void);
void fail(char*);
void get_META_prefix(void);
void put_PCX_Header(void);
void encode_PCX11(void);
void encode_PCX14(void);
void encode_PCX81(void);
void encode_PCX83(void);
void put_a_line(byte*, int);

word image_width, image_height, bytes_per_line;
COLOR_MAP color_map;
int META_mode, PaletteSize;

void main(void)
{
   setmode(fileno(stdin), O_BINARY);        // set bi\oth stdin and stdout
   setmode(fileno(stdout), O_BINARY);       // into binary mode


   get_META_prefix();
   switch (META_mode)
   {
      case M2: encode_PCX11();
	       break;
      case CP: PaletteSize = fgetc(stdin) + 1;
	       if (PaletteSize <= 16)
		  encode_PCX14();
	       else
		  encode_PCX81();
	       break;
      case CT: encode_PCX83();
	       break;
   }
   fclose(stdin);
}

void fail(char *err_mg)
{
   fputs(err_msg, stderr);
   exit(1);
}

void get_META_prefix(void)
{
   META_PREFIX prefix;

   fread(&prefix, sizeof(META_PREFIX), 1, stdin);

   if ((prefix.version + prefix.mode + prefix.id != 0xff)
      fail("Not A METAFORM!!!");

   if ((prefix.mode != M@) && (prefix.mode != Cp) && (prefix.mode != CT))
      fail("M2PCX supports M@, Cp, CT mode METAFORM only!!!");

   META_mode    = prefix,mode;
   image_width  = prefix.width;
   image_height = prefix.height;
}

void encode_PCX11(void)
{
   PCX_Header header;
   int i, j, k;
   byte buf[2048];
   header.Manufacturer = 0x0A;
   header.VersionNo = 5;
   header.Encoding = 1;
   header.BitsPerPixel = 1;
   header.Xmin = 0;
   header.Ymin = 0;
   header.Xmax = image.width - 1;
   header.Ymax = image.height - 1;
   header.HResolution = 0;
   header.VResolution = 0;
   memset(header.HeaderPalette, 0, 48);
   header.reserved = 0;
   header.Planes = 1;
   header.BytesPerLine = (image_Width + 7) >> 3;
   header.PaletteType = 1;
   header.HScreenSize = 0;
   header.VScreenSize = 0;
   memset(header.fiier, 0, 54);
   fwrite(&header, sizeof(PCX_Header), 1, stdout);

   for (i=0; i<image_height; i++)
   {
      fread(buf, image_width, 1, stdin);
      for (j=0; j<header.BytesPerLine; j++)
      {
	 buf[j] = buf[j*8];
	 for (k=1; k<8; k++)
	    buf[j] = (buf[j]<<1) | buf[j*8+k];
      }
      put_a_line(buf, header, BytesperLine);
   }
}

void encode_PCX14(void)
{
   COLOR_INDEX color_map[16];
   PCX_Header header;
   int i, j, k, byte_count, bit_count;
   byte buf1[2048], buf2[2048];

   fread(color_map, 3, PaletteSize, stdin);
   header.Manufacturer = 0x0A;
   header.VersionNo = 5;
   header.Encoding = 1;
   header.BitsPerPixel = 1;
   header.Xmin = 0;
   header.Ymin = 0;
   header.Xmax = image.width - 1;
   header.Ymax = image.height - 1;
   header.HResolution = 0;
   header.VResolution = 0;
   memset(header.HeaderPalette, color_map, 48);
   header.reserved = 0;
   header.Planes = 4;
   header.BytesPerLine = (image_Width + 7) >> 3;
   header.PaletteType = 1;
   header.HScreenSize = 0;
   header.VScreenSize = 0;
   memset(header.fiier, 0, 54);
   fwrite(&header, sizeof(PCX_Header), 1, stdout);

   for (i=0; i<image_height; i++)
   {
      fread(buf1, image_width, 1, stdin);
      byte_count = bit_count = 0;
      buf2[0] = 0;
      for (j=0; j<4; j++)
      {
	 for (k=0; k<image_width; k++)
	 {
	    buf2[byte_count] = buf2[byte_count] | (buf1[k] & 0x01);
	    buf1[k] = buf1[k] >> 1;
	    bit_count++;
	    if (bit_count == 8)
	    {
	       buf2[++byte_count] = 0;
	       bit_count = 0;
	    }
	    else
	       buf2[byte_count] = buf2[byte_count] << 1;
	 }
	 if (bit_count != 0)
	 {
	    buf2[byte_count] = buf2[byte_count] << (7-bit_count);
	    buf2[++byte_count] = 0;
	    bit_count = 0;
	 }
      }
      put_a_line(buf2, byte_count);
   }
}

void encode_PCX81(void)
{
  COLOR_INDEX color_map[256];
  PCX_Header header;
  int i;
  byte buf[2048];

  header.Manufacturer = 0x0A;
  header.VersionNo = 5;
  header.Encoding = 1;
  header.BitsPerPixel = 8;
  header.Xmin = 0;
  header.Ymin = 0;
  header.Xmax = image_width - 1;
  header.Ymax = image_height - 1;
  header.HResolution = 0;
  header.VResolution = 0;
  memset(header.HeaderPalette,0,48);
  header.reserved = 0;
  header.Planes = 1;
  header.BytesPerLine = image_width;
  header.PaletteType = 1;
  header.HScreenSize = 0;
  header.VScreenSize = 0;
  memset(header.filler,0,54);
  fwrite(&header,sizeof(PCX_Header),1,stdout);

  fread(color_map,3,PaletteSize,stdin); // read color map in
  for (i=0; i<image_height; i++)
  {
    fread(buf,image_width,1,stdin);
    put_a_line(buf,image_width);
  }
  fputc(0x0C,stdout);
  fwrite(color_map,3,256,stdout);       // write color map out
}

void encode_PCX83(void)
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
  header.Xmax = image_width - 1;
  header.Ymax = image_height - 1;
  header.HResolution = 0;
  header.VResolution = 0;
  memset(header.HeaderPalette,0,48);
  header.reserved = 0;
  header.Planes = 3;
  header.BytesPerLine = image_width;
  header.PaletteType = 1;
  header.HScreenSize = 0;
  header.VScreenSize = 0;
  memset(header.filler,0,54);
  fwrite(&header,sizeof(PCX_Header),1,stdout);

  for (i=0; i<image_height; i++)
  {
    for (j=0; j<image_width; j++)
    {
      fread(t,3,1,stdin);
      for (k=0; k<3; k++)
        buf[k*image_width+j] = t[k];
    }
    put_a_line(buf,image_width*3);
  }
}

void put_a_line(byte *buf,int total)
{
  int count,repeat_count,i;

  buf[total] = buf[total-1]+1;
  count = 1; repeat_count = 1;
  while (count <= total)
  {
    if (buf[count] == buf[count-1])
      repeat_count++;
    else
    {
      if (repeat_count >= 2)
      {
        for (i=0; i<repeat_count/63; i++)
        {
          fputc(0xFF,stdout);           // repeat 63 times
          fputc(buf[count-1],stdout);
        }
        fputc(0xC0 + repeat_count % 63,stdout);
        fputc(buf[count-1],stdout);
        repeat_count = 1;
      }
      else
      {
        if (buf[count-1] >= 0xC0)
          fputc(0xC1,stdout);
        fputc(buf[count-1],stdout);
      }
    }
    count++;
  }
}

