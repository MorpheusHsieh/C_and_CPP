// Image.h
#ifndef _IMAGE_H_
#define _IMAGE_H_

class Image
{
  private:
    FILE *fptr;
    char Name[81];
    long Size;
    unsigned char *Pixel;
  public:
    Image(char *fn, int size);
    ~Image();
    void Show() const;
    void Read_From_File();
    void Write_To_File();
    void Equal(Image img);
    void Read_Pixel(unsigned char *pixel);
    void Write_Pixel(unsigned char *pixel);
};
#endif




