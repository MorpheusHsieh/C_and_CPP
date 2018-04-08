// Program : Bin2ASC.CPP
// Author  : Avatar
// Date    : 98.09.21
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

void main(void)
{
  clrscr();
  printf("Convert Binary code to ASCII code.\n");

  FILE *fin, *fout;
  char infile[81], outfile[81];
  printf("\nEnter input file (Raw File): ");
  scanf("%s", infile);
  strcpy(outfile, infile);
  strcat(infile, ".raw"); strcat(outfile, ".txt");
  if ((fin=fopen(infile, "rb")) == NULL)
  {
    printf("\nOpen %s fail...", infile);
    getch();
    exit(1);
  }

  if ((fout=fopen(outfile, "wb")) == NULL)
  {
    printf("\nOpen %s fail...", outfile);
    getch();
    exit(1);
  }

  int size;
  printf("\nEnter size of image: ");
  scanf("%d", &size);

  unsigned char *Pixel = new unsigned char[size*size];
  fread(Pixel,size,size,fin);
  fclose(fin);

  for (int i=0; i<size; i++)
  {
    for (int j=0; j<size; j++)
      fprintf(fout, "%d,\n", Pixel[i*size+j]);
  }
  fclose(fout);

  printf("\nPress any key to exit program...");
  getch();
}




