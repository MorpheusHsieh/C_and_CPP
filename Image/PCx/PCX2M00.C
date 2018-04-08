// Program : PCX2M.CPP
// Author  : Avatar
// Date    : 98.06.01

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <flips.h>
#include <pcx.h>

void fail(char *);
void Get_Pcx_Header(void);
void Put_Meta_Prefix(byte);
void Put_Meta_ColorMap(void);
void Decode_PCX11(void);
void Decode_PCX14(void);
void Decode_PCX81(void);
void Decode_PCX83(void);
void Get_A_Line(byte *buf, int total);
void Display_Header(PCX_Header);


FILE *fin, *fout;
int PCX_mode;
char fn1[20], infile[20], outfile[20];
COLOR_MAP color_map;
word image_width, image_height, bytes_per_line;

void main(void)
{
   clrscr();
   printf("PCX decoder, convert into metaform.\n");

   printf("\nEnter input filename (0-8 character): ");
   scanf("%s", fn1);
   strcpy(infile,  fn1); strcat(infile,  ".pcx");
   strcpy(outfile, fn1); strcat(outfile, ".mta");
   if ( (fin=fopen(infile, "rb"))==NULL ) {
      printf("\nErroe at open input filename...");
      getch();
      exit(1);
   }
   if ( (fout=fopen(outfile, "wb"))==NULL ) {
      printf("\nErroe at open output filename...");
      getch();
      exit(1);
   }

   Get_Pcx_Header();
   printf("\nNow, Wait for convert PCX format to META format...");

   switch(PCX_mode)
   {
      case PCX11: Put_Meta_Prefix(M2);
                  Decode_PCX11();
                  break;
      case PCX14: Put_Meta_Prefix(CP);
                  Put_Meta_ColorMap();
                  Decode_PCX14();
                  break;
      case PCX81: Put_Meta_Prefix(CP);
                  Put_Meta_ColorMap();
                  Decode_PCX81();
                  break;
      case PCX83: Put_Meta_Prefix(CT);
                  Put_Meta_ColorMap();
                  Decode_PCX83();
                  break;
      default:    printf("\nNot support such mode PCX: %x %x",
                       PCX_mode/10, PCX_mode%10);
   }
   fclose(fin);
   fclose(fout);

   printf("\n\nPress any key exit this program...");
   getch();
}

void fail(char *err_msg)
{
   printf(err_msg, stderr);
   getch();
   exit(1);
}

void Get_Pcx_Header(void)
{
   PCX_Header header;

   int i, buf[128];
   for (i=0; i<128; i++)
      buf[i] = fgetc(fin);

   header.Manufacturer = (byte)buf[0];
   header.VersionNo    = (byte)buf[1];
   header.Encoding     = (byte)buf[2];
   header.BitsPerPixel = (byte)buf[3];
   header.Xmin         = (short)((buf[5]<<8)|buf[4]);
   header.Ymin         = (short)((buf[7]<<8)|buf[6]);
   header.Xmax         = (short)((buf[9]<<8)|buf[8]);
   header.Ymax         = (short)((buf[11]<<8)|buf[10]);
   header.HResolution  = (short)((buf[13]<<8)|buf[12]);
   header.VResolution  = (short)((buf[15]<<8)|buf[14]);
   for (i=0; i<48; i++)
      header.HeaderPalette[i] = (byte)buf[i+16];
   header.reserved     = (byte)buf[64];
   header.Planes       = (byte)buf[65];
   header.BytesPerLine = (short)((buf[67]<<8)|buf[66]);
   header.PaletteType  = (short)((buf[69]<<8)|buf[68]);
   header.HScreenSize  = (short)((buf[71]<<8)|buf[70]);
   header.VScreenSize  = (short)((buf[73]<<8)|buf[72]);
   for (i=0; i<54; i++)
      header.filler[i] = (byte)buf[i+74];
//   fread(&header, sizeof(PCX_Header), 1, fin);

   if (header.Manufacturer != 0x0A)
      fail("Not a PCX file format.");

   PCX_mode = header.BitsPerPixel*10 + header.Planes;
//   Display_Header(header); getch();

   if (PCX_mode == PCX14 )              // 16 colors
   {
      color_map.size = 16;
      memcpy(color_map.color, header.HeaderPalette, 48);
   }
   else if (PCX_mode == PCX81)          // 256 colors
   {
      color_map.size = 256;
      fseek(fin, -768L, SEEK_END);      // go to palette position
      fread(color_map.color, 768, 1, fin);
      fseek(fin, 128L, SEEK_SET);       // back to end of header
   }
   image_width    = (short)(header.Xmax - header.Xmin + 1);
   image_height   = (short)(header.Ymax - header.Ymin + 1);
   bytes_per_line = (short)header.BytesPerLine;
}

