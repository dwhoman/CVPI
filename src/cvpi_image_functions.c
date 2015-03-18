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

const VGuint cvpi_binary_array[256] = {0, ~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0,~0};

const VGuint cvpi_binary_array_inverted[256] = {~0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

const VGubyte cvpi_zeros_array[256] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

const VGubyte cvpi_255_array[256] = {255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255};

const VGubyte cvpi_identity_array[256] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255};

const VGubyte cvpi_inversion_array[256] = {255,254,253,252,251,250,249,248,247,246,245,244,243,242,241,240,239,238,237,236,235,234,233,232,231,230,229,228,227,226,225,224,223,222,221,220,219,218,217,216,215,214,213,212,211,210,209,208,207,206,205,204,203,202,201,200,199,198,197,196,195,194,193,192,191,190,189,188,187,186,185,184,183,182,181,180,179,178,177,176,175,174,173,172,171,170,169,168,167,166,165,164,163,162,161,160,159,158,157,156,155,154,153,152,151,150,149,148,147,146,145,144,143,142,141,140,139,138,137,136,135,134,133,132,131,130,129,128,127,126,125,124,123,122,121,120,119,118,117,116,115,114,113,112,111,110,109,108,107,106,105,104,103,102,101,100,99,98,97,96,95,94,93,92,91,90,89,88,87,86,85,84,83,82,81,80,79,78,77,76,75,74,73,72,71,70,69,68,67,66,65,64,63,62,61,60,59,58,57,56,55,54,53,52,51,50,49,48,47,46,45,44,43,42,41,40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0};

/* kernel is rotated 90 degrees counter-clockwise from Math definition
   to array index VG spec p. 179 */
const VGshort cvpi_filter_sobel_x[9] ={1,2,1,
				       0,0,0,
				       -1,-2,-1};
const VGshort cvpi_filter_sobel_y[9] = {1,0,-1,
					2,0,-2,
					1,0,-1};

const VGshort cvpi_filter_scharr_x[9] = {-3,-10,-3,
					 0,0,0,
					 3,10,3};
const VGshort cvpi_filter_scharr_y[9] = {3,0,-3,
					 10,0,-10,
					 3,0,-3};

const VGshort cvpi_filter_prewitt_x[9] = {1,1,1,
					  0,0,0,
					  -1,-1,-1};

const VGshort cvpi_filter_prewitt_y[9] = {-1,0,1,
					  -1,0,1,
					  -1,0,1};

const VGshort cvpi_filter_roberts_cross_x[4] = {0,-1,
						1,0};
const VGshort cvpi_filter_roberts_cross_y[4] = {1,0,
						0,-1};

#define cvpi_vg_error_check()\
  do {\
  VGErrorCode error = vgGetError();\
  if(error != VG_NO_ERROR) {\
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error)); \
  }\
  } while(0)

VGImage cvpi_yuyv2yuva(const VGImage yuyv_image) {

  VGErrorCode error;

  VGint yuyv_width = vgGetParameteri(yuyv_image, VG_IMAGE_WIDTH);
#if CVPI_CAREFUL == 1
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    return VG_INVALID_HANDLE;
  }
#endif
  VGint height = vgGetParameteri(yuyv_image, VG_IMAGE_HEIGHT);
#if CVPI_CAREFUL == 1
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    return VG_INVALID_HANDLE;
  }
#endif
  const VGfloat yuyv2yuva_conversion_1[20] = {
  0, 0, 0, 1,
  0, 1, 0, 0,
  0, 0, 1, 0,
  1, 0, 0, 0,
  0, 0, 0, 0
  };
  const VGfloat yuyv2yuva_conversion_2[20] = {
  0,0,1,0,
  0,1,0,0,
  0,0,0,1,
  1,0,0,0,

  0,0,0,0
  };

  VGint yuva_width = yuyv_width*2;

  unsigned long itter;

  VGImage mod_image_1 = vgCreateImage(CVPI_COLOR_SPACE, yuyv_width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    return VG_INVALID_HANDLE;
  }
  VGImage mod_image_2 = vgCreateImage(CVPI_COLOR_SPACE, yuyv_width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    vgDestroyImage(mod_image_1);
    vgFlush();
    return VG_INVALID_HANDLE;
  }
  VGImage mod_image_3 = vgCreateImage(CVPI_COLOR_SPACE, yuyv_width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    vgDestroyImage(mod_image_1);
    vgDestroyImage(mod_image_2);
    vgFlush();
    return VG_INVALID_HANDLE;
  }

  VGImage mod_image_4 = vgCreateImage(CVPI_COLOR_SPACE, yuyv_width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    vgDestroyImage(mod_image_1);
    vgDestroyImage(mod_image_2);
    vgDestroyImage(mod_image_3);
    vgFlush();
    return VG_INVALID_HANDLE;
  }
  /* set red in RGBA to 255 */
  vgLookup(mod_image_1, yuyv_image, cvpi_255_array, cvpi_identity_array, cvpi_identity_array, cvpi_identity_array, OUTPUT_LINEAR, VG_FALSE);
#if CVPI_CAREFUL == 1
  /* Should never error, all pre-conditions are covered. Might internally run out of memory if not in place. */
  cvpi_vg_error_check();
#endif

  /* set blue in RGBA to 255 */
  vgLookup(mod_image_2, yuyv_image, cvpi_identity_array, cvpi_identity_array, cvpi_255_array, cvpi_identity_array, OUTPUT_LINEAR, VG_FALSE);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  vgFinish();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif

  vgColorMatrix(mod_image_3, mod_image_1, yuyv2yuva_conversion_1);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  vgColorMatrix(mod_image_4, mod_image_2, yuyv2yuva_conversion_2);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  vgFinish();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif

  /* images no longer needed */
  vgDestroyImage(mod_image_1);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  vgDestroyImage(mod_image_2);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif

  VGImage output_image = vgCreateImage(CVPI_COLOR_SPACE, yuva_width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    output_image = VG_INVALID_HANDLE;
    goto cvpi_yuyv2yuva_skip_loop;
  }

  /* merge mod 3,4 images */
  for(itter = 0; itter < yuyv_width; ++itter) {
    vgCopyImage(output_image, itter*2, 0, mod_image_3, itter, 0, 1, height, VG_FALSE);
#if CVPI_CAREFUL == 1
    cvpi_vg_error_check();
#endif
    vgCopyImage(output_image, itter*2+1, 0, mod_image_4, itter, 0, 1, height, VG_FALSE);
#if CVPI_CAREFUL == 1
    cvpi_vg_error_check();
#endif
    vgFinish();
#if CVPI_CAREFUL == 1
    cvpi_vg_error_check();
#endif
  }
 cvpi_yuyv2yuva_skip_loop:
  vgDestroyImage(mod_image_3);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  vgDestroyImage(mod_image_4);
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  vgFlush();
#if CVPI_CAREFUL == 1
  cvpi_vg_error_check();
#endif
  return output_image;
}

VGImage cvpi_add_images(const VGImage img1, const VGImage img2, VGshort a, VGshort b, VGfloat scale, VGfloat bias) {
/* add_images adds two images by adding the upper halves and the lower
   halves separately.  Done this way so that the intermediate image
   does not exceed allowable dimensions. */

  /* TODO: images of height 1024 or less can be added using a single image buffer */
  VGErrorCode error;

  VGint img1_width = vgGetParameteri(img1, VG_IMAGE_WIDTH);
#if CVPI_CAREFUL == 1
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    return VG_INVALID_HANDLE;
  }
#endif
  VGint img1_height = vgGetParameteri(img1, VG_IMAGE_HEIGHT);
#if CVPI_CAREFUL == 1
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    return VG_INVALID_HANDLE;
  }
#endif

  VGint img2_width = vgGetParameteri(img2, VG_IMAGE_WIDTH);
#if CVPI_CAREFUL == 1
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    return VG_INVALID_HANDLE;
  }
#endif
  VGint img2_height = vgGetParameteri(img2, VG_IMAGE_HEIGHT);
#if CVPI_CAREFUL == 1
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    return VG_INVALID_HANDLE;
  }
