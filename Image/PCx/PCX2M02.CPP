// Program : PCX2M02.CPP -- PCX convert into Metaform
// Author  : Avatar
// Date    : 98.06.16

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <flips.h>
#include <pcx.h>

void Detect_Image_Mode(void);
int Decide_PCX_Mode(void);
void Write_Meta_Prefix(int Mode);
void Encode_PCX_BW(void);
void Encode_16Color(void);
void Encode_256Color(void);
void Encode_TrueColor(void);

FILE *fin, *fout;
PCX_Header Header;
META_PREFIX Prefix;
COLOR_MAP ColorMap;
char cFilename[20], cInFile[20], cOutFile[20];
byte MetaData[1024*1024+769+128];
long lTotalBytes;

void main(void)
{
  clrscr();
  printf("PCX image format convert into Metaform.\n");

  printf("\nEnter PCX image format filename: ");
  scanf("%s", cInFile);  strcpy(cOutFile, cInFile);
  strcat(cInFile, ".pcx");
  if ((fin=fopen(cInFile, "rb")) == NULL)
  {
    printf("\nOpen %s fail...", cInFile);
    getch();
    exit(1);
  }

  strcat(cOutFile, ".mta");
  if ((fout=fopen(cOutFile, "wb")) == NULL)
  {
    printf("\nOpen %s fail...", cOutFile);
    getch();
    exit(1);
  }

  fread(&Header,sizeof(Header),1,fin);  // Read image header
  Detect_Image_Mode();                  // Detect Image format.

  int iPCXMode = Decide_PCX_Mode();     // Decide PCX mode
  switch(iPCXMode)
  {
    case   M2: printf("\nNow, Convert B&W mode to Metaform, Waiting...");
               Write_Meta_Prefix(M2);   // Write Metaform Prefix
               Encode_PCX_BW();
               break;
    case M256: printf("\nThis image file is 256 grayscale mode.");
               Write_Meta_Prefix(M256); // Write Metaform Prefix
               Encode_256Color();
               break;
    case  C16: printf("\nThis image file is 16 color mode.");
               Write_Meta_Prefix(C16);   // Write Metaform Prefix
               Encode_16Color();
               break;
    case  CP : printf("\nThis image file is 256 color mode.");
               Write_Meta_Prefix(CP);   // Write Metaform Prefix
               Encode_256Color();
               break;
    case  CT : printf("\nThis image file is true color mode.");
               Write_Meta_Prefix(CT);   // Write Metaform Prefix
               Encode_TrueColor();
               break;
    default  : printf("\nNot support such that file.");
               getch();
               exit(1);
  }

  fclose(fin);
  fclose(fout);
  printf("\n\nPress any key to exit this program...");
  getch();
}

void Detect_Image_Mode()
{
  if (Header.Manufacturer != 0x0A) {
    printf("\nNot is PCX image file, Press any key exit this program...");
    getch();
    exit(1);
  }
}

int Decide_PCX_Mode()
{
  fseek(fin,0L,SEEK_SET);
  fread(&Header,sizeof(Header),1,fin);

  int iPCXMode;
  iPCXMode = Header.BitsPerPixel * 10 + Header.Planes;
  switch(iPCXMode)
  {
    case 11: return M2;
    case 14: return C16;
    case 81: if (Header.PaletteType == 0x01)
               return CP;
             else if (Header.PaletteType == 0x02)
               return M256;
    case 83: return CT;
    default: printf("\nI don't know what's this PCX Mode???");
             getch();
             return -1;
  }
}

void Write_Meta_Prefix(int Mode)
{
  fseek(fout,0L,SEEK_SET);
  Prefix.version = (byte)0x10;
  Prefix.mode = (byte)Mode;
  Prefix.id = (byte)(0xFF - Prefix.version - Prefix.mode);
  Prefix.width = (word)(Header.Xmax + 1);
  Prefix.height = (word)(Header.Ymax + 1);
  fwrite(&Prefix,sizeof(Prefix),1,fout);
}

void Encode_PCX_BW()
{
  fseek(fin,0L,SEEK_END);
  long lTotalBytes = ftell(fin) - 128;  // Calculator all bytes

  fseek(fin,128L,SEEK_SET);             // Goto Image data start byte

  int i, j, bit;
  byte buffer[2048];
  long lCnt = 0, lCnt1 = 0, lRepeatCount = 0;
  char cOldChar, cNewChar;
  while (lCnt < lTotalBytes)
  {
    cOldChar = (byte)fgetc(fin); lCnt++;
    if ((byte)cOldChar > 0xC0)
    {
      lRepeatCount = ((cOldChar - 0xC0) & 0xFF);
      cNewChar = (byte)fgetc(fin); lCnt++;
    }
    else if ((byte)cOldChar <= 0xC0)
    {
      lRepeatCount = 1;
      cNewChar = cOldChar;
    }

    for (i=0; i<lRepeatCount; i++)
    {
      for (j=0; j<8; j++)
      {
        bit = (cNewChar >>(7-j)) & 0x01;
        if (bit == 0x01)
           buffer[lCnt1] = 0x01;
        else if (bit == 0x0)
           buffer[lCnt1] = 0x0;
        lCnt1++;
        if (lCnt1 == Prefix.width)
        {
           fwrite(buffer,Prefix.width,1,fout);
           lCnt1 = 0;
        }
      }
    }
  }
}