void Put_Meta_Prefix(byte mode)
{
   META_PREFIX prefix;

   prefix.version  = 0x10;
   prefix.mode     = mode;
   prefix.id       = (byte)(0xFF - 0x10 - mode);
   prefix.width    = image_width;
   prefix.height   = image_height;
   memset(prefix.reserved, '\0', 9);
   fwrite(&prefix, sizeof(META_PREFIX), 1, fout);
}   

void Put_Meta_ColorMap(void)
{
   fputc((byte)(color_map.size-1), fout);
   fwrite(color_map.color, 3, color_map.size, fout);
}

void Decode_PCX11(void)
{
   byte buf[128], *x, remained_bits;
   int line, count;

   for (line=0; line<image_height; line++)
   {
      Get_A_Line(buf, bytes_per_line);
      x = buf;
      count = 0;
      remained_bits = 8;
      while (count < image_width)
      {
         fputc((*x * 0x80) >> 7, fout);
         count++;
         if (--remained_bits == 0)
         {
            x++;
            remained_bits = 8;
         }
         else
            *x = *x << 1;
      }
   }
}

void Decode_PCX14(void)         // 16 color mode
{
  byte buf[1024],*iptr,*rptr,*gptr,*bptr,remained_bits;
//  int  line,i,j,count;
  int line, count;

  for (line=0; line<image_height; line++)
  {
    Get_A_Line(buf,bytes_per_line*4);           // get 4 color plane data
    iptr = buf;
    rptr = iptr + bytes_per_line;
    gptr = rptr + bytes_per_line;
    bptr = gptr + bytes_per_line;
    count         = 0;
    remained_bits = 8;
    while (count < image_width)
    {
      fputc(((*iptr & 0x80) >> 7) | ((*rptr & 0x80) >> 6) |
            ((*gptr & 0x80) >> 5) | ((*bptr & 0x80) >> 4) ,stdout);
      count++;
      if (--remained_bits == 0)
      {
        iptr++; rptr++; gptr++; bptr++;
        remained_bits = 8;
      }
      else
      {
        *iptr = *iptr << 1; *rptr = *rptr << 1;
        *gptr = *gptr << 1; *bptr = *bptr << 1;
      }
    }
  }
}

void Decode_PCX81(void)
{
   byte buf[2048];
   int line;

   for (line=0; line<image_height; line++)
   {
      Get_A_Line(buf, bytes_per_line);
      fwrite(buf, bytes_per_line, 1, fout);
   }
}

void Decode_PCX83(void)         // true color mode
{
  byte buf[3*2048];
  int  line,i;

  for (line=0; line<image_height; line++)
  {
    Get_A_Line(buf,bytes_per_line*3);   // get 3 color plane data
    for (i=0; i<bytes_per_line; i++)
    {
      fputc(buf[i],stdout);
      fputc(buf[i+bytes_per_line],stdout);
      fputc(buf[i+2*bytes_per_line],stdout);
    }
  }
}

void Get_A_Line(byte *buf, int total)
{
   int count = 0, repeat_count, i;
   byte x;

   while (count < total)
   {
      x = (byte)fgetc(fin);
      if (x >= 0xC0)
      {
         repeat_count = x & 0x3F;
         x = (byte)fgetc(fin);
         for (i=0; i<repeat_count; i++)
            buf[count++] = x;
      }
      else
         buf[count++] = x;
   }
}

void Display_Header(PCX_Header header)
{
   int i;
   printf("\nManufacturer  : %02x", header.Manufacturer);
   printf("\nVersion No    : %02x", header.VersionNo);
   printf("\nEnCode        : %02x", header.Encoding);
   printf("\nBitS Per Pixel: %02x", header.BitsPerPixel);
   printf("\nXmin          : %02x", header.Xmin);
   printf("\nYmin          : %02x", header.Ymin);
   printf("\nXmax          : %02x", header.Xmax);
   printf("\nYmax          : %02x", header.Ymax);
   printf("\nHResolution   : %02x", header.HResolution);
   printf("\nVResolution   : %02x", header.VResolution);

   printf("\nHeader Palette: ");
   for (i=0; i<48; i++) {
      if (((i%16)==0) && (i!=0)) printf("\n\t\t");
      printf("%02x ", header.HeaderPalette[i]);
   }

   printf("\nreserved      : %02x", header.reserved);
   printf("\nPlanes        : %02x", header.Planes);
   printf("\nBytes per Line: %02x", header.BytesPerLine);
   printf("\nPalette Type  : %02x", header.PaletteType);
   printf("\nHScreen Size  : %02x", header.HScreenSize);
   printf("\nVScreen Size  : %02x", header.VScreenSize);

   printf("\nfiller        : ");
   for (i=0; i<54; i++) {
      if (((i%16)==0) && (i!=0)) printf("\n\t\t");
      printf("%02x ", header.filler[i]);
   }
}