#endif

  if(img1_width != img2_width || img1_height != img2_height) {
    cvpi_log_6("%s:%d: Images have differing dimensions:\nimg1: %dx%d\nimg2: %dx%d\n",
	       __func__, __LINE__, img1_width, img1_height, img2_width, img2_height);
    return VG_INVALID_HANDLE;
  }

  VGshort kernel[2] = {b,a};	/* only place where `a' and `b' are used */
  unsigned long itter = 0;
  VGImage output = vgCreateImage(CVPI_COLOR_SPACE, img1_width, img1_height, VG_IMAGE_QUALITY_NONANTIALIASED);
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    return VG_INVALID_HANDLE;
  }

  if(2*img1_height > EGL_CONFIG_MAX_HEIGHT) {
    VGImage half;			/* image half original */
    VGImage half_c;		/* image half convolved */
    VGImage last_row;
    VGImage last_row_c;		/* last row convolved */
    VGint max;
    if(!(img1_height % 2)) { 	/* even height */
      half = vgCreateImage(CVPI_COLOR_SPACE, img1_width, img1_height, VG_IMAGE_QUALITY_NONANTIALIASED);
      error = vgGetError();
      if(error != VG_NO_ERROR) {
	cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
	return VG_INVALID_HANDLE;
      }
      half_c = vgCreateImage(CVPI_COLOR_SPACE, img1_width, img1_height, VG_IMAGE_QUALITY_NONANTIALIASED);
      error = vgGetError();
      if(error != VG_NO_ERROR) {
	cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
	vgDestroyImage(half);
	vgFlush();
	return VG_INVALID_HANDLE;
      }
      max = img1_height;
    } else {                      /* odd height */
      last_row = vgCreateImage(CVPI_COLOR_SPACE, img1_width, 2, VG_IMAGE_QUALITY_NONANTIALIASED);
      error = vgGetError();
      if(error != VG_NO_ERROR) {
	cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
	return VG_INVALID_HANDLE;
      }
      last_row_c = vgCreateImage(CVPI_COLOR_SPACE, img1_width, 2, VG_IMAGE_QUALITY_NONANTIALIASED);
      error = vgGetError();
      if(error != VG_NO_ERROR) {
	cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
	vgDestroyImage(last_row);
	vgFlush();
	return VG_INVALID_HANDLE;
      }
      if(img1_height > 1) {
	half = vgCreateImage(CVPI_COLOR_SPACE, img1_width, img1_height-1, VG_IMAGE_QUALITY_NONANTIALIASED);
	error = vgGetError();
	if(error != VG_NO_ERROR) {
	  cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
	  vgDestroyImage(last_row);
	  vgDestroyImage(last_row_c);
	  vgFlush();
	  return VG_INVALID_HANDLE;
	}
	half_c = vgCreateImage(CVPI_COLOR_SPACE, img1_width, img1_height-1, VG_IMAGE_QUALITY_NONANTIALIASED);
	error = vgGetError();
	if(error != VG_NO_ERROR) {
	  cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
	  vgDestroyImage(last_row);
	  vgDestroyImage(last_row_c);
	  vgDestroyImage(half);
	  vgFlush();
	  return VG_INVALID_HANDLE;
	}
	max = img1_height-1;
      } else {			/* 1 pixel high image */
	max = 0;
      }
    }

    if(img1_height > 1) {
      /* add the two upper halves */
      for(itter = 0; itter < max/2; ++itter) {
	vgCopyImage(half, 0, 2*itter, img1, 0, itter, img1_width, 1, VG_FALSE);
	vgCopyImage(half, 0, 2*itter+1, img2, 0, itter, img1_width, 1, VG_FALSE);
	vgFinish();
      }
      /* add the current pixel with the one below it */
      /* tiling mode does not matter since the last row is not returned */
      vgConvolveNormal(half_c, half, 1, 2, 0, 0, kernel, scale, bias, VG_TILE_PAD);

      /* copy added upper half to output */
      unsigned long i;
      for(i = 0; i < max/2; ++i) {
	vgCopyImage(output, 0, i, half_c, 0, 2*i, img1_width, 1, VG_FALSE);
	vgFinish();
      }
      /* add the two lower halves */
      for(; itter < max; ++itter) {
	vgCopyImage(half, 0, 2*itter, img1, 0, itter, img1_width, 1, VG_FALSE);
	vgCopyImage(half, 0, 2*itter+1, img2, 0, itter, img1_width, 1, VG_FALSE);
	vgFinish();
      }
      vgConvolveNormal(half_c, half, 1, 2, 0, 0, kernel, scale, bias, VG_TILE_PAD);

      /* copy added lower half to output */
      unsigned long j;
      for(j = 0; j < max/2; ++i, ++j) {
	vgCopyImage(output, 0, i, half_c, 0, 2*j, img1_width, 1, VG_FALSE);
	vgFinish();
      }
    }
    /* for odd height images, add the last rows */
    if(img1_height % 2) {
      vgCopyImage(last_row, 0, 0, img1, 0, img1_height-1, img1_width, 1, VG_FALSE);
      vgCopyImage(last_row, 0, 1, img2, 0, img1_height-1, img1_width, 1, VG_FALSE);
      vgConvolveNormal(last_row_c, last_row, 1, 2, 0, 0, kernel, scale, bias, VG_TILE_PAD);
      vgCopyImage(output, 0, img1_height-1, last_row_c, 0, 0, img1_width, 1, VG_FALSE);
      vgFinish();
    }
    vgDestroyImage(half);
    vgDestroyImage(half_c);
    vgDestroyImage(last_row);
    vgDestroyImage(last_row_c);
  } else {
    /* both images together fit in a single buffer */

    /* combine the two images */
    VGImage both = vgCreateImage(CVPI_COLOR_SPACE, img1_width, 2*img1_height, VG_IMAGE_QUALITY_NONANTIALIASED);
      error = vgGetError();
      if(error != VG_NO_ERROR) {
	cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
	return VG_INVALID_HANDLE;
      }
    VGImage added = vgCreateImage(CVPI_COLOR_SPACE, img1_width, 2*img1_height, VG_IMAGE_QUALITY_NONANTIALIASED);
      error = vgGetError();
      if(error != VG_NO_ERROR) {
	cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
	vgDestroyImage(added);
	vgFlush();
	return VG_INVALID_HANDLE;
      }
    for(; itter < img1_height; ++itter) {
      vgCopyImage(both, 0, 2*itter, img1, 0, itter, img1_width, 1, VG_FALSE);
      vgCopyImage(both, 0, 2*itter+1, img2, 0, itter, img1_width, 1, VG_FALSE);
      vgFinish();
    }

    /* add */
    vgConvolveNormal(added, both, 1, 2, 0, 0, kernel, scale, bias, VG_TILE_PAD);
    vgFinish();

    /* put the added rows into output */
    for(itter = 0; itter < img1_height; ++itter) {
      vgCopyImage(output, 0, itter, added, 0, 2*itter, img1_width, 1, VG_FALSE);
      vgFinish();
    }

    vgDestroyImage(both);
    vgDestroyImage(added);
  }
  vgFlush();
  return output;
}

VGImage cvpi_add_channels(const VGImage image, VGImageChannel channel1, VGImageChannel channel2,
			  VGshort a, VGshort b, VGfloat scale, VGfloat bias, VGbitfield output_channels) {
  /* Could rewrite this function to do the computation just using vgColorMatrix */
  VGErrorCode error;
  VGint image_width = vgGetParameteri(image, VG_IMAGE_WIDTH);
#if CVPI_CAREFUL == 1
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    return VG_INVALID_HANDLE;
  }
#endif
  VGint image_height = vgGetParameteri(image, VG_IMAGE_HEIGHT);
#if CVPI_CAREFUL == 1
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    return VG_INVALID_HANDLE;
  }
#endif
  VGImage channel1_img = vgCreateImage(CVPI_COLOR_SPACE, image_width, image_height, VG_IMAGE_QUALITY_NONANTIALIASED);
      error = vgGetError();
      if(error != VG_NO_ERROR) {
	cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
	return VG_INVALID_HANDLE;
      }
  VGImage channel2_img = vgCreateImage(CVPI_COLOR_SPACE, image_width, image_height, VG_IMAGE_QUALITY_NONANTIALIASED);
      error = vgGetError();
      if(error != VG_NO_ERROR) {
	cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
	vgDestroyImage(channel1_img);
	vgFlush();
	return VG_INVALID_HANDLE;
      }
  /* Copy the channel to be added to all of the other channels */
  VGfloat copy_channel1[20] = {
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0
  };

  VGfloat copy_channel2[20] = {
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0
  };

  switch(channel1) {
  case VG_RED:
    copy_channel1[0] = 1;
    copy_channel1[1] = 1;
    copy_channel1[2] = 1;
    copy_channel1[3] = 1;
    break;
  case VG_GREEN:
    copy_channel1[4] = 1;
    copy_channel1[5] = 1;
    copy_channel1[6] = 1;
    copy_channel1[7] = 1;
    break;
  case VG_BLUE:
    copy_channel1[8] = 1;
    copy_channel1[9] = 1;
    copy_channel1[10] = 1;
    copy_channel1[11] = 1;
    break;
  case VG_ALPHA:
    copy_channel1[12] = 1;
    copy_channel1[13] = 1;
    copy_channel1[14] = 1;
    copy_channel1[15] = 1;
    break;
  default:
    break;
  }

  switch(channel2) {
  case VG_RED:
    copy_channel2[0] = 1;
    copy_channel2[1] = 1;
    copy_channel2[2] = 1;
    copy_channel2[3] = 1;
    break;
  case VG_GREEN:
    copy_channel2[4] = 1;
    copy_channel2[5] = 1;
    copy_channel2[6] = 1;
    copy_channel2[7] = 1;
    break;
  case VG_BLUE:
    copy_channel2[8] = 1;
    copy_channel2[9] = 1;
    copy_channel2[10] = 1;
    copy_channel2[11] = 1;
    break;
  case VG_ALPHA:
    copy_channel2[12] = 1;
    copy_channel2[13] = 1;
    copy_channel2[14] = 1;
    copy_channel2[15] = 1;
    break;
  default:
    break;
  }

  vgColorMatrix(channel1_img, image, copy_channel1);
  vgColorMatrix(channel2_img, image, copy_channel2);
  vgFinish();

  /* add the channels */
  VGImage result = cvpi_add_images(channel1_img, channel2_img, a, b, scale, bias);
  vgFinish();
      if(result == VG_INVALID_HANDLE) {
	cvpi_log_1("%s:%d:Unable to add channels\n", __func__, __LINE__);
	vgDestroyImage(channel1_img);
	vgDestroyImage(channel2_img);
	vgFlush();
	return VG_INVALID_HANDLE;
      }
  if(output_channels == (VG_RED | VG_GREEN | VG_BLUE | VG_ALPHA) || result == VG_INVALID_HANDLE) {
    vgDestroyImage(channel1_img);
    vgDestroyImage(channel2_img);
    vgFlush();
    return result;
  } else {
    VGImage result_combo = cvpi_combine_channelwise(result, image, output_channels);
    vgDestroyImage(channel1_img);
    vgDestroyImage(channel2_img);
    vgDestroyImage(result);
    vgFlush();
    return result_combo;
  }
}

VGImage cvpi_add_channels_color(const VGImage image, VGfloat scale_r, VGfloat scale_g,
				VGfloat scale_b, VGfloat bias, VGbitfield output_channels) {
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
    matrix[16] = bias;
  }
  if(output_channels & VG_GREEN) {
    matrix[1] = scale_r;
    matrix[5] = scale_g;
    matrix[9] = scale_b;
    matrix[17] = bias;
  }
  if(output_channels & VG_BLUE) {
    matrix[2] = scale_r;
    matrix[6] = scale_g;
    matrix[10] = scale_b;
    matrix[18] = bias;
  }
  if(output_channels & VG_ALPHA) {
    matrix[3] = scale_r;
    matrix[7] = scale_g;
    matrix[11] = scale_b;
    matrix[15] = 0;
    matrix[19] = bias;
  }

  VGint image_width = vgGetParameteri(image, VG_IMAGE_WIDTH);
  VGint image_height = vgGetParameteri(image, VG_IMAGE_HEIGHT);
  VGImage output = vgCreateImage(CVPI_COLOR_SPACE, image_width, image_height, VG_IMAGE_QUALITY_NONANTIALIASED);
      error = vgGetError();
      if(error != VG_NO_ERROR) {
	cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
	return VG_INVALID_HANDLE;
      }
  vgColorMatrixNormal(output, image, matrix);
  vgFinish();
  return output;
}

