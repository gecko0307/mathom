/*
 * Copyright (c) 2005-2007 David HENRY
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "image.h"

/* DirectDraw's structures */
struct DDPixelFormat
{
  unsigned int size;
  unsigned int flags;
  unsigned int fourCC;
  unsigned int bpp;
  unsigned int redMask;
  unsigned int greenMask;
  unsigned int blueMask;
  unsigned int alphaMask;
};

struct DDSCaps
{
  unsigned int caps;
  unsigned int caps2;
  unsigned int caps3;
  unsigned int caps4;
};

struct DDColorKey
{
  unsigned int lowVal;
  unsigned int highVal;
};

struct DDSurfaceDesc
{
  unsigned int size;
  unsigned int flags;
  unsigned int height;
  unsigned int width;
  unsigned int pitch;
  unsigned int depth;
  unsigned int mipMapLevels;
  unsigned int alphaBitDepth;
  unsigned int reserved;
  unsigned int surface;

  struct DDColorKey ckDestOverlay;
  struct DDColorKey ckDestBlt;
  struct DDColorKey ckSrcOverlay;
  struct DDColorKey ckSrcBlt;

  struct DDPixelFormat format;
  struct DDSCaps caps;

  unsigned int textureStage;
};

#define MAKEFOURCC(ch0, ch1, ch2, ch3) \
  (unsigned int)( \
    (((unsigned int)(unsigned char)(ch3) << 24) & 0xFF000000) | \
    (((unsigned int)(unsigned char)(ch2) << 16) & 0x00FF0000) | \
    (((unsigned int)(unsigned char)(ch1) <<  8) & 0x0000FF00) | \
     ((unsigned int)(unsigned char)(ch0)        & 0x000000FF) )

#define FOURCC_DXT1 MAKEFOURCC('D', 'X', 'T', '1')
#define FOURCC_DXT3 MAKEFOURCC('D', 'X', 'T', '3')
#define FOURCC_DXT5 MAKEFOURCC('D', 'X', 'T', '5')

#ifndef max
static int max (int a, int b)
{
  return ((a > b) ? a : b);
}
#endif

#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT   0x83F0
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT  0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT  0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT  0x83F3

int ImageDDSRead(const char *filename, struct image_t * texinfo)
{
  struct DDSurfaceDesc ddsd;
  //struct dds_texture_t *texinfo;
  FILE *fp;
  char magic[4];
  int mipmapFactor;
  long bufferSize, curr, end;

  /* Open the file */
  fp = fopen (filename, "rb");
  if (!fp)
    {
      fprintf (stderr, "error: couldn't open \"%s\"!\n", filename);
      return 0;
    }

  /* Read magic number and check if valid .dds file */
  fread (&magic, sizeof (char), 4, fp);

  if (strncmp (magic, "DDS ", 4) != 0)
    {
      fprintf (stderr, "the file \"%s\" doesn't appear to be"
	       "a valid .dds file!\n", filename);
      fclose (fp);
      return 0;
    }

  /* Get the surface descriptor */
  fread (&ddsd, sizeof (ddsd), 1, fp);

  /* texinfo = (struct dds_texture_t *)
    calloc (sizeof (struct dds_texture_t), 1); */
  texinfo->width = ddsd.width;
  texinfo->height = ddsd.height;
  texinfo->numMipmaps = ddsd.mipMapLevels;

  switch (ddsd.format.fourCC)
    {
    case FOURCC_DXT1:
      /* DXT1's compression ratio is 8:1 */
      texinfo->format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
      texinfo->internalFormat = 3;
      mipmapFactor = 2;
      break;

    case FOURCC_DXT3:
      /* DXT3's compression ratio is 4:1 */
      texinfo->format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
      texinfo->internalFormat = 4;
      mipmapFactor = 4;
      break;

    case FOURCC_DXT5:
      /* DXT5's compression ratio is 4:1 */
      texinfo->format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
      texinfo->internalFormat = 4;
      mipmapFactor = 4;
      break;

    default:
      /* Bad fourCC, unsupported or bad format */
      fprintf (stderr, "the file \"%s\" doesn't appear to be"
	       "compressed using DXT1, DXT3, or DXT5! [%i]\n",
	       filename, ddsd.format.fourCC);
      free (texinfo);
      fclose (fp);
      return 0;
    }

  /* Calculate pixel data size */
  curr = ftell (fp);
  fseek (fp, 0, SEEK_END);
  end = ftell (fp);
  fseek (fp, curr, SEEK_SET);
  bufferSize = end - curr;

  /* Read pixel data with mipmaps */
  texinfo->texels = (unsigned char *)malloc (bufferSize * sizeof (unsigned char));
  fread (texinfo->texels, sizeof (unsigned char), bufferSize, fp);

  /* Close the file */
  fclose (fp);
  /* return texinfo; */
  return 1;
}

