// Image Structure
#include <conio.h>
#include <stdio.h>
#define MAXSIZE   512
#define BS          8

typedef struct {
  FILE *fptr;
  char name[81];
  int  Width;
  int  Height;
  unsigned char pixel[MAXSIZE*MAXSIZE];
} Image;
