// Image.CPP     // Image class methods
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <iostream.h>
#include "Image.h"

// constructors
Image::Image(char *fn, int size)        // default constructor
{
  strcpy(Name, fn);
  Size = size;
  Pixel = new unsigned char [Size*Size];
}

// class destructor
Image::~Image()                         // class destructor
{
  delete [] Pixel;
}

// other method
void Image::Show() const
{
  cout << "\nName: " << Name;
  cout << "\nSize: " << Size << "\n";
}

void Image::Read_From_File()
{
  if ((fptr=fopen(Name,"rb")) != NULL) {
    fread(Pixel,Size,Size,fptr);
    fclose(fptr);
  }
  else {
    cout << "\nOpen " << Name << " failure...";
    getch();
  }
}

void Image::Write_To_File()
{
  if ((fptr=fopen(Name,"wb")) != NULL) {
    fwrite(Pixel,Size,Size,fptr);
    fclose(fptr);
  }
  else {
    cout << "\nOpen " << Name << " failure...";
    getch();
  }
}

void Image::Equal(Image img)
{
  int i, j;
  for (i=0; i<Size; i++)
    for (j=0; j<Size; j++)
      Pixel[i*Size+j] = img.Pixel[i*Size+j];
}

void Image::Read_Pixel(unsigned char *pixel)
{
  int i, j;
  for (i=0; i<Size; i++)
    for (j=0; j<Size; j++)
      pixel[i*Size+j] = Pixel[i*Size+j];
}

void Image::Write_Pixel(unsigned char *pixel)
{
  int i, j;
  for (i=0; i<Size; i++)
    for (j=0; j<Size; j++)
      Pixel[i*Size+j] = pixel[i*Size+j];
}