void Encode_16Color()
{
  fputc(0x0F, fout);                    // write color size

  // Write Color Platte
  byte ColorPlatte[48];
  fseek(fin,0L,SEEK_SET);
  fread(&Header,sizeof(Header),1,fin);
  memcpy(ColorPlatte, Header.HeaderPalette, 48);
  fwrite(ColorPlatte,48,1,fout);

  fseek(fin,0L,SEEK_END);
  long lTotalBytes = ftell(fin) - 128;

  fseek(fin,128L,SEEK_SET);
  fseek(fout,65L,SEEK_SET);

  byte buffer[1024];                            // Default width is 2048 pixel
  byte cOld, cNew, cData;
  byte *ptr0, *ptr1, *ptr2, *ptr3;
  long lCnt = 0, lCnt1 = 0, lCnt2, lCnt3, lRepeatCnt;
  while (lCnt < lTotalBytes)
  {
    cOld = (byte)fgetc(fin); lCnt++;
    if (cOld > 0xC0)
    {
      lRepeatCnt = (cOld & 0xFF) - 0xC0;
      cNew = (byte)fgetc(fin); lCnt++;
    }
    else if (cOld <= 0xC0)
    {
      lRepeatCnt = 1;
      cNew = cOld;
    }

    for (int i=0; i<lRepeatCnt; i++)
    {
      buffer[lCnt1++] = cNew;
      if (lCnt1 == (Prefix.width / 8 * 4))
      {
        lCnt2 = 0;
        while (lCnt2 < (Prefix.width / 8))
        {
          ptr3 = &buffer[lCnt2];
          ptr2 = ptr3 + (Prefix.width / 8);
          ptr1 = ptr2 + (Prefix.width / 8);
          ptr0 = ptr1 + (Prefix.width / 8);
          lCnt3 = 0;
          while (lCnt3 < 8)
          {
            cData=(byte)(((*ptr3 & 0x80) >> 7) | ((*ptr2 & 0x80) >> 6) |
                         ((*ptr1 & 0x80) >> 5) | ((*ptr0 & 0x80) >> 4));
            fputc(cData, fout);
            *ptr3 <<= 1; *ptr2 <<= 1; *ptr1 <<= 1; *ptr0 <<= 1;
            lCnt3++;
          }
          ptr3++; ptr2++; ptr1++; ptr0++;
          lCnt2++;
        }
        lCnt1 = 0;
      }
    }
  }
}

void Encode_256Color()
{
  // Write color size
  fputc(0xFF, fout);

  // Write color palette
  fseek(fin,-768L,SEEK_END);
  fread(&ColorMap,3,256,fin);
  fwrite(&ColorMap,3,256,fout);

  fseek(fin,0L,SEEK_END);
  lTotalBytes = ftell(fin) - 128 - (768 + 1);
  fseek(fin,128L,SEEK_SET);

  byte cOld, cNew;
  long lCnt = 0, lCnt1 = 0, lRepeatCnt;
  while (lCnt < lTotalBytes)
  {
    cOld = (byte)fgetc(fin); lCnt++;
    if (cOld > 0xC0)
    {
      lRepeatCnt = (cOld & 0xFF) - 0xC0;
      cNew = (byte)fgetc(fin); lCnt++;
    }
    else if (cOld <= 0xC0)
    {
      lRepeatCnt = 1;
      cNew = cOld;
    }

    int i;
    for (i=0; i<lRepeatCnt; i++)
    {
      MetaData[lCnt1] = cNew;
      lCnt1++;
    }
  }
  fwrite(MetaData,lCnt1,1,fout);
//  printf("\nPCX raw data byte: %ld", lCnt1); getch();
}

void Encode_TrueColor()
{
  fputc(0xFF, fout);

  fseek(fin,0L,SEEK_END);
  long lTotalBytes = (ftell(fin)-128);

  fseek(fin,128L,SEEK_SET);
  fseek(fout,17L,SEEK_SET);

  byte cOld, cNew;
  byte buffer[2048*3];
  long lCnt1 = 0, lCnt2 = 0, lRepeatCnt;
  long lOffset = 0, lBytePosi = 0;
  while (lCnt1 < lTotalBytes)
  {
    cOld = (byte)fgetc(fin); lCnt1++;
    if (cOld > 0xC0)
    {
      lRepeatCnt = (cOld - 0xC0);
      cNew = (byte)fgetc(fin); lCnt1++;
    }
    else
    {
      lRepeatCnt = 1;
      cNew = cOld;
    }

    for (int i=0; i<lRepeatCnt; i++)
    {
      buffer[lBytePosi] = cNew;
      lCnt2++;
      lBytePosi+=3;
      if (((lCnt2%Prefix.width) == 0) && (lCnt2 != 0))
      {
        lOffset = (lOffset++) % 3;
        lBytePosi = lOffset;

      }
      if (((lCnt2%(Prefix.width*3)) == 0) && (lCnt2 != 0))
      {
        fwrite(buffer,Prefix.width*3,1,fout);
        lCnt2 = 0;
      }  
    }
  }
}


