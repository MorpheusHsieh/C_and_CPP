// Program : M2PCX01.CPP -- Convert Meta format to PCX format
// Author  : Morpheus
// Date    : 98.06.01

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <flips.h>
#include <pcx.h>

void fail(char *err_msg);
void Get_Meta_Prefix(void);
void encode_PCX11(void);
void encode_PCX14(void);
void encode_PCX81(void);
void encode_PCX83(void);
void put_a_line(byte *buf,int total);

FILE *fin, *fout;
META_PREFIX prefix;
COLOR_MAP color_map;
int META_mode, PaletteSize;
char infile[20], outfile[20];
long lTotalBytes;
byte MetaData[1024*1024+769+128];
word image_width, image_height, bytes_per_line;

void main(void)
{
   clrscr();
   printf("PCX encoder, convert metaform into PCX.\n");

   printf("\nEnter input filename (0-8 character): ");
   scanf("%s", infile); stpcpy(outfile, infile);
   strcat(infile,  ".mta");
   if ( (fin=fopen(infile, "rb"))==NULL ) {
      printf("\nErroe at open input filename...");
      getch();
      exit(1);
   }

   printf("\nEnter output filename (0-8 character): ");
   scanf("%s", outfile);
   strcat(outfile, ".pcx");
   if ( (fout=fopen(outfile, "wb"))==NULL ) {
      printf("\nErroe at open output filename...");
      getch();
      exit(1);
   }

   Get_Meta_Prefix();
   printf("\nNow, Wait for convert META into PCX...");

   switch(META_mode)
   {
      case M2  : encode_PCX11();
                 break;
      case C16 :
      case M256:
      case CP  : PaletteSize = fgetc(fin)+ 1;
                 if (PaletteSize <= 16)
                    encode_PCX14();
                 else
                    encode_PCX81();
                 break;
      case CT  : encode_PCX83();
                 break;
   }
   fclose(fin);
   fclose(fout);

   printf("\n\nPress any key exit this program...");
   getch();
}

void fail(char *err_msg)
{
   fputs(err_msg, stderr);
   exit(1);
}

void Get_Meta_Prefix(void)
{
   fread(&prefix, sizeof(META_PREFIX), 1, fin);

   if ((prefix.version + prefix.mode + prefix.id) != 0xFF)
      fail("Not A Metaform!!!");

   if ((prefix.mode != M2) && (prefix.mode != CP) && (prefix.mode != CT) &&
       (prefix.mode != M256) && (prefix.mode != C16))
      fail("M2PCX supports M2, CP, CT, M256 mode METAFORM only!!!");

   META_mode    = prefix.mode;
   image_width  = prefix.width;
   image_height = prefix.height;
}

void encode_PCX11(void)
{
  PCX_Header header;
  int i,j,k;
  byte buf[2048];

  header.Manufacturer = 0x0A;
  header.VersionNo = 5;
  header.Encoding = 1;
  header.BitsPerPixel = 1;
  header.Xmin = 0;
  header.Ymin = 0;
  header.Xmax = (word)(image_width - 1);
  header.Ymax = (word)(image_height - 1);
  header.HResolution = 0;
  header.VResolution = 0;
  memset(header.HeaderPalette,0,48);
  header.reserved = 0;
  header.Planes = 1;
  header.BytesPerLine = (short)((image_width + 7)>>3);
  header.PaletteType = 1;
  header.HScreenSize = 0;
  header.VScreenSize = 0;
  memset(header.filler,0,54);
  fwrite(&header, sizeof(PCX_Header), 1, fout);

  for (i=0; i<image_height; i++)
  {
    fread(buf,image_width,1,fin);
    for (j=0; j<header.BytesPerLine; j++)
    {
      buf[j] = buf[j*8];
      for (k=1; k<8; k++)
        buf[j] =(byte)((buf[j] << 1) | buf[j*8+k]);
    }
    put_a_line(buf,header.BytesPerLine);
  }
}

