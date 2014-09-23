#ifndef CVPI_EGL_CONFIG
#include "cvpi_egl_config.h"
#endif

#ifndef CVPI_VG_EXT
#include "cvpi_vg_ext.h"
#endif

#ifndef _OPENVG_H
#include <VG/openvg.h>
#endif

#ifndef _STDIO_H
#include <stdio.h>
#endif

#ifndef _STDLIB_H
#include <stdlib.h>
#endif

#ifndef _UNISTD_H
#include <unistd.h>
#endif

#ifndef CVPI_IMAGE_FUNCTIONS
#include "cvpi_image_functions.h"
#endif

#define OUTPUT_LINEAR VG_FALSE

const VGfloat cvpi_invert_colors[20] = {
  -1, 0, 0, 0,
  0, -1, 0, 0,
  0, 0, -1, 0,
  0, 0, 0, 1,

  255, 255, 255, 0
};

const VGfloat cvpi_yuva2rgba[20] = {
  0.114, 0.436, -0.10001, 0, 
  0.587, -0.28886, -0.51499, 0, 
  0.299, -0.14713, 0.615, 0, 
  0, 0, 0, 1,

  0,0,0,0
};

const VGubyte cvpi_zeros_array[256] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

const VGubyte cvpi_255_array[256] = {255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255};

const VGubyte cvpi_identity_array[256] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255};

const VGubyte cvpi_inversion_array[256] = {255,254,253,252,251,250,249,248,247,246,245,244,243,242,241,240,239,238,237,236,235,234,233,232,231,230,229,228,227,226,225,224,223,222,221,220,219,218,217,216,215,214,213,212,211,210,209,208,207,206,205,204,203,202,201,200,199,198,197,196,195,194,193,192,191,190,189,188,187,186,185,184,183,182,181,180,179,178,177,176,175,174,173,172,171,170,169,168,167,166,165,164,163,162,161,160,159,158,157,156,155,154,153,152,151,150,149,148,147,146,145,144,143,142,141,140,139,138,137,136,135,134,133,132,131,130,129,128,127,126,125,124,123,122,121,120,119,118,117,116,115,114,113,112,111,110,109,108,107,106,105,104,103,102,101,100,99,98,97,96,95,94,93,92,91,90,89,88,87,86,85,84,83,82,81,80,79,78,77,76,75,74,73,72,71,70,69,68,67,66,65,64,63,62,61,60,59,58,57,56,55,54,53,52,51,50,49,48,47,46,45,44,43,42,41,40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0};

const VGfloat yuyv2yuva_conversion_1_argb[20] = {
  0, 0, 0, 1, 
  0, 1, 0, 0, 
  0, 0, 1, 0,
  1, 0, 0, 0,
  0, 0, 0, 0
};

const VGfloat yuyv2yuva_conversion_2_argb[20] = {
  0,0,1,0,
  0,1,0,0,
  0,0,0,1,
  1,0,0,0,
  
  0,0,0,0
};

