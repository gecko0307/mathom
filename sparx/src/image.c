#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "image.h"

int memset(void*, int, int);

struct image_t ImageCreate(unsigned int w, unsigned int h, unsigned int channels)
{
 	struct image_t img;
 	
	img.width = w;
	img.height = h;
	img.channels = channels;
	
	if (channels == 1) img.format = GL_LUMINANCE;
	else if (channels == 2) img.format = GL_LUMINANCE_ALPHA;
	else if (channels == 3) img.format = GL_RGB;
	else if (channels == 4) img.format = GL_RGBA;
	img.internalFormat = img.format;
	
	img.texels = (unsigned char*)malloc(w * h * channels);
	memset(img.texels, 0, w * h * channels);
	return img;
}

void ImageFree(struct image_t *img)
{
     if (img->texels)
     {
        free(img->texels);
        img->texels = NULL;
     }
}

void ImageSetPixelRGBA(struct image_t* img, int x, int y, struct color_t color)
{
	unsigned int offset;
    
    offset = (x + y * img->width) * img->channels;
	img->texels[offset + 0] = color.r;
	img->texels[offset + 1] = color.g;
	img->texels[offset + 2] = color.b;
	img->texels[offset + 3] = color.a;
}

struct color_t ImageGetPixelRGBA(struct image_t* img, int x, int y)
{
    struct color_t color;
	unsigned int offset;
    
    offset = (x + y * img->width) * img->channels;
	color.r = img->texels[offset + 0];
	color.g = img->texels[offset + 1];
	color.b = img->texels[offset + 2];
	color.a = img->texels[offset + 3];
	
	return color;
}

void ImageRenderSinePlasmaRGBA(struct image_t* img, float factor)
{	
	unsigned int x, y;
	unsigned char value;
	struct color_t color;
	
	color.a = 255;
	
	for (x = 0; x < img->width; ++x)
	for (y = 0; y < img->height; ++y)
	{
		value = 127 + 63.5 * sin(x * factor) + 63.5 * sin(y * factor);
		color.r = color.g = color.b = value;
		ImageSetPixelRGBA(img, x, y, color);
	}
}

