// Input image information
#include "ImgStrut.h"
#include <string.h>

int Input_Image_Info(Image *img, int sign)
{
  printf("\nEnter filename (Raw File): ");
  scanf("%s", img->name);
  strcat(img->name, ".raw");

  int size;
  printf("Enter image size: ");
  scanf("%d", &size);
  img->Width = size;
  img->Height = size;

  switch(sign)
  {
    case  1: if ((img->fptr=fopen(img->name, "rb")) == NULL)
             {
               printf("\nOpen %s failure...", img->name);
               getch();
               return 0;
             }
             else
             {
               fread(img->pixel,size,size,img->fptr);
               fclose(img->fptr);
             }
             break;
    case  2: if ((img->fptr=fopen(img->name, "wb")) == NULL)
             {
               printf("\nOpen %s failure...", img->name);
               getch();
               return 0;
             }
             break;
    default: printf("\nError!!"); getch();
  }
  return 1;
}