VGImage cvpi_add_channels_all(const VGImage image, VGfloat scale_r, VGfloat scale_g,
			      VGfloat scale_b, VGfloat scale_a,VGfloat bias, VGbitfield output_channels) {
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
    matrix[12] = scale_a;
    matrix[16] = bias;
  }
  if(output_channels & VG_GREEN) {
    matrix[1] = scale_r;
    matrix[5] = scale_g;
    matrix[9] = scale_b;
    matrix[13] = scale_a;
    matrix[17] = bias;
  }
  if(output_channels & VG_BLUE) {
    matrix[2] = scale_r;
    matrix[6] = scale_g;
    matrix[10] = scale_b;
    matrix[14] = scale_a;
    matrix[18] = bias;
  }
  if(output_channels & VG_ALPHA) {
    matrix[3] = scale_r;
    matrix[7] = scale_g;
    matrix[11] = scale_b;
    matrix[15] = scale_a;
    matrix[19] = bias;
  }

  VGint image_width = vgGetParameteri(image, VG_IMAGE_WIDTH);
  VGint image_height = vgGetParameteri(image, VG_IMAGE_HEIGHT);
  VGImage output = vgCreateImage(CVPI_COLOR_SPACE, image_width, image_height, VG_IMAGE_QUALITY_NONANTIALIASED);
      error = vgGetError();
      if(error != VG_NO_ERROR) {
	cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
	return VG_INVALID_HANDLE;
      }
  vgColorMatrixNormal(output, image, matrix);
  vgFinish();
  return output;
}

VGImage cvpi_combine_channelwise(const VGImage img1, const VGImage img2, VGbitfield img1_channels) {
  VGErrorCode error;
  VGint img1_width = vgGetParameteri(img1, VG_IMAGE_WIDTH);
#if CVPI_CAREFUL == 1
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    return VG_INVALID_HANDLE;
  }
#endif
  VGint img1_height = vgGetParameteri(img1, VG_IMAGE_HEIGHT);
#if CVPI_CAREFUL == 1
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    return VG_INVALID_HANDLE;
  }
#endif
  VGint img2_width = vgGetParameteri(img2, VG_IMAGE_WIDTH);
#if CVPI_CAREFUL == 1
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    return VG_INVALID_HANDLE;
  }
#endif
  VGint img2_height = vgGetParameteri(img2, VG_IMAGE_HEIGHT);
#if CVPI_CAREFUL == 1
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    return VG_INVALID_HANDLE;
  }
#endif

  if(img1_width != img2_width || img1_height != img2_height) {
    cvpi_log_6("%s:%d: Images have differing dimensions:\nimg1: %dx%d\nimg2: %dx%d\n",
	       __func__, __LINE__, img1_width, img1_height, img2_width, img2_height);
    return VG_INVALID_HANDLE;
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

  VGImage img1_filtered = vgCreateImage(CVPI_COLOR_SPACE, img1_width, img1_height, VG_IMAGE_QUALITY_NONANTIALIASED);
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    return VG_INVALID_HANDLE;
  }
  VGImage img2_filtered = vgCreateImage(CVPI_COLOR_SPACE, img2_width, img2_height, VG_IMAGE_QUALITY_NONANTIALIASED);
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    vgDestroyImage(img1_filtered);
    vgFlush();
    return VG_INVALID_HANDLE;
  }
  vgColorMatrix(img1_filtered, img1, img1_channel_matrix);
  vgColorMatrix(img2_filtered, img2, img2_channel_matrix);
  vgFinish();
  VGImage output = cvpi_add_images(img1_filtered, img2_filtered, 1, 1, 1, 0);
  vgFinish();
  if(output == VG_INVALID_HANDLE) {
    cvpi_log_1("%s:%d:Unable to add channels\n", __func__, __LINE__);
  }
  vgDestroyImage(img1_filtered);
  vgDestroyImage(img2_filtered);
  vgFlush();
  return output;
}

VGImage cvpi_channel_threshold(const VGImage image, VGImageChannel channel,
			       VGubyte bound1, VGubyte bound2,
			       VGubyte fill, CVPI_BOOL invert) {
  VGubyte upper = bound1 > bound2 ? bound1 : bound2;
  VGubyte lower = bound1 > bound2 ? bound2 : bound1;
  VGubyte threshold_array[256];

  unsigned int itter = 0;
  if(CVPI_TRUE_TEST(invert)) {
    /* keep values outside the bounds and fill the others */
    for(; itter < 256; ++itter) {
      if(itter < lower || itter > upper) {
	threshold_array[itter] = itter;
      } else {
	threshold_array[itter] = fill;
      }
    }
  } else {
    /* keep values inside the bounds and fill the others */
    for(; itter < 256; ++itter) {
      if(itter < lower || itter > upper) {
	threshold_array[itter] = fill;
      } else {
	threshold_array[itter] = itter;
      }
    }
  }
  VGErrorCode error;
  VGint img1_width = vgGetParameteri(image, VG_IMAGE_WIDTH);
#if CVPI_CAREFUL == 1
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    return VG_INVALID_HANDLE;
  }
#endif
  VGint img1_height = vgGetParameteri(image, VG_IMAGE_HEIGHT);
#if CVPI_CAREFUL == 1
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    return VG_INVALID_HANDLE;
  }
#endif
  VGImage output = vgCreateImage(CVPI_COLOR_SPACE, img1_width, img1_height, VG_IMAGE_QUALITY_NONANTIALIASED);
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    return VG_INVALID_HANDLE;
  }
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
  vgFinish();
  return output;
}


VGImage cvpi_image_threshold(const VGImage image,
			     const VGubyte* lower_bound, const VGubyte* upper_bound,
			     const VGubyte* fill, const CVPI_BOOL* invert, CVPI_BOOL dependent) {
  VGubyte threshold_array[4][256];

  unsigned int i = 0;
  unsigned char j = 0;
  if(CVPI_TRUE_TEST(dependent)) {
    int inv = CVPI_TRUE_TEST(invert[0])
      || CVPI_TRUE_TEST(invert[1])
      || CVPI_TRUE_TEST(invert[2])
      || CVPI_TRUE_TEST(invert[3]);
    for(; i < 256; ++i) {
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
    for(; i < 256; ++i) {
      j = 0;
      for(; j < 4; ++j) {
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
  VGErrorCode error;
  VGint img1_width = vgGetParameteri(image, VG_IMAGE_WIDTH);
#if CVPI_CAREFUL == 1
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    return VG_INVALID_HANDLE;
  }
#endif
  VGint img1_height = vgGetParameteri(image, VG_IMAGE_HEIGHT);
#if CVPI_CAREFUL == 1
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    return VG_INVALID_HANDLE;
  }
#endif
  VGImage output = vgCreateImage(CVPI_COLOR_SPACE, img1_width, img1_height, VG_IMAGE_QUALITY_NONANTIALIASED);
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    return VG_INVALID_HANDLE;
  }
  vgLookup(output, image,
	   threshold_array[0],
	   threshold_array[1],
	   threshold_array[2],
	   threshold_array[3],
	   OUTPUT_LINEAR, VG_FALSE);
  vgFinish();
  return output;
}

VGImage cvpi_image_threshold_sector(const VGImage image, unsigned int sector_width, unsigned int sector_height, const VGubyte* fill, const CVPI_BOOL* invert, CVPI_BOOL dependent, const VGuint (*statistic)(const VGImage image)) {

  VGubyte invert_array[4] = {0,0,0,0};

  VGErrorCode error;
  VGint image_width = vgGetParameteri(image, VG_IMAGE_WIDTH);
#if CVPI_CAREFUL == 1
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    return VG_INVALID_HANDLE;
  }
#endif
  VGint image_height = vgGetParameteri(image, VG_IMAGE_HEIGHT);
#if CVPI_CAREFUL == 1
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    return VG_INVALID_HANDLE;
  }
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

  VGImage output_image = vgCreateImage(CVPI_COLOR_SPACE, image_width, image_height, VG_IMAGE_QUALITY_NONANTIALIASED);
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    return VG_INVALID_HANDLE;
  }
  cvpi_pixel sector_threshold;

  sector_threshold.all = 0;

  int i = 0;
  int j = 0;
  VGImage sector_image;
  /* threshold normal sectors */
  for(; i < short_sector_num; ++i) {
    for(; j < thin_sector_num; ++j) {
      /* get the sector mean and store it in sector_threshold */
      sector_threshold.all = statistic(vgChildImage(image, i*sector_width, j*sector_height,
						    sector_width, sector_height));
      vgFinish();
      /* threshold the sector and insert it into the output image */
      sector_image = cvpi_image_threshold(vgChildImage(image, i*sector_width, j*sector_height,
						       sector_width, sector_height),
					  invert_array,
					  sector_threshold.channel,
					  fill, invert, dependent);
      if(sector_image == VG_INVALID_HANDLE) {
	cvpi_log_1("%s:%d:Unable to threshold sector\n", __func__, __LINE__);
	vgDestroyImage(output_image);
	vgFlush();
	return sector_image;
      }
      vgCopyImage(output_image, i*sector_width, j*sector_height,
		  sector_image,
		  0, 0, sector_width, sector_height, VG_FALSE);
      vgDestroyImage(sector_image);
      vgFinish();
    }
  }

  /* threshold right sectors */
  for(j = 0; j < thin_sector_num; ++j) {

    sector_threshold.all = statistic(vgChildImage(image, image_width - thinness,
						  j*sector_height, thinness, sector_height));
    vgFinish();
    sector_image = cvpi_image_threshold(vgChildImage(image, image_width - thinness, j*sector_height,
						     thinness, sector_height),
					invert_array,
					sector_threshold.channel,
					fill, invert, dependent);
      if(sector_image == VG_INVALID_HANDLE) {
	cvpi_log_1("%s:%d:Unable to threshold sector\n", __func__, __LINE__);
	vgDestroyImage(output_image);
	vgFlush();
	return sector_image;
      }
    vgCopyImage(output_image, image_width - thinness, j*sector_height,
		sector_image,
		0, 0, thinness, sector_height, VG_FALSE);
    vgDestroyImage(sector_image);
    vgFinish();
  }

  /* threshold bottom sectors */
  for(i = 0; i < short_sector_num; ++i) {
    sector_threshold.all = statistic(vgChildImage(image, i*sector_width, image_height - shortness,
						  sector_width, shortness));
    vgFinish();
    sector_image = cvpi_image_threshold(vgChildImage(image, i*sector_width, image_height - shortness, 
						     sector_width, shortness),
					invert_array,
					sector_threshold.channel,
					fill, invert, dependent);
      if(sector_image == VG_INVALID_HANDLE) {
	cvpi_log_1("%s:%d:Unable to threshold sector\n", __func__, __LINE__);
	vgDestroyImage(output_image);
	vgFlush();
	return sector_image;
      }
    vgCopyImage(output_image, i*sector_width, image_height - shortness,
		sector_image,
		0, 0, sector_width, shortness, VG_FALSE);
    vgDestroyImage(sector_image);
    vgFinish();
  }

  /* threshold the bottom right sector */
  sector_threshold.all = statistic(vgChildImage(image, image_width - thinness, image_height - shortness,
						thinness, shortness));
  vgFinish();
  sector_image = cvpi_image_threshold(vgChildImage(image, image_width - thinness, image_height - shortness,
						   thinness, shortness),
				      invert_array,
				      sector_threshold.channel,
				      fill, invert, dependent);
      if(sector_image == VG_INVALID_HANDLE) {
	cvpi_log_1("%s:%d:Unable to threshold sector\n", __func__, __LINE__);
	vgDestroyImage(output_image);
	vgFlush();
	return sector_image;
      }
  vgCopyImage(output_image, image_width - thinness, image_height - shortness,
	      sector_image,
	      0, 0, thinness, shortness, VG_FALSE);
  vgDestroyImage(sector_image);
  vgFinish();

  return output_image;
}

