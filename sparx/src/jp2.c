/*
 * Copyright (c) 2011 Timur GAFAROV
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
#include "openjpeg.h"

int ImageJP2Read(const char *filename, struct image_t * texinfo)
{
	FILE *reader;
	unsigned char *buf;
	long buf_len; 
  
	opj_dparameters_t parameters; 
	opj_image_t *image; 
	opj_dinfo_t *dinfo; 
	opj_cio_t* cio;
  
	reader = fopen(filename, "rb"); 
	fseek(reader, 0, SEEK_END); 
	buf_len = ftell(reader); 
	fseek(reader, 0, SEEK_SET); 
	buf = (unsigned char*)malloc(buf_len); 
	fread(buf, 1, buf_len, reader); 
	fclose(reader); 

	opj_set_default_decoder_parameters(&parameters); 
	dinfo = opj_create_decompress(CODEC_JP2); 
	opj_setup_decoder(dinfo, &parameters); 
	cio = opj_cio_open((opj_common_ptr)dinfo, buf, buf_len); 
	image = opj_decode(dinfo, cio); 
	opj_cio_close(cio);
	opj_destroy_decompress(dinfo);
  
	unsigned char* data = NULL;
	int dataSize = 0;
	
	int i;
	/* printf("Number of components: %d\n", image->numcomps); */
	for(i = 0; i < image->numcomps; ++i)
	{
		opj_image_comp_t* comp = &image->comps[i];
		/* printf("Component %d", i);
		printf("Offset: %dx%d\n", comp->x0, comp->y0);
		printf("Size: %dx%d\n", comp->w, comp->h);
		printf("BPP: %d\n", comp->bpp); */
		dataSize += comp->w * comp->h;
		
		texinfo->width = comp->w;
		texinfo->height = comp->h;
	}
	
	/* Convert data from planar to interleaved format */
	/* Optimize this? Maybe... */
	data = (unsigned char*)malloc(dataSize);
	for(i = 0; i < image->numcomps; ++i)
	{
		opj_image_comp_t* comp = &image->comps[i];
		int len = comp->w * comp->h;
		int j;
		for(j = 0; j < len; ++j)
		{
			data[i + j*image->numcomps] = comp->data[j];
		}
	}	
	
	if (image->numcomps == 1) texinfo->format = GL_LUMINANCE;
	else if (image->numcomps == 2) texinfo->format = GL_LUMINANCE_ALPHA;
	else if (image->numcomps == 3) texinfo->format = GL_RGB;
	else if (image->numcomps == 4) texinfo->format = GL_RGBA;
	texinfo->internalFormat = texinfo->format;
	texinfo->texels = data;
	texinfo->numMipmaps = 1;
	
	opj_image_destroy(image);

	return 1;
}

