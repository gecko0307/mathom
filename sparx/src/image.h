#ifndef __IMAGE_H__
#define __IMAGE_H__

struct image_t
{
  int width;
  int height;
  unsigned int channels;

  unsigned int format;
  int internalFormat;
  unsigned int id;

  unsigned char *texels;

  int numMipmaps;
};

struct color_t
{
  unsigned char r;
  unsigned char g;
  unsigned char b;
  unsigned char a;
};

#define GL_LUMINANCE                            0x1909
#define GL_LUMINANCE_ALPHA                      0x190A
#define GL_RGB                                  0x1907
#define GL_RGBA                                 0x1908

#endif