VGImage cvpi_yuyv2yuva(VGImage yuyv_image) {
  VGfloat* yuyv2yuva_conversion_1;
  VGfloat* yuyv2yuva_conversion_2;
  yuyv2yuva_conversion_1 = &yuyv2yuva_conversion_1_argb;
  yuyv2yuva_conversion_2 = &yuyv2yuva_conversion_2_argb;

  VGint yuyv_width = vgGetParameteri(yuyv_image, VG_IMAGE_WIDTH);
  VGint height = vgGetParameteri(yuyv_image, VG_IMAGE_HEIGHT);
  VGint yuva_width = yuyv_width*2;
  VGint yuyv_stride = yuyv_width*4;
  VGint yuva_stride = yuva_width*4;

  unsigned long itter;

  VGImage mod_image_1 = vgCreateImage(CVPI_COLOR_SPACE, yuyv_width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  VGImage mod_image_2 = vgCreateImage(CVPI_COLOR_SPACE, yuyv_width, height, VG_IMAGE_QUALITY_NONANTIALIASED);  
  VGImage mod_image_3 = vgCreateImage(CVPI_COLOR_SPACE, yuyv_width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  VGImage mod_image_4 = vgCreateImage(CVPI_COLOR_SPACE, yuyv_width, height, VG_IMAGE_QUALITY_NONANTIALIASED);  

  /* set blue in RGBA to 255 */
  vgLookup(mod_image_2, yuyv_image, cvpi_identity_array, cvpi_identity_array, cvpi_255_array, cvpi_identity_array, OUTPUT_LINEAR, VG_FALSE);
  /* set red in RGBA to 255 */
  vgLookup(mod_image_1, yuyv_image, cvpi_255_array, cvpi_identity_array, cvpi_identity_array, cvpi_identity_array, OUTPUT_LINEAR, VG_FALSE);
  vgFinish();

  vgColorMatrix(mod_image_3, mod_image_1, yuyv2yuva_conversion_1);
  vgColorMatrix(mod_image_4, mod_image_2, yuyv2yuva_conversion_2);
  vgFinish();

  /* images no longer needed */
  vgDestroyImage(mod_image_1);
  vgDestroyImage(mod_image_2);

  VGImage output_image = vgCreateImage(CVPI_COLOR_SPACE, yuva_width, height, VG_IMAGE_QUALITY_NONANTIALIASED);

  /* merge mod 3,4 images */
  for(itter = 0; itter < yuyv_width; ++itter) {
    vgCopyImage(output_image, itter*2, 0, mod_image_3, itter, 0, 1, height, VG_FALSE);
    vgCopyImage(output_image, itter*2+1, 0, mod_image_4, itter, 0, 1, height, VG_FALSE);
    vgFinish();
  }
  vgDestroyImage(mod_image_3);
  vgDestroyImage(mod_image_4);
  vgFlush();
  return output_image;
}

VGImage cvpi_add_images(VGImage img1, VGImage img2, VGshort a, VGshort b, VGfloat scale, VGfloat bias) {
/* add_images adds two images by adding the upper halves and the lower
   halves separately.  Done this way so that the intermediate image
   does not exceed allowable dimensions. */

  VGint img1_width = vgGetParameteri(img1, VG_IMAGE_WIDTH);
  VGint img1_height = vgGetParameteri(img1, VG_IMAGE_HEIGHT);  

  VGint img2_width = vgGetParameteri(img2, VG_IMAGE_WIDTH);
  VGint img2_height = vgGetParameteri(img2, VG_IMAGE_HEIGHT);  

  if(img1_width != img2_width || img1_height != img2_height) {
    fprintf(stderr, "add_images: Images have differing dimensions:\nimg1: %dx%d\nimg2: %dx%d\n",
	    img1_width, img1_height, img2_width, img2_height);
    return VG_INVALID_HANDLE;
  }

  VGshort kernel[2] = {a,b};	/* only place where `a' and `b' are used */
  unsigned long itter = 0;
  VGImage output = vgCreateImage(CVPI_COLOR_SPACE, img1_width, img1_height, VG_IMAGE_QUALITY_NONANTIALIASED);
  VGImage half;
  VGImage half_c;
  VGImage last_row;
  VGImage last_row_c;
  VGint max;
  if(!(img1_height % 2)) { 	/* even height */
    half = vgCreateImage(CVPI_COLOR_SPACE, img1_width, img1_height, VG_IMAGE_QUALITY_NONANTIALIASED);
    half_c = vgCreateImage(CVPI_COLOR_SPACE, img1_width, img1_height, VG_IMAGE_QUALITY_NONANTIALIASED);
    max = img1_height;
  } else {                      /* odd height */
    last_row = vgCreateImage(CVPI_COLOR_SPACE, img1_width, 2, VG_IMAGE_QUALITY_NONANTIALIASED);
    last_row_c = vgCreateImage(CVPI_COLOR_SPACE, img1_width, 2, VG_IMAGE_QUALITY_NONANTIALIASED);
    if(img1_height > 1) {
      half = vgCreateImage(CVPI_COLOR_SPACE, img1_width, img1_height-1, VG_IMAGE_QUALITY_NONANTIALIASED);
      half_c = vgCreateImage(CVPI_COLOR_SPACE, img1_width, img1_height-1, VG_IMAGE_QUALITY_NONANTIALIASED);
      max = img1_height-1;
    } else {			/* 1 pixel high image */
      max = 0;
    }
  }

  if(img1_height > 1) {
    for(itter = 0; itter < max/2; ++itter) {
      vgCopyImage(half, 0, 2*itter, img1, 0, itter, img1_width, 1, VG_FALSE);
      vgCopyImage(half, 0, 2*itter+1, img2, 0, itter, img1_width, 1, VG_FALSE);
      vgFinish();
    }
    /* add the current pixel with the one below it */
    /* tiling mode does not matter since the last row is not returned */
    vgConvolve(half_c, half, 1, 2, 0, 0, &kernel, scale, bias, VG_TILE_PAD);
   
    /* upper half */
    unsigned long i;
    for(i = 0; i < max/2; ++i) {
      vgCopyImage(output, 0, i, half_c, 0, 2*i+1, img1_width, 1, VG_FALSE);
      vgFinish();
    }
    
    for(; itter < max; ++itter) {
      vgCopyImage(half, 0, 2*itter, img1, 0, itter, img1_width, 1, VG_FALSE);
      vgCopyImage(half, 0, 2*itter+1, img2, 0, itter, img1_width, 1, VG_FALSE);
      vgFinish();
    }
    vgConvolve(half_c, half, 1, 2, 0, 0, &kernel, scale, bias, VG_TILE_PAD);

    /* lower half */
    unsigned long j;
    for(j = 0; j < max/2; ++i, ++j) {
      vgCopyImage(output, 0, i, half_c, 0, 2*j+1, img1_width, 1, VG_FALSE);
      vgFinish();
    }
  }
  if(img1_height % 2) {
    vgCopyImage(last_row, 0, 0, img1, 0, img1_height-1, img1_width, 1, VG_FALSE);
    vgCopyImage(last_row, 0, 1, img2, 0, img1_height-1, img1_width, 1, VG_FALSE);
    vgConvolve(last_row_c, last_row, 1, 2, 0, 0, &kernel, scale, bias, VG_TILE_PAD);
    vgCopyImage(output, 0, img1_height-1, last_row_c, 0, 1, img1_width, 1, VG_FALSE);
    vgFinish();
  }
  vgDestroyImage(half);
  vgDestroyImage(half_c);
  vgDestroyImage(last_row);
  vgDestroyImage(last_row_c);
  vgFlush();
  return output;
}