VGImage cvpi_image_channel_mask(const VGImage image, VGImageChannel maskChannel, CVPI_BOOL add) {
  VGfloat matrix[20] = {
    1,0,0,0,
    0,1,0,0,
    0,0,1,0,
    0,0,0,1,
    0,0,0,0
  };

  VGfloat mask_value = CVPI_TRUE_TEST(add) ? VG_MAX_FLOAT : -VG_MAX_FLOAT;


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
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    return VG_INVALID_HANDLE;
  }
#endif
  VGint image_height = vgGetParameteri(image, VG_IMAGE_HEIGHT);
#if CVPI_CAREFUL == 1
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    return VG_INVALID_HANDLE;
  }
#endif
  VGImage output_image = vgCreateImage(CVPI_COLOR_SPACE, image_width, image_height, VG_IMAGE_QUALITY_NONANTIALIASED);
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    return VG_INVALID_HANDLE;
  }
  vgColorMatrix(output_image, image, matrix);
  vgFinish();
  return output_image;
}

VGImage cvpi_image_mean_gpu(const VGImage image) {
  /* See project.org (heading: image mean) for detailed description of
     the algorithm being used. */
  VGErrorCode error;
  VGint image_width = vgGetParameteri(image, VG_IMAGE_WIDTH);
#if CVPI_CAREFUL == 1
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    return VG_INVALID_HANDLE;
  }
#endif
  VGint image_height = vgGetParameteri(image, VG_IMAGE_HEIGHT);
#if CVPI_CAREFUL == 1
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    return VG_INVALID_HANDLE;
  }
#endif
  VGshort kernel[2] = {1,1};

  VGImage mean_image;
  VGImage first_image = vgCreateImage(CVPI_COLOR_SPACE, image_width, image_height, VG_IMAGE_QUALITY_NONANTIALIASED);
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    return VG_INVALID_HANDLE;
  }
  vgFinish();
  vgCopyImage(first_image, 0, 0, image, 0, 0, image_width, image_height, VG_FALSE);
  vgFinish();
#if CVPI_DEBUG == 1
  char* error = cvpi_vg_error_string(vgGetError());
  unsigned long *debug_memory;
  debug_memory = malloc(4*image_height*image_width);
  vgGetImageSubData(first_image, (void*)debug_memory,
		    4*image_width, CVPI_COLOR_SPACE, 0, 0, image_width, image_height);
  free(debug_memory);
#endif

  while(1 < image_height) {
    --image_height;
    mean_image = vgCreateImage(CVPI_COLOR_SPACE, image_width, image_height, VG_IMAGE_QUALITY_NONANTIALIASED);
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    vgDestroyImage(first_image);
    vgFlush();
    return VG_INVALID_HANDLE;
  }
    vgConvolveNormal(mean_image, first_image, 1, 2, 0, 0, kernel, 0.5, 0, VG_TILE_PAD);
    vgFinish();
    vgDestroyImage(first_image);
    vgFinish();
    first_image = vgCreateImage(CVPI_COLOR_SPACE, image_width, image_height, VG_IMAGE_QUALITY_NONANTIALIASED);
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    vgDestroyImage(mean_image);
    vgFlush();
    return VG_INVALID_HANDLE;
  }
    /* remove the last row of the resulting image */

    vgCopyImage(first_image, 0, 0, mean_image, 0, 0, image_width, image_height, VG_FALSE);
    vgFinish();
#if CVPI_DEBUG == 1
    char* error = cvpi_vg_error_string(vgGetError());
    debug_memory = malloc(4*image_height*image_width);
    vgGetImageSubData(first_image, debug_memory,
		      4*image_width, CVPI_COLOR_SPACE, 0, 0, image_width, image_height);
    free(debug_memory);
#endif
    vgDestroyImage(mean_image);
    vgFinish();
    /* Repeat until there is only one row left. Turn the row into a
       column and repeat the averaging until there is only one cell
       left. */

#if CVPI_DEBUG == 1
    free(debug_memory);
#endif
  }

  while(1 < image_width) {
#if CVPI_DEBUG == 1
  debug_memory = malloc(4*image_height*image_width);
  vgGetImageSubData(first_image, (void*)debug_memory,
		    4*image_width, CVPI_COLOR_SPACE, 0, 0, image_width, image_height);
  free(debug_memory);
#endif
    --image_width;
    mean_image = vgCreateImage(CVPI_COLOR_SPACE, image_width, image_height, VG_IMAGE_QUALITY_NONANTIALIASED);
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    vgDestroyImage(first_image);
    vgFlush();
    return VG_INVALID_HANDLE;
  }
    vgConvolveNormal(mean_image, first_image, 2, 1, 0, 0, kernel, 0.5, 0, VG_TILE_PAD);
    vgFinish();
#if CVPI_DEBUG == 1
    debug_memory = malloc(4*image_height*image_width);
    vgGetImageSubData(mean_image, (void*)debug_memory,
		      4*image_width, CVPI_COLOR_SPACE, 0, 0, image_width, image_height);
    free(debug_memory);
#endif
    vgDestroyImage(first_image);
    vgFinish();
    first_image = vgCreateImage(CVPI_COLOR_SPACE, image_width, image_height, VG_IMAGE_QUALITY_NONANTIALIASED);
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    vgDestroyImage(mean_image);
    vgFlush();
    return VG_INVALID_HANDLE;
  }
    vgFinish();
    vgCopyImage(first_image, 0, 0, mean_image, 0, 0, image_width, image_height, VG_FALSE);
    vgFinish();
#if CVPI_DEBUG == 1
    error = cvpi_vg_error_string(vgGetError());
    debug_memory = malloc(4*image_height*image_width);
    vgGetImageSubData(first_image, (void*)debug_memory,
		      4*image_width, CVPI_COLOR_SPACE, 0, 0, image_width, image_height);
    free(debug_memory);
#endif
    vgDestroyImage(mean_image);
    vgFinish();
  }
  //  vgDestroyImage(mean_image);

  return first_image;
}

VGImage cvpi_image_mean_arithmetic(const VGImage image) {
  VGErrorCode error;
  VGint width = vgGetParameteri(image, VG_IMAGE_WIDTH);
#if CVPI_CAREFUL == 1
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    return VG_INVALID_HANDLE;
  }
#endif
  VGint height = vgGetParameteri(image, VG_IMAGE_HEIGHT);
#if CVPI_CAREFUL == 1
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    return VG_INVALID_HANDLE;
  }
#endif
  VGImageFormat imageFormat = (VGImageFormat)vgGetParameteri(image, VG_IMAGE_FORMAT);
#if CVPI_CAREFUL == 1
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    return VG_INVALID_HANDLE;
  }
#endif

  VGint pixel_bytes = vgPixelBits(imageFormat)/8;

  unsigned long image_size = width * height;
  unsigned long image_mem_size = pixel_bytes * image_size;
  unsigned char* image_cpu = malloc(image_mem_size);
  if(image_cpu == NULL) {
    cvpi_log_3("%s:%d: malloc returned NULL: errno = %d\n", __func__, __LINE__, errno);
    return VG_INVALID_HANDLE;
  }
  vgGetImageSubData(image, (void*)image_cpu, pixel_bytes, imageFormat, 0, 0, width, height);

  unsigned long average_channel[4] = {0,0,0,0};

  /* currently only works with VGImageFormat = VG_*_8888 */
  cvpi_pixel offsets;
  offsets.channel[0] = 0;
  offsets.channel[1] = 1;
  offsets.channel[2] = 2;
  offsets.channel[3] = 3;

  unsigned long i = 0;

  for(; i < image_mem_size; i += pixel_bytes) {
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

  VGImage pixel = vgCreateImage(CVPI_COLOR_SPACE, 1, 1, VG_IMAGE_QUALITY_NONANTIALIASED);
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    free(image_cpu);
    return VG_INVALID_HANDLE;
  }
  vgImageSubData(pixel, &offsets, CVPI_PIXEL_BYTES, CVPI_COLOR_SPACE, 0, 0, 1, 1);

  vgFinish();

  free(image_cpu);

  return pixel;
}