void encode_PCX14(void)
{
  COLOR_INDEX color_map[16];
  PCX_Header header;
  int i,j,k,byte_count,bit_count;
  byte buf1[2048],buf2[2048];

  fread(color_map,3,PaletteSize,fin);
  header.Manufacturer = 0x0A;
  header.VersionNo = 5;
  header.Encoding = 1;
  header.BitsPerPixel = 1;
  header.Xmin = 0;
  header.Ymin = 0;
  header.Xmax = (short)(image_width - 1);
  header.Ymax = (short)(image_height - 1);
  header.HResolution = 0;
  header.VResolution = 0;
  memcpy(header.HeaderPalette,color_map,48);
  header.reserved = 0;
  header.Planes = 4;
  header.BytesPerLine = (short)((image_width + 7) >> 3);
  header.PaletteType = 1;
  header.HScreenSize = 0;
  header.VScreenSize = 0;
  memset(header.filler,0,54);
  fwrite(&header,sizeof(PCX_Header),1,fout);

  for (i=0; i<image_height; i++)
  {
    fread(buf1,image_width,1,fin);
    byte_count = bit_count = 0;
    buf2[0] = 0;
    for (j=0; j<4; j++)
    {
      for (k=0; k<image_width; k++)
      {
        buf2[byte_count] =(byte)(buf2[byte_count] | (buf1[k] & 0x01));
        buf1[k] =(byte)(buf1[k] >> 1);
        bit_count++;
        if (bit_count == 8)
        {
          buf2[++byte_count] = 0;
          bit_count=0;
        }
        else
          buf2[byte_count] =(byte)(buf2[byte_count] << 1);
      }
      if (bit_count != 0)
      {
        buf2[byte_count]   =(byte)(buf2[byte_count] << (7-bit_count));
        buf2[++byte_count] = 0;
        bit_count=0;
      }
    }
    put_a_line(buf2,byte_count);
  }
}

void encode_PCX81(void)
{
  COLOR_INDEX color_map[256];
  PCX_Header header;
//  int i;
//  byte buf[2048];

  header.Manufacturer = 0x0A;
  header.VersionNo    = 5;
  header.Encoding     = 1;
  header.BitsPerPixel = 8;
  header.Xmin         = 0;
  header.Ymin         = 0;
  header.Xmax         = (word)(image_width - 1);
  header.Ymax         = (word)(image_height - 1);
  header.HResolution  = 0x00;
  header.VResolution  = 0x00;
  memset(header.HeaderPalette,0,48);
  header.reserved     = 0x00;
  header.Planes       = 0x01;
  header.BytesPerLine = (word)image_width;
  header.PaletteType  = 1;
  header.HScreenSize  = 0;
  header.VScreenSize  = 0;
  memset(header.filler,0,54);
  fwrite(&header, sizeof(PCX_Header),1,fout);

  fread(color_map,3,PaletteSize,fin); // read color map in

  fseek(fin,0L,SEEK_END);
  lTotalBytes = ftell(fin) - 769 - 16;
  fseek(fin,785L,SEEK_SET);
  fread(MetaData,lTotalBytes,1,fin);

  char cOld, cNew;
  long lCnt = 0, lRepeatCnt = 1;
  cOld = MetaData[lCnt++];
  while (lCnt <= lTotalBytes)
  {
    cNew = MetaData[lCnt++];
    while (cOld == cNew)
    {
      lRepeatCnt++;
      cNew = MetaData[lCnt++];
    }

    if (lRepeatCnt == 1)
    {
      if ((cOld & 0xFF) < 0xC0)
        fputc(cOld, fout);
      else
      {
        fputc(0xC1, fout);
        fputc(cOld, fout);
      }
    }
    else if (lRepeatCnt > 1)
    {
      if (lRepeatCnt <= 63)
      {
        fputc(0xC0+lRepeatCnt, fout);
        fputc(cOld, fout);
      }
      else if (lRepeatCnt > 63)
      {
        while (lRepeatCnt > 63)
        {
          lRepeatCnt -= 63;
          fputc(0xFF, fout);
          fputc(cOld, fout);
        }
        fputc(0xC0+lRepeatCnt, fout);
        fputc(cOld, fout);
      }
    }
    cOld = cNew; lRepeatCnt = 1;
  }
/*  for (i=0; i<image_height; i++)
  {
    fread(buf,image_width,1,fin);
    put_a_line(buf,image_width);
  }*/

  // Write Color Map
  fputc(0x0C,fout);
  fwrite(color_map,3,256,fout);       // write color map out
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

void put_a_line(byte *buf,int total)
{
  int count,repeat_count,i;

  buf[total] =(byte)(buf[total-1]+1);
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
          fputc(0xFF,fout);           // repeat 63 times
          fputc(buf[count-1],fout);
        }
        fputc(0xC0 + repeat_count % 63,fout);
        fputc(buf[count-1],fout);
        repeat_count = 1;
      }
      else
      {
        if (buf[count-1] >= 0xC0)
          fputc(0xC1,fout);
        fputc(buf[count-1],fout);
      }
    }
    count++;
  }
}

