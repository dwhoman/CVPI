#ifndef transConfig_h
#include "transConfig.h"
#endif

#ifndef EGL_CONFIG
#include "eglConfig.h"
#endif

#ifndef VG_CONFIG
#include "vgConfig.h"
#endif

#ifndef _OPENVG_H
#include <VG/openvg.h>
#endif

#ifndef BITMAP_H
#include "bitmap.h"
#endif

#ifndef _STDIO_H
#include <stdio.h>
#endif

#ifndef _STDLIB_H
#include <stdlib.h>
#endif

#define COLOR_SPACE VG_sARGB_8888
#define OUTPUT_LINEAR VG_FALSE

const VGubyte ones_array[256] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

const VGubyte zeros_array[256] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

const VGubyte two_55_array[256] = {255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255};

const VGubyte identity_array[256] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255};

const VGfloat conversion_1[20] = {
  0,0,1,0,
  1,0,0,0,
  0,1,0,0,
  0,0,0,1,

  0,0,0,0
};

const VGfloat conversion_2[20] = {
  0, 1, 0, 0, 
  1, 0, 0, 0, 
  0, 0, 1, 0, 
  0, 0, 0, 1, 
  
  0,0,0,0
};

static const pixel_format bmp_VG_sARGB_8888 = {4,
					      0x00FF0000, /* 2#00000000111111110000000000000000 */
					      0x0000FF00, /* 2#00000000000000001111111100000000 */
					      0x000000FF, /* 2#00000000000000000000000011111111 */
					      0xFF000000,
					      0,
					      0,
					      0,
					      0,
					      4,
					       LCS_sRGB};

const int height = 960;
const int rgb_width = 1280;
const int yuyv_width = 640;
const int rgb_stride = 5120;
const int yuyv_stride = 2560;

