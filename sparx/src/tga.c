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

#include "image.h"

#pragma pack(push, 1)
/* TGA header */
struct tga_header_t
{
  unsigned char id_lenght;          /* size of image id */
  unsigned char colormap_type;      /* 1 is has a colormap */
  unsigned char image_type;         /* compression type */

  short	cm_first_entry;       /* colormap origin */
  short	cm_length;            /* colormap length */
  unsigned char cm_size;            /* colormap size */

  short	x_origin;             /* bottom left x coord origin */
  short	y_origin;             /* bottom left y coord origin */

  short	width;                /* picture width (in pixels) */
  short	height;               /* picture height (in pixels) */

  unsigned char pixel_depth;        /* bits per pixel: 8, 16, 24 or 32 */
  unsigned char image_descriptor;   /* 24 bits = 0x00; 32 bits = 0x80 */
};
#pragma pack(pop)

void GetTextureInfo (const struct tga_header_t *header, struct image_t *texinfo)
{
  texinfo->width = header->width;
  texinfo->height = header->height;

  switch (header->image_type)
    {
    case 3:  /* Grayscale 8 bits */
    case 11: /* Grayscale 8 bits (RLE) */
      {
	if (header->pixel_depth == 8)
	  {
	    texinfo->format = GL_LUMINANCE;
	    texinfo->internalFormat = 1;
	  }
	else /* 16 bits */
	  {
	    texinfo->format = GL_LUMINANCE_ALPHA;
	    texinfo->internalFormat = 2;
	  }

	break;
      }

    case 1:  /* 8 bits color index */
    case 2:  /* BGR 16-24-32 bits */
    case 9:  /* 8 bits color index (RLE) */
    case 10: /* BGR 16-24-32 bits (RLE) */
      {
	/* 8 bits and 16 bits images will be converted to 24 bits */
	if (header->pixel_depth <= 24)
	  {
	    texinfo->format = GL_RGB;
	    texinfo->internalFormat = 3;
	  }
	else /* 32 bits */
	  {
	    texinfo->format = GL_RGBA;
	    texinfo->internalFormat = 4;
	  }

	break;
      }
    }
}

void ReadTGA8bits (FILE *fp, const unsigned char *colormap, struct image_t *texinfo)
{
  int i;
  unsigned char color;

  for (i = 0; i < texinfo->width * texinfo->height; ++i)
    {
      /* Read index color byte */
      color = (unsigned char)fgetc (fp);

      /* Convert to RGB 24 bits */
      texinfo->texels[(i * 3) + 2] = colormap[(color * 3) + 0];
      texinfo->texels[(i * 3) + 1] = colormap[(color * 3) + 1];
      texinfo->texels[(i * 3) + 0] = colormap[(color * 3) + 2];
    }
}

void ReadTGA16bits (FILE *fp, struct image_t *texinfo)
{
  int i;
  unsigned short color;

  for (i = 0; i < texinfo->width * texinfo->height; ++i)
    {
      /* Read color word */
      color = fgetc (fp) + (fgetc (fp) << 8);

      /* Convert BGR to RGB */
      texinfo->texels[(i * 3) + 0] = (unsigned char)(((color & 0x7C00) >> 10) << 3);
      texinfo->texels[(i * 3) + 1] = (unsigned char)(((color & 0x03E0) >>  5) << 3);
      texinfo->texels[(i * 3) + 2] = (unsigned char)(((color & 0x001F) >>  0) << 3);
    }
}

void ReadTGA24bits (FILE *fp, struct image_t *texinfo)
{
  int i;

  for (i = 0; i < texinfo->width * texinfo->height; ++i)
    {
      /* Read and convert BGR to RGB */
      texinfo->texels[(i * 3) + 2] = (unsigned char)fgetc (fp);
      texinfo->texels[(i * 3) + 1] = (unsigned char)fgetc (fp);
      texinfo->texels[(i * 3) + 0] = (unsigned char)fgetc (fp);
    }
}

