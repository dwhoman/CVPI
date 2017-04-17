/*
  This file is part of CVPI.

  Copyright 2017 Devin Homan
 
  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at
 
      http://www.apache.org/licenses/LICENSE-2.0
 
  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/

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

#ifndef _ERRNO_H
#include <errno.h>
#endif

#ifndef	_MATH_H
#include <math.h>
#endif

#ifndef CVPI_IMAGE_FUNCTIONS
#include "cvpi_image_functions.h"
#endif

#ifndef CVPI_BASE
#include "cvpi_base.h"
#endif

#define OUTPUT_LINEAR VG_FALSE

const VGfloat cvpi_invert_colors[20] = {
  -1, 0, 0, 0,
  0, -1, 0, 0,
  0, 0, -1, 0,
  0, 0, 0, 1,

  1, 1, 1, 0
};

const VGfloat cvpi_avuy2ayuv[20] = {
  0, 0, 1, 0,
  0, 1, 0, 0,
  1, 0, 0, 0,
  0, 0, 0, 1,

  0,0,0,0
};

const VGfloat cvpi_pixel_average[20] = {
  0.25, 0.25, 0.25, 0.25,
  0.25, 0.25, 0.25, 0.25,
  0.25, 0.25, 0.25, 0.25,
  0.25, 0.25, 0.25, 0.25,

  0, 0, 0, 0
};

const VGfloat cvpi_pixel_color_average[20] = {
  1.0/3, 1.0/3, 1.0/3, 0,
  1.0/3, 1.0/3, 1.0/3, 0,
  1.0/3, 1.0/3, 1.0/3, 0,
  0,0,0,1,
  0,0,0,0
};

const VGfloat cvpi_channel_red[20] = {
  1,1,1,1,
  0,0,0,0,
  0,0,0,0,
  0,0,0,0,
  0,0,0,0
};

const VGfloat cvpi_channel_green[20] = {
  0,0,0,0,
  1,1,1,1,
  0,0,0,0,
  0,0,0,0,
  0,0,0,0
};

const VGfloat cvpi_channel_blue[20] = {
  0,0,0,0,
  0,0,0,0,
  1,1,1,1,
  0,0,0,0,
  0,0,0,0
};

const VGfloat cvpi_channel_alpha[20] = {
  0,0,0,0,
  0,0,0,0,
  0,0,0,0,
  1,1,1,1,
  0,0,0,0
};

const VGubyte cvpi_sqrt_array_floor[256] = {0,1,1,1,2,2,2,2,2,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,6,6,6,6,6,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15};
const VGubyte cvpi_sqrt_array_ceil[256] = {0,1,2,2,2,3,3,3,3,3,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,6,6,6,7,7,7,7,7,7,7,7,7,7,7,7,7,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16};
const VGubyte cvpi_sqrt_array_round[256] = {0,1,1,2,2,2,2,3,3,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,6,6,6,6,7,7,7,7,7,7,7,7,7,7,7,7,7,7,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16};

const VGuint cvpi_binary_array[256] = {0, [1 ... 255] = ~0};

const VGuint cvpi_binary_array_inverted[256] = {~0, [1 ... 255] = 0};

const VGubyte cvpi_zeros_array[256] = {[0 ... 255] = 0};

const VGubyte cvpi_255_array[256] = {[0 ... 255] = 255};

const VGubyte cvpi_identity_array[256] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255};

const VGubyte cvpi_inversion_array[256] = {255,254,253,252,251,250,249,248,247,246,245,244,243,242,241,240,239,238,237,236,235,234,233,232,231,230,229,228,227,226,225,224,223,222,221,220,219,218,217,216,215,214,213,212,211,210,209,208,207,206,205,204,203,202,201,200,199,198,197,196,195,194,193,192,191,190,189,188,187,186,185,184,183,182,181,180,179,178,177,176,175,174,173,172,171,170,169,168,167,166,165,164,163,162,161,160,159,158,157,156,155,154,153,152,151,150,149,148,147,146,145,144,143,142,141,140,139,138,137,136,135,134,133,132,131,130,129,128,127,126,125,124,123,122,121,120,119,118,117,116,115,114,113,112,111,110,109,108,107,106,105,104,103,102,101,100,99,98,97,96,95,94,93,92,91,90,89,88,87,86,85,84,83,82,81,80,79,78,77,76,75,74,73,72,71,70,69,68,67,66,65,64,63,62,61,60,59,58,57,56,55,54,53,52,51,50,49,48,47,46,45,44,43,42,41,40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0};

/* kernel is rotated 90 degrees counter-clockwise from Math definition
   to array index VG spec p. 179 */
const VGshort cvpi_filter_sobel_y[9] = {1,0,-1,
					2,0,-2,
					1,0,-1};
const VGshort cvpi_filter_sobel_x[9] ={-1,-2,-1,
				       0,0,0,
				       1,2,1};

const VGshort cvpi_filter_scharr_y[9] = {-3,0,3,
					 -10,0,10,
					 -3,0,3};
const VGshort cvpi_filter_scharr_x[9] = {3,10,3,
					 0,0,0,
					 -3,-10,-3};

const VGshort cvpi_filter_prewitt_y[9] = {1,0,-1,
					  1,0,-1,
					  1,0,-1};
const VGshort cvpi_filter_prewitt_x[9] = {-1,-1,-1,
					  0,0,0,
					  1,1,1};

const VGshort cvpi_filter_roberts_cross_y[4] = {-1,0,
						0,1};
const VGshort cvpi_filter_roberts_cross_x[4] = {0,1,
						-1,0};

#define vgDestroyImageSafe(image)do{if(image != VG_INVALID_HANDLE){vgDestroyImage(image); image=VG_INVALID_HANDLE;}}while(0)

#define cvpi_vg_error_check()\
  do {\
  VGErrorCode error = vgGetError();\
  if(error != VG_NO_ERROR) {\
    cvpi_log_2("", __FILE__, __LINE__, cvpi_vg_error_string(error)); \
    BADSTATE = 1;\
    goto TAKEDOWN;\
  }\
  } while(0)

#define cvpi_image_error_check(image)\
  do {\
  if((image) == VG_INVALID_HANDLE) {\
    cvpi_log_1("", __FILE__, __LINE__); \
    BADSTATE = 1;\
    goto TAKEDOWN;\
  }\
  } while(0)

/* use inside TAKEDOWN */
#define cvpi_vg_error_takedown()\
  do {\
  VGErrorCode error = vgGetError();\
  if(error != VG_NO_ERROR) {\
    cvpi_log_2("", __FILE__, __LINE__, cvpi_vg_error_string(error)); \
    BADSTATE = 1;\
  }\
  } while(0)

VGImage cvpi_yuyv2yuva(const VGImage yuyv_image) {
  return cvpi_yuyv2yuva_alpha(yuyv_image, 255);
}