int main(int argc, char** argv) {
  if(argc < 3) {
    printf("Too few arguments.\n");
    return 1;
  }
  FILE * input = fopen(argv[1],"r");

  FILE * output = fopen(argv[2], "w");

  fseek(input, 0L, SEEK_END);
  int f_size = ftell(input);		/* file size */
  fseek(input, 0L, SEEK_SET);

  void* image_in = malloc(f_size);
  void* image_out = malloc(2*f_size);

  fread(image_in, 1, f_size, input);

  change_renderable_api(egl_renderable_api_openvg);
  change_buffer_size(32);
  change_red_size(8);
  change_green_size(8);
  change_blue_size(8);
  change_alpha_size(8);
  egl_deletable* takedown;
  takedown = setupEGL(rgb_width, height, rgb_stride, EGL_PIXEL_FORMAT_ARGB_8888_BRCM, 0);  

  if(takedown == NULL) {
    printf("setupEGL returned NULL pointer \n");
    goto skip_vg;
  }

  //printf("Creating VGImage\n");

  VGImage input_image_1 = vgCreateImage(COLOR_SPACE, yuyv_width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  VGImage input_image_2 = vgCreateImage(COLOR_SPACE, yuyv_width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  VGImage mod_image_1 = vgCreateImage(COLOR_SPACE, yuyv_width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  VGImage mod_image_2 = vgCreateImage(COLOR_SPACE, yuyv_width, height, VG_IMAGE_QUALITY_NONANTIALIASED);

  //printf("created initial VGImages\n");

  vgImageSubData(input_image_1, image_in, yuyv_stride, COLOR_SPACE, 0, 0, yuyv_width, height);
  vgImageSubData(input_image_2, image_in, yuyv_stride, COLOR_SPACE, 0, 0, yuyv_width, height);
  vgFinish();
  //printf("Got image data\n");

  /* intermediate output */
  FILE * input_check_1 = fopen("/home/pi/egl_openvg/yuv_test/input_image_1", "w");
  FILE * input_check_2 = fopen("/home/pi/egl_openvg/yuv_test/input_image_2", "w");
  void * input_check_1_m = malloc(f_size);
  void * input_check_2_m = malloc(f_size);
  printf("back to memory 0\n");
  vgGetImageSubData(input_image_1, input_check_1_m, yuyv_stride, COLOR_SPACE, 0, 0, yuyv_width, height);
  printf("back to memory 1\n");
  vgGetImageSubData(input_image_2, input_check_2_m, yuyv_stride, COLOR_SPACE, 0, 0, yuyv_width, height);
  printf("back to memory 2\n");
  vgFinish();
  printf("back to memory 3\n");
  fwrite(input_check_1_m, 1, f_size, input_check_1);
  fflush(input_check_1);
  fwrite(input_check_2_m, 1, f_size, input_check_2);
  fflush(input_check_2);
  fclose(input_check_1);
  fclose(input_check_2);
  printf("created intermediate output\n");

  /* set blue in ARGB to 1 */
  vgLookup(mod_image_2, input_image_2, identity_array, identity_array, two_55_array, identity_array, OUTPUT_LINEAR, VG_FALSE);
  /* set red in ARGB to 1 */
  vgLookup(mod_image_1, input_image_1, two_55_array, identity_array, identity_array, identity_array, OUTPUT_LINEAR, VG_FALSE);
  vgFinish();

  /* intermediate output */
  input_check_1 = fopen("/home/pi/egl_openvg/yuv_test/mod_image_1", "w");
  input_check_2 = fopen("/home/pi/egl_openvg/yuv_test/mod_image_2", "w");
  printf("back to memory 0\n");
  vgGetImageSubData(mod_image_1, input_check_1_m, yuyv_stride, COLOR_SPACE, 0, 0, yuyv_width, height);
  printf("back to memory 1\n");
  vgGetImageSubData(mod_image_2, input_check_2_m, yuyv_stride, COLOR_SPACE, 0, 0, yuyv_width, height);
  printf("back to memory 2\n");
  vgFinish();
  printf("back to memory 3\n");
  fwrite(input_check_1_m, 1, f_size, input_check_1);
  fflush(input_check_1);
  fwrite(input_check_2_m, 1, f_size, input_check_2);
  fflush(input_check_2);
  fclose(input_check_1);
  fclose(input_check_2);
  printf("created intermediate output\n");

  /* reuse input images */

  vgColorMatrix(input_image_1, mod_image_1, conversion_1);
  vgColorMatrix(input_image_2, mod_image_2, conversion_2);
  vgFinish();

  /* intermediate output */
  input_check_1 = fopen("/home/pi/egl_openvg/yuv_test/input_image_1a", "w");
  input_check_2 = fopen("/home/pi/egl_openvg/yuv_test/input_image_2a", "w");
  printf("back to memory 0\n");
  vgGetImageSubData(input_image_1, input_check_1_m, yuyv_stride, COLOR_SPACE, 0, 0, yuyv_width, height);
  printf("back to memory 1\n");
  vgGetImageSubData(input_image_2, input_check_2_m, yuyv_stride, COLOR_SPACE, 0, 0, yuyv_width, height);
  printf("back to memory 2\n");
  vgFinish();
  printf("back to memory 3\n");
  fwrite(input_check_1_m, 1, f_size, input_check_1);
  fflush(input_check_1);
  fwrite(input_check_2_m, 1, f_size, input_check_2);
  fflush(input_check_2);
  fclose(input_check_1);
  fclose(input_check_2);
  printf("created intermediate output\n");

  /* mod images no longer needed */
  vgDestroyImage(mod_image_1);
  vgDestroyImage(mod_image_2);

  VGImage output_image = vgCreateImage(COLOR_SPACE, rgb_width, height, VG_IMAGE_QUALITY_NONANTIALIASED);

  /* merge input images */
  unsigned int x = 0;
  for(; x < yuyv_width; ++x) {
    vgCopyImage(output_image, x*2, 0, input_image_1, x, 0, 1, height, VG_FALSE);
    vgCopyImage(output_image, x*2+1, 0, input_image_2, x, 0, 1, height, VG_FALSE);
    vgFinish();
  }
  vgDestroyImage(input_image_1);
  vgDestroyImage(input_image_2);
  /* zero the alpha channel */
  vgGetImageSubData(output_image, image_out, rgb_stride, COLOR_SPACE, 0, 0, rgb_width, height);
  vgFinish();

  vgDestroyImage(output_image);
  /*
  BITMAPHEADER* header = createBMPHeader(rgb_width, height, 90, &bmp_VG_sARGB_8888);
  writeBMPHeader(header, output);
  */
  fwrite(image_out, 1, 2*f_size, output);
  fflush(output);

  //printf("Created BMP image\n");

  takedownEGL(takedown);

 skip_vg:

  //printf("%x %x\n", image_in, image_out);

  fclose(output);

  fclose(input);

  free(image_in);
  free(image_out);

  return 0;
}