VGImage cvpi_image_rgba_to_bw(const VGImage image, VGImageChannel sourceChannel, CVPI_BOOL true_color) {
  VGErrorCode error;
  VGint img1_width = vgGetParameteri(image, VG_IMAGE_WIDTH);
  VGint img1_height = vgGetParameteri(image, VG_IMAGE_HEIGHT);
  VGImageFormat imageFormat = (VGImageFormat)vgGetParameteri(image, VG_IMAGE_FORMAT);

  VGImage bw = vgCreateImage(VG_BW_1, img1_width, img1_height, VG_IMAGE_QUALITY_NONANTIALIASED);
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    return VG_INVALID_HANDLE;
  }
  VGImage intermediate = vgCreateImage(imageFormat, img1_width, img1_height, VG_IMAGE_QUALITY_NONANTIALIASED);
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    vgDestroyImage(bw);
    vgFlush();
    return VG_INVALID_HANDLE;
  }
  const VGuint* array = CVPI_TRUE_TEST(true_color) ? cvpi_binary_array : cvpi_binary_array_inverted;
  vgLookupSingle(intermediate, image, array, sourceChannel, OUTPUT_LINEAR, VG_FALSE);

  vgCopyImage(bw, 0, 0, intermediate, 0, 0, img1_width, img1_height, VG_FALSE);
  vgFinish();
  vgDestroyImage(intermediate);
  vgFlush();
  return bw;
}

static VGImage cvpi_image_logcial_common(const VGImage image1, const VGImage image2,
					 VGubyte true_value1, VGubyte true_value2,
					 VGubyte true_color, VGubyte false_color, CVPI_BOOL nonzero_true) {
  VGErrorCode error;
  VGint img1_width = vgGetParameteri(image1, VG_IMAGE_WIDTH);
  VGint img1_height = vgGetParameteri(image1, VG_IMAGE_HEIGHT);
  //  VGint img2_width = vgGetParameteri(image2, VG_IMAGE_WIDTH);
  //  VGint img2_height = vgGetParameteri(image2, VG_IMAGE_HEIGHT);

  VGImage image1_scaled = vgCreateImage(CVPI_COLOR_SPACE, img1_width, img1_height, VG_IMAGE_QUALITY_NONANTIALIASED);
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    return VG_INVALID_HANDLE;
  }
  VGImage image2_scaled = vgCreateImage(CVPI_COLOR_SPACE, img1_width, img1_height, VG_IMAGE_QUALITY_NONANTIALIASED);
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    vgDestroyImage(image1_scaled);
    vgFlush();
    return VG_INVALID_HANDLE;
  }
  static const VGubyte cvpi_binary_array_img1[256] = {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
  static const VGubyte cvpi_binary_array_img2[256] = {0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2};
  static const VGubyte cvpi_binary_array_img1_i[256] = {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  static const VGubyte cvpi_binary_array_img2_i[256] = {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

  if(CVPI_TRUE_TEST(nonzero_true)) {
    vgLookup(image1_scaled, image1,
	     cvpi_binary_array_img1, cvpi_binary_array_img1, cvpi_binary_array_img1, cvpi_binary_array_img1,
	     OUTPUT_LINEAR, VG_FALSE);
    vgLookup(image2_scaled, image2,
	     cvpi_binary_array_img2, cvpi_binary_array_img2, cvpi_binary_array_img2, cvpi_binary_array_img2,
	     OUTPUT_LINEAR, VG_FALSE);
  } else {
    vgLookup(image1_scaled, image1,
	     cvpi_binary_array_img1_i, cvpi_binary_array_img1_i, cvpi_binary_array_img1_i, cvpi_binary_array_img1_i,
	     OUTPUT_LINEAR, VG_FALSE);
    vgLookup(image2_scaled, image2,
	     cvpi_binary_array_img2_i, cvpi_binary_array_img2_i, cvpi_binary_array_img2_i, cvpi_binary_array_img2_i,
	     OUTPUT_LINEAR, VG_FALSE);
  }
  vgFinish();

#if CVPI_DEBUG == 1
  VGuint* debug_memory;
  debug_memory = malloc(4*img1_height*img1_width);
  vgGetImageSubData(image1, (void*)debug_memory,
		   4*img1_width, CVPI_COLOR_SPACE, 0, 0, img1_width, img1_height);
  free(debug_memory);

  debug_memory = malloc(4*img1_height*img1_width);
  vgGetImageSubData(image2, (void*)debug_memory,
		   4*img1_width, CVPI_COLOR_SPACE, 0, 0, img1_width, img1_height);
  free(debug_memory);

  debug_memory = malloc(4*img1_height*img1_width);
  vgGetImageSubData(image1_scaled, (void*)debug_memory,
		   4*img1_width, CVPI_COLOR_SPACE, 0, 0, img1_width, img1_height);
  free(debug_memory);
  debug_memory = malloc(4*img1_height*img1_width);
  vgGetImageSubData(image2_scaled, (void*)debug_memory,
		   4*img1_width, CVPI_COLOR_SPACE, 0, 0, img1_width, img1_height);
  free(debug_memory);
#endif

  VGImage sum = cvpi_add_images(image1_scaled, image2_scaled, 1,1,1,0);
  vgFinish();

  if(sum == VG_INVALID_HANDLE) {
    cvpi_log_1("%s:%d:Add images failed\n", __func__, __LINE__);
    vgDestroyImage(image1_scaled);
    vgDestroyImage(image2_scaled);
    vgFlush();
    return VG_INVALID_HANDLE;
  }
#if CVPI_DEBUG == 1
  debug_memory = malloc(4*img1_height*img1_width);
  vgGetImageSubData(sum, (void*)debug_memory,
		   4*img1_width, CVPI_COLOR_SPACE, 0, 0, img1_width, img1_height);
  free(debug_memory);
#endif
  VGubyte logic_array[256];

  int i = 0;
  if(true_value1 == true_value2) {
    for(; i < 256; ++i) {
      logic_array[i] = i != true_value1 ? false_color : true_color;
    }
  } else {
    for(; i < 256; ++i) {
      logic_array[i] = (i != true_value1 && i != true_value2) ? false_color : true_color;
    }
  }

  /* reuse image1_scaled for the result of the logical operation */
  vgLookup(image1_scaled, sum,
	   logic_array, logic_array, logic_array, logic_array,
	   OUTPUT_LINEAR, VG_FALSE);
  vgFinish();
  vgDestroyImage(sum);
  vgDestroyImage(image2_scaled);
  vgFlush();
  return image1_scaled;
}

inline VGImage cvpi_image_logical_and(const VGImage image1, const VGImage image2, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true) {
  return cvpi_image_logcial_common(image1, image2, 3, 3, t_c, f_c, nonzero_true);
}

inline VGImage cvpi_image_logical_nand(const VGImage image1, const VGImage image2, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true) {
  return cvpi_image_logical_and(image1, image2, f_c, t_c, nonzero_true);
}

inline VGImage cvpi_image_logical_nor(const VGImage image1, const VGImage image2, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true) {
  return cvpi_image_logcial_common(image1, image2, 0, 0, t_c, f_c, nonzero_true);
}

inline VGImage cvpi_image_logical_or(const VGImage image1, const VGImage image2, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true) {
  return cvpi_image_logical_nor(image1, image2, f_c, t_c, nonzero_true);
}

inline VGImage cvpi_image_logical_xor(const VGImage image1, const VGImage image2, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true) {
  return cvpi_image_logcial_common(image1, image2, 1, 2, t_c, f_c, nonzero_true);
}

inline VGImage cvpi_image_logical_xnor(const VGImage image1, const VGImage image2, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true) {
  return cvpi_image_logical_xor(image1, image2, f_c, t_c, nonzero_true);
}

inline VGImage cvpi_image_logical_complement(const VGImage image1, const VGImage image2, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true) {
  return cvpi_image_logcial_common(image1, image2, 1, 1, t_c, f_c, nonzero_true);
}

inline VGImage cvpi_image_logical_complement_inv(const VGImage image1, const VGImage image2, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true) {
  return cvpi_image_logical_complement(image1, image2, f_c, t_c, nonzero_true);
}

static VGImage cvpi_image_morph_common(const VGImage image, const VGshort * kernel, CVPI_BOOL nonzero_true) {
  VGErrorCode error;
  VGint image_width = vgGetParameteri(image, VG_IMAGE_WIDTH);
  VGint image_height = vgGetParameteri(image, VG_IMAGE_HEIGHT);
  // VGint fill_color_size = vgGetVectorSize(VG_TILE_FILL_COLOR);
  // VGfloat * original_fill = malloc(sizeof(* original_fill) * fill_color_size);
  // VGfloat * new_fill = calloc(fill_color_size, sizeof(* new_fill));

  VGImage binary = vgCreateImage(CVPI_COLOR_SPACE, image_width, image_height, VG_IMAGE_QUALITY_NONANTIALIASED);
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    return VG_INVALID_HANDLE;
  }
  VGImage convolved = vgCreateImage(CVPI_COLOR_SPACE, image_width, image_height, VG_IMAGE_QUALITY_NONANTIALIASED);
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    vgDestroyImage(binary);
    vgFlush();
    return VG_INVALID_HANDLE;
  }
  VGubyte logic_array[256];
  VGubyte true_color, false_color;
  if(CVPI_TRUE_TEST(nonzero_true)) {
    true_color = 1;
    false_color = 0;
  } else {
    true_color = 0;
    false_color = 1;
  }
  int i = 0;
  for(; i < 256; ++i) {
    logic_array[i] = i == 0 ? false_color : true_color;
  }

  vgFinish();

  vgLookup(binary, image,
	   logic_array, logic_array, logic_array, logic_array,
	   OUTPUT_LINEAR, VG_FALSE);
  vgFinish();

  // vgGetfv(VG_TILE_FILL_COLOR, fill_color_size, original_fill);
  // vgSetfv(VG_TILE_FILL_COLOR, fill_color_size, new_fill);

  /* Upper right element of the input sub-image in the convolution
     becomes the center element (see VG 1.1 spec p. 179 fig. 27). Is the
     full sum of products between the kernel and sub-image. */
  vgConvolve(convolved, binary, 3, 3, 1, 1, kernel, 1, 0, VG_TILE_REFLECT);
  //  vgSetfv(VG_TILE_FILL_COLOR, fill_color_size, original_fill);
  vgFinish();

  // free(original_fill);
  // free(new_fill);

  vgDestroyImage(binary);
  vgFlush();
  return convolved;
}

