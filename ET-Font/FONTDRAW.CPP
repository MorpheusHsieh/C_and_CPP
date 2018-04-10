#include <io.h>
#include <conio.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main(void)
{
   int i, j=0;
   int Handle, Hex_code;
   char one_char, bytes[65];
   int four_bits[4]={ 8, 4, 2, 1 };
   bytes[65]=NULL;

   clrscr();
   Handle=open("c:\\temp\\fontdata.txt", O_RDONLY);
   for (i=0; i<96; i++) {
      read(Handle, &one_char, 1);
      if (one_char>='0' && one_char<='9')
	 bytes[j++]=one_char-'0';
      else if (one_char>='A' && one_char<='F')
	 bytes[j++]=one_char-'A'+10;
      else if (one_char>='a' && one_char<='f')
	 bytes[j++]=one_char-'a'+10;
   }
   for (i=0; i<64; i++) {
      for (j=0; j<4; j++)
	 if (bytes[i] & four_bits[j])
	    printf("\333\333");
	 else
	    printf("\260\260");
      if (i%4==3)
	 printf("\n");

   }
   getch();
   close(Handle);
}