void ReadTGA32bits (FILE *fp, struct image_t *texinfo)
{
  int i;

  for (i = 0; i < texinfo->width * texinfo->height; ++i)
    {
      /* Read and convert BGRA to RGBA */
      texinfo->texels[(i * 4) + 2] = (unsigned char)fgetc (fp);
      texinfo->texels[(i * 4) + 1] = (unsigned char)fgetc (fp);
      texinfo->texels[(i * 4) + 0] = (unsigned char)fgetc (fp);
      texinfo->texels[(i * 4) + 3] = (unsigned char)fgetc (fp);
    }
}

void ReadTGAgray8bits (FILE *fp, struct image_t *texinfo)
{
  int i;

  for (i = 0; i < texinfo->width * texinfo->height; ++i)
    {
      /* Read grayscale color byte */
      texinfo->texels[i] = (unsigned char)fgetc (fp);
    }
}

void ReadTGAgray16bits (FILE *fp, struct image_t *texinfo)
{
  int i;

  for (i = 0; i < texinfo->width * texinfo->height; ++i)
    {
      /* Read grayscale color + alpha channel bytes */
      texinfo->texels[(i * 2) + 0] = (unsigned char)fgetc (fp);
      texinfo->texels[(i * 2) + 1] = (unsigned char)fgetc (fp);
    }
}

static void ReadTGA8bitsRLE (FILE *fp, const unsigned char *colormap, struct image_t *texinfo)
{
  int i, size;
  unsigned char color;
  unsigned char packet_header;
  unsigned char *ptr = texinfo->texels;

  while (ptr < texinfo->texels + (texinfo->width * texinfo->height) * 3)
    {
      /* Read first byte */
      packet_header = (unsigned char)fgetc (fp);
      size = 1 + (packet_header & 0x7f);

      if (packet_header & 0x80)
	{
	  /* Run-length packet */
	  color = (unsigned char)fgetc (fp);

	  for (i = 0; i < size; ++i, ptr += 3)
	    {
	      ptr[0] = colormap[(color * 3) + 2];
	      ptr[1] = colormap[(color * 3) + 1];
	      ptr[2] = colormap[(color * 3) + 0];
	    }
	}
      else
	{
	  /* Non run-length packet */
	  for (i = 0; i < size; ++i, ptr += 3)
	    {
	      color = (unsigned char)fgetc (fp);

	      ptr[0] = colormap[(color * 3) + 2];
	      ptr[1] = colormap[(color * 3) + 1];
	      ptr[2] = colormap[(color * 3) + 0];
	    }
	}
    }
}

static void ReadTGA16bitsRLE (FILE *fp, struct image_t *texinfo)
{
  int i, size;
  unsigned short color;
  unsigned char packet_header;
  unsigned char *ptr = texinfo->texels;

  while (ptr < texinfo->texels + (texinfo->width * texinfo->height) * 3)
    {
      /* Read first byte */
      packet_header = fgetc (fp);
      size = 1 + (packet_header & 0x7f);

      if (packet_header & 0x80)
	{
	  /* Run-length packet */
	  color = fgetc (fp) + (fgetc (fp) << 8);

	  for (i = 0; i < size; ++i, ptr += 3)
	    {
	      ptr[0] = (unsigned char)(((color & 0x7C00) >> 10) << 3);
	      ptr[1] = (unsigned char)(((color & 0x03E0) >>  5) << 3);
	      ptr[2] = (unsigned char)(((color & 0x001F) >>  0) << 3);
	    }
	}
      else
	{
	  /* Non run-length packet */
	  for (i = 0; i < size; ++i, ptr += 3)
	    {
	      color = fgetc (fp) + (fgetc (fp) << 8);

	      ptr[0] = (unsigned char)(((color & 0x7C00) >> 10) << 3);
	      ptr[1] = (unsigned char)(((color & 0x03E0) >>  5) << 3);
	      ptr[2] = (unsigned char)(((color & 0x001F) >>  0) << 3);
	    }
	}
    }
}