VGImage cvpi_image_dialate(const VGImage image, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true) {
  const VGshort kernel[9] = {1,1,1,
			     1,1,1,
			     1,1,1};
  VGErrorCode error;
  VGint image_width = vgGetParameteri(image, VG_IMAGE_WIDTH);
  VGint image_height = vgGetParameteri(image, VG_IMAGE_HEIGHT);
  VGImage convolved = cvpi_image_morph_common(image, kernel, nonzero_true);
  if(convolved == VG_INVALID_HANDLE) {
    cvpi_log_1("%s:%d:cvpi_image_morph_common failed\n", __func__, __LINE__);
    return VG_INVALID_HANDLE;
  }
  VGImage dialated = vgCreateImage(CVPI_COLOR_SPACE, image_width, image_height, VG_IMAGE_QUALITY_NONANTIALIASED);
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    vgDestroyImage(convolved);
    vgFlush();
    return VG_INVALID_HANDLE;
  }
  VGubyte dialation[256];
  int i = 0;
  for(; i < 256; ++i) {
    dialation[i] = i != 0 ? t_c : f_c;
  }

  vgFinish();
  vgLookup(dialated, convolved,
	   dialation,dialation,dialation,dialation,
	   OUTPUT_LINEAR, VG_FALSE);
  vgFinish();
  vgDestroyImage(convolved);
  vgFlush();
  return dialated;
}

VGImage cvpi_image_erode(const VGImage image, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true) {
  const VGshort kernel[9] = {1,1,1,
			     1,1,1,
			     1,1,1};
  VGErrorCode error;
  VGint image_width = vgGetParameteri(image, VG_IMAGE_WIDTH);
  VGint image_height = vgGetParameteri(image, VG_IMAGE_HEIGHT);
  VGImage convolved = cvpi_image_morph_common(image, kernel, nonzero_true);
  vgFinish();
  if(convolved == VG_INVALID_HANDLE) {
    cvpi_log_1("%s:%d:cvpi_image_morph_common failed\n", __func__, __LINE__);
    return VG_INVALID_HANDLE;
  }
  VGImage eroded = vgCreateImage(CVPI_COLOR_SPACE, image_width, image_height, VG_IMAGE_QUALITY_NONANTIALIASED);
  vgFinish();
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    vgDestroyImage(convolved);
    vgFlush();
    return VG_INVALID_HANDLE;
  }
  VGubyte erosion[256];
  int i = 0;
  for(; i < 256; ++i) {
    erosion[i] = i >= 9 ? t_c : f_c;
  }

  vgFinish();
  vgLookup(eroded, convolved,
	   erosion,erosion,erosion,erosion,
	   OUTPUT_LINEAR, VG_FALSE);
  vgFinish();
  vgDestroyImage(convolved);
  vgFlush();
  return eroded;
}

VGImage cvpi_image_hit_miss(const VGImage image, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true) {
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
  //  VGint image_width = vgGetParameteri(image, VG_IMAGE_WIDTH);
  //  VGint image_height = vgGetParameteri(image, VG_IMAGE_HEIGHT);
  VGImage convolved0 = cvpi_image_morph_common(image, kernel0, nonzero_true);
  vgFinish();
  if(convolved0 == VG_INVALID_HANDLE) {
    cvpi_log_1("%s:%d:cvpi_image_morph_common failed\n", __func__, __LINE__);
    return VG_INVALID_HANDLE;
  }
  VGImage convolved1 = cvpi_image_morph_common(image, kernel1, nonzero_true);
  vgFinish();
  if(convolved1 == VG_INVALID_HANDLE) {
    cvpi_log_1("%s:%d:cvpi_image_morph_common failed\n", __func__, __LINE__);
    vgDestroyImage(convolved0);
    vgFlush();
    return VG_INVALID_HANDLE;
  }
  VGImage convolved2 = cvpi_image_morph_common(image, kernel2, nonzero_true);
  vgFinish();
  if(convolved2 == VG_INVALID_HANDLE) {
    cvpi_log_1("%s:%d:cvpi_image_morph_common failed\n", __func__, __LINE__);
    vgDestroyImage(convolved0);
    vgDestroyImage(convolved1);
    vgFlush();
    return VG_INVALID_HANDLE;
  }
  VGImage convolved3 = cvpi_image_morph_common(image, kernel3, nonzero_true);
  vgFinish();
  if(convolved3 == VG_INVALID_HANDLE) {
    cvpi_log_1("%s:%d:cvpi_image_morph_common failed\n", __func__, __LINE__);
    vgDestroyImage(convolved0);
    vgDestroyImage(convolved1);
    vgDestroyImage(convolved2);
    vgFlush();
    return VG_INVALID_HANDLE;
  }
  VGImage or01 = cvpi_image_logical_or(convolved0, convolved1, t_c, f_c, nonzero_true);
  vgFinish();
  if(or01 == VG_INVALID_HANDLE) {
    cvpi_log_1("%s:%d:cvpi_image_logical_or failed\n", __func__, __LINE__);
    vgDestroyImage(convolved0);
    vgDestroyImage(convolved1);
    vgDestroyImage(convolved2);
    vgDestroyImage(convolved3);
    vgFlush();
    return VG_INVALID_HANDLE;
  }
  VGImage or23 = cvpi_image_logical_or(convolved2, convolved3, t_c, f_c, nonzero_true);
  vgFinish();
  if(or23 == VG_INVALID_HANDLE) {
    cvpi_log_1("%s:%d:cvpi_image_logical_or failed\n", __func__, __LINE__);
    vgDestroyImage(convolved0);
    vgDestroyImage(convolved1);
    vgDestroyImage(convolved2);
    vgDestroyImage(convolved3);
    vgDestroyImage(or01);
    vgFlush();
    return VG_INVALID_HANDLE;
  }
  VGImage or0123 = cvpi_image_logical_or(or01, or23, t_c, f_c, nonzero_true);
  vgFinish();

  vgDestroyImage(convolved0);
  vgDestroyImage(convolved1);
  vgDestroyImage(convolved2);
  vgDestroyImage(convolved3);
  vgDestroyImage(or01);
  vgDestroyImage(or23);
  vgFlush();
  return or0123;
}

VGImage cvpi_image_thin(const VGImage image, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true) {
  VGImage hm = cvpi_image_hit_miss(image, t_c, f_c, nonzero_true);
  vgFinish();
  if(hm == VG_INVALID_HANDLE) {
    cvpi_log_1("%s:%d:cvpi_image_hit_miss failed\n", __func__, __LINE__);
    return VG_INVALID_HANDLE;
  }
  VGImage thinned = cvpi_image_logical_complement(image, hm, t_c, f_c, nonzero_true);
  vgFinish();
  vgDestroyImage(hm);
  vgFlush();
  return thinned;
}

VGImage cvpi_image_thicken(const VGImage image, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true) {
  VGImage hm = cvpi_image_hit_miss(image, t_c, f_c, nonzero_true);
  vgFinish();
  if(hm == VG_INVALID_HANDLE) {
    cvpi_log_1("%s:%d:cvpi_image_hit_miss failed\n", __func__, __LINE__);
    return VG_INVALID_HANDLE;
  }
  VGImage thickened = cvpi_image_logical_or(image, hm, t_c, f_c, nonzero_true);
  vgFinish();
  vgDestroyImage(hm);
  vgFlush();
  return thickened;
}

static VGubyte* channel_to_data(const VGImage image, VGImageChannel channel) {
  VGErrorCode error;
  VGint image_width = vgGetParameteri(image, VG_IMAGE_WIDTH);
  VGint image_height = vgGetParameteri(image, VG_IMAGE_HEIGHT);

  VGImage mono = vgCreateImage(VG_A_8, image_width, image_height, VG_IMAGE_QUALITY_NONANTIALIASED);
      error = vgGetError();
      if(error != VG_NO_ERROR) {
	cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
	return NULL;
      }
  if(channel != VG_ALPHA) {
    /* put the desired channel data into alpha */
    VGImage alpha = vgCreateImage(CVPI_COLOR_SPACE, image_width, image_height, VG_IMAGE_QUALITY_NONANTIALIASED);
      error = vgGetError();
      if(error != VG_NO_ERROR) {
	cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
	vgDestroyImage(mono);
	vgFlush();
	return NULL;
      }
    /* matrix by default switches red to alpha */
    VGfloat alpha_switch[20] = {
      0, 0, 0, 1,
      0, 1, 0, 0,
      0, 0, 1, 0,
      1, 0, 0, 0,

      0, 0, 0, 0
    };
    /* modify matrix to switch the right channel */
    switch(channel) {
    case VG_GREEN:
      alpha_switch[3] = 0;
      alpha_switch[7] = 1;
      break;
    case VG_BLUE:
      alpha_switch[3] = 0;
      alpha_switch[11] = 1;
      break;
    default:
      break;
    }
    vgColorMatrix(alpha, image, alpha_switch);
    vgCopyImage(mono, 0, 0, alpha, 0, 0, image_width, image_height, VG_FALSE);
    vgDestroyImage(alpha);
  } else {
    vgCopyImage(mono, 0, 0, image, 0, 0, image_width, image_height, VG_FALSE);
  }
  vgFinish();

  VGubyte* data = malloc(image_height * image_width * sizeof(*data));
  if(data == NULL) {
    cvpi_log_3("%s:%d: malloc returned NULL: errno = %d\n", __func__, __LINE__, errno);
    return NULL;
  }
  /* copy the data from alpha into memory */
  vgGetImageSubData(mono, (void*)data, image_width * sizeof(*data), VG_A_8, 0, 0, image_width, image_height);
  vgFinish();
  vgDestroyImage(mono);
  vgFlush();
  return data;
}

VGubyte cvpi_channel_max(const VGImage image, VGImageChannel channel) {
  VGint image_width = vgGetParameteri(image, VG_IMAGE_WIDTH);
  VGint image_height = vgGetParameteri(image, VG_IMAGE_HEIGHT);

  VGubyte* data = channel_to_data(image, channel);
  if(data == NULL) {
    cvpi_log_1("%s:%d: channel_to_data returned NULL\n", __func__, __LINE__);
    return 0;
  }
  VGubyte max = 0;
  unsigned long i = 0;
  unsigned long area = image_width * image_height;

  VGubyte current;
  for(; i < area && max < 255; ++i) {
    current = data[i];
    if(current > max) {
      max = current;
    }
  }

  free(data);

  return max;
}

