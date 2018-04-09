// Program : B&W PCX format convert into Metaform
// Author  : Morpheus
// Date    : 98.06.16

FILE *fin, *fout;

void Encode_PCX_BW(FILE *fin, FILE *fout)
{
  printf("\nNow, Convert B&W mode to Metaform, Waiting...");

  fseek(fin,0L,SEEK_END);
  long lTotalBytes = ftell(fin);        // Calculator all bytes

  fseek(fin,128L,SEEK_SET);             // Goto Image data start byte

  int i, j, bit;
  long lCount = 0, lRepeatCount = 0;
  char cOldChar, cNewChar;
  while (lCount < lTotalBytes)
  {
    cOldChar = (byte)fgetc(fin);
    if ((byte)cOldChar > 0xC0)
    {
      lRepeatCount = ((cOldChar - 0xC0) & 0xFF);
      cNewChar = (byte)fgetc(fin);
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
           fputc(0x01, fout);
        else if (bit == 0x0)
           fputc(0x0, fout);
      }
    }
  lCount ++;
  }
}