void ReadTGA24bitsRLE (FILE *fp, struct image_t *texinfo)
{
  int i, size;
  unsigned char rgb[3];
  unsigned char packet_header;
  unsigned char *ptr = texinfo->texels;

  while (ptr < texinfo->texels + (texinfo->width * texinfo->height) * 3)
    {
      /* Read first byte */
      packet_header = (unsigned char)fgetc (fp);
      size = 1 + (packet_header & 0x7f);

      if (packet_header & 0x80)
	{
	  /* Run-length packet */
	  fread (rgb, sizeof (unsigned char), 3, fp);

	  for (i = 0; i < size; ++i, ptr += 3)
	    {
	      ptr[0] = rgb[2];
	      ptr[1] = rgb[1];
	      ptr[2] = rgb[0];
	    }
	}
      else
	{
	  /* Non run-length packet */
	  for (i = 0; i < size; ++i, ptr += 3)
	    {
	      ptr[2] = (unsigned char)fgetc (fp);
	      ptr[1] = (unsigned char)fgetc (fp);
	      ptr[0] = (unsigned char)fgetc (fp);
	    }
	}
    }
}

void ReadTGA32bitsRLE (FILE *fp, struct image_t *texinfo)
{
  int i, size;
  unsigned char rgba[4];
  unsigned char packet_header;
  unsigned char *ptr = texinfo->texels;

  while (ptr < texinfo->texels + (texinfo->width * texinfo->height) * 4)
    {
      /* Read first byte */
      packet_header = (unsigned char)fgetc (fp);
      size = 1 + (packet_header & 0x7f);

      if (packet_header & 0x80)
	{
	  /* Run-length packet */
	  fread (rgba, sizeof (unsigned char), 4, fp);

	  for (i = 0; i < size; ++i, ptr += 4)
	    {
	      ptr[0] = rgba[2];
	      ptr[1] = rgba[1];
	      ptr[2] = rgba[0];
	      ptr[3] = rgba[3];
	    }
	}
      else
	{
	  /* Non run-length packet */
	  for (i = 0; i < size; ++i, ptr += 4)
	    {
	      ptr[2] = (unsigned char)fgetc (fp);
	      ptr[1] = (unsigned char)fgetc (fp);
	      ptr[0] = (unsigned char)fgetc (fp);
	      ptr[3] = (unsigned char)fgetc (fp);
	    }
	}
    }
}

void ReadTGAgray8bitsRLE (FILE *fp, struct image_t *texinfo)
{
  int i, size;
  unsigned char color;
  unsigned char packet_header;
  unsigned char *ptr = texinfo->texels;

  while (ptr < texinfo->texels + (texinfo->width * texinfo->height))
    {
      /* Read first byte */
      packet_header = (unsigned char)fgetc (fp);
      size = 1 + (packet_header & 0x7f);

      if (packet_header & 0x80)
	{
	  /* Run-length packet */
	  color = (unsigned char)fgetc (fp);

	  for (i = 0; i < size; ++i, ptr++)
	    *ptr = color;
	}
      else
	{
	  /* Non run-length packet */
	  for (i = 0; i < size; ++i, ptr++)
	    *ptr = (unsigned char)fgetc (fp);
	}
    }
}