VGImage cvpi_yuyv2yuva_alpha(const VGImage yuyv_image, const VGubyte alpha) {
#define TAKEDOWN cvpi_yuyv2yuva_takedown
  int BADSTATE = 0;
  VGImage mod_image_1 = VG_INVALID_HANDLE;
  VGImage mod_image_2 = VG_INVALID_HANDLE;
  VGImage output_image = VG_INVALID_HANDLE; /* return value */

  VGint yuyv_width = vgGetParameteri(yuyv_image, VG_IMAGE_WIDTH);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  VGint height = vgGetParameteri(yuyv_image, VG_IMAGE_HEIGHT);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  const VGfloat yuyv2yuva_conversion_1[20] = {
  0, 0, 0, 0,
  0, 1, 0, 0,
  0, 0, 1, 0,
  1, 0, 0, 0,

  0, 0, 0, alpha
  };
  const VGfloat yuyv2yuva_conversion_2[20] = {
  0, 0, 1, 0,
  0, 1, 0, 0,
  0, 0, 0, 0,
  1, 0, 0, 0,

  0, 0, 0, alpha
  };

  VGint yuva_width = yuyv_width*2;

  mod_image_1 = vgCreateImage(CVPI_COLOR_SPACE, yuyv_width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();

  mod_image_2 = vgCreateImage(CVPI_COLOR_SPACE, yuyv_width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();

  vgColorMatrixNormal(mod_image_1, yuyv_image, yuyv2yuva_conversion_1);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  vgColorMatrixNormal(mod_image_2, yuyv_image, yuyv2yuva_conversion_2);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  vgFinish();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif

  output_image = vgCreateImage(CVPI_COLOR_SPACE, yuva_width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();

  /* merge mod 3,4 images */
  for(unsigned long itter = yuyv_width; itter != 0;) {
    --itter;
    vgCopyImage(output_image, itter*2, 0, mod_image_1, itter, 0, 1, height, VG_FALSE);
#if CVPI_CAREFUL == 1
    cvpi_vg_error_check();
#endif
    vgCopyImage(output_image, itter*2+1, 0, mod_image_2, itter, 0, 1, height, VG_FALSE);
#if CVPI_CAREFUL == 1
    cvpi_vg_error_check();
#endif
    vgFinish();
#if CVPI_CAREFUL == 1
    cvpi_vg_error_check();
#endif
  }
 TAKEDOWN:
  vgDestroyImageSafe(mod_image_1);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(mod_image_2);
  cvpi_vg_error_takedown();
  if(BADSTATE) {
    /* set output image to  */
    vgDestroyImageSafe(output_image);
    cvpi_vg_error_takedown();
  }
  vgFlush();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_takedown();
#endif
  return output_image;
#undef TAKEDOWN
}

VGImage cvpi_image_add_vert(const VGImage img1, const VGImage img2, VGshort a, VGshort b, VGfloat scale, VGfloat bias) {
  /* add images by interlacing them row by row */
}

VGImage cvpi_image_add_horiz(const VGImage img1, const VGImage img2, VGshort a, VGshort b, VGfloat scale, VGfloat bias) {
  /* add images by interlacing them column by column */
}

VGImage cvpi_image_add(const VGImage img1, const VGImage img2, VGshort a, VGshort b, VGfloat scale, VGfloat bias) {
/* add_images adds two images by adding the upper halves and the lower
   halves separately.  Done this way so that the intermediate image
   does not exceed allowable dimensions. */
#define TAKEDOWN cvpi_image_add_takedown
  int BADSTATE = 0;

  VGImage output = VG_INVALID_HANDLE;
  VGImage half_img1 = VG_INVALID_HANDLE;			/* image half original */
  VGImage half_img2 = VG_INVALID_HANDLE;
  VGImage half_added = VG_INVALID_HANDLE;
  /* if a tall image has an odd value height, add last row separately */
  VGImage l_row_img = VG_INVALID_HANDLE;
  VGImage l_row_added = VG_INVALID_HANDLE; 
  VGImage both = VG_INVALID_HANDLE;        /* both images combined into a single image */
  VGImage added = VG_INVALID_HANDLE;	   /* sum of the two images */

  VGint img1_width = vgGetParameteri(img1, VG_IMAGE_WIDTH);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  VGint img1_height = vgGetParameteri(img1, VG_IMAGE_HEIGHT);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif

  VGint img2_width = vgGetParameteri(img2, VG_IMAGE_WIDTH);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  VGint img2_height = vgGetParameteri(img2, VG_IMAGE_HEIGHT);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif

  if(img1_width != img2_width || img1_height != img2_height) {
    cvpi_log_6("Images have differing dimensions:",
	       __func__, __LINE__, img1_width, img1_height, img2_width, img2_height);
    BADSTATE = 1;
    goto TAKEDOWN;
  }

  VGshort kernel[2] = {b,a};	/* only place where `a' and `b' are used */
  unsigned long itter = 0;
  output = vgCreateImage(CVPI_COLOR_SPACE, img1_width, img1_height, VG_IMAGE_QUALITY_NONANTIALIASED);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  VGint imageMaxHeight = vgGeti(VG_MAX_IMAGE_HEIGHT);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  if(2*img1_height > imageMaxHeight) {
    VGint half_height = img1_height/2;
    half_img1 = vgCreateImage(CVPI_COLOR_SPACE, img1_width, half_height, VG_IMAGE_QUALITY_NONANTIALIASED);
#if CVPI_CAREFUL == 1
      cvpi_vg_error_check();
#endif
    half_img2 = vgCreateImage(CVPI_COLOR_SPACE, img1_width, half_height, VG_IMAGE_QUALITY_NONANTIALIASED);
#if CVPI_CAREFUL == 1
      cvpi_vg_error_check();
#endif

    /* add top halves */
    vgCopyImage(half_img1, 0, 0, img1, 0, 0, img1_width, half_height, VG_FALSE);
#if CVPI_CAREFUL == 1
      cvpi_vg_error_check();
#endif
    cvpi_vg_error_check();
    vgCopyImage(half_img2, 0, 0, img2, 0, 0, img1_width, half_height, VG_FALSE);
#if CVPI_CAREFUL == 1
      cvpi_vg_error_check();
#endif
    vgFinish();
    half_added = cvpi_image_add(half_img1, half_img2, a, b, scale, bias);
    if(half_added == VG_INVALID_HANDLE) {
      cvpi_log_2("Failed to add top halves:", __func__, __LINE__);
      BADSTATE = 1;
      goto TAKEDOWN;
    }
    vgCopyImage(output, 0, 0, half_added, 0, 0, img1_width, half_height, VG_FALSE);
#if CVPI_CAREFUL == 1
      cvpi_vg_error_check();
#endif
    vgDestroyImageSafe(half_added);
#if CVPI_CAREFUL == 1
    cvpi_vg_error_check();
#endif
    /* add bottom halves */
    vgCopyImage(half_img1, 0, 0, img1, 0, half_height, img1_width, half_height, VG_FALSE);
#if CVPI_CAREFUL == 1
    cvpi_vg_error_check();
#endif
    vgCopyImage(half_img2, 0, 0, img2, 0, half_height, img1_width, half_height, VG_FALSE);
#if CVPI_CAREFUL == 1
    cvpi_vg_error_check();
#endif
    half_added = cvpi_image_add(half_img1, half_img2, a, b, scale, bias);
    if(half_added == VG_INVALID_HANDLE) {
      cvpi_log_2("Failed to add bottom halves:", __func__, __LINE__);
      BADSTATE = 1;
      goto TAKEDOWN;
    }
    vgCopyImage(output, 0, half_height, half_added, 0, 0, img1_width, half_height, VG_FALSE);
    vgFinish();
    vgDestroyImageSafe(half_added);

    /* if odd height, add last rows */
    if(img1_height % 2) {
      l_row_img = vgCreateImage(CVPI_COLOR_SPACE, img1_width, 2, VG_IMAGE_QUALITY_NONANTIALIASED);
      l_row_added = vgCreateImage(CVPI_COLOR_SPACE, img1_width, 2, VG_IMAGE_QUALITY_NONANTIALIASED);
      vgCopyImage(l_row_img, 0, 0, img1, 0, img1_height-1, img1_width, 1, VG_FALSE);
#if CVPI_CAREFUL == 1
      cvpi_vg_error_check();
#endif
      vgCopyImage(l_row_img, 0, 1, img2, 0, img1_height-1, img1_width, 1, VG_FALSE);
#if CVPI_CAREFUL == 1
      cvpi_vg_error_check();
#endif
      vgConvolveNormal(l_row_added, l_row_img, 1, 2, 0, 0, kernel, scale, bias, VG_TILE_PAD);
#if CVPI_CAREFUL == 1
      cvpi_vg_error_check();
#endif
      vgCopyImage(output, 0, img1_height-1, l_row_added, 0, 0, img1_width, 1, VG_FALSE);
#if CVPI_CAREFUL == 1
      cvpi_vg_error_check();
#endif
      vgFinish();
    }
  } else {
    /* both images together fit in a single buffer */

    /* combine the two images */
    both = vgCreateImage(CVPI_COLOR_SPACE, img1_width, 2*img1_height, VG_IMAGE_QUALITY_NONANTIALIASED);
#if CVPI_CAREFUL == 1
    cvpi_vg_error_check();
#endif
    added = vgCreateImage(CVPI_COLOR_SPACE, img1_width, 2*img1_height, VG_IMAGE_QUALITY_NONANTIALIASED);
#if CVPI_CAREFUL == 1
    cvpi_vg_error_check();
#endif
    for(; itter < img1_height; ++itter) {
      vgCopyImage(both, 0, 2*itter, img1, 0, itter, img1_width, 1, VG_FALSE);
#if CVPI_CAREFUL == 1
      cvpi_vg_error_check();
#endif
      vgCopyImage(both, 0, 2*itter+1, img2, 0, itter, img1_width, 1, VG_FALSE);
#if CVPI_CAREFUL == 1
      cvpi_vg_error_check();
#endif
      vgFinish();
#if CVPI_CAREFUL == 1
      cvpi_vg_error_check();
#endif
    }

    /* add */
    vgConvolveNormal(added, both, 1, 2, 0, 0, kernel, scale, bias, VG_TILE_PAD);
#if CVPI_CAREFUL == 1
    cvpi_vg_error_check();
#endif
    vgFinish();
#if CVPI_CAREFUL == 1
    cvpi_vg_error_check();
#endif
    /* put the added rows into output */
    for(itter = 0; itter < img1_height; ++itter) {
      vgCopyImage(output, 0, itter, added, 0, 2*itter, img1_width, 1, VG_FALSE);
#if CVPI_CAREFUL == 1
      cvpi_vg_error_check();
#endif
      vgFinish();
#if CVPI_CAREFUL == 1
      cvpi_vg_error_check();
#endif
    }
  }
 TAKEDOWN:
  vgDestroyImageSafe(half_img1);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(half_img2);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(half_added);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(l_row_img);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(l_row_added);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(both);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(added);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(half_added);
  cvpi_vg_error_takedown();
  if(BADSTATE == 1) {
    vgDestroyImageSafe(output);
    cvpi_vg_error_takedown();
  }
  vgFlush();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_takedown();
#endif
  return output;
#undef TAKEDOWN
}

VGImage cvpi_channel_add(const VGImage image, VGImageChannel channel1, VGImageChannel channel2,
			  VGshort a, VGshort b, VGfloat scale,
			  VGfloat bias_r, VGfloat bias_g, VGfloat bias_b, VGfloat bias_a,
			  VGbitfield output_channels) {
#define TAKEDOWN cvpi_channel_add_takedown
  int BADSTATE = 0;
  VGImage output_image = VG_INVALID_HANDLE;

  /* channel1 and channel2 determin the C rows, output_channels
     determine the C columns */

  VGfloat matrix[20] = {
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0
  };

  /* set the matrix elements to add channels and output */
  if(output_channels & VG_RED) {
    matrix[0] = channel1 == VG_RED ? a : 0;
    matrix[0] += channel2 == VG_RED ? b : 0;
    matrix[4] = channel1 == VG_GREEN ? a : 0;
    matrix[4] += channel2 == VG_GREEN ? b : 0;
    matrix[8] = channel1 == VG_BLUE ? a : 0;
    matrix[8] += channel2 == VG_BLUE ? b : 0;
    matrix[12] = channel1 == VG_ALPHA ? a : 0;
    matrix[12] += channel2 == VG_ALPHA ? b : 0;

    matrix[16] = bias_r;
  }

  if(output_channels & VG_GREEN) {
    matrix[1] = channel1 == VG_RED ? a : 0;
    matrix[1] += channel2 == VG_RED ? b : 0;
    matrix[5] = channel1 == VG_GREEN ? a : 0;
    matrix[5] += channel2 == VG_GREEN ? b : 0;
    matrix[9] = channel1 == VG_BLUE ? a : 0;
    matrix[9] += channel2 == VG_BLUE ? b : 0;
    matrix[13] = channel1 == VG_ALPHA ? a : 0;
    matrix[13] += channel2 == VG_ALPHA ? b : 0;

    matrix[17] = bias_g;
  }

  if(output_channels & VG_BLUE) {
    matrix[2] = channel1 == VG_RED ? a : 0;
    matrix[2] += channel2 == VG_RED ? b : 0;
    matrix[6] = channel1 == VG_GREEN ? a : 0;
    matrix[6] += channel2 == VG_GREEN ? b : 0;
    matrix[10] = channel1 == VG_BLUE ? a : 0;
    matrix[10] += channel2 == VG_BLUE ? b : 0;
    matrix[14] = channel1 == VG_ALPHA ? a : 0;
    matrix[14] += channel2 == VG_ALPHA ? b : 0;

    matrix[18] = bias_b;
  }

  if(output_channels & VG_ALPHA) {
    matrix[3] = channel1 == VG_RED ? a : 0;
    matrix[3] += channel2 == VG_RED ? b : 0;
    matrix[7] = channel1 == VG_GREEN ? a : 0;
    matrix[7] += channel2 == VG_GREEN ? b : 0;
    matrix[11] = channel1 == VG_BLUE ? a : 0;
    matrix[11] += channel2 == VG_BLUE ? b : 0;
    matrix[15] = channel1 == VG_ALPHA ? a : 0;
    matrix[15] += channel2 == VG_ALPHA ? b : 0;

    matrix[19] = bias_a;
  }
  /* scale */
  matrix[0] *= scale;
  matrix[1] *= scale;
  matrix[2] *= scale;
  matrix[3] *= scale;
  matrix[4] *= scale;
  matrix[5] *= scale;
  matrix[6] *= scale;
  matrix[7] *= scale;
  matrix[8] *= scale;
  matrix[9] *= scale;
  matrix[10] *= scale;
  matrix[11] *= scale;
  matrix[12] *= scale;
  matrix[13] *= scale;
  matrix[14] *= scale;
  matrix[15] *= scale;

  /* set matrix elements to pass through non-output channels */
  if(!(output_channels & VG_RED)) {
    matrix[0] = 1;
  }

  if(!(output_channels & VG_GREEN)) {
    matrix[5] = 1;
  }

  if(!(output_channels & VG_BLUE)) {
    matrix[10] = 1;
  }

  if(!(output_channels & VG_ALPHA)) {
    matrix[15] = 1;
  }

  VGint image_width = vgGetParameteri(image, VG_IMAGE_WIDTH);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  VGint image_height = vgGetParameteri(image, VG_IMAGE_HEIGHT);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  output_image = vgCreateImage(CVPI_COLOR_SPACE, image_width, image_height,
				     VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();

  vgColorMatrixNormal(output_image, image, matrix);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif

 TAKEDOWN:
  if(BADSTATE) {
    vgDestroyImageSafe(output_image);
    cvpi_vg_error_takedown();
  }
  vgFlush();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_takedown();
#endif
  return output_image;

#undef TAKEDOWN
}

/* VGImage cvpi_channel_add_old(const VGImage image, VGImageChannel channel1, VGImageChannel channel2, */
/* 			  VGshort a, VGshort b, VGfloat scale, VGfloat bias, VGbitfield output_channels) { */
/* #define TAKEDOWN cvpi_channel_add_old_takedown */
/*   int BADSTATE = 0; */
/*   /\* Could rewrite this function to do the computation just using vgColorMatrix *\/ */
/*   VGErrorCode error; */
/*   VGint image_width = vgGetParameteri(image, VG_IMAGE_WIDTH); */
/* #if CVPI_CAREFUL == 1 */
/*   error = vgGetError(); */
/*   if(error != VG_NO_ERROR) { */
/*     cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error)); */
/*     return VG_INVALID_HANDLE; */
/*   } */
/* #endif */
/*   VGint image_height = vgGetParameteri(image, VG_IMAGE_HEIGHT); */
/* #if CVPI_CAREFUL == 1 */
/*   error = vgGetError(); */
/*   if(error != VG_NO_ERROR) { */
/*     cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error)); */
/*     return VG_INVALID_HANDLE; */
/*   } */
/* #endif */
/*   VGImage channel1_img = vgCreateImage(CVPI_COLOR_SPACE, image_width, image_height, VG_IMAGE_QUALITY_NONANTIALIASED); */
/*       error = vgGetError(); */
/*       if(error != VG_NO_ERROR) { */
/* 	cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error)); */
/* 	return VG_INVALID_HANDLE; */
/*       } */
/*   VGImage channel2_img = vgCreateImage(CVPI_COLOR_SPACE, image_width, image_height, VG_IMAGE_QUALITY_NONANTIALIASED); */
/*       error = vgGetError(); */
/*       if(error != VG_NO_ERROR) { */
/* 	cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error)); */
/* 	vgDestroyImageSafe(channel1_img); */
/* 	vgFlush(); */
/* 	return VG_INVALID_HANDLE; */
/*       } */
/*   /\* Copy the channel to be added to all of the other channels *\/ */
/*   VGfloat copy_channel1[20] = { */
/*     0,0,0,0, */
/*     0,0,0,0, */
/*     0,0,0,0, */
/*     0,0,0,0, */
/*     0,0,0,0 */
/*   }; */

/*   VGfloat copy_channel2[20] = { */
/*     0,0,0,0, */
/*     0,0,0,0, */
/*     0,0,0,0, */
/*     0,0,0,0, */
/*     0,0,0,0 */
/*   }; */

/*   switch(channel1) { */
/*   case VG_RED: */
/*     copy_channel1[0] = 1; */
/*     copy_channel1[1] = 1; */
/*     copy_channel1[2] = 1; */
/*     copy_channel1[3] = 1; */
/*     break; */
/*   case VG_GREEN: */
/*     copy_channel1[4] = 1; */
/*     copy_channel1[5] = 1; */
/*     copy_channel1[6] = 1; */
/*     copy_channel1[7] = 1; */
/*     break; */
/*   case VG_BLUE: */
/*     copy_channel1[8] = 1; */
/*     copy_channel1[9] = 1; */
/*     copy_channel1[10] = 1; */
/*     copy_channel1[11] = 1; */
/*     break; */
/*   case VG_ALPHA: */
/*     copy_channel1[12] = 1; */
/*     copy_channel1[13] = 1; */
/*     copy_channel1[14] = 1; */
/*     copy_channel1[15] = 1; */
/*     break; */
/*   default: */
/*     break; */
/*   } */

/*   switch(channel2) { */
/*   case VG_RED: */
/*     copy_channel2[0] = 1; */
/*     copy_channel2[1] = 1; */
/*     copy_channel2[2] = 1; */
/*     copy_channel2[3] = 1; */
/*     break; */
/*   case VG_GREEN: */
/*     copy_channel2[4] = 1; */
/*     copy_channel2[5] = 1; */
/*     copy_channel2[6] = 1; */
/*     copy_channel2[7] = 1; */
/*     break; */
/*   case VG_BLUE: */
/*     copy_channel2[8] = 1; */
/*     copy_channel2[9] = 1; */
/*     copy_channel2[10] = 1; */
/*     copy_channel2[11] = 1; */
/*     break; */
/*   case VG_ALPHA: */
/*     copy_channel2[12] = 1; */
/*     copy_channel2[13] = 1; */
/*     copy_channel2[14] = 1; */
/*     copy_channel2[15] = 1; */
/*     break; */
/*   default: */
/*     break; */
/*   } */

/*   vgColorMatrix(channel1_img, image, copy_channel1); */
/*   vgColorMatrix(channel2_img, image, copy_channel2); */
/*   vgFinish(); */

/*   /\* add the channels *\/ */
/*   VGImage result = cvpi_image_add(channel1_img, channel2_img, a, b, scale, bias); */
/*   vgFinish(); */
/*       if(result == VG_INVALID_HANDLE) { */
/* 	cvpi_log_1("%s:%d:Unable to add channels\n", __func__, __LINE__); */
/* 	vgDestroyImageSafe(channel1_img); */
/* 	vgDestroyImageSafe(channel2_img); */
/* 	vgFlush(); */
/* 	return VG_INVALID_HANDLE; */
/*       } */
/*   if(output_channels == (VG_RED | VG_GREEN | VG_BLUE | VG_ALPHA) || result == VG_INVALID_HANDLE) { */
/*     vgDestroyImageSafe(channel1_img); */
/*     vgDestroyImageSafe(channel2_img); */
/*     vgFlush(); */
/*     return result; */
/*   } else { */
/*     VGImage result_combo = cvpi_image_combine_channelwise(result, image, output_channels); */
/*     vgDestroyImageSafe(channel1_img); */
/*     vgDestroyImageSafe(channel2_img); */
/*     vgDestroyImageSafe(result); */
/*     vgFlush(); */
/*     return result_combo; */
/*   } */
/* #undef TAKEDOWN */
/* } */

VGImage cvpi_color_channels_add(const VGImage image, VGfloat scale_r, VGfloat scale_g,
				VGfloat scale_b,
				VGfloat bias_r, VGfloat bias_g, VGfloat bias_b,
				VGbitfield output_channels) {
#define TAKEDOWN cvpi_color_channels_add_takedown
  int BADSTATE = 0;
  VGErrorCode error;
  VGfloat matrix[20] = {
    1,0,0,0,
    0,1,0,0,
    0,0,1,0,
    0,0,0,1,
    0,0,0,0
  };

  if(output_channels & VG_RED) {
    matrix[0] = scale_r;
    matrix[4] = scale_g;
    matrix[8] = scale_b;
    matrix[16] = bias_r;
  }
  if(output_channels & VG_GREEN) {
    matrix[1] = scale_r;
    matrix[5] = scale_g;
    matrix[9] = scale_b;
    matrix[17] = bias_g;
  }
  if(output_channels & VG_BLUE) {
    matrix[2] = scale_r;
    matrix[6] = scale_g;
    matrix[10] = scale_b;
    matrix[18] = bias_b;
  }
  if(output_channels & VG_ALPHA) {
    matrix[3] = scale_r;
    matrix[7] = scale_g;
    matrix[11] = scale_b;
    matrix[15] = 0;
    matrix[19] = 0;
  }

  VGImage output = VG_INVALID_HANDLE;

  VGint image_width = vgGetParameteri(image, VG_IMAGE_WIDTH);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  VGint image_height = vgGetParameteri(image, VG_IMAGE_HEIGHT);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  output = vgCreateImage(CVPI_COLOR_SPACE, image_width, image_height,
				 VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();
  vgColorMatrixNormal(output, image, matrix);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  vgFinish();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif

 TAKEDOWN:
  if(BADSTATE) {
    /* set output image to  */
    vgDestroyImageSafe(output);
    cvpi_vg_error_takedown();
  }
  vgFlush();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_takedown();
#endif
  return output;
#undef TAKEDOWN
}

VGImage cvpi_all_channels_add(const VGImage image, VGfloat scale_r, VGfloat scale_g,
			      VGfloat scale_b, VGfloat scale_a,
			      VGfloat bias_r, VGfloat bias_g, VGfloat bias_b, VGfloat bias_a,
			      VGbitfield output_channels) {
#define TAKEDOWN cvpi_all_channels_add_takedown
  int BADSTATE = 0;

  VGfloat matrix[20] = {
    1,0,0,0,
    0,1,0,0,
    0,0,1,0,
    0,0,0,1,
    0,0,0,0
  };

  if(output_channels & VG_RED) {
    matrix[0] = scale_r;
    matrix[4] = scale_g;
    matrix[8] = scale_b;
    matrix[12] = scale_a;
    matrix[16] = bias_r;
  }
  if(output_channels & VG_GREEN) {
    matrix[1] = scale_r;
    matrix[5] = scale_g;
    matrix[9] = scale_b;
    matrix[13] = scale_a;
    matrix[17] = bias_g;
  }
  if(output_channels & VG_BLUE) {
    matrix[2] = scale_r;
    matrix[6] = scale_g;
    matrix[10] = scale_b;
    matrix[14] = scale_a;
    matrix[18] = bias_b;
  }
  if(output_channels & VG_ALPHA) {
    matrix[3] = scale_r;
    matrix[7] = scale_g;
    matrix[11] = scale_b;
    matrix[15] = scale_a;
    matrix[19] = bias_a;
  }
  VGImage output = VG_INVALID_HANDLE;
  VGint image_width = vgGetParameteri(image, VG_IMAGE_WIDTH);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  VGint image_height = vgGetParameteri(image, VG_IMAGE_HEIGHT);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  output = vgCreateImage(CVPI_COLOR_SPACE, image_width, image_height, VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();

  vgColorMatrixNormal(output, image, matrix);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
 TAKEDOWN:
  if(BADSTATE) {
    /* set output image to  */
    vgDestroyImageSafe(output);
    cvpi_vg_error_takedown();
  }
  vgFlush();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_takedown();
#endif

  return output;
#undef TAKEDOWN
}

VGImage cvpi_image_combine_channelwise(const VGImage img1, const VGImage img2, VGbitfield img1_channels) {
#define TAKEDOWN cvpi_image_combine_channelwise_takedown
  int BADSTATE = 0;
  VGImage img1_filtered = VG_INVALID_HANDLE;
  VGImage img2_filtered = VG_INVALID_HANDLE;
  VGImage output = VG_INVALID_HANDLE;
  VGint img1_width = vgGetParameteri(img1, VG_IMAGE_WIDTH);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  VGint img1_height = vgGetParameteri(img1, VG_IMAGE_HEIGHT);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  VGint img2_width = vgGetParameteri(img2, VG_IMAGE_WIDTH);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  VGint img2_height = vgGetParameteri(img2, VG_IMAGE_HEIGHT);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif

  if(img1_width != img2_width || img1_height != img2_height) {
    cvpi_log_6("Images have differing dimensions:",
	       __func__, __LINE__, img1_width, img1_height, img2_width, img2_height);
    BADSTATE = 1;
    goto TAKEDOWN;
  }

  VGbitfield img2_channels = (VG_RED | VG_GREEN | VG_BLUE | VG_ALPHA) ^ img1_channels;
  const VGfloat img1_channel_matrix[20] = {
    !!(VG_RED & img1_channels), 0, 0, 0,
    0, !!(VG_GREEN & img1_channels), 0, 0,
    0, 0, !!(VG_BLUE & img1_channels), 0,
    0, 0, 0, !!(VG_ALPHA & img1_channels),

    0,0,0,0
  };

  const VGfloat img2_channel_matrix[20] = {
    !!(VG_RED & img2_channels), 0, 0, 0,
    0, !!(VG_GREEN & img2_channels), 0, 0,
    0, 0, !!(VG_BLUE & img2_channels), 0,
    0, 0, 0, !!(VG_ALPHA & img2_channels),

    0,0,0,0
  };

  img1_filtered = vgCreateImage(CVPI_COLOR_SPACE, img1_width, img1_height,
				VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();

  img2_filtered = vgCreateImage(CVPI_COLOR_SPACE, img2_width, img2_height,
				VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();

  vgColorMatrix(img1_filtered, img1, img1_channel_matrix);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  vgColorMatrix(img2_filtered, img2, img2_channel_matrix);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  vgFinish();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  output = cvpi_image_add(img1_filtered, img2_filtered, 1, 1, 1, 0);
  cvpi_image_error_check(output);

 TAKEDOWN:
  vgDestroyImageSafe(img1_filtered);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(img2_filtered);
  cvpi_vg_error_takedown();
  if(BADSTATE) {
    /* set output image to  */
    vgDestroyImageSafe(output);
    cvpi_vg_error_takedown();
  }
  vgFlush();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_takedown();
#endif
  return output;
#undef TAKEDOWN
}

VGImage cvpi_channel_threshold(const VGImage image, VGImageChannel channel,
			       VGubyte bound1, VGubyte bound2,
			       VGubyte fill, CVPI_BOOL invert) {
#define TAKEDOWN cvpi_channel_threshold_takedown
  int BADSTATE = 0;

  VGImage output = VG_INVALID_HANDLE;
  VGubyte upper = bound1 > bound2 ? bound1 : bound2;
  VGubyte lower = bound1 > bound2 ? bound2 : bound1;
  VGubyte threshold_array[256];

  if(CVPI_TRUE_TEST(invert)) {
    /* keep values outside the bounds and fill the others */
    for(unsigned int itter = 256; itter != 0;) {
      --itter;
      if(itter < lower || itter > upper) {
	threshold_array[itter] = itter;
      } else {
	threshold_array[itter] = fill;
      }
    }
  } else {
    /* keep values inside the bounds and fill the others */
    for(unsigned int itter = 256; itter != 0;) {
      --itter;
      if(itter < lower || itter > upper) {
	threshold_array[itter] = fill;
      } else {
	threshold_array[itter] = itter;
      }
    }
  }
  VGint img1_width = vgGetParameteri(image, VG_IMAGE_WIDTH);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  VGint img1_height = vgGetParameteri(image, VG_IMAGE_HEIGHT);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  output = vgCreateImage(CVPI_COLOR_SPACE, img1_width, img1_height, VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();

  const VGubyte* red_array = VG_RED == channel ? threshold_array : cvpi_identity_array;
  const VGubyte* green_array = VG_GREEN == channel ? threshold_array : cvpi_identity_array;
  const VGubyte* blue_array = VG_BLUE == channel ? threshold_array : cvpi_identity_array;
  const VGubyte* alpha_array = VG_ALPHA == channel ? threshold_array : cvpi_identity_array;

  vgLookup(output, image,
	   red_array,
	   green_array,
	   blue_array,
	   alpha_array,
	   OUTPUT_LINEAR, VG_FALSE);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  vgFinish();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
 TAKEDOWN:
  if(BADSTATE) {
    /* set output image to  */
    vgDestroyImageSafe(output);
    cvpi_vg_error_takedown();
  }
  vgFlush();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_takedown();
#endif
  return output;
#undef TAKEDOWN
}


VGImage cvpi_image_threshold(const VGImage image,
			     const VGubyte* lower_bound, const VGubyte* upper_bound,
			     const VGubyte* fill, const CVPI_BOOL* invert, CVPI_BOOL dependent) {
#define TAKEDOWN cvpi_image_threshold_takedown
  int BADSTATE = 0;
  VGImage output = VG_INVALID_HANDLE;
  VGubyte threshold_array[4][256];

  if(CVPI_TRUE_TEST(dependent)) {
    int inv = CVPI_TRUE_TEST(invert[0])
      || CVPI_TRUE_TEST(invert[1])
      || CVPI_TRUE_TEST(invert[2])
      || CVPI_TRUE_TEST(invert[3]);
    for(unsigned int i = 256; i != 0;) {
      --i;
      if(inv) {
	if(i < lower_bound[0] || i < lower_bound[1]
	   || i < lower_bound[2] || i < lower_bound[3]
	   || i > upper_bound[0] || i > upper_bound[1]
	   || i > upper_bound[2] || i > upper_bound[3]) {
	  threshold_array[0][i] = i;
	  threshold_array[1][i] = i;
	  threshold_array[2][i] = i;
	  threshold_array[3][i] = i;
	} else {
	  threshold_array[0][i] = fill[0];
	  threshold_array[1][i] = fill[1];
	  threshold_array[2][i] = fill[2];
	  threshold_array[3][i] = fill[3];
	}
      } else {
	if(i < lower_bound[0] || i < lower_bound[1]
	   || i < lower_bound[2] || i < lower_bound[3]
	   || i > upper_bound[0] || i > upper_bound[1]
	   || i > upper_bound[2] || i > upper_bound[3]) {
	  threshold_array[0][i] = fill[0];
	  threshold_array[1][i] = fill[1];
	  threshold_array[2][i] = fill[2];
	  threshold_array[3][i] = fill[3];
	} else {
	  threshold_array[0][i] = i;
	  threshold_array[1][i] = i;
	  threshold_array[2][i] = i;
	  threshold_array[3][i] = i;
	}
      }
    }
  } else {
    unsigned char j;
    for(unsigned int i = 256; i != 0;) {
      --i;
      for(j = 4; j != 0;) {
	--j;
	if(CVPI_TRUE_TEST(invert[j])) {
	  if(i < lower_bound[j] || i > upper_bound[j]) {
	    threshold_array[j][i] = i;
	  } else {
	    threshold_array[j][i] = fill[j];
	  }
	} else {
	  if(i < lower_bound[j] || i > upper_bound[j]) {
	    threshold_array[j][i] = fill[j];
	  } else {
	    threshold_array[j][i] = i;
	  }
	}
      }
    }
  }

  VGint img1_width = vgGetParameteri(image, VG_IMAGE_WIDTH);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  VGint img1_height = vgGetParameteri(image, VG_IMAGE_HEIGHT);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  output = vgCreateImage(CVPI_COLOR_SPACE, img1_width, img1_height, VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();

  vgLookup(output, image,
	   threshold_array[0],
	   threshold_array[1],
	   threshold_array[2],
	   threshold_array[3],
	   OUTPUT_LINEAR, VG_FALSE);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  vgFinish();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
 TAKEDOWN:
  if(BADSTATE) {
    /* set output image to  */
    vgDestroyImageSafe(output);
    cvpi_vg_error_takedown();
  }
  vgFlush();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_takedown();
#endif
  return output;
#undef TAKEDOWN
}

VGImage cvpi_image_threshold_sector(const VGImage image, unsigned int sector_width,
				    unsigned int sector_height, const VGubyte* fill,
				    const CVPI_BOOL* invert, CVPI_BOOL dependent,
				    const VGuint (*statistic)(const VGImage image)) {
#define TAKEDOWN cvpi_image_threshold_sector_takedown
  int BADSTATE = 0;
  /* If invert is true, lower color bound of what is being inverted,
     0's in all channels. If invert is true, lower bound of what is
     being kept.  */
  VGubyte zero_array[4] = {0,0,0,0};
  VGImage child = VG_INVALID_HANDLE;
  VGImage output_image = VG_INVALID_HANDLE;
  VGImage sector_image = VG_INVALID_HANDLE;

  VGint image_width = vgGetParameteri(image, VG_IMAGE_WIDTH);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  VGint image_height = vgGetParameteri(image, VG_IMAGE_HEIGHT);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  if(sector_width > image_width) {
    sector_width = image_width;
  } else if(sector_width == 0) {
    sector_width = 1;
  }

  if(sector_height > image_height) {
    sector_height = image_height;
  } else if(sector_height == 0) {
    sector_height = 1;
  }

  /* width of sectors on the right side of the input image */
  int thinness = image_width % sector_width;

  /* height of sectors on the bottom side of the input image */
  int shortness = image_height % sector_height;

  /* Number of sectors with a height < sector_height */
  int short_sector_num = image_height / sector_height;
  /* Number of sectors with a width < sector_width */
  int thin_sector_num = image_width / sector_width;

  output_image = vgCreateImage(CVPI_COLOR_SPACE, image_width, image_height,
			       VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();

  /* to hold the statistic */
  cvpi_pixel sector_threshold;

  sector_threshold.all = 0;

  int i;
  int j;
  /* threshold normal sectors */
  for(i = short_sector_num; i != 0;) {
    --i;
    for(j = thin_sector_num; j != 0;) {
      --j;
      /* get the sector mean and store it in sector_threshold */
      child = vgChildImage(image, i*sector_width, j*sector_height,
			   sector_width, sector_height);
#if CVPI_CAREFUL == 1
      cvpi_vg_error_check();
#endif
      sector_threshold.all = statistic(child);
      vgFinish();
#if CVPI_CAREFUL == 1
      cvpi_vg_error_check();
#endif
      /* threshold the sector and insert it into the output image */
      child = vgChildImage(image, i*sector_width, j*sector_height,
			   sector_width, sector_height);
#if CVPI_CAREFUL == 1
      cvpi_vg_error_check();
#endif
      sector_image = cvpi_image_threshold(child,
					  zero_array,
					  sector_threshold.channel,
					  fill, invert, dependent);
      if(sector_image == VG_INVALID_HANDLE) {
	cvpi_log_1("Unable to threshold sector", __FILE__, __LINE__);
	BADSTATE = 1;
	goto TAKEDOWN;
      }
      vgCopyImage(output_image, i*sector_width, j*sector_height,
		  sector_image,
		  0, 0, sector_width, sector_height, VG_FALSE);
#if CVPI_CAREFUL == 1
      cvpi_vg_error_check();
#endif
      vgDestroyImageSafe(sector_image);
      cvpi_vg_error_check();
      vgFinish();
#if CVPI_CAREFUL == 1
      cvpi_vg_error_check();
#endif
    }
  }

  /* threshold right sectors */
  for(j = thin_sector_num; j != 0;) {
    --j;
    child = vgChildImage(image, image_width - thinness,
			 j*sector_height, thinness, sector_height);
#if CVPI_CAREFUL == 1
      cvpi_vg_error_check();
#endif
    sector_threshold.all = statistic(child);

    child = vgChildImage(image, image_width - thinness, j*sector_height,
			 thinness, sector_height);
#if CVPI_CAREFUL == 1
      cvpi_vg_error_check();
#endif
    sector_image = cvpi_image_threshold(child,
					zero_array,
					sector_threshold.channel,
					fill, invert, dependent);
    if(sector_image == VG_INVALID_HANDLE) {
      cvpi_log_1("Unable to threshold sector", __FILE__, __LINE__);
      BADSTATE = 1;
      goto TAKEDOWN;
    }
    vgCopyImage(output_image, image_width - thinness, j*sector_height,
		sector_image,
		0, 0, thinness, sector_height, VG_FALSE);
#if CVPI_CAREFUL == 1
    cvpi_vg_error_check();
#endif
    vgDestroyImageSafe(sector_image);
    cvpi_vg_error_check();

    vgFinish();
    cvpi_vg_error_check();
  }

  /* threshold bottom sectors */
  for(i = short_sector_num; i != 0 ;) {
    --i;
    child = vgChildImage(image, i*sector_width, image_height - shortness,
			 sector_width, shortness);
#if CVPI_CAREFUL == 1
    cvpi_vg_error_check();
#endif
    sector_threshold.all = statistic(child);

    child = vgChildImage(image, i*sector_width, image_height - shortness,
			 sector_width, shortness);
#if CVPI_CAREFUL == 1
    cvpi_vg_error_check();
#endif
    sector_image = cvpi_image_threshold(child,
					zero_array,
					sector_threshold.channel,
					fill, invert, dependent);
    if(sector_image == VG_INVALID_HANDLE) {
      cvpi_log_1("Unable to threshold sector", __FILE__, __LINE__);
      BADSTATE = 1;
      goto TAKEDOWN;
    }
    vgCopyImage(output_image, i*sector_width, image_height - shortness,
		sector_image, 0, 0, sector_width, shortness, VG_FALSE);
#if CVPI_CAREFUL == 1
    cvpi_vg_error_check();
#endif
    vgDestroyImageSafe(sector_image);
    cvpi_vg_error_check();
    vgFinish();
    cvpi_vg_error_check();
  }

  /* threshold the bottom right sector */
  child = vgChildImage(image, image_width - thinness, image_height - shortness,
		       thinness, shortness);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  sector_threshold.all = statistic(child);

  child = vgChildImage(image, image_width - thinness, image_height - shortness,
		       thinness, shortness);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  sector_image = cvpi_image_threshold(child,
				      zero_array,
				      sector_threshold.channel,
				      fill, invert, dependent);
  if(sector_image == VG_INVALID_HANDLE) {
    cvpi_log_1("Unable to threshold sector", __FILE__, __LINE__);
    BADSTATE = 1;
    goto TAKEDOWN;
  }
  vgCopyImage(output_image, image_width - thinness, image_height - shortness,
	      sector_image, 0, 0, thinness, shortness, VG_FALSE);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  vgDestroyImageSafe(sector_image);
  cvpi_vg_error_check();
  vgFinish();
  cvpi_vg_error_check();

 TAKEDOWN:
  vgDestroyImageSafe(sector_image);
  cvpi_vg_error_takedown();
  if(BADSTATE == 1) {
    vgDestroyImageSafe(output_image);
    cvpi_vg_error_takedown();
  }
  vgFlush();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_takedown();
#endif

  return output_image;
#undef TAKEDOWN
}

VGImage cvpi_channel_threshold_sector(const VGImage image, VGImageChannel channel,
				      unsigned int sector_width, unsigned int sector_height,
				      const VGubyte fill, const CVPI_BOOL invert,
				      const VGubyte (*statistic)(const VGImage image)) {
#define TAKEDOWN cvpi_channel_threshold_sector_takedown
  int BADSTATE = 0;
  /* If invert is true, lower color bound of what is being inverted,
     0's in all channels. If invert is true, lower bound of what is
     being kept.  */
  VGImage child = VG_INVALID_HANDLE;
  VGImage output_image = VG_INVALID_HANDLE;
  VGImage sector_image = VG_INVALID_HANDLE;

  VGint image_width = vgGetParameteri(image, VG_IMAGE_WIDTH);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  VGint image_height = vgGetParameteri(image, VG_IMAGE_HEIGHT);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  if(sector_width > image_width) {
    sector_width = image_width;
  } else if(sector_width == 0) {
    sector_width = 1;
  }

  if(sector_height > image_height) {
    sector_height = image_height;
  } else if(sector_height == 0) {
    sector_height = 1;
  }

  /* width of sectors on the right side of the input image */
  int thinness = image_width % sector_width;

  /* height of sectors on the bottom side of the input image */
  int shortness = image_height % sector_height;

  /* Number of sectors with a height < sector_height */
  int short_sector_num = image_height / sector_height;
  /* Number of sectors with a width < sector_width */
  int thin_sector_num = image_width / sector_width;

  output_image = vgCreateImage(CVPI_COLOR_SPACE, image_width, image_height,
			       VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();

  /* to hold the statistic */
  VGubyte sector_threshold = 0;
  int i;
  int j;
  /* threshold normal sectors */
  for(i = short_sector_num; i != 0;) {
    --i;
    for(j = thin_sector_num; j != 0;) {
      --j;
      /* get the sector mean and store it in sector_threshold */
      child = vgChildImage(image, i*sector_width, j*sector_height,
			   sector_width, sector_height);
#if CVPI_CAREFUL == 1
      cvpi_vg_error_check();
#endif
      sector_threshold = statistic(child);
      vgFinish();
#if CVPI_CAREFUL == 1
      cvpi_vg_error_check();
#endif
      /* threshold the sector and insert it into the output image */
      child = vgChildImage(image, i*sector_width, j*sector_height,
			   sector_width, sector_height);
#if CVPI_CAREFUL == 1
      cvpi_vg_error_check();
#endif
      sector_image = cvpi_channel_threshold(child, channel,
					    0,
					    sector_threshold,
					    fill, invert);
      if(sector_image == VG_INVALID_HANDLE) {
	cvpi_log_1("Unable to threshold sector", __FILE__, __LINE__);
	BADSTATE = 1;
	goto TAKEDOWN;
      }
      vgCopyImage(output_image, i*sector_width, j*sector_height,
		  sector_image,
		  0, 0, sector_width, sector_height, VG_FALSE);
#if CVPI_CAREFUL == 1
      cvpi_vg_error_check();
#endif
      vgDestroyImageSafe(sector_image);
      cvpi_vg_error_check();
      vgFinish();
#if CVPI_CAREFUL == 1
      cvpi_vg_error_check();
#endif
    }
  }

  /* threshold right sectors */
  for(j = thin_sector_num; j != 0;) {
    --j;
    child = vgChildImage(image, image_width - thinness,
			 j*sector_height, thinness, sector_height);
#if CVPI_CAREFUL == 1
      cvpi_vg_error_check();
#endif
    sector_threshold = statistic(child);

    child = vgChildImage(image, image_width - thinness, j*sector_height,
			 thinness, sector_height);
#if CVPI_CAREFUL == 1
    cvpi_vg_error_check();
#endif
    sector_image = cvpi_channel_threshold(child, channel,
					  0, sector_threshold,
					  fill, invert);
    if(sector_image == VG_INVALID_HANDLE) {
      cvpi_log_1("Unable to threshold sector", __FILE__, __LINE__);
      BADSTATE = 1;
      goto TAKEDOWN;
    }
    vgCopyImage(output_image, image_width - thinness, j*sector_height,
		sector_image,
		0, 0, thinness, sector_height, VG_FALSE);
#if CVPI_CAREFUL == 1
    cvpi_vg_error_check();
#endif
    vgDestroyImageSafe(sector_image);
    cvpi_vg_error_check();

    vgFinish();
    cvpi_vg_error_check();
  }

  /* threshold bottom sectors */
  for(i = short_sector_num; i != 0;) {
    --i;
    child = vgChildImage(image, i*sector_width, image_height - shortness,
			 sector_width, shortness);
#if CVPI_CAREFUL == 1
    cvpi_vg_error_check();
#endif
    sector_threshold = statistic(child);

    child = vgChildImage(image, i*sector_width, image_height - shortness,
			 sector_width, shortness);
#if CVPI_CAREFUL == 1
    cvpi_vg_error_check();
#endif
    sector_image = cvpi_channel_threshold(child, channel,
					  0, sector_threshold,
					  fill, invert);
    if(sector_image == VG_INVALID_HANDLE) {
      cvpi_log_1("Unable to threshold sector", __FILE__, __LINE__);
      BADSTATE = 1;
      goto TAKEDOWN;
    }
    vgCopyImage(output_image, i*sector_width, image_height - shortness,
		sector_image, 0, 0, sector_width, shortness, VG_FALSE);
#if CVPI_CAREFUL == 1
    cvpi_vg_error_check();
#endif
    vgDestroyImageSafe(sector_image);
    cvpi_vg_error_check();
    vgFinish();
    cvpi_vg_error_check();
  }

  /* threshold the bottom right sector */
  child = vgChildImage(image, image_width - thinness, image_height - shortness,
		       thinness, shortness);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  sector_threshold = statistic(child);

  child = vgChildImage(image, image_width - thinness, image_height - shortness,
		       thinness, shortness);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  sector_image = cvpi_channel_threshold(child, channel,
				      0, sector_threshold,
				      fill, invert);
  if(sector_image == VG_INVALID_HANDLE) {
    cvpi_log_1("Unable to threshold sector", __FILE__, __LINE__);
    BADSTATE = 1;
    goto TAKEDOWN;
  }
  vgCopyImage(output_image, image_width - thinness, image_height - shortness,
	      sector_image, 0, 0, thinness, shortness, VG_FALSE);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  vgDestroyImageSafe(sector_image);
  cvpi_vg_error_check();
  vgFinish();
  cvpi_vg_error_check();

   TAKEDOWN:
  vgDestroyImageSafe(sector_image);
  cvpi_vg_error_takedown();
  if(BADSTATE == 1) {
    vgDestroyImageSafe(output_image);
    cvpi_vg_error_takedown();
  }
  vgFlush();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_takedown();
#endif

  return output_image;
#undef TAKEDOWN
}

VGImage cvpi_image_threshold_adaptive_mean(const VGImage image, VGshort kernel_size,
					   CVPI_BOOL above, CVPI_BOOL white, CVPI_BOOL dependent,
					   VGfloat bias, VGTilingMode tilingMode) {
#define TAKEDOWN cvpi_image_threshold_adaptive_mean_takedown
  int BADSTATE = 0;

  VGshort* kernel = NULL;

  VGImage mean_image = VG_INVALID_HANDLE;
  VGImage difference_image = VG_INVALID_HANDLE;
  VGImage thresholded_image = VG_INVALID_HANDLE;

  /* these images are used only if channels are dependent */
  VGImage red_image = VG_INVALID_HANDLE;
  VGImage green_image = VG_INVALID_HANDLE;
  VGImage blue_image = VG_INVALID_HANDLE;
  VGImage alpha_image = VG_INVALID_HANDLE;
  VGImage AND1_image = VG_INVALID_HANDLE;
  VGImage AND2_image = VG_INVALID_HANDLE;
  VGImage AND3_image = VG_INVALID_HANDLE;

  unsigned int kernel_2 = kernel_size * kernel_size;
  VGfloat scale = 1.0/kernel_2;

  VGint max_kernel_size = vgGetParameteri(image, VG_MAX_KERNEL_SIZE);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif

  if(kernel_size < 1 || kernel_2 > max_kernel_size) {
    cvpi_log_1("bad kernel size", __FILE__, __LINE__);
    BADSTATE = 1;
    goto TAKEDOWN;
  }

  VGint image_width = vgGetParameteri(image, VG_IMAGE_WIDTH);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  VGint image_height = vgGetParameteri(image, VG_IMAGE_HEIGHT);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif

  kernel = malloc(sizeof(*kernel) * kernel_2);
  if(kernel == NULL) {
    cvpi_log_3("malloc returned NULL: errno = ", __FILE__, __LINE__, errno);
    BADSTATE = 1;
    goto TAKEDOWN;
  }

  mean_image = vgCreateImage(CVPI_COLOR_SPACE, image_width, image_height,
			     VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();

  for(unsigned int i = kernel_2; i != 0;) {
    kernel[--i] = 1;
  }

  vgConvolveNoShift(mean_image, image, kernel_size, kernel_size,
		    kernel, scale, 0, tilingMode);
  cvpi_vg_error_check();

  vgFinish();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif

  if(kernel != NULL) {
    free(kernel);
    kernel = NULL;
  }

  if(CVPI_TRUE_TEST(above)) {
    /* keep stuff above threshold */
    difference_image = cvpi_image_add(image, mean_image, 1, -1, 1, bias);
  } else {     /* keep stuff below threshold */
    difference_image = cvpi_image_add(mean_image, image, 1, -1, 1, bias);
  }
  cvpi_image_error_check(difference_image);

  if(CVPI_TRUE_TEST(dependent)) {
    /* channels are dependent */

    red_image = vgCreateImage(CVPI_COLOR_SPACE, image_width, image_height,
			      VG_IMAGE_QUALITY_NONANTIALIASED);
    cvpi_vg_error_check();
    green_image = vgCreateImage(CVPI_COLOR_SPACE, image_width, image_height,
				VG_IMAGE_QUALITY_NONANTIALIASED);
    cvpi_vg_error_check();
    blue_image = vgCreateImage(CVPI_COLOR_SPACE, image_width, image_height,
			       VG_IMAGE_QUALITY_NONANTIALIASED);
    cvpi_vg_error_check();
    alpha_image = vgCreateImage(CVPI_COLOR_SPACE, image_width, image_height,
				VG_IMAGE_QUALITY_NONANTIALIASED);
    cvpi_vg_error_check();

    vgColorMatrix(red_image, difference_image, cvpi_channel_red);
#if CVPI_CAREFUL == 1
    cvpi_vg_error_check();
#endif
    vgColorMatrix(green_image, difference_image, cvpi_channel_green);
#if CVPI_CAREFUL == 1
    cvpi_vg_error_check();
#endif
    vgColorMatrix(blue_image, difference_image, cvpi_channel_blue);
#if CVPI_CAREFUL == 1
    cvpi_vg_error_check();
#endif
    vgColorMatrix(alpha_image, difference_image, cvpi_channel_alpha);
#if CVPI_CAREFUL == 1
    cvpi_vg_error_check();
#endif

    /* do three AND's */
    AND1_image = cvpi_image_logical_and(red_image, green_image, 255, 0, CVPI_TRUE);
    cvpi_image_error_check(AND1_image);
    AND2_image = cvpi_image_logical_and(blue_image, alpha_image, 255, 0, CVPI_TRUE);
    cvpi_image_error_check(AND2_image);
    AND3_image = cvpi_image_logical_and(AND1_image, AND2_image, 255, 0, CVPI_TRUE);
    cvpi_image_error_check(AND3_image);

    /* free memory */
    vgDestroyImageSafe(red_image);
    cvpi_vg_error_check();
    vgDestroyImageSafe(green_image);
    cvpi_vg_error_check();
    vgDestroyImageSafe(blue_image);
    cvpi_vg_error_check();
    vgDestroyImageSafe(alpha_image);
    cvpi_vg_error_check();
    vgDestroyImageSafe(AND1_image);
    cvpi_vg_error_check();
    vgDestroyImageSafe(AND2_image);
    cvpi_vg_error_check();


    /* map 0 -> 255, !0 -> 0 */
    /* reuse mean_image for mask */
    vgLookup(mean_image, AND3_image,
	     cvpi_binary_array_inverted, cvpi_binary_array_inverted,
	     cvpi_binary_array_inverted, cvpi_binary_array_inverted,
	     OUTPUT_LINEAR, VG_FALSE);
  } else { 			/* channels are independent */
    /* reuse mean_image for mask */
    vgLookup(mean_image, difference_image,
	     cvpi_binary_array_inverted, cvpi_binary_array_inverted,
	     cvpi_binary_array_inverted, cvpi_binary_array_inverted,
	     OUTPUT_LINEAR, VG_FALSE);
  }
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif

  if(CVPI_TRUE_TEST(white)) {
    /* add mask */
    thresholded_image = cvpi_image_add(image, mean_image, 1, 1, 1, 0);
  } else {
    /* subtract mask */
    thresholded_image = cvpi_image_add(image, mean_image, 1, -1, 1, 0);
  }
  cvpi_image_error_check(thresholded_image);

 TAKEDOWN:
  vgFinish();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_takedown();
#endif

  if(kernel != NULL) {
    free(kernel);
  }

  vgDestroyImageSafe(mean_image);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(difference_image);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(red_image);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(green_image);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(blue_image);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(alpha_image);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(AND1_image);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(AND2_image);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(AND3_image);
  cvpi_vg_error_takedown();

  if(BADSTATE) {
    vgDestroyImageSafe(thresholded_image);
    cvpi_vg_error_takedown();
  }
  vgFlush();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_takedown();
#endif

  return thresholded_image;
#undef TAKEDOWN
}

VGImage cvpi_image_mask_channel(const VGImage image, VGImageChannel maskChannel, CVPI_BOOL add) {
#define TAKEDOWN cvpi_image_mask_channel_takedown
  int BADSTATE = 0;
  VGImage output_image = VG_INVALID_HANDLE;
  VGfloat matrix[20] = {
    1,0,0,0,
    0,1,0,0,
    0,0,1,0,
    0,0,0,1,
    0,0,0,0
  };

  VGfloat mask_value = CVPI_TRUE_TEST(add) ? 1 : -1;


  switch(maskChannel) {
  case VG_RED:
    matrix[1] = mask_value;
    matrix[2] = mask_value;
    matrix[3] = mask_value;
    break;
  case VG_GREEN:
    matrix[4] = mask_value;
    matrix[6] = mask_value;
    matrix[7] = mask_value;
    break;
  case VG_BLUE:
    matrix[8] = mask_value;
    matrix[9] = mask_value;
    matrix[11] = mask_value;
    break;
  default:
    matrix[12] = mask_value;
    matrix[13] = mask_value;
    matrix[14] = mask_value;			/* alpha */
    break;
  }
  VGErrorCode error;
  VGint image_width = vgGetParameteri(image, VG_IMAGE_WIDTH);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  VGint image_height = vgGetParameteri(image, VG_IMAGE_HEIGHT);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  output_image = vgCreateImage(CVPI_COLOR_SPACE, image_width, image_height,
			       VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();

  vgColorMatrix(output_image, image, matrix);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  vgFinish();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif

 TAKEDOWN:
  if(BADSTATE) {
    vgDestroyImageSafe(output_image);
    cvpi_vg_error_takedown();
  }
  vgFlush();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_takedown();
#endif
  return output_image;
#undef TAKEDOWN
}

/* in testing this function did not run any faster than
   cvpi_image_mean_arithmetic, not a real statistic but does
   produce values close to the true mean. Could be used for the first
   few itterations and then the result passed to the CPU version.
*/
VGImage cvpi_image_mean_gpu(const VGImage image, int itterations) {
#define TAKEDOWN cvpi_image_mean_gpu_takedown
  int BADSTATE = 0;
  /* See project.org (heading: image mean) for detailed description of
     the algorithm being used. */
  VGImage first_image = VG_INVALID_HANDLE;
  VGImage mean_image = VG_INVALID_HANDLE;
  VGint image_width = vgGetParameteri(image, VG_IMAGE_WIDTH);

  ++itterations;		/* count stating from 1 */
  int itter = 1;

#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  VGint image_height = vgGetParameteri(image, VG_IMAGE_HEIGHT);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  VGshort kernel[2] = {1,1};

  first_image = vgCreateImage(CVPI_COLOR_SPACE, image_width, image_height,
			      VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();
  vgCopyImage(first_image, 0, 0, image, 0, 0, image_width, image_height, VG_FALSE);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  vgFinish();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif

  while(1 < image_height && itter != itterations) {
    ++itter;
    --image_height;
    mean_image = vgCreateImage(CVPI_COLOR_SPACE, image_width, image_height,
			       VG_IMAGE_QUALITY_NONANTIALIASED);
    cvpi_vg_error_check();

    vgConvolveNormal(mean_image, first_image, 1, 2, 0, 0, kernel, 0.5, 0, VG_TILE_PAD);
#if CVPI_CAREFUL == 1
    cvpi_vg_error_check();
#endif
    vgFinish();
#if CVPI_CAREFUL == 1
    cvpi_vg_error_check();
#endif
    vgDestroyImageSafe(first_image);
    cvpi_vg_error_check();
    vgFinish();
    cvpi_vg_error_check();
    first_image = vgCreateImage(CVPI_COLOR_SPACE, image_width, image_height,
				VG_IMAGE_QUALITY_NONANTIALIASED);
    cvpi_vg_error_check();

    /* remove the last row of the resulting image */

    vgCopyImage(first_image, 0, 0, mean_image, 0, 0, image_width, image_height, VG_FALSE);
#if CVPI_CAREFUL == 1
    cvpi_vg_error_check();
#endif
    vgFinish();
#if CVPI_CAREFUL == 1
    cvpi_vg_error_check();
#endif
    vgDestroyImageSafe(mean_image);
    cvpi_vg_error_check();
    vgFinish();
    cvpi_vg_error_check();
    /* Repeat until there is only one row left. Turn the row into a
       column and repeat the averaging until there is only one cell
       left. */
  }

  if(itterations < 0) {
    while(1 < image_width ) {
      --image_width;
      mean_image = vgCreateImage(CVPI_COLOR_SPACE, image_width, image_height,
				 VG_IMAGE_QUALITY_NONANTIALIASED);
      cvpi_vg_error_check();

      vgConvolveNormal(mean_image, first_image, 2, 1, 0, 0, kernel, 0.5, 0, VG_TILE_PAD);
#if CVPI_CAREFUL == 1
      cvpi_vg_error_check();
#endif
      vgFinish();
#if CVPI_CAREFUL == 1
      cvpi_vg_error_check();
#endif
      vgDestroyImageSafe(first_image);
      cvpi_vg_error_check();
      vgFinish();
      cvpi_vg_error_check();
      first_image = vgCreateImage(CVPI_COLOR_SPACE, image_width, image_height,
				  VG_IMAGE_QUALITY_NONANTIALIASED);
      cvpi_vg_error_check();
      vgFinish();
      cvpi_vg_error_check();
      vgCopyImage(first_image, 0, 0, mean_image, 0, 0, image_width, image_height, VG_FALSE);
#if CVPI_CAREFUL == 1
      cvpi_vg_error_check();
#endif
      vgFinish();
#if CVPI_CAREFUL == 1
      cvpi_vg_error_check();
#endif
      vgDestroyImageSafe(mean_image);
      cvpi_vg_error_check();
      vgFinish();
      cvpi_vg_error_check();
    }
  }

 TAKEDOWN:
  vgDestroyImageSafe(mean_image);
  cvpi_vg_error_takedown();
  if(BADSTATE == 1) {
    vgDestroyImageSafe(first_image);
    cvpi_vg_error_takedown();
  }
  vgFlush();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_takedown();
#endif

  return first_image;
#undef TAKEDOWN
}

VGImage cvpi_image_mean_arithmetic(const VGImage image) {
#define TAKEDOWN cvpi_image_mean_arithmetic_takedown
  int BADSTATE = 0;

  VGImage pixel = VG_INVALID_HANDLE;
  unsigned char* image_cpu = NULL;

  VGint width = vgGetParameteri(image, VG_IMAGE_WIDTH);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  VGint height = vgGetParameteri(image, VG_IMAGE_HEIGHT);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  VGImageFormat imageFormat = (VGImageFormat)vgGetParameteri(image, VG_IMAGE_FORMAT);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif

  VGint pixel_bytes = vgPixelBits(imageFormat)/8;

  unsigned long image_size = width * height;
  unsigned long image_mem_size = pixel_bytes * image_size;
  image_cpu = malloc(image_mem_size);
  if(image_cpu == NULL) {
    cvpi_log_3("malloc returned NULL: errno = ", __func__, __LINE__, errno);
    BADSTATE = 1;
    goto TAKEDOWN;
  }
  vgGetImageSubData(image, (void*)image_cpu, pixel_bytes, imageFormat, 0, 0, width, height);

  unsigned long average_channel[4] = {0,0,0,0};

  /* currently only works with VGImageFormat = VG_*_8888 */
  cvpi_pixel offsets;
  offsets.channel[0] = 0;
  offsets.channel[1] = 1;
  offsets.channel[2] = 2;
  offsets.channel[3] = 3;

  for(unsigned long i = 0; i < image_mem_size; i += pixel_bytes) {
    average_channel[0] += *(image_cpu + i + offsets.channel[0]);
    average_channel[1] += *(image_cpu + i + offsets.channel[1]);
    average_channel[2] += *(image_cpu + i + offsets.channel[2]);
    average_channel[3] += *(image_cpu + i + offsets.channel[3]);
  }

  average_channel[0] /= image_size;
  average_channel[1] /= image_size;
  average_channel[2] /= image_size;
  average_channel[3] /= image_size;

  /* reuse offsets */
  offsets.channel[0] = average_channel[0] > 255 ? 255 : average_channel[0];
  offsets.channel[1] = average_channel[1] > 255 ? 255 : average_channel[1];
  offsets.channel[2] = average_channel[2] > 255 ? 255 : average_channel[2];
  offsets.channel[3] = average_channel[3] > 255 ? 255 : average_channel[3];

  pixel = vgCreateImage(CVPI_COLOR_SPACE, 1, 1, VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();

  vgImageSubData(pixel, &offsets, CVPI_PIXEL_BYTES, CVPI_COLOR_SPACE, 0, 0, 1, 1);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  vgFinish();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
 TAKEDOWN:
  if(image_cpu != NULL) {
    free(image_cpu);
  }
  if(BADSTATE == 1) {
    vgDestroyImageSafe(pixel);
    cvpi_vg_error_takedown();
  }
  vgFlush();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_takedown();
#endif
  return pixel;
#undef TAKEDOWN
}


VGImage cvpi_image_rgba2bw(const VGImage image, VGImageChannel sourceChannel, CVPI_BOOL true_color) {
#define TAKEDOWN cvpi_image_rgba2bw_takedown
  int BADSTATE = 0;

  VGImage bw = VG_INVALID_HANDLE;
  VGImage intermediate = VG_INVALID_HANDLE;
  VGint img1_width = vgGetParameteri(image, VG_IMAGE_WIDTH);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  VGint img1_height = vgGetParameteri(image, VG_IMAGE_HEIGHT);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  VGImageFormat imageFormat = (VGImageFormat)vgGetParameteri(image, VG_IMAGE_FORMAT);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  bw = vgCreateImage(VG_BW_1, img1_width, img1_height, VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();

  intermediate = vgCreateImage(imageFormat, img1_width, img1_height, VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();

  const VGuint* array = CVPI_TRUE_TEST(true_color) ? cvpi_binary_array : cvpi_binary_array_inverted;
  switch(sourceChannel) {
  case VG_RED:
    vgLookup(intermediate, image, array, cvpi_zeros_array, cvpi_zeros_array, cvpi_zeros_array,
	     OUTPUT_LINEAR, VG_FALSE);
    break;
  case VG_GREEN:
    vgLookup(intermediate, image, cvpi_zeros_array, array, cvpi_zeros_array, cvpi_zeros_array,
	     OUTPUT_LINEAR, VG_FALSE);
    break;
  case VG_BLUE:
    vgLookup(intermediate, image, cvpi_zeros_array, cvpi_zeros_array, array, cvpi_zeros_array,
	     OUTPUT_LINEAR, VG_FALSE);
    break;
  default:
    vgLookup(intermediate, image, cvpi_zeros_array, cvpi_zeros_array, cvpi_zeros_array, array,
	     OUTPUT_LINEAR, VG_FALSE);
    break;
  }
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  vgCopyImage(bw, 0, 0, intermediate, 0, 0, img1_width, img1_height, VG_FALSE);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  vgFinish();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
 TAKEDOWN:

  vgDestroyImageSafe(intermediate);
  cvpi_vg_error_takedown();
  if(BADSTATE == 1) {
    vgDestroyImageSafe(bw);
    cvpi_vg_error_takedown();
  }
  vgFlush();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_takedown();
#endif
  vgFlush();
  return bw;
#undef TAKEDOWN
}

static VGImage cvpi_image_logical_common(const VGImage image1, const VGImage image2,
					 VGubyte true_value1, VGubyte true_value2,
					 VGubyte true_color, VGubyte false_color,
					 CVPI_BOOL nonzero_true) {
#define TAKEDOWN cvpi_image_logical_common_takedown
  int BADSTATE = 0;

  VGImage image1_scaled = VG_INVALID_HANDLE;
  VGImage image2_scaled = VG_INVALID_HANDLE;
  VGImage sum = VG_INVALID_HANDLE;
  VGint img1_width = vgGetParameteri(image1, VG_IMAGE_WIDTH);
  VGint img1_height = vgGetParameteri(image1, VG_IMAGE_HEIGHT);

  image1_scaled = vgCreateImage(CVPI_COLOR_SPACE, img1_width, img1_height,
				VG_IMAGE_QUALITY_NONANTIALIASED);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  image2_scaled = vgCreateImage(CVPI_COLOR_SPACE, img1_width, img1_height,
				VG_IMAGE_QUALITY_NONANTIALIASED);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif

  static const VGubyte cvpi_binary_array_img1[256] = {0, [1 ... 255] = 1};
  static const VGubyte cvpi_binary_array_img2[256] = {0, [1 ... 255] = 2};
  static const VGubyte cvpi_binary_array_img1_i[256] = {1, [1 ... 255] = 0};
  static const VGubyte cvpi_binary_array_img2_i[256] = {2, [1 ... 255] = 0};

  if(CVPI_TRUE_TEST(nonzero_true)) {
    /* use cvpi_binary_array_img1 and cvpi_binary_array_img2 */
    /* set pixels to 1's and 0's for T and F */
    vgLookup(image1_scaled, image1,
	     cvpi_binary_array_img1, cvpi_binary_array_img1, cvpi_binary_array_img1,
	     cvpi_binary_array_img1, OUTPUT_LINEAR, VG_FALSE);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  /* set pixels to 2's and 0's for T and F */
    vgLookup(image2_scaled, image2,
	     cvpi_binary_array_img2, cvpi_binary_array_img2, cvpi_binary_array_img2,
	     cvpi_binary_array_img2, OUTPUT_LINEAR, VG_FALSE);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  } else { /* use cvpi_binary_array_img1_i and cvpi_binary_array_img2_i */
    /* set pixels to 0's and 1's for T and F */
    vgLookup(image1_scaled, image1,
	     cvpi_binary_array_img1_i, cvpi_binary_array_img1_i, cvpi_binary_array_img1_i,
	     cvpi_binary_array_img1_i, OUTPUT_LINEAR, VG_FALSE);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  /* set pixels to 0's and 2's for T and F */
    vgLookup(image2_scaled, image2,
	     cvpi_binary_array_img2_i, cvpi_binary_array_img2_i, cvpi_binary_array_img2_i,
	     cvpi_binary_array_img2_i, OUTPUT_LINEAR, VG_FALSE);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  }
  vgFinish();
  /* image full of 0's, 1's 2's, and 3's */
  sum = cvpi_image_add(image1_scaled, image2_scaled, 1,1,1,0);
  cvpi_image_error_check(sum);

  VGubyte logic_array[256];

  /* create lookup table for particular logic operation */
  if(true_value1 == true_value2) {
    /* logical operation has 1 true value */
    for(int i = 256; i != 0;) {
      --i;
      logic_array[i] = i != true_value1 ? false_color : true_color;
    }
  } else {
    /* logical operation has 2 true values */
    for(int i = 256; i != 0;) {
      --i;
      logic_array[i] = (i != true_value1 && i != true_value2) ? false_color : true_color;
    }
  }

  /* reuse image1_scaled for the result of the logical operation */
  vgLookup(image1_scaled, sum,
	   logic_array, logic_array, logic_array, logic_array,
	   OUTPUT_LINEAR, VG_FALSE);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  vgFinish();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
 TAKEDOWN:
  vgDestroyImageSafe(sum);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(image2_scaled);
  cvpi_vg_error_takedown();
  if(BADSTATE) {
    /* set output image to  */
    vgDestroyImageSafe(image1_scaled);
#if CVPI_CAREFUL == 1
    cvpi_vg_error_takedown();
#endif
  }
  vgFlush();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_takedown();
#endif
  return image1_scaled;
  #undef TAKEDOWN
}

VGImage cvpi_image_logical_and(const VGImage image1, const VGImage image2, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true) {
  return cvpi_image_logical_common(image1, image2, 3, 3, t_c, f_c, nonzero_true);
}

VGImage cvpi_image_logical_nand(const VGImage image1, const VGImage image2, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true) {
  return cvpi_image_logical_and(image1, image2, f_c, t_c, nonzero_true);
}

VGImage cvpi_image_logical_nor(const VGImage image1, const VGImage image2, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true) {
  return cvpi_image_logical_common(image1, image2, 0, 0, t_c, f_c, nonzero_true);
}

VGImage cvpi_image_logical_or(const VGImage image1, const VGImage image2, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true) {
  return cvpi_image_logical_nor(image1, image2, f_c, t_c, nonzero_true);
}

VGImage cvpi_image_logical_xor(const VGImage image1, const VGImage image2, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true) {
  return cvpi_image_logical_common(image1, image2, 1, 2, t_c, f_c, nonzero_true);
}

VGImage cvpi_image_logical_xnor(const VGImage image1, const VGImage image2, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true) {
  return cvpi_image_logical_xor(image1, image2, f_c, t_c, nonzero_true);
}

VGImage cvpi_image_logical_complement(const VGImage image1, const VGImage image2, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true) {
  return cvpi_image_logical_common(image1, image2, 1, 1, t_c, f_c, nonzero_true);
}

VGImage cvpi_image_logical_complement_inv(const VGImage image1, const VGImage image2, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true) {
  return cvpi_image_logical_complement(image1, image2, f_c, t_c, nonzero_true);
}

/* Returns a "binary" image with 1 = True and 0 = False */
static VGImage cvpi_image_morph_common(const VGImage image, const VGshort * kernel, CVPI_BOOL nonzero_true) {
#define TAKEDOWN cvpi_image_morph_common_takedown
  int BADSTATE = 0;

  VGImage binary = VG_INVALID_HANDLE;
  VGImage convolved = VG_INVALID_HANDLE;

  VGint image_width = vgGetParameteri(image, VG_IMAGE_WIDTH);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  VGint image_height = vgGetParameteri(image, VG_IMAGE_HEIGHT);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif

  binary = vgCreateImage(CVPI_COLOR_SPACE, image_width, image_height,
				 VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();

  convolved = vgCreateImage(CVPI_COLOR_SPACE, image_width, image_height, VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();

  VGubyte true_color, false_color;
  if(CVPI_TRUE_TEST(nonzero_true)) {
    true_color = 1;
    false_color = 0;
  } else {
    true_color = 0;
    false_color = 1;
  }

  /* not sure if this is legal in c99, but it works in gcc */
  VGubyte logic_array[256] = { [0] = false_color, [1 ... 255] = true_color };

  vgLookup(binary, image,
	   logic_array, logic_array, logic_array, logic_array,
	   OUTPUT_LINEAR, VG_FALSE);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif

  /* Upper right element of the input sub-image in the convolution
     becomes the center element (see VG 1.1 spec p. 179 fig. 27). Is the
     full sum of products between the kernel and sub-image. */
  vgConvolveNoShift(convolved, binary, 3, 3, kernel, 1, 0, VG_TILE_REFLECT);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  vgFinish();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif

 TAKEDOWN:
  vgDestroyImageSafe(binary);
  cvpi_vg_error_takedown();

  if(BADSTATE) {
    /* set output image to  */
    vgDestroyImageSafe(convolved);
#if CVPI_CAREFUL == 1
    cvpi_vg_error_takedown();
#endif
  }
  vgFlush();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_takedown();
#endif

  return convolved;
#undef TAKEDOWN
}

VGImage cvpi_image_dilate(const VGImage image, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true) {
#define TAKEDOWN cvpi_image_dilate_takedown
  int BADSTATE = 0;
  const VGshort kernel[9] = {1,1,1,
			     1,1,1,
			     1,1,1};
  VGImage convolved = VG_INVALID_HANDLE;
  VGImage dilated = VG_INVALID_HANDLE;

  VGint image_width = vgGetParameteri(image, VG_IMAGE_WIDTH);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  VGint image_height = vgGetParameteri(image, VG_IMAGE_HEIGHT);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  convolved = cvpi_image_morph_common(image, kernel, nonzero_true);
  if(convolved == VG_INVALID_HANDLE) {
    cvpi_log_1("cvpi_image_morph_common failed", __FILE__, __LINE__);
    BADSTATE = 1;
    goto TAKEDOWN;
  }
  dilated = vgCreateImage(CVPI_COLOR_SPACE, image_width, image_height, VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();

  VGubyte dilation[256] = {[0] = f_c, [1 ... 255] = t_c};

  vgLookup(dilated, convolved,
	   dilation,dilation,dilation,dilation,
	   OUTPUT_LINEAR, VG_FALSE);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  vgFinish();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
 TAKEDOWN:
  vgDestroyImageSafe(convolved);
  cvpi_vg_error_takedown();

  if(BADSTATE) {
    /* set output image to  */
    vgDestroyImageSafe(dilated);
#if CVPI_CAREFUL == 1
    cvpi_vg_error_takedown();
#endif
  }
  vgFlush();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_takedown();
#endif

  return dilated;
#undef TAKEDOWN
}

VGImage cvpi_image_erode(const VGImage image, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true) {
  return cvpi_image_dilate(image, f_c, t_c, CVPI_NOT(nonzero_true));
}

VGImage cvpi_image_hit_miss(const VGImage image, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true) {
#define TAKEDOWN cvpi_image_hit_miss_takedown
  int BADSTATE = 0;
  const VGshort kernel0[9] = {0,1,0,
			      0,1,1,
			      0,0,0};
  const VGshort kernel1[9] = {0,0,0,
			      0,1,1,
			      0,1,0};
  const VGshort kernel2[9] = {0,1,0,
			      1,1,0,
			      0,0,0};
  const VGshort kernel3[9] = {0,0,0,
			      1,1,0,
			      0,1,0};
  VGImage convolved0 = VG_INVALID_HANDLE;
  VGImage convolved1 = VG_INVALID_HANDLE;
  VGImage convolved2 = VG_INVALID_HANDLE;
  VGImage convolved3 = VG_INVALID_HANDLE;
  VGImage or01 = VG_INVALID_HANDLE;
  VGImage or23 = VG_INVALID_HANDLE;
  VGImage or0123 = VG_INVALID_HANDLE;

  /* cvpi_image_morph_common returns an image with 1 = true, and 0 = false */
  /* if the user specified nonzero_true = false, then
     cvpi_image_morph_common maps non-zero values to 0 and zero to 1 */
  convolved0 = cvpi_image_morph_common(image, kernel0, nonzero_true);
  if(convolved0 == VG_INVALID_HANDLE) {
    cvpi_log_1("cvpi_image_morph_common failed", __FILE__, __LINE__);
    BADSTATE = 1;
    goto TAKEDOWN;
  }
  convolved1 = cvpi_image_morph_common(image, kernel1, nonzero_true);
  if(convolved1 == VG_INVALID_HANDLE) {
    cvpi_log_1("cvpi_image_morph_common failed", __FILE__, __LINE__);
    BADSTATE = 1;
    goto TAKEDOWN;
  }

  convolved2 = cvpi_image_morph_common(image, kernel2, nonzero_true);
  if(convolved2 == VG_INVALID_HANDLE) {
    cvpi_log_1("cvpi_image_morph_common failed", __FILE__, __LINE__);
    BADSTATE = 1;
    goto TAKEDOWN;
  }

  convolved3 = cvpi_image_morph_common(image, kernel3, nonzero_true);
  if(convolved3 == VG_INVALID_HANDLE) {
    cvpi_log_1("cvpi_image_morph_common failed", __FILE__, __LINE__);
    BADSTATE = 1;
    goto TAKEDOWN;
  }

  or01 = cvpi_image_logical_or(convolved0, convolved1, 255, 0, CVPI_TRUE);
  if(or01 == VG_INVALID_HANDLE) {
    cvpi_log_1("cvpi_image_logical_or failed", __FILE__, __LINE__);
    BADSTATE = 1;
    goto TAKEDOWN;
  }

  or23 = cvpi_image_logical_or(convolved2, convolved3, 255, 0, CVPI_TRUE);
  if(or23 == VG_INVALID_HANDLE) {
    cvpi_log_1("cvpi_image_logical_or failed", __FILE__, __LINE__);
    BADSTATE = 1;
    goto TAKEDOWN;
  }

  or0123 = cvpi_image_logical_or(or01, or23, t_c, f_c, CVPI_TRUE);
  if(or0123 == VG_INVALID_HANDLE) {
    cvpi_log_1("cvpi_image_logical_or failed", __FILE__, __LINE__);
    BADSTATE = 1;
    goto TAKEDOWN;
  }

 TAKEDOWN:

  vgDestroyImageSafe(convolved0);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(convolved1);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(convolved2);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(convolved3);
  vgDestroyImageSafe(or01);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(or23);
  cvpi_vg_error_takedown();

  if(BADSTATE) {
    /* set output image to  */
    vgDestroyImageSafe(or0123);
#if CVPI_CAREFUL == 1
    cvpi_vg_error_takedown();
#endif
  }
  vgFlush();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_takedown();
#endif

  return or0123;
#undef TAKEDOWN
}

VGImage cvpi_image_thin(const VGImage image, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true) {
#define TAKEDOWN cvpi_image_thin_takedown
  int BADSTATE = 0;
  VGImage hm = VG_INVALID_HANDLE;
  VGImage thinned = VG_INVALID_HANDLE;

  VGubyte truth;
  VGubyte falsity;

  if(CVPI_TRUE_TEST(nonzero_true)) {
    truth = ~0;
    falsity = 0;
  } else {
    truth = 0;
    falsity = ~0;
  }

  hm = cvpi_image_hit_miss(image, truth, falsity, nonzero_true);
  if(hm == VG_INVALID_HANDLE) {
    cvpi_log_1("cvpi_image_hit_miss failed", __FILE__, __LINE__);
    BADSTATE = 1;
    goto TAKEDOWN;
  }

  thinned = cvpi_image_logical_complement(image, hm, t_c, f_c, nonzero_true);
  if(thinned == VG_INVALID_HANDLE) {
    cvpi_log_1("cvpi_image_logical_complement failed", __FILE__, __LINE__);
    BADSTATE = 1;
    goto TAKEDOWN;
  }

 TAKEDOWN:
  vgDestroyImageSafe(hm);
  cvpi_vg_error_takedown();

  if(BADSTATE) {
    /* set output image to  */
    vgDestroyImageSafe(thinned);
#if CVPI_CAREFUL == 1
    cvpi_vg_error_takedown();
#endif
  }
  vgFlush();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_takedown();
#endif

  return thinned;
#undef TAKEDOWN
}

VGImage cvpi_image_thicken(const VGImage image, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true) {
#define TAKEDOWN cvpi_image_thicken_takedown
  int BADSTATE = 0;
  VGImage hm = VG_INVALID_HANDLE;
  VGImage thickened = VG_INVALID_HANDLE;

  VGubyte truth;
  VGubyte falsity;

  if(CVPI_TRUE_TEST(nonzero_true)) {
    truth = ~0;
    falsity = 0;
  } else {
    truth = 0;
    falsity = ~0;
  }

  hm = cvpi_image_hit_miss(image, truth, falsity, nonzero_true);

  if(hm == VG_INVALID_HANDLE) {
    cvpi_log_1("cvpi_image_hit_miss failed", __FILE__, __LINE__);
    BADSTATE = 1;
    goto TAKEDOWN;
  }

  thickened = cvpi_image_logical_or(image, hm, t_c, f_c, nonzero_true);
  if(thickened == VG_INVALID_HANDLE) {
    cvpi_log_1("cvpi_image_logical_or failed", __FILE__, __LINE__);
    BADSTATE = 1;
    goto TAKEDOWN;
  }
 TAKEDOWN:
  vgDestroyImageSafe(hm);
  cvpi_vg_error_takedown();
  if(BADSTATE) {
    /* set output image to  */
    vgDestroyImageSafe(thickened);
#if CVPI_CAREFUL == 1
    cvpi_vg_error_takedown();
#endif
  }
  vgFlush();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_takedown();
#endif
  return thickened;
#undef TAKEDOWN
}

VGubyte* cvpi_channel2data(const VGImage image, VGImageChannel channel) {
#define TAKEDOWN cvpi_channel2data_takedown
  int BADSTATE = 0;
  VGImage mono = VG_INVALID_HANDLE;
  VGImage alpha = VG_INVALID_HANDLE;
  VGubyte* data = NULL;
  VGint image_width = vgGetParameteri(image, VG_IMAGE_WIDTH);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  VGint image_height = vgGetParameteri(image, VG_IMAGE_HEIGHT);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif

  mono = vgCreateImage(VG_A_8, image_width, image_height, VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();

  if(channel != VG_ALPHA) {
    /* put the desired channel data into alpha */
    alpha = vgCreateImage(CVPI_COLOR_SPACE, image_width, image_height, VG_IMAGE_QUALITY_NONANTIALIASED);
    cvpi_vg_error_check();

    /* matrix by default switches red to alpha */
    VGfloat alpha_switch[20] = {
      0, 0, 0, 0,
      0, 0, 0, 0,
      0, 0, 0, 0,
      0, 0, 0, 0,

      0, 0, 0, 0
    };
    /* modify matrix to switch the right channel */
    switch(channel) {
    case VG_GREEN:
      alpha_switch[7] = 1;
      break;
    case VG_BLUE:
      alpha_switch[11] = 1;
      break;
    default:			/* channel is VG_RED */
      alpha_switch[3] = 1;
      break;
    }
    vgColorMatrix(alpha, image, alpha_switch);
#if CVPI_CAREFUL == 1
    cvpi_vg_error_check();
#endif
    vgCopyImage(mono, 0, 0, alpha, 0, 0, image_width, image_height, VG_FALSE);
#if CVPI_CAREFUL == 1
    cvpi_vg_error_check();
#endif

  } else {			/* channel is VG_ALPHA */
    vgCopyImage(mono, 0, 0, image, 0, 0, image_width, image_height, VG_FALSE);
#if CVPI_CAREFUL == 1
    cvpi_vg_error_check();
#endif
  }
  vgFinish();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif

  data = malloc(image_height * image_width * sizeof(*data));
  if(data == NULL) {
    cvpi_log_3("malloc returned NULL: errno = ", __FILE__, __LINE__, errno);
    BADSTATE = 1;
    goto TAKEDOWN;
  }
  /* copy the data from alpha into memory */
  vgGetImageSubData(mono, (void*)data, image_width * sizeof(*data), VG_A_8, 0, 0, image_width, image_height);
  cvpi_vg_error_check();

  vgFinish();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif

 TAKEDOWN:

  vgDestroyImageSafe(alpha);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(mono);
  cvpi_vg_error_takedown();

  if(BADSTATE && data != NULL) {
    free(data);
    data = NULL;
  }
  vgFlush();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_takedown();
#endif

  return data;
#undef TAKEDOWN
}

VGubyte cvpi_channel_max(const VGImage image, VGImageChannel channel) {
#define TAKEDOWN cvpi_channel_max_takedown
  int BADSTATE = 0;
  VGubyte* data = NULL;
  VGubyte max = 0;
  VGint image_width = vgGetParameteri(image, VG_IMAGE_WIDTH);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  VGint image_height = vgGetParameteri(image, VG_IMAGE_HEIGHT);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif

  data = cvpi_channel2data(image, channel);
  if(data == NULL) {
    cvpi_log_1("cvpi_channel2data returned NULL", __FILE__, __LINE__);
    BADSTATE = 1;
    goto TAKEDOWN;
  }

  unsigned long area = image_width * image_height;

  VGubyte current;
  for(unsigned long i = 0; i < area && max < 255; ++i) {
    current = data[i];
    if(current > max) {
      max = current;
    }
  }

 TAKEDOWN:

  if(data != NULL) {
    free(data);
  }

  return max;
#undef TAKEDOWN
}

VGubyte cvpi_channel_min(const VGImage image, VGImageChannel channel) {
#define TAKEDOWN cvpi_channel_min_takedown
  int BADSTATE = 0;
  VGubyte* data = NULL;
  VGubyte min = 255;
  VGint image_width = vgGetParameteri(image, VG_IMAGE_WIDTH);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  VGint image_height = vgGetParameteri(image, VG_IMAGE_HEIGHT);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  data = cvpi_channel2data(image, channel);
  if(data == NULL) {
    cvpi_log_1("cvpi_channel2data returned NULL", __FILE__, __LINE__);
    BADSTATE = 1;
    goto TAKEDOWN;
  }

  unsigned long area = image_width * image_height;

  VGubyte current;
  for(unsigned long i = 0; i < area && min > 0; ++i) {
    current = data[i];
    if(current < min) {
      min = current;
    }
  }

 TAKEDOWN:

  if(data != NULL) {
    free(data);
  }

  return min;
#undef TAKEDOWN
}

cvpi_max_min cvpi_channel_max_min(const VGImage image, VGImageChannel channel) {
#define TAKEDOWN cvpi_channel_max_min_takedown
  int BADSTATE = 0;
  VGubyte* data = NULL;
  cvpi_max_min ret_val;
  ret_val.all = ~0;

  VGint image_width = vgGetParameteri(image, VG_IMAGE_WIDTH);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  VGint image_height = vgGetParameteri(image, VG_IMAGE_HEIGHT);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif

  data = cvpi_channel2data(image, channel);
  if(data == NULL) {
    cvpi_log_1("cvpi_channel2data returned NULL", __func__, __LINE__);
    BADSTATE = 1;
    goto TAKEDOWN;
  }

  VGubyte min = 255;
  VGubyte max = 0;

  unsigned long area = image_width * image_height;

  VGubyte current;
  for(unsigned long i = 0; i < area && (min > 0 || max < 255); ++i) {
    current = data[i];
    if(current < min) {
      min = current;
    }
    if(current > max) {
      max = current;
    }
  }

  ret_val.max_min[0] = max;
  ret_val.max_min[1] = min;

 TAKEDOWN:
  if(data != NULL) {
    free(data);
  }

  return ret_val;
#undef TAKEDOWN
}

unsigned int* cvpi_channel_histogram(const VGImage image, VGImageChannel channel) {
#define TAKEDOWN cvpi_channel_histogram_takedown
  int BADSTATE = 0;

  VGubyte* data = NULL;
  unsigned int* histogram = NULL;

  VGint image_width = vgGetParameteri(image, VG_IMAGE_WIDTH);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  VGint image_height = vgGetParameteri(image, VG_IMAGE_HEIGHT);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif

  data = cvpi_channel2data(image, channel);
  if(data == NULL) {
    cvpi_log_1("cvpi_channel2data returned NULL", __FILE__, __LINE__);
    BADSTATE = 1;
    goto TAKEDOWN;
  }
  histogram = calloc(256, sizeof(*histogram));
  if(histogram == NULL) {
    cvpi_log_3("calloc returned NULL: errno = ", __FILE__, __LINE__, errno);
    BADSTATE = 1;
    goto TAKEDOWN;
  }

  for(unsigned int i = image_width * image_height; i != 0;) {
    histogram[data[--i]]++;
  }

 TAKEDOWN:
  if(data != NULL) {
    free(data);
  }

  if(BADSTATE && histogram != NULL) {
    free(histogram);
    histogram = NULL;
  }

  return histogram;
#undef TAKEDOWN
}

unsigned int* cvpi_color_channels_histogram(const VGImage image) {
#define TAKEDOWN cvpi_color_channels_histogram_takedown
  int BADSTATE = 0;

  cvpi_pixel* data = NULL;
  unsigned int* histogram = NULL;

  VGint image_width = vgGetParameteri(image, VG_IMAGE_WIDTH);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  VGint image_height = vgGetParameteri(image, VG_IMAGE_HEIGHT);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif

  unsigned int stride = CVPI_PIXEL_BYTES * image_width;

  data = malloc(stride * image_height);
  if(data == NULL) {
    cvpi_log_3("malloc returned NULL: errno = ", __FILE__, __LINE__, errno);
    BADSTATE = 1;
    goto TAKEDOWN;
  }

  vgGetImageSubData(image, (void*)data,
		    stride, CVPI_COLOR_SPACE,
		    0, 0, image_width, image_height);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  vgFinish();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  histogram = calloc(768, sizeof(*histogram));
  if(data == NULL) {
    cvpi_log_3("malloc returned NULL: errno = ", __FILE__, __LINE__, errno);
    BADSTATE = 1;
    goto TAKEDOWN;
  }

  for(unsigned int i = image_width * image_height; i != 0;) {
    histogram[data[--i].channel[cvpi_pixel_red]]++;
    histogram[data[i].channel[cvpi_pixel_green] + 256]++;
    histogram[data[i].channel[cvpi_pixel_blue] + 512]++;
  }

 TAKEDOWN:
  if(data != NULL) {
    free(data);
  }

  if(BADSTATE && histogram != NULL) {
    free(histogram);
    histogram = NULL;
  }

  return histogram;
#undef TAKEDOWN
}

unsigned int* cvpi_image_histogram(const VGImage image) {
#define TAKEDOWN cvpi_image_histogram_takedown
  int BADSTATE = 0;

  cvpi_pixel* data = NULL;
  unsigned int* histogram = NULL;

  /* max value of a column is 2048*2048 = 4194304 */
  VGint image_width = vgGetParameteri(image, VG_IMAGE_WIDTH);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  VGint image_height = vgGetParameteri(image, VG_IMAGE_HEIGHT);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  unsigned int stride = CVPI_PIXEL_BYTES * image_width;
  data = malloc(stride * image_height);
  if(data == NULL) {
    cvpi_log_3("malloc returned NULL: errno = ", __FILE__, __LINE__, errno);
    BADSTATE = 1;
    goto TAKEDOWN;
  }

  vgGetImageSubData(image, (void*)data,
		    stride, CVPI_COLOR_SPACE,
		    0, 0, image_width, image_height);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  vgFinish();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  histogram = calloc(1024, sizeof(*histogram));
  if(histogram == NULL) {
    cvpi_log_3("calloc returned NULL: errno = ", __FILE__, __LINE__, errno);
    BADSTATE = 1;
    goto TAKEDOWN;
  }

  for(unsigned int i = image_width * image_height; i != 0;) {
    histogram[data[--i].channel[cvpi_pixel_red]]++;
    histogram[data[i].channel[cvpi_pixel_green] + 256]++;
    histogram[data[i].channel[cvpi_pixel_blue] + 512]++;
    histogram[data[i].channel[cvpi_pixel_alpha] + 768]++;
  }

 TAKEDOWN:

  if(data != NULL) {
    free(data);
  }

  if(BADSTATE && histogram != NULL) {
    free(histogram);
    histogram = NULL;
  }

  return histogram;
#undef TAKEDOWN
}

VGubyte* cvpi_channel_cumulative_distribution(const unsigned int* histogram, VGint width, VGint height) {
  VGubyte* cd = malloc(sizeof(*cd) * 256);
  if(cd == NULL) {
    cvpi_log_3("malloc returned NULL: errno = ", __func__, __LINE__, errno);
    return NULL;
  }
  unsigned long N = width * height;
  unsigned long accumulate = 0;

  /* don't optimize by counting down to 0 */
  for(int i = 0; i < 256; ++i) {
    cd[i] = (VGubyte)((accumulate += histogram[i])/N);
  }

  return cd;
}

VGubyte* cvpi_color_channels_cumulative_distribution(const unsigned int* histogram, VGint width, VGint height) {
  VGubyte* cd = malloc(sizeof(*cd) * 768);
  if(cd == NULL) {
    cvpi_log_3("malloc returned NULL: errno = ", __func__, __LINE__, errno);
    return NULL;
  }
  unsigned long N = width * height;
  unsigned long accumulate_r = 0;
  unsigned long accumulate_g = 0;
  unsigned long accumulate_b = 0;

  /* don't optimize by counting down to 0 */
  int i = 0; int j = 256; int k = 512;
  for(; i < 256; ++i, ++j, ++k) {
    cd[i] = (VGubyte)((accumulate_r += histogram[i])/N);
    cd[j] = (VGubyte)((accumulate_g += histogram[j])/N);
    cd[k] = (VGubyte)((accumulate_b += histogram[k])/N);
  }

  return cd;
}

VGubyte* cvpi_image_cumulative_distribution(const unsigned int* histogram, VGint width, VGint height) {
  VGubyte* cd = malloc(sizeof(*cd) * 1024);
  if(cd == NULL) {
    cvpi_log_3("malloc returned NULL: errno = ", __func__, __LINE__, errno);
    return NULL;
  }
  unsigned long N = width * height;
  unsigned long accumulate_r = 0;
  unsigned long accumulate_g = 0;
  unsigned long accumulate_b = 0;
  unsigned long accumulate_a = 0;

  /* don't optimize by counting down to 0 */
  int i = 0; int j = 256; int k = 512; int m = 768;
  for(; i < 256; ++i, ++j, ++k, ++m) {
    cd[i] = (VGubyte)((accumulate_r += histogram[i])/N);
    cd[j] = (VGubyte)((accumulate_g += histogram[j])/N);
    cd[k] = (VGubyte)((accumulate_b += histogram[k])/N);
    cd[m] = (VGubyte)((accumulate_a += histogram[m])/N);
  }

  return cd;
}

VGImage cvpi_channel_histogram_equalization(const VGImage image, VGImageChannel channel,
					    CVPI_BOOL scale) {
#define TAKEDOWN cvpi_channel_histogram_equalization_takedown
  int BADSTATE = 0;

  VGImage return_image = VG_INVALID_HANDLE;
  unsigned int* hist = NULL;
  VGubyte* cdf = NULL;

  hist = cvpi_channel_histogram(image, channel);
  if(hist == NULL) {
    cvpi_log_3("histogram returned NULL: errno = ", __FILE__, __LINE__, errno);
    BADSTATE = 1;
    goto TAKEDOWN;
  }

  VGint image_width = vgGetParameteri(image, VG_IMAGE_WIDTH);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  VGint image_height = vgGetParameteri(image, VG_IMAGE_HEIGHT);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif

  cdf = cvpi_channel_cumulative_distribution(hist, image_width, image_height);

  if(cdf == NULL) {
    cvpi_log_3("CDF returned NULL: errno = ", __FILE__, __LINE__, errno);
    BADSTATE = 1;
    goto TAKEDOWN;
  }

  if(hist != NULL) {
    free(hist);
    hist = NULL;
  }

  if(CVPI_TRUE_TEST(scale)) {
    /* find the cdf_min */
    int i = 255;
    VGubyte min = cdf[i];
    min = min > 0 ? min : 255;
    do {
      --i;
      if(cdf[i] > 0 && cdf[i] < min) {
	min = cdf[i];
      }
    } while(i != 0);

    /* scale 0 to 255 */
    double scaler = 255/(image_height * image_width - min);

    for(i = 256; i != 0;) {
      --i;
      cdf[i] = (VGubyte)(0.5 + (cdf[i] - min) * scaler);
    }
  }

  return_image = vgCreateImage(CVPI_COLOR_SPACE, image_width, image_height,
			       VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();

  switch(channel) {
  case VG_RED:
    vgLookup(return_image, image,
    cdf, cvpi_identity_array,cvpi_identity_array,cvpi_identity_array,
    OUTPUT_LINEAR, VG_FALSE);
    break;
  case VG_GREEN:
    vgLookup(return_image, image,
    cvpi_identity_array, cdf, cvpi_identity_array,cvpi_identity_array,
    OUTPUT_LINEAR, VG_FALSE);
    break;
  case VG_BLUE:
    vgLookup(return_image, image,
    cvpi_identity_array,cvpi_identity_array, cdf, cvpi_identity_array,
    OUTPUT_LINEAR, VG_FALSE);
    break;
  default:			/* alpah */
    vgLookup(return_image, image,
    cvpi_identity_array,cvpi_identity_array,cvpi_identity_array,cdf,
    OUTPUT_LINEAR, VG_FALSE);
    break;
  }
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif

 TAKEDOWN:
  vgFinish();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_takedown();
#endif

  if(hist != NULL) {
    free(hist);
  }

  if(cdf != NULL) {
    free(cdf);
  }

  if(BADSTATE) {
    /* set output image to  */
    vgDestroyImageSafe(return_image);
    cvpi_vg_error_takedown();
  }
  vgFlush();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_takedown();
#endif

  return return_image;

#undef TAKEDOWN
}
VGImage cvpi_color_channels_histogram_equalization(const VGImage image, CVPI_BOOL scale) {
#define TAKEDOWN cvpi_color_channels_histogram_equalization_takedown
  int BADSTATE = 0;

  VGImage return_image = VG_INVALID_HANDLE;
  unsigned int* hist = NULL;

  VGubyte* cdf = NULL;

  hist = cvpi_color_channels_histogram(image);
  if(hist == NULL) {
    cvpi_log_3("histogram returned NULL: errno = ", __FILE__, __LINE__, errno);
    BADSTATE = 1;
    goto TAKEDOWN;
  }

  VGint image_width = vgGetParameteri(image, VG_IMAGE_WIDTH);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  VGint image_height = vgGetParameteri(image, VG_IMAGE_HEIGHT);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif

  cdf = cvpi_color_channels_cumulative_distribution(hist, image_width, image_height);

  if(cdf == NULL) {
    cvpi_log_3("CDF returned NULL: errno = ", __FILE__, __LINE__, errno);
    BADSTATE = 1;
    goto TAKEDOWN;
  }

  if(hist != NULL) {
    free(hist);
    hist = NULL;
  }

  if(CVPI_TRUE_TEST(scale)) {
    /* find the cdf_min */
    int i = 767;
    VGubyte min = cdf[i];
    min = min > 0 ? min : 255;
    do {
      --i;
      if(cdf[i] > 0 && cdf[i] < min) {
	min = cdf[i];
      }
    } while(i != 0);

    /* scale 0 to 255 */
    double scaler = 255/(image_height * image_width - min);

    for(i = 768; i != 0;) {
      --i;
      cdf[i] = (VGubyte)(0.5 + (cdf[i] - min) * scaler);
    }
  }

  return_image = vgCreateImage(CVPI_COLOR_SPACE, image_width, image_height,
			       VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();

  vgLookup(return_image, image,
	   &cdf[0], &cdf[256], &cdf[512], cvpi_identity_array,
	   OUTPUT_LINEAR, VG_FALSE);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif

 TAKEDOWN:
  vgFinish();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_takedown();
#endif

  if(hist != NULL) {
    free(hist);
  }

  if(cdf != NULL) {
    free(cdf);
  }

  if(BADSTATE) {
    /* set output image to  */
    vgDestroyImageSafe(return_image);
    cvpi_vg_error_takedown();
  }
  vgFlush();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_takedown();
#endif

  return return_image;
#undef TAKEDOWN
}
VGImage cvpi_image_histogram_equalization(const VGImage image, CVPI_BOOL scale) {
#define TAKEDOWN cvpi_image_histogram_equalization_takedown
  int BADSTATE = 0;

  VGImage return_image = VG_INVALID_HANDLE;
  unsigned int* hist = NULL;

  VGubyte* cdf = NULL;

  hist = cvpi_image_histogram(image);
  if(hist == NULL) {
    cvpi_log_3("histogram returned NULL: errno = ", __FILE__, __LINE__, errno);
    BADSTATE = 1;
    goto TAKEDOWN;
  }

  VGint image_width = vgGetParameteri(image, VG_IMAGE_WIDTH);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  VGint image_height = vgGetParameteri(image, VG_IMAGE_HEIGHT);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif

  cdf = cvpi_image_cumulative_distribution(hist, image_width, image_height);

  if(cdf == NULL) {
    cvpi_log_3("CDF returned NULL: errno = ", __FILE__, __LINE__, errno);
    BADSTATE = 1;
    goto TAKEDOWN;
  }

  if(hist != NULL) {
    free(hist);
    hist = NULL;
  }

  if(CVPI_TRUE_TEST(scale)) {
    /* find the cdf_min */
    int i = 1023;
    VGubyte min = cdf[i];
    min = min > 0 ? min : 255;
    do {
      --i;
      if(cdf[i] > 0 && cdf[i] < min) {
	min = cdf[i];
      }
    } while(i != 0);

    /* scale 0 to 255 */
    double scaler = 255/(image_height * image_width - min);

    for(i = 1024; i != 0;) {
      --i;
      cdf[i] = (VGubyte)(0.5 + (cdf[i] - min) * scaler);
    }
  }

  return_image = vgCreateImage(CVPI_COLOR_SPACE, image_width, image_height,
			       VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();

  vgLookup(return_image, image,
	   &cdf[0], &cdf[256], &cdf[512], &cdf[768],
	   OUTPUT_LINEAR, VG_FALSE);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif

 TAKEDOWN:
  vgFinish();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_takedown();
#endif

  if(hist != NULL) {
    free(hist);
  }

  if(cdf != NULL) {
    free(cdf);
  }

  if(BADSTATE) {
    /* set output image to  */
    vgDestroyImageSafe(return_image);
    cvpi_vg_error_takedown();
  }
  vgFlush();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_takedown();
#endif

  return return_image;
#undef TAKEDOWN
}

VGImage cvpi_image_magnitude(const VGImage image1, const VGImage image2, enum cvpi_integer_truncation trunc) {
#define TAKEDOWN cvpi_image_magnitude_takedown
  int BADSTATE = 0;

  VGImage img1_2 = VG_INVALID_HANDLE;
  VGImage img2_2 = VG_INVALID_HANDLE;
  VGImage sum = VG_INVALID_HANDLE;

  VGint img1_width = vgGetParameteri(image1, VG_IMAGE_WIDTH);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  VGint img1_height = vgGetParameteri(image1, VG_IMAGE_HEIGHT);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  VGint img2_width = vgGetParameteri(image2, VG_IMAGE_WIDTH);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  VGint img2_height = vgGetParameteri(image2, VG_IMAGE_HEIGHT);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif

  if(img1_width != img2_width || img1_height != img2_height) {
    cvpi_log_6("Images have differing dimensions:", __func__, __LINE__,
	       img1_width, img1_height, img2_width, img2_height);
    BADSTATE = 1;
    goto TAKEDOWN;
  }

  /* 255/sqrt(127.5) */
  const static VGfloat scaler[20] = {
    22.5831795813, 0, 0, 0,
    0, 22.5831795813, 0, 0,
    0, 0, 22.5831795813, 0,
    0, 0, 0, 22.5831795813,

    0, 0, 0, 0
  };

  const static VGubyte cvpi_square_array_scaled_floor[256] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,3,3,3,3,3,3,4,4,4,4,4,5,5,5,5,5,6,6,6,6,7,7,7,7,8,8,8,8,9,9,9,9,10,10,10,11,11,11,11,12,12,12,13,13,13,14,14,14,15,15,15,16,16,16,17,17,18,18,18,19,19,20,20,20,21,21,22,22,22,23,23,24,24,25,25,25,26,26,27,27,28,28,29,29,30,30,31,31,32,32,33,33,34,34,35,35,36,36,37,37,38,38,39,40,40,41,41,42,42,43,44,44,45,45,46,47,47,48,48,49,50,50,51,52,52,53,54,54,55,56,56,57,58,58,59,60,60,61,62,62,63,64,64,65,66,67,67,68,69,70,70,71,72,73,73,74,75,76,76,77,78,79,80,80,81,82,83,84,84,85,86,87,88,88,89,90,91,92,93,94,94,95,96,97,98,99,100,101,101,102,103,104,105,106,107,108,109,110,111,112,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127 };
  const static VGubyte cvpi_square_array_scaled_ceil[256] = { 0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,4,4,4,4,4,4,5,5,5,5,5,6,6,6,6,6,7,7,7,7,8,8,8,8,9,9,9,9,10,10,10,10,11,11,11,12,12,12,12,13,13,13,14,14,14,15,15,15,16,16,16,17,17,17,18,18,19,19,19,20,20,21,21,21,22,22,23,23,23,24,24,25,25,26,26,26,27,27,28,28,29,29,30,30,31,31,32,32,33,33,34,34,35,35,36,36,37,37,38,38,39,39,40,41,41,42,42,43,43,44,45,45,46,46,47,48,48,49,49,50,51,51,52,53,53,54,55,55,56,57,57,58,59,59,60,61,61,62,63,63,64,65,65,66,67,68,68,69,70,71,71,72,73,74,74,75,76,77,77,78,79,80,81,81,82,83,84,85,85,86,87,88,89,89,90,91,92,93,94,95,95,96,97,98,99,100,101,102,102,103,104,105,106,107,108,109,110,111,112,113,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128 };
  const static VGubyte cvpi_square_array_scaled_round[256] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,4,4,4,4,4,5,5,5,5,5,6,6,6,6,6,7,7,7,7,8,8,8,8,9,9,9,9,10,10,10,10,11,11,11,12,12,12,13,13,13,14,14,14,15,15,15,16,16,16,17,17,17,18,18,18,19,19,20,20,20,21,21,22,22,22,23,23,24,24,25,25,25,26,26,27,27,28,28,29,29,30,30,31,31,32,32,33,33,34,34,35,35,36,36,37,37,38,38,39,40,40,41,41,42,42,43,44,44,45,45,46,47,47,48,48,49,50,50,51,51,52,53,53,54,55,55,56,57,57,58,59,59,60,61,61,62,63,64,64,65,66,66,67,68,69,69,70,71,72,72,73,74,75,75,76,77,78,78,79,80,81,82,82,83,84,85,86,86,87,88,89,90,91,91,92,93,94,95,96,97,98,98,99,100,101,102,103,104,105,106,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128 };

  img1_2 = vgCreateImage(CVPI_COLOR_SPACE, img1_width, img1_height, VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();

  img2_2 = vgCreateImage(CVPI_COLOR_SPACE, img1_width, img1_height, VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();

  /* stack pointers */
  const VGubyte* square_function;
  const VGubyte* sqrt_function;
  switch(trunc) {
  case cvpi_floor:
    square_function = cvpi_square_array_scaled_floor;
    sqrt_function = cvpi_sqrt_array_floor;
    break;
  case cvpi_ceil:
    square_function = cvpi_square_array_scaled_ceil;
    sqrt_function = cvpi_sqrt_array_ceil;
    break;
  case cvpi_round:
    square_function = cvpi_square_array_scaled_round;
    sqrt_function = cvpi_sqrt_array_round;
    break;
  default:			/* should never happen */
    cvpi_log_1("Bad truncation type, using rounding.", __func__, __LINE__);
    square_function = cvpi_square_array_scaled_round;
    sqrt_function = cvpi_sqrt_array_round;
    break;
  }

  /* map image1 to sqare function */
  vgLookup(img1_2, image1, square_function,square_function,square_function,square_function, OUTPUT_LINEAR, VG_FALSE);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif

  /* map image2 to sqare function */
  vgLookup(img2_2, image2, square_function,square_function,square_function,square_function, OUTPUT_LINEAR, VG_FALSE);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif

  /* add */
  sum = cvpi_image_add(img1_2, img2_2, 1, 1, 1, 0);
  cvpi_image_error_check(sum);

  /* map sum to sqrt function, reuse imag1_2 */
  vgLookup(img1_2, sum, sqrt_function,sqrt_function,sqrt_function,sqrt_function, OUTPUT_LINEAR, VG_FALSE);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif

  /* scale, reuse img2_2 */
  vgColorMatrix(img2_2, img1_2, scaler);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif

 TAKEDOWN:
  vgFinish();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_takedown();
#endif

  vgDestroyImageSafe(img1_2);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(sum);
  cvpi_vg_error_takedown();

  if(BADSTATE) {
    /* set output image to  */
    vgDestroyImageSafe(img2_2);
    cvpi_vg_error_takedown();
  }
  vgFlush();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_takedown();
#endif

  vgFlush();
  return img2_2;
#undef TAKEDOWN
}

cvpi_coordinate_table* cvpi_image_coordinate_table(const VGImage image, VGbitfield channels,
						   CVPI_BOOL origin) {
#define TAKEDOWN cvpi_image_coordinate_table_takedown
  int BADSTATE = 0;

  cvpi_pixel* data = NULL;
  cvpi_coordinate* coordinates = NULL;
  cvpi_coordinate_table *cv = NULL;

  VGint width = vgGetParameteri(image, VG_IMAGE_WIDTH);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  VGint height = vgGetParameteri(image, VG_IMAGE_HEIGHT);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif

  VGint stride = CVPI_PIXEL_BYTES * width;

  data = malloc(sizeof(*data) * width * height);
  if(data == NULL) {
    cvpi_log_3("malloc returned NULL: errno = ", __func__, __LINE__, errno);
    BADSTATE = 1;
    goto TAKEDOWN;
  }
  coordinates = malloc(sizeof(*coordinates) * height * width);
  if(coordinates == NULL) {
    cvpi_log_3("malloc returned NULL: errno = ", __func__, __LINE__, errno);
    BADSTATE = 1;
    goto TAKEDOWN;
  }
  vgGetImageSubData(image, (void*)data, stride, CVPI_COLOR_SPACE, 0, 0, width, height);
  cvpi_vg_error_check();
  vgFinish();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif

  unsigned char r,g,b,a;

  if(channels & VG_RED) {
    r = 1;
  } else {
    r = 0;
  }

  if(channels & VG_GREEN) {
    g = 1;
  } else {
    g = 0;
  }

  if(channels & VG_BLUE) {
    b = 1;
  } else {
    b = 0;
  }

  if(channels & VG_ALPHA) {
    a = 1;
  } else {
    a = 0;
  }

  unsigned long w = 0;
  unsigned long h = 0;
  unsigned long i = width*height;
  unsigned long j = 0;
  if(CVPI_TRUE_TEST(origin)) {
    /* flip the y-axis */
    while(i != 0) {
      --i;
      if((r && data[i].channel[cvpi_pixel_red])
	 || g && (data[i].channel[cvpi_pixel_green])
	 || b && (data[i].channel[cvpi_pixel_blue])
	 || a && (data[i].channel[cvpi_pixel_alpha])) {
	coordinates[j].xy[0] = w;
	coordinates[j].xy[1] = height - h;
	++j;
      }
      ++w;

      if(w >= width) {
	w = 0;
	++h;
      }
    }
  } else {			/* don't flip the y-axis */
    while(i != 0) {
      --i;
      if(r && (data[i].channel[cvpi_pixel_red])
	 || g && (data[i].channel[cvpi_pixel_green])
	 || b && (data[i].channel[cvpi_pixel_blue])
	 || a && (data[i].channel[cvpi_pixel_alpha])) {
	coordinates[j].xy[0] = w;
	coordinates[j].xy[1] = h;
	++j;
      }
      ++w;

      if(w >= width) {
	w = 0;
	++h;
      }
    }
  }
  if(j < i) {
    /* there were some all zero pixels */
    cvpi_coordinate* crdnts_realloc = realloc(coordinates, sizeof(*coordinates) * j);

    if(crdnts_realloc != coordinates) {
      cvpi_log_5("realloc does not point to the same memory:", __func__, __LINE__, coordinates, crdnts_realloc, errno);
      BADSTATE = 1;
      goto TAKEDOWN;
    }

  }

  cv = malloc(sizeof(*cv));
  if(cv == NULL) {
    cvpi_log_3("malloc returned NULL: errno = ", __func__, __LINE__, errno);
    BADSTATE = 1;
    goto TAKEDOWN;
  }
  cv->coordinates = coordinates;
  cv->length = j;

 TAKEDOWN:
  vgFinish();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_takedown();
#endif

  if(data != NULL) {
    free(data);
  }

  if(BADSTATE && coordinates != NULL) {
    free(coordinates);
  }

  if(BADSTATE && cv != NULL) {
    free(cv);
    cv = NULL;
  }

  vgFlush();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_takedown();
#endif

  return cv;
#undef TAKEDOWN
}

void cvpi_image_coordinate_table_destroy(cvpi_coordinate_table *cv) {
  if(cv != NULL) {
    if(cv->coordinates != NULL) {
      free(cv->coordinates);
      cv->coordinates = NULL;
    }
    free(cv);
  }
}

CVPI_BOOL cvpi_avuy2argb(cvpi_pixel* yuva, cvpi_pixel* rgba,
			 unsigned long width, unsigned long height) {
  int32_t c,d,e, r_unusat, g_unusat, b_unusat;

  if(yuva == NULL || rgba == NULL) {
    return CVPI_FALSE;
  }
  
  for(unsigned long i = width * height; i != 0;) {
    --i;
    /* copy over the alpha channel if yuva and rgba are different pointers */
    if(yuva != rgba) {
      rgba[i] = yuva[i];
    }

    c = yuva[i].channel[cvpi_pixel_y] - 16;
    d = yuva[i].channel[cvpi_pixel_u] - 128;
    e = yuva[i].channel[cvpi_pixel_v] - 128;
    
    /* could be done in parallel with NEON, exceeds ARMv6 16bit width */
    r_unusat = 298 * c + 409 * e + 128;
    g_unusat = 298 * c - 100 * d - 208 * e + 128;
    b_unusat = 298 * c + 516 * d + 128;
    
    /* Use usat to do the right arithmetic shift and clipping. GCC
       does not have the necessry intrinsics. */
    
    /* TODO detect ARMv6 or higher, will not work in lower versions */
#if CVPI_ASSEMBLY == 1
    /* Use usat to do the right arithmetic shift and clipping. GCC
       does not have the necessry intrinsics. */
    asm("usat %[value], #8, %[value], ASR #8\n\t":[value] "+r"(r_unusat));
    asm("usat %[value], #8, %[value], ASR #8\n\t":[value] "+r"(g_unusat));
    asm("usat %[value], #8, %[value], ASR #8\n\t":[value] "+r"(b_unusat));
#else
#pragma message __FILE__ ": Not using inline assembly."
    /* shift */
    r_unusat = r_unusat >> 8;
    g_unusat = g_unusat >> 8;
    b_unusat = b_unusat >> 8;
    
    /* clip */
    r_unusat = r_unusat > 255 ? 255 : r_unusat;
    r_unusat = r_unusat < 0 ? 0 : r_unusat;
    
    g_unusat = g_unusat > 255 ? 255 : g_unusat;
    g_unusat = g_unusat < 0 ? 0 : g_unusat;
    
    b_unusat = b_unusat > 255 ? 255 : b_unusat;
    b_unusat = b_unusat < 0 ? 0 : b_unusat;
#endif
    rgba[i].channel[cvpi_pixel_red] = r_unusat;
    rgba[i].channel[cvpi_pixel_green] = g_unusat;
    rgba[i].channel[cvpi_pixel_blue] = b_unusat;
  }

  return CVPI_TRUE;
}

cvpi_pixel* cvpi_image2argb(const VGImage image) {
#define TAKEDOWN cvpi_image2argb_takedown
  int BADSTATE = 0;

  cvpi_pixel* data = NULL;
  
  VGint width = vgGetParameteri(image, VG_IMAGE_WIDTH);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  VGint height = vgGetParameteri(image, VG_IMAGE_HEIGHT);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  
  VGint stride = CVPI_PIXEL_BYTES * width;
  data = malloc(sizeof(*data) * width * height);
  if(data == NULL) {
    cvpi_log_3("malloc returned NULL: errno = ", __func__, __LINE__, errno);
    BADSTATE = 1;
    goto TAKEDOWN;
  }
  vgGetImageSubData(image, (void*)data, stride, CVPI_COLOR_SPACE, 0, 0, width, height);
  cvpi_vg_error_check();
  
  vgFinish();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  cvpi_avuy2argb(data, data, width, height);

 TAKEDOWN:
  if(BADSTATE == 1 && data != NULL) {
    free(data);
    data = NULL;
  }
  
  return data;
#undef TAKEDOWN
}