VGubyte cvpi_channel_min(const VGImage image, VGImageChannel channel) {
  VGint image_width = vgGetParameteri(image, VG_IMAGE_WIDTH);
  VGint image_height = vgGetParameteri(image, VG_IMAGE_HEIGHT);

  VGubyte* data = channel_to_data(image, channel);
  if(data == NULL) {
    cvpi_log_1("%s:%d: channel_to_data returned NULL\n", __func__, __LINE__);
    return 0;
  }
  VGubyte min = 255;
  unsigned long i = 0;
  unsigned long area = image_width * image_height;

  VGubyte current;
  for(; i < area && min > 0; ++i) {
    current = data[i];
    if(current < min) {
      min = current;
    }
  }

  free(data);

  return min;
}

VGubyte* cvpi_channel_max_min(const VGImage image, VGImageChannel channel) {
  VGint image_width = vgGetParameteri(image, VG_IMAGE_WIDTH);
  VGint image_height = vgGetParameteri(image, VG_IMAGE_HEIGHT);

  VGubyte* data = channel_to_data(image, channel);
  if(data == NULL) {
    cvpi_log_1("%s:%d: channel_to_data returned NULL\n", __func__, __LINE__);
    return NULL;
  }

  VGubyte min = 255;
  VGubyte max = 0;
  unsigned long i = 0;
  unsigned long area = image_width * image_height;

  VGubyte current;
  for(; i < area && (min > 0 || max < 255); ++i) {
    current = data[i];
    if(current < min) {
      min = current;
    }
    if(current > max) {
      max = current;
    }
  }

  free(data);

  VGubyte* ret_val = malloc(sizeof(VGubyte)*2);
  if(ret_val == NULL) {
    cvpi_log_3("%s:%d: malloc returned NULL: errno = %d\n", __func__, __LINE__, errno);
    return NULL;
  }
  ret_val[0] = max;
  ret_val[1] = min;

  return ret_val;
}

unsigned int* cvpi_channel_histogram(const VGImage image, VGImageChannel channel) {
  VGint image_width = vgGetParameteri(image, VG_IMAGE_WIDTH);
  VGint image_height = vgGetParameteri(image, VG_IMAGE_HEIGHT);

  VGubyte* data = channel_to_data(image, channel);
  if(data == NULL) {
    cvpi_log_1("%s:%d: channel_to_data returned NULL\n", __func__, __LINE__);
    return NULL;
  }
  unsigned int* histogram = calloc(256, sizeof(*histogram));
  if(histogram == NULL) {
    cvpi_log_3("%s:%d: calloc returned NULL: errno = %d\n", __func__, __LINE__, errno);
    free(data);
    return NULL;
  }
  int i, size;
  size = image_width * image_height;
  for(i = 0; i < size; ++i) {
    histogram[data[i]]++;
  }

  free(data);

  return histogram;
}

unsigned int* cvpi_color_channels_histogram(const VGImage image) {

  VGint image_width = vgGetParameteri(image, VG_IMAGE_WIDTH);
  VGint image_height = vgGetParameteri(image, VG_IMAGE_HEIGHT);

  unsigned int stride = CVPI_PIXEL_BYTES * image_width;

  cvpi_pixel* data = malloc(stride * image_height);
  if(data == NULL) {
    cvpi_log_3("%s:%d: malloc returned NULL: errno = %d\n", __func__, __LINE__, errno);
    return NULL;
  }

  vgGetImageSubData(image, (void*)data,
		    stride, CVPI_COLOR_SPACE,
		    0, 0, image_width, image_height);

  unsigned int* histogram = calloc(768, sizeof(*histogram));
  if(data == NULL) {
    cvpi_log_3("%s:%d: malloc returned NULL: errno = %d\n", __func__, __LINE__, errno);
    free(data);
    return NULL;
  }
  int i, size;
  size = image_width * image_height;

  unsigned char* pixel;
  vgFinish();
  for(i = 0; i < size; ++i) {
    histogram[data[i].channel[cvpi_pixel_red]]++;
    histogram[data[i].channel[cvpi_pixel_green] + 256]++;
    histogram[data[i].channel[cvpi_pixel_blue] + 512]++;
  }

  free(data);

  return histogram;
}

unsigned int* cvpi_image_histogram(const VGImage image) {
  /* max value of a column is 2048*2048 = 4194304 */
  VGint image_width = vgGetParameteri(image, VG_IMAGE_WIDTH);
  VGint image_height = vgGetParameteri(image, VG_IMAGE_HEIGHT);
  unsigned int stride = CVPI_PIXEL_BYTES * image_width;
  cvpi_pixel* data = malloc(stride * image_height);
  if(data == NULL) {
    cvpi_log_3("%s:%d: malloc returned NULL: errno = %d\n", __func__, __LINE__, errno);
    return NULL;
  }

  vgGetImageSubData(image, (void*)data,
		    stride, CVPI_COLOR_SPACE,
		    0, 0, image_width, image_height);

  unsigned int* histogram = calloc(1024, sizeof(*histogram));
  if(histogram == NULL) {
    cvpi_log_3("%s:%d: calloc returned NULL: errno = %d\n", __func__, __LINE__, errno);
    free(data);
    return NULL;
  }
  int i, size;
  size = image_width * image_height;

  unsigned char* pixel;
  vgFinish();
  for(i = 0; i < size; ++i) {
    histogram[data[i].channel[cvpi_pixel_red]]++;
    histogram[data[i].channel[cvpi_pixel_green] + 256]++;
    histogram[data[i].channel[cvpi_pixel_blue] + 512]++;
    histogram[data[i].channel[cvpi_pixel_alpha] + 768]++;
  }

  free(data);

  return histogram;
}

double* cvpi_channel_cumulative_distribution(const unsigned int* histogram, VGint width, VGint height) {
  double N = width * height;
  unsigned long accumulate = 0;
  double* cd = malloc(sizeof(*cd) * 256);
  if(cd == NULL) {
    cvpi_log_3("%s:%d: malloc returned NULL: errno = %d\n", __func__, __LINE__, errno);
    return NULL;
  }
  int i = 0;
  for(; i < 256; ++i) {
    cd[i] = (accumulate += histogram[i])/N;
  }

  return cd;
}

double* cvpi_color_channels_cumulative_distribution(const unsigned int* histogram, VGint width, VGint height) {
  double N = width * height;
  unsigned long accumulate_r = 0;
  unsigned long accumulate_g = 0;
  unsigned long accumulate_b = 0;
  double* cd = malloc(sizeof(*cd) * 768);
  if(cd == NULL) {
    cvpi_log_3("%s:%d: malloc returned NULL: errno = %d\n", __func__, __LINE__, errno);
    return NULL;
  }
  int i = 0;
  for(; i < 256; ++i) {
    cd[i] = (accumulate_r += histogram[i])/N;
    cd[i+256] = (accumulate_g += histogram[i+256])/N;
    cd[i+512] = (accumulate_b += histogram[i+512])/N;
  }

  return cd;
}

double* cvpi_image_cumulative_distribution(const unsigned int* histogram, VGint width, VGint height) {
  double N = width * height;
  unsigned long accumulate_r = 0;
  unsigned long accumulate_g = 0;
  unsigned long accumulate_b = 0;
  unsigned long accumulate_a = 0;
  double* cd = malloc(sizeof(*cd) * 1024);
  if(cd == NULL) {
    cvpi_log_3("%s:%d: malloc returned NULL: errno = %d\n", __func__, __LINE__, errno);
    return NULL;
  }
  if(cd == NULL) {
    return NULL;
  }
  int i = 0;
  for(; i < 256; ++i) {
    cd[i] = (accumulate_r += histogram[i])/N;
    cd[i+256] = (accumulate_g += histogram[i+256])/N;
    cd[i+512] = (accumulate_b += histogram[i+512])/N;
    cd[i+768] = (accumulate_a += histogram[i+768])/N;
  }

  return cd;
}

VGImage cvpi_magnitude(const VGImage image1, const VGImage image2, enum cvpi_integer_truncation trunc) {
  VGErrorCode error;
  VGint img1_width = vgGetParameteri(image1, VG_IMAGE_WIDTH);
  VGint img1_height = vgGetParameteri(image1, VG_IMAGE_HEIGHT);
  VGint img2_width = vgGetParameteri(image2, VG_IMAGE_WIDTH);
  VGint img2_height = vgGetParameteri(image2, VG_IMAGE_HEIGHT);

  if(img1_width != img2_width || img1_height != img2_height) {
    cvpi_log_6("%s:%d: Images have differing dimensions:\nimg1: %dx%d\nimg2: %dx%d\n", __func__, __LINE__,
	       img1_width, img1_height, img2_width, img2_height);
    return VG_INVALID_HANDLE;
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

  VGImage img1_2 = vgCreateImage(CVPI_COLOR_SPACE, img1_width, img1_height, VG_IMAGE_QUALITY_NONANTIALIASED);
      error = vgGetError();
      if(error != VG_NO_ERROR) {
	cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
	return VG_INVALID_HANDLE;
      }
  VGImage img2_2 = vgCreateImage(CVPI_COLOR_SPACE, img1_width, img1_height, VG_IMAGE_QUALITY_NONANTIALIASED);
      error = vgGetError();
      if(error != VG_NO_ERROR) {
	cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
	vgDestroyImage(img1_2);
	vgFlush();
	return VG_INVALID_HANDLE;
      }
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
    cvpi_log_1("%s:%d:Bad truncation type, using rounding.\n", __func__, __LINE__);
    square_function = cvpi_square_array_scaled_round;
    sqrt_function = cvpi_sqrt_array_round;
    break;
  }

  /* map image1 to sqare function */
  vgLookup(img1_2, image1, square_function,square_function,square_function,square_function, OUTPUT_LINEAR, VG_FALSE);

  /* map image2 to sqare function */
  vgLookup(img2_2, image2, square_function,square_function,square_function,square_function, OUTPUT_LINEAR, VG_FALSE);
  /* add */
  VGImage sum = cvpi_add_images(img1_2, img2_2, 1, 1, 1, 0);
  vgFinish();
  if(sum == VG_INVALID_HANDLE) {
    cvpi_log_1("%s:%d:Add images failed\n", __func__, __LINE__);
    vgDestroyImage(img1_2);
    vgDestroyImage(sum);
    vgFlush();
    return VG_INVALID_HANDLE;
  }
  /* map sum to sqrt function, reuse imag1_2 */
  vgLookup(img1_2, sum, sqrt_function,sqrt_function,sqrt_function,sqrt_function, OUTPUT_LINEAR, VG_FALSE);

  /* scale, reuse img2_2 */
  vgColorMatrix(img2_2, img1_2, scaler);
  vgFinish();

  vgDestroyImage(img1_2);
  vgDestroyImage(sum);

  vgFlush();
  return img2_2;
}