void ReadTGAgray16bitsRLE (FILE *fp, struct image_t *texinfo)
{
  int i, size;
  unsigned char color, alpha;
  unsigned char packet_header;
  unsigned char *ptr = texinfo->texels;

  while (ptr < texinfo->texels + (texinfo->width * texinfo->height) * 2)
    {
      /* Read first byte */
      packet_header = (unsigned char)fgetc (fp);
      size = 1 + (packet_header & 0x7f);

      if (packet_header & 0x80)
	{
	  /* Run-length packet */
	  color = (unsigned char)fgetc (fp);
	  alpha = (unsigned char)fgetc (fp);

	  for (i = 0; i < size; ++i, ptr += 2)
	    {
	      ptr[0] = color;
	      ptr[1] = alpha;
	    }
	}
      else
	{
	  /* Non run-length packet */
	  for (i = 0; i < size; ++i, ptr += 2)
	    {
	      ptr[0] = (unsigned char)fgetc (fp);
	      ptr[1] = (unsigned char)fgetc (fp);
	    }
	}
    }
}

int ImageTGARead(const char *filename, struct image_t * texinfo)
{
  FILE *fp;
  /* struct texture_t *texinfo; */
  struct tga_header_t header;
  unsigned char *colormap = NULL;

  fp = fopen (filename, "rb");
  if (!fp)
    {
      fprintf (stderr, "error: couldn't open \"%s\"!\n", filename);
      return 0;
    }

  /* Read header */
  fread (&header, sizeof (struct tga_header_t), 1, fp);

  /* texinfo = (struct texture_t *)
    malloc (sizeof (struct texture_t)); */
  GetTextureInfo (&header, texinfo);
  fseek (fp, header.id_lenght, SEEK_CUR);

  /* Memory allocation */
  texinfo->texels = (unsigned char *)malloc (sizeof (unsigned char) *
	texinfo->width * texinfo->height * texinfo->internalFormat);
  if (!texinfo->texels)
    {
      //free (texinfo);
      return 0;
    }

  /* Read color map */
  if (header.colormap_type)
    {
      /* NOTE: color map is stored in BGR format */
      colormap = (unsigned char *)malloc (sizeof (unsigned char)
	* header.cm_length * (header.cm_size >> 3));
      fread (colormap, sizeof (unsigned char), header.cm_length
	* (header.cm_size >> 3), fp);
    }

  /* Read image data */
  switch (header.image_type)
    {
    case 0:
      /* No data */
      break;

    case 1:
      /* Uncompressed 8 bits color index */
      ReadTGA8bits (fp, colormap, texinfo);
      break;

    case 2:
      /* Uncompressed 16-24-32 bits */
      switch (header.pixel_depth)
	{
	case 16:
	  ReadTGA16bits (fp, texinfo);
	  break;

	case 24:
	  ReadTGA24bits (fp, texinfo);
	  break;

	case 32:
	  ReadTGA32bits (fp, texinfo);
	  break;
	}

      break;

    case 3:
      /* Uncompressed 8 or 16 bits grayscale */
      if (header.pixel_depth == 8)
	ReadTGAgray8bits (fp, texinfo);
      else /* 16 */
	ReadTGAgray16bits (fp, texinfo);

      break;

    case 9:
      /* RLE compressed 8 bits color index */
      ReadTGA8bitsRLE (fp, colormap, texinfo);
      break;

    case 10:
      /* RLE compressed 16-24-32 bits */
      switch (header.pixel_depth)
	{
	case 16:
	  ReadTGA16bitsRLE (fp, texinfo);
	  break;

	case 24:
	  ReadTGA24bitsRLE (fp, texinfo);
	  break;

	case 32:
	  ReadTGA32bitsRLE (fp, texinfo);
	  break;
	}

      break;

    case 11:
      /* RLE compressed 8 or 16 bits grayscale */
      if (header.pixel_depth == 8)
	ReadTGAgray8bitsRLE (fp, texinfo);
      else /* 16 */
	ReadTGAgray16bitsRLE (fp, texinfo);

      break;

    default:
      /* Image type is not correct */
      fprintf (stderr, "error: unknown TGA image type %i!\n", header.image_type);
      free (texinfo->texels);
      //free (texinfo);
      //texinfo = 0;
      break;
    }

  /* No longer need colormap data */
  if (colormap)
    free (colormap);

  fclose (fp);
  return 1;
}

