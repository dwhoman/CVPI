/*
 This file is part of CVPI.
 
 Copyright (C) 2015 Devin Homan
 
 This program is free software: you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public License
 as published by the Free Software Foundation, either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>. */

/* TODO: file is old, either need to update or remove */

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
  if(argc < 5) {
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

  int bmp_p = 0;
  int itter = 0;
  while ((itter = getopt(argc, argv, "b")) != -1) {
    switch(itter) {
    case 'b':			/* add bitmap V4 header */
      bmp_p = 1;
      break;
    default:
      break;
    }
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
    //takedown = setupEGL(width, height, 0, 0, 0);

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
    /* FILE * input_check_1 = fopen("/tmp/input_image_1", "w"); */
    /* FILE * input_check_2 = fopen("/tmp/input_image_2", "w"); */
    /* void * input_check_1_m = malloc(f_size); */
    /* void * input_check_2_m = malloc(f_size); */
    /* printf("back to memory 0\n"); */
    /* vgGetImageSubData(input_image_1, input_check_1_m, yuyv_stride, COLOR_SPACE, 0, 0, yuyv_width, height); */
    /* printf("back to memory 1\n"); */
    /* vgGetImageSubData(input_image_2, input_check_2_m, yuyv_stride, COLOR_SPACE, 0, 0, yuyv_width, height); */
    /* printf("back to memory 2\n"); */
    /* vgFinish(); */
    /* printf("back to memory 3\n"); */
    /* fwrite(input_check_1_m, 1, f_size, input_check_1); */
    /* fflush(input_check_1); */
    /* fwrite(input_check_2_m, 1, f_size, input_check_2); */
    /* fflush(input_check_2); */
    /* fclose(input_check_1); */
    /* fclose(input_check_2); */
    /* printf("created intermediate output\n"); */

    /* set blue in ARGB to 1 */
    vgLookup(mod_image_1, input_image_1, identity_array, ones_array, identity_array, identity_array, OUTPUT_LINEAR, VG_FALSE);
    /* set red in ARGB to 1 */
    vgLookup(mod_image_2, input_image_2, identity_array, identity_array, identity_array, ones_array, OUTPUT_LINEAR, VG_FALSE);
    vgFinish();

    /* intermediate output */
    /* input_check_1 = fopen("/tmp/mod_image_1", "w"); */
    /* input_check_2 = fopen("/tmp/mod_image_2", "w"); */
    /* printf("back to memory 0\n"); */
    /* vgGetImageSubData(mod_image_1, input_check_1_m, yuyv_stride, COLOR_SPACE, 0, 0, yuyv_width, height); */
    /* printf("back to memory 1\n"); */
    /* vgGetImageSubData(mod_image_2, input_check_2_m, yuyv_stride, COLOR_SPACE, 0, 0, yuyv_width, height); */
    /* printf("back to memory 2\n"); */
    /* vgFinish(); */
    /* printf("back to memory 3\n"); */
    /* fwrite(input_check_1_m, 1, f_size, input_check_1); */
    /* fflush(input_check_1); */
    /* fwrite(input_check_2_m, 1, f_size, input_check_2); */
    /* fflush(input_check_2); */
    /* fclose(input_check_1); */
    /* fclose(input_check_2); */
    /* printf("created intermediate output\n"); */

    /* reuse input images */

    vgColorMatrix(input_image_1, mod_image_1, conversion_1);
    vgColorMatrix(input_image_2, mod_image_2, conversion_2);
    vgFinish();

    /* intermediate output */
    /* input_check_1 = fopen("/tmp/input_image_1a", "w"); */
    /* input_check_2 = fopen("/tmp/input_image_2a", "w"); */
    /* printf("back to memory 0\n"); */
    /* vgGetImageSubData(input_image_1, input_check_1_m, yuyv_stride, COLOR_SPACE, 0, 0, yuyv_width, height); */
    /* printf("back to memory 1\n"); */
    /* vgGetImageSubData(input_image_2, input_check_2_m, yuyv_stride, COLOR_SPACE, 0, 0, yuyv_width, height); */
    /* printf("back to memory 2\n"); */
    /* vgFinish(); */
    /* printf("back to memory 3\n"); */
    /* fwrite(input_check_1_m, 1, f_size, input_check_1); */
    /* fflush(input_check_1); */
    /* fwrite(input_check_2_m, 1, f_size, input_check_2); */
    /* fflush(input_check_2); */
    /* fclose(input_check_1); */
    /* fclose(input_check_2); */
    /* printf("created intermediate output\n"); */

    /* mod images no longer needed */
    vgDestroyImage(mod_image_1);
    vgDestroyImage(mod_image_2);

    VGImage output_image = vgCreateImage(COLOR_SPACE, rgb_width, height, VG_IMAGE_QUALITY_NONANTIALIASED);

    /* merge input images */
    unsigned int x = 0;
    for(; x < yuyv_width; ++x) {
      vgCopyImage(output_image, x*2, 0, input_image_1, x, 0, 1, height, VG_FALSE);
      vgCopyImage(output_image, x*2+1, 0, input_image_2, x+1, 0, 1, height, VG_FALSE);
      vgFinish();
    }
    vgDestroyImage(input_image_1);
    vgDestroyImage(input_image_2);
    /* zero the alpha channel */
    vgGetImageSubData(output_image, image_out, rgb_stride, COLOR_SPACE, 0, 0, rgb_width, height);
    vgFinish();

    vgDestroyImage(output_image);

    BITMAPHEADER* header = createBMPHeader(rgb_width, height, 90, &bmp_VG_sARGB_8888);
    writeBMPHeader(header, output);

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