cvpi_coordinate_table* cvpi_image_coordinate_table(const VGImage image, VGbitfield channels) {
  VGint width = vgGetParameteri(image, VG_IMAGE_WIDTH);
  VGint height = vgGetParameteri(image, VG_IMAGE_HEIGHT);

  VGint stride = CVPI_PIXEL_BYTES * width;

  cvpi_pixel* data = malloc(sizeof(*data) * width * height);
  if(data == NULL) {
    cvpi_log_3("%s:%d: malloc returned NULL: errno = %d\n", __func__, __LINE__, errno);
    return NULL;
  }
  cvpi_coordinate* coordinates = malloc(sizeof(*coordinates) * height * width);
  if(coordinates == NULL) {
    cvpi_log_3("%s:%d: malloc returned NULL: errno = %d\n", __func__, __LINE__, errno);
    free(data);
    return NULL;
  }
  vgGetImageSubData(image, (void*)data, stride, CVPI_COLOR_SPACE, 0, 0, width, height);
  vgFinish();

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
  unsigned long i = 0;
  unsigned long j = 0;
  for(; i < width*height; ++i) {
    if((data[i].channel[cvpi_pixel_red] && r)
       || (data[i].channel[cvpi_pixel_green] && g)
       || (data[i].channel[cvpi_pixel_blue] && b)
       || (data[i].channel[cvpi_pixel_alpha] && a)) {
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

  if(j < i) {
    cvpi_coordinate* crdnts_realloc = realloc(coordinates, sizeof(*coordinates) * j);

    if(crdnts_realloc != coordinates) {
      cvpi_log_5("%s:%d: realloc does not point to the same memory:\noriginal = %p\nnew = %p\n errno = %d\n", __func__, __LINE__, coordinates, crdnts_realloc, errno);
      free(data);
      free(coordinates);
      return NULL;
    }

  }

  cvpi_coordinate_table *cv = malloc(sizeof(*cv));
  if(cv == NULL) {
    cvpi_log_3("%s:%d: malloc returned NULL: errno = %d\n", __func__, __LINE__, errno);
    free(data);
    free(coordinates);
    return NULL;
  }
  cv->coordinates = coordinates;
  cv->length = j;

  free(data);
  return cv;
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

cvpi_precise_coordinate_table* cvpi_coordinate_table_lens_correct(const cvpi_coordinate_table *ct, 
								  double k1, double k2, double p1, double p2, double k3) {
  cvpi_precise_coordinate* coordinates = malloc(sizeof(*coordinates) * ct->length);
  if(coordinates == NULL) {
    cvpi_log_3("%s:%d: malloc returned NULL: errno = %d\n", __func__, __LINE__, errno);
    return NULL;
  }
  cvpi_precise_coordinate_table* ct_out = malloc(sizeof(*ct_out));
  if(ct_out == NULL) {
    cvpi_log_3("%s:%d: malloc returned NULL: errno = %d\n", __func__, __LINE__, errno);
    free(coordinates);
    return NULL;
  }
  ct_out->length = ct->length;

  unsigned long i = 0;
  double r2 = 0;
  double r4 = 0;
  double r6 = 0;
  double radial = 0;
  double x = 0;
  double y = 0;
  double x2 = 0;
  double y2 = 0;
  double xy = 0;

  for(; i < ct_out->length; ++i) {
    x = ct->coordinates[i].xy[0];
    y = ct->coordinates[i].xy[1];
    x2 = x*x;
    y2 = y*y;
    xy = x*y;
    r2 = x2 + y2;
    r4 = r2*r2;
    r6 = r4*r2;
    radial = 1 + k1*r2 + k2*r4 + k3*r6;
    coordinates[i].x = x*radial + 2*p1*xy + p2*(r2 + 2*x2);
    coordinates[i].y = y*radial + p1*(r2 + 2*y2) + 2*p2*xy;
  }

  ct_out->coordinates = coordinates;

  return ct_out;
}

void cvpi_precise_coordinate_table_destroy(cvpi_precise_coordinate_table *coordinate_table) {
  if(coordinate_table != NULL) {
    if(coordinate_table->coordinates != NULL) {
      free(coordinate_table->coordinates);
      coordinate_table->coordinates = NULL;
    }
    free(coordinate_table);
  }
}

cvpi_polar_coordinate_table* cvpi_image_polar_coordinate_table(const cvpi_coordinate_table *cv) {
  cvpi_polar_coordinate* pc = malloc(sizeof(*pc) * cv->length);
  if(pc == NULL) {
    cvpi_log_3("%s:%d: malloc returned NULL: errno = %d\n", __func__, __LINE__, errno);
    return NULL;
  }
  cvpi_polar_coordinate_table* pct = malloc(sizeof(*pct));
  if(pct == NULL) {
    cvpi_log_3("%s:%d: malloc returned NULL: errno = %d\n", __func__, __LINE__, errno);
    free(pc);
    return NULL;
  }
  unsigned long i = 0;

  pct->length = cv->length;

  for(; i < cv->length; ++i) {
    pc[i].radius = sqrt(cv->coordinates[i].xy[0] * cv->coordinates[i].xy[0]
			+ cv->coordinates[i].xy[1] * cv->coordinates[i].xy[1]);
    pc[i].theta = atan(cv->coordinates[i].xy[1] / cv->coordinates[i].xy[0]);
  }

  pct->coordinates = pc;

  return pct;
}

cvpi_polar_coordinate_table* cvpi_image_polar_coordinate_table_lens_corrected(const cvpi_precise_coordinate_table *cv) {
  cvpi_polar_coordinate* pc = malloc(sizeof(*pc) * cv->length);
  if(pc == NULL) {
    cvpi_log_3("%s:%d: malloc returned NULL: errno = %d\n", __func__, __LINE__, errno);
    return NULL;
  }
  cvpi_polar_coordinate_table* pct = malloc(sizeof(*pct));
  if(pct == NULL) {
    cvpi_log_3("%s:%d: malloc returned NULL: errno = %d\n", __func__, __LINE__, errno);
    free(pc);
    return NULL;
  }
  unsigned long i = 0;

  pct->length = cv->length;

  for(; i < cv->length; ++i) {
    pc[i].radius = sqrt(cv->coordinates[i].x * cv->coordinates[i].x
			+ cv->coordinates[i].y * cv->coordinates[i].y);
    pc[i].theta = atan(cv->coordinates[i].y / cv->coordinates[i].x);
  }

  pct->coordinates = pc;

  return pct;
}

void cvpi_image_polar_coordinate_table_destroy(cvpi_polar_coordinate_table *cv) {
  if(cv != NULL) {
    if(cv->coordinates != NULL) {
      free(cv->coordinates);
      cv->coordinates = NULL;
    }
    free(cv);
  }
}

void cvpi_avuy2argb(cvpi_pixel* yuva, cvpi_pixel* rgba, unsigned long width, unsigned long height) {
  unsigned long i = 0;
  int32_t c,d,e, r_unusat, g_unusat, b_unusat;
  if(yuva == rgba) {
    for(; i < width * height; ++i) {
      c = yuva[i].channel[cvpi_pixel_y] - 16;
      d = yuva[i].channel[cvpi_pixel_u] - 128;
      e = yuva[i].channel[cvpi_pixel_v] - 128;
      r_unusat = 298 * c + 409 * e + 128;
      g_unusat = 298 * c - 100 * d - 208 * e + 128;
      b_unusat = 298 * c + 516 * d + 128;
      
      /* Use usat to do the right arithmetic shift and clipping. GCC
	 does not have the necessry intrinsics. */
      asm("usat %[value], #8, %[value], ASR #8":[value] "+r"(r_unusat));
      asm("usat %[value], #8, %[value], ASR #8":[value] "+r"(g_unusat));
      asm("usat %[value], #8, %[value], ASR #8":[value] "+r"(b_unusat));
      rgba[i].channel[cvpi_pixel_red] = r_unusat;
      rgba[i].channel[cvpi_pixel_green] = g_unusat;
      rgba[i].channel[cvpi_pixel_blue] = b_unusat;
    }
  } else {
    for(; i < width * height; ++i) {
      c = yuva[i].channel[cvpi_pixel_y] - 16;
      d = yuva[i].channel[cvpi_pixel_u] - 128;
      e = yuva[i].channel[cvpi_pixel_v] - 128;
      r_unusat = 298 * c + 409 * e + 128;
      g_unusat = 298 * c - 100 * d - 208 * e + 128;
      b_unusat = 298 * c + 516 * d + 128;
      
      /* Use usat to do the right arithmetic shift and clipping. GCC
	 does not have the necessry intrinsics. */
      asm("usat %[value], #8, %[value], ASR #8":[value] "+r"(r_unusat));
      asm("usat %[value], #8, %[value], ASR #8":[value] "+r"(g_unusat));
      asm("usat %[value], #8, %[value], ASR #8":[value] "+r"(b_unusat));
      rgba[i].channel[cvpi_pixel_red] = r_unusat;
      rgba[i].channel[cvpi_pixel_green] = g_unusat;
      rgba[i].channel[cvpi_pixel_blue] = b_unusat;
      rgba[i].channel[cvpi_pixel_alpha] = yuva[i].channel[cvpi_pixel_alpha];
    }
  }
}

cvpi_pixel* cvpi_image_rgba(const VGImage image) {
  VGint width = vgGetParameteri(image, VG_IMAGE_WIDTH);
  VGint height = vgGetParameteri(image, VG_IMAGE_HEIGHT);

  VGint stride = CVPI_PIXEL_BYTES * width;
  cvpi_pixel* data = malloc(sizeof(*data) * width * height);
  if(data == NULL) {
    cvpi_log_3("%s:%d: malloc returned NULL: errno = %d\n", __func__, __LINE__, errno);
    return NULL;
  }
  vgGetImageSubData(image, (void*)data, stride, CVPI_COLOR_SPACE, 0, 0, width, height);
  vgFinish();
  cvpi_avuy2argb(data, data, width, height);
  return data;
}
