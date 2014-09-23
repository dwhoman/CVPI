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

#ifndef _UNISTD_H
#include <unistd.h>
#endif

#include "image_functions.h"

#define TEST_COLOR_SPACE VG_sARGB_8888

static const pixel_format bmp_VG_sARGB_8888 = {4,
					      0xFF000000,
					      0x00FF0000, /* 2#00000000111111110000000000000000 */
					      0x0000FF00, /* 2#00000000000000001111111100000000 */
					      0x000000FF, /* 2#00000000000000000000000011111111 */
					      0,
					      0,
					      0,
					      0,
					      4,
					       LCS_sRGB};

int main(int argc, char** argv) {
  if(argc < 7) {
    printf("Too few arguments.\n");
    return 1;
  }
  FILE * input = fopen(argv[argc-2],"r");

  FILE * output = fopen(argv[argc-1], "w");

  int height = atoi(argv[argc-3]);
  if(height <= 0) {
    printf("Invalid height\n");
    return 2;
  }  
  int yuva_width = atoi(argv[argc-4]); 
  if(yuva_width <= 0) {
    printf("Invalid width\n");
    return 3;
  }

  char* out_type = argv[argc-5];
  char* in_type = argv[argc-6];
  /* yuyv -> yuva */
  /* yuyv -> rgba */
  /* yuva -> rgba */
  /* rgba -> bmp add header */
  /* bmp -> rgba remove header */
  /* yuva -> bmp add header */
  /* bmp -> yuva remove header */

  int bmp_p = 0;
  int itter = 0;
  while ((itter = getopt(argc, argv, "bscgCB:")) != -1) {
    switch(itter) {
    case 'b':			/* add bitmap V4 header */
      bmp_p = 1;
      break;
    case 's':			/* Sobel */
      break;
    case 'c':			/* Canny */
      break;
    case 'g':			/* Gaussian */
      break;
    case 'C':			/* Corner */
      break;
    case 'B':			/* box K argument */
      break;
    default:
      break;
    }
  }

  

  int yuyv_width = yuva_width/2;
  int yuva_stride = yuva_width*4;
  int yuyv_stride = yuva_width*2;

  fseek(input, 0L, SEEK_END);
  int f_size = ftell(input);		/* file size */
  fseek(input, 0L, SEEK_SET);

  void* image_in = malloc(f_size);
  void* image_out = malloc(2*f_size);

  fread(image_in, 1, f_size, input);
  unsigned long * pointer = image_in;
  printf("0x%X 0x%X\n", *pointer, *(pointer+1));

  change_renderable_api(egl_renderable_api_openvg);
  change_buffer_size(32);
  change_red_size(8);
  change_green_size(8);
  change_blue_size(8);
  change_alpha_size(8);
  egl_deletable* takedown;
  takedown = setupEGL(yuva_width, height, yuva_stride, EGL_PIXEL_FORMAT_ARGB_8888_BRCM, 0);  

  if(takedown == NULL) {
    printf("setupEGL returned NULL pointer \n");
    goto skip_vg;
  }

  VGImage input_image = vgCreateImage(TEST_COLOR_SPACE, yuyv_width, height, VG_IMAGE_QUALITY_NONANTIALIASED);

  vgImageSubData(input_image, image_in, yuyv_stride, TEST_COLOR_SPACE, 0, 0, yuyv_width, height);
  vgFinish();
  VGImage output_image = yuyv2yuva(input_image);
  vgFinish();
  vgDestroyImage(input_image);

  vgGetImageSubData(output_image, image_out, yuva_stride, TEST_COLOR_SPACE, 0, 0, yuva_width, height);
  vgFinish();

  vgDestroyImage(output_image);

  if(bmp_p) {
    BITMAPHEADER* header = createBMPHeader(yuva_width, height, 90, &bmp_VG_sARGB_8888);
    writeBMPHeader(header, output);
    free(header);
  }
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
