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
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _STDIO_H
#include <stdio.h>
#endif

#ifndef	_STDLIB_H
#include <stdlib.h>
#endif

#ifndef _STRING_H
#include <string.h>
#endif

#ifndef CVPI
#include "cvpi.h"
#endif

#ifndef CVPI_EGL_CONFIG_TESTS
#include "cvpi_egl_config_tests.h"
#endif

#ifndef CVPI_EGL_SURFACE_FUNCTIONS
#include "cvpi_egl_surface_functions.h"
#endif

#ifndef CVPI_IMAGE_TESTS
#include "cvpi_image_tests.h"
#endif

#define seek_error_check(seeker)\
  do {\
  int val = seeker;\
  if(val == -1) {\
  cvpi_log_3("fseek error, errno = ", __func__, __LINE__, errno);\
  BADSTATE = 1;\
  goto TAKEDOWN;\
  }} while(0)

#define pixelFormatTest(format) do{if(string_equal((#format), s)) { return format; }} while(0)

static cvpi_egl_instance cvpi_egl_wrapper_create() {
  printf("At cvpi_egl_wrapper_create\n");
  cvpi_egl_settings settings = cvpi_egl_settings_create();
  printf("created egl settings struct \n");
  if(settings == NULL) {
    fprintf(stderr, "cvpi_egl_settings_create failed\n");
    return NULL;
  }

  settings->surface_pixmap_create_function = cvpi_egl_surface_pixmap_native_creator;
  settings->surface_pixmap_destroy_function = cvpi_egl_surface_pixmap_native_destroyer;
  settings->renderable_api = cvpi_egl_renderable_api_openvg;
  settings->current_surface_type = cvpi_egl_surface_type_pixmap;

  CVPI_BOOL good = CVPI_TRUE;
  good &=
    CVPI_TRUE_TEST(cvpi_egl_settings_conformant(settings, cvpi_egl_conformant_bit_openvg, cvpi_egl_settings_add));
  good &=
    CVPI_TRUE_TEST(cvpi_egl_settings_renderable_type(settings, cvpi_egl_renderable_bits_openvg, cvpi_egl_settings_add));
#if HAVE_BCM_HOST == 1
  good &=
    CVPI_TRUE_TEST(cvpi_egl_settings_pixel_format_brcm(settings, cvpi_egl_pixel_format_vg_image_brcm, cvpi_egl_settings_add));
#endif
#if HAVE_BCM_HOST == 1
  good &=
    CVPI_TRUE_TEST(cvpi_egl_settings_pixel_format_brcm(settings,cvpi_egl_pixel_format_render_vg_brcm, cvpi_egl_settings_add));
#endif
#if HAVE_BCM_HOST == 1
  good &=
    CVPI_TRUE_TEST(cvpi_egl_settings_pixel_format_brcm(settings,cvpi_egl_pixel_format_usage_mask_brcm, cvpi_egl_settings_add));
#endif
#if HAVE_BCM_HOST == 1
  good &=
    CVPI_TRUE_TEST(cvpi_egl_settings_pixel_format_brcm(settings,cvpi_egl_pixel_format_texture_mask_brcm, cvpi_egl_settings_add));
#endif
#if HAVE_BCM_HOST == 1
  good &=
    CVPI_TRUE_TEST(cvpi_egl_settings_pixel_format_brcm(settings,cvpi_egl_pixel_format_render_mask_brcm, cvpi_egl_settings_add));
#endif

  good &=
    CVPI_TRUE_TEST(cvpi_egl_settings_surface_type(settings,cvpi_egl_surface_bits_colorspace, cvpi_egl_settings_add));
  good &=
    CVPI_TRUE_TEST(cvpi_egl_settings_surface_type(settings,cvpi_egl_surface_bits_alpha, cvpi_egl_settings_add));
  good &=
    CVPI_TRUE_TEST(cvpi_egl_settings_surface_type(settings,cvpi_egl_surface_bits_multisample, cvpi_egl_settings_add));
  good &=
    CVPI_TRUE_TEST(cvpi_egl_settings_surface_type(settings,cvpi_egl_surface_bits_swap, cvpi_egl_settings_add));


  good &=
    CVPI_TRUE_TEST(cvpi_egl_settings_buffer_size(settings, 32));

  good &=
    CVPI_TRUE_TEST(cvpi_egl_settings_surface_type(settings, cvpi_egl_surface_bits_pixmap, cvpi_egl_settings_add));
  good &=
    CVPI_TRUE_TEST(cvpi_egl_settings_surface_type(settings, cvpi_egl_surface_bits_pbuffer, cvpi_egl_settings_remove));
  good &=
    CVPI_TRUE_TEST(cvpi_egl_settings_surface_type(settings, cvpi_egl_surface_bits_window, cvpi_egl_settings_remove));

  printf("modified settings \n");
  if(!good) {
    fprintf(stderr, "%s: invalid EGL settings\n", __func__);
    if(settings != NULL) {
      free(settings);
      settings = NULL;
    }
    return NULL;
  }

  printf("setting up EGL \n");
  return cvpi_egl_instance_setup(settings);
}

static void cvpi_egl_wrapper_destroy(cvpi_egl_instance instance) {
  if(instance->egl_settings != NULL) {
    free(instance->egl_settings);
  }
  cvpi_egl_instance_takedown(instance);
}

static VGImageFormat stringToPixelFormat(char* s) {
  pixelFormatTest(VG_sRGBX_8888);
  pixelFormatTest(VG_sRGBA_8888);
  pixelFormatTest(VG_sRGBA_8888_PRE);
  pixelFormatTest(VG_lRGBA_8888_PRE);
  pixelFormatTest(VG_lRGBX_8888);
  pixelFormatTest(VG_lRGBA_8888);
  pixelFormatTest(VG_sXRGB_8888);
  pixelFormatTest(VG_sARGB_8888);
  pixelFormatTest(VG_sARGB_8888_PRE);
  pixelFormatTest(VG_lXRGB_8888);
  pixelFormatTest(VG_lARGB_8888);
  pixelFormatTest(VG_lARGB_8888_PRE);
  pixelFormatTest(VG_sBGRX_8888);
  pixelFormatTest(VG_sBGRA_8888);
  pixelFormatTest(VG_sBGRA_8888_PRE);
  pixelFormatTest(VG_lBGRX_8888);
  pixelFormatTest(VG_lBGRA_8888);
  pixelFormatTest(VG_lBGRA_8888_PRE);
  pixelFormatTest(VG_sXBGR_8888);
  pixelFormatTest(VG_sABGR_8888);
  pixelFormatTest(VG_sABGR_8888_PRE);
  pixelFormatTest(VG_lXBGR_8888);
  pixelFormatTest(VG_lABGR_8888);
  pixelFormatTest(VG_lABGR_8888_PRE);
  pixelFormatTest(VG_sBGR_565);
  pixelFormatTest(VG_sRGB_565);
  pixelFormatTest(VG_sABGR_1555);
  pixelFormatTest(VG_sBGRA_5551);
  pixelFormatTest(VG_sARGB_1555);
  pixelFormatTest(VG_sRGBA_5551);
  pixelFormatTest(VG_sARGB_4444);
  pixelFormatTest(VG_sRGBA_4444);
  pixelFormatTest(VG_sBGRA_4444);
  pixelFormatTest(VG_sABGR_4444);
  pixelFormatTest(VG_sL_8);
  pixelFormatTest(VG_lL_8);
  pixelFormatTest(VG_A_8);
  pixelFormatTest(VG_A_4);
  pixelFormatTest(VG_A_1);
  pixelFormatTest(VG_BW_1);
  return 0;
}

#undef pixelFormatTest

/* Open files for reading or writing as specified in the file file
   <file_path>. Allocate CPU memory for the test function. Read input
   files before running the test function, and write output files
   after the test function has run.  Return 0 if the test function was
   successful, else return 1. The parameter function takes two arrays
   do data, and will stop looking for data when a NULL pointer is
   reached.  */
int test_runner(CVPI_BOOL(*test_function)(TestImage** data), char* file_path, int file_length) {
#define TAKEDOWN test_runner_takedown
  int return_value = 1;
  CVPI_BOOL BADSTATE = CVPI_FALSE;

  if(file_path != NULL) {
    FILE* settings_file = NULL;
    
    TestImage* data[file_length + 1];
    for(int i = 0; i <= file_length; ++i) {
      data[i] = NULL;
    }
    settings_file = fopen(file_path, "r");
    if(settings_file == NULL) {
      fprintf(stderr, "settings_file %s failed to open, errno = %d\n", file_path, errno);
      BADSTATE = CVPI_TRUE;
      goto test_runner_takedown;
    }

    /* getline variables */
    char* line = NULL;
    size_t len = 0;

    /* file line data */
    char* image_file_path = NULL;
    char* format_s = NULL;
    char* width_s = NULL;
    char* height_s = NULL;
    char* rw = NULL;
    char* line_dup = NULL;
    
    /* populate data array with file data */
    for(int i = 0; -1 != getline(&line, &len, settings_file); ++i) {
      line_dup = strdup(line);
      image_file_path = strtok(line_dup, ',');
      format_s = strtok(NULL, ',');
      width_s = strtok(NULL, ',');
      height_s = strtok(NULL, ',');
      rw = strtok(NULL, ',');
      
      if(image_file_path == NULL || format_s == NULL || width_s == NULL || height_s == NULL || rw == NULL) {
	fprintf(stderr, "Incomplete settings file\n");
	BADSTATE = CVPI_TRUE;
	goto test_runner_takedown;
      }
      
      long width = strtol(width_s, NULL, 10);
      long height = strtol(height_s, NULL, 10);
      
      if(width <= 0) {
	fprintf(stderr, "Bad width: %d", width);
	BADSTATE = CVPI_TRUE;
	goto test_runner_takedown;
      }

      if(height <= 0) {
	fprintf(stderr, "Bad height: %d", height);
	BADSTATE = CVPI_TRUE;
	goto test_runner_takedown;
      }

      long stride;
      if(string_equal(format_s, "YUYV")) {
	stride = 2*width;
      } else if(string_equal(format_s, "CVPI_COLOR_SPACE")) {
	stride = 4*width;
      } else {
	VGImageFormat format = stringToPixelFormat(format_s);
	if(format) {
	  stride = vgPixelBits(format) * width / 8;
	} else {
	  fprintf(stderr, "Invalid image format %s\n", format_s);
	  BADSTATE = CVPI_TRUE;
	  goto test_runner_takedown;
	}
      }
      
      long file_size = height * stride;
      data[i] = malloc(sizeof(TestImage) + file_size);
      if(data[i] == NULL) {
	fprintf(stderr, "Failed to allocate memory for TestImage: errno = %d\n", errno);
	BADSTATE = CVPI_TRUE;
	goto test_runner_takedown;
      }

      data[i]->width = width;
      data[i]->height = height;
      data[i]->stride = stride;

      switch(rw[0]) {
      case 'R':
      case 'r':
	data[i]->writable = VG_FALSE;
	data[i]->file = fopen(image_file_path, "r");
	if(data[i]->file == NULL) {
	  fprintf(stderr, "Failed to open file %s for reading\n", image_file_path);
	  BADSTATE = CVPI_TRUE;
	  goto test_runner_takedown;
	}

	/* read the file */
	/* get the file size */
	seek_error_check(fseek(data[i]->file, 0L, SEEK_END));
	int input_size = ftell(data[i]->file);		/* input file size */
	seek_error_check(input_size);
	seek_error_check(fseek(data[i]->file, 0L, SEEK_SET));

	if(input_size != file_size) {
	  fprintf(stderr, "Inconsistent file size: actual %d: expected: %d\n", input_size, file_size);
	  BADSTATE = CVPI_TRUE;
	  goto test_runner_takedown;
	}

	if(input_size != fread(data[i]->data, 1, input_size, data[i]->file)) {
	  fprintf(stderr, "Failed to correctly read input file\n", input_size, file_size);
	  BADSTATE = CVPI_TRUE;
	  goto test_runner_takedown;	  
	}

	break;
      case 'W':
      case 'w':
	data[i]->writable = VG_TRUE;
	data[i]->file = fopen(image_file_path, "w");
	if(data[i]->file == NULL) {
	  fprintf(stderr, "Failed to open file %s for writing\n", image_file_path);
	  BADSTATE = CVPI_TRUE;
	  goto test_runner_takedown;
	}
	break;
      default:
	fprintf(stderr, "Invalid read/write specifier\n");
	BADSTATE = CVPI_TRUE;
	goto test_runner_takedown;
      }

      free(line_dup);
      line_dup = NULL;
    }

    return_value = !CVPI_TRUE_TEST(test_function(data));

  test_runner_takedown:
    if(file_path != NULL) {
      for(int k = 0; k < file_length + 1; ++k) {
	if(data[k] != NULL) {
	  /* write data to files */
	  if(CVPI_TRUE_TEST(data[k]->writable) && CVPI_FALSE_TEST(BADSTATE)) {
	    fwrite(data[k]->data, 1, data[k]->height * data[k]->stride, data[k]->file);
	    fflush(data[k]->file);
	  }
	  /* close files */
	  if(data[k]->file != NULL && fclose(data[k]->file)) {
	    fprintf(stderr, "failed to close file, errno = %d\n", errno);
	  }
	
	  /* free memory */
	  free(data[k]);
	  data[k] = NULL;
	}
      }
      if(line_dup != NULL) {
	free(line_dup);
      }
    }
  } else {
    return_value = !CVPI_TRUE_TEST(test_function(NULL));
  }
  return return_value;
#undef TAKEDOWN
}

CVPI_BOOL string_equal(char* str1, char* str2) {
  int i = -1;
  do {
    ++i;
    if(str1[i] != str2[i]) {
      return CVPI_FALSE;
    }
  } while(str1[i] != '\0');

  return CVPI_TRUE;
}

/* input: <function_to_call> [<data_file>, <file_line_count>]

   data_file contents
   <image_file_path>, <format>, <width>, <height>, R|W
*/


/* output: using shell convention of 0 for success and non-zero for failure */

/* run test function, f, if selected by the first program argument */
#define TEST(f) do {if(CVPI_TRUE_TEST(string_equal(argv[0],(#f)))){RETURN_VALUE = test_runner(f, argc == 3 ? argv[1] : NULL, argc == 3 ? strtol(argv[2],NULL,10) : 0); goto TAKEDOWN;}} while(0)

int main(int argc, char** argv) {
#define TAKEDOWN main_takedown
  printf("At main\n");
  int RETURN_VALUE = 1;
  cvpi_egl_instance instance = NULL;

  ++argv, --argc;  /* skip over program name */

  if(!argc) {
    fprintf(stderr, "Too few arguments.");
    return 1;
  }

  /* CVPI_LOG_FILE = stderr; */

 /*  /\* egl setup tests *\/ */
 /*  TEST(test_cvpi_egl_settings_create); */
 /*  TEST(test_cvpi_egl_instance_setup_takedown); */
 /*  TEST(test_cvpi_egl_instance_openvg_pixmap); */

 /*  /\* tests not requiring egl *\/ */
 /*  TEST(test_cvpi_avuy2argb); */
 /*  TEST(test_cvpi_bmp_header_alloc_write); */
 /*  /\* tests requiring egl *\/ */
  printf("Setting-up EGL\n");
  instance = cvpi_egl_wrapper_create();
  if(instance == NULL) {
    fprintf(stderr, "cvpi_tests.c: cvpi_egl_instance creation failed\n", __func__);
    return 1;
  }
  printf("Setup EGL\n");
  TEST(test_endianness_channel_red);
  TEST(test_endianness_channel_green);
  TEST(test_endianness_channel_blue);
  TEST(test_endianness_channel_alpha);
  
 /*  TEST(test_cvpi_pixel); */

 /*  TEST(test_vgConvolveNoShift); */

 /*  TEST(test_cvpi_yuyv2yuva); */
 /*  TEST(test_cvpi_image_add_odd); */
 /*  TEST(test_cvpi_image_add_single); */
 /*  TEST(test_cvpi_image_add_even); */
 /*  TEST(test_cvpi_subtract_images_odd); */
 /*  TEST(test_cvpi_image_add_odd_scale_add); */
 /*  TEST(test_cvpi_subtract_images_odd_scale_add); */

 /*  TEST(test_cvpi_channel_add_RB); */
 /*  TEST(test_cvpi_channel_add_RR); */
 /*  TEST(test_cvpi_color_channels_add); */
 /*  TEST(test_cvpi_all_channels_add); */
 /*  TEST(test_cvpi_image_combine_channelwise); */

 /*  TEST(test_cvpi_channel_threshold); */
 /*  TEST(test_cvpi_channel_threshold_inv); */
 /*  TEST(test_cvpi_channel_threshold_same); */
 /*  TEST(test_cvpi_channel_threshold_inv_same); */

 /*  TEST(test_cvpi_image_threshold); */
 /*  TEST(test_cvpi_image_threshold_inv); */
 /*  TEST(test_cvpi_image_threshold_dependent); */
 /*  TEST(test_cvpi_image_threshold_inv_dependent); */

 /*  TEST(test_cvpi_image_threshold_sector); */
 /*  TEST(test_cvpi_image_threshold_sector_inv); */
 /*  TEST(test_cvpi_image_threshold_sector_dependent); */
 /*  TEST(test_cvpi_image_threshold_sector_inv_dependent); */

 /*  TEST(test_cvpi_image_mask_channel_add); */
 /*  TEST(test_cvpi_image_mask_channel_subtract); */

 /*  TEST(test_cvpi_image_logical_and); */
 /*  TEST(test_cvpi_image_logical_and_NOT); */
 /*  TEST(test_cvpi_image_logical_nand); */
 /*  TEST(test_cvpi_image_logical_nand_NOT); */
 /*  TEST(test_cvpi_image_logical_or); */
 /*  TEST(test_cvpi_image_logical_or_NOT); */
 /*  TEST(test_cvpi_image_logical_nor); */
 /*  TEST(test_cvpi_image_logical_nor_NOT); */
 /*  TEST(test_cvpi_image_logical_xor); */
 /*  TEST(test_cvpi_image_logical_xor_NOT); */
 /*  TEST(test_cvpi_image_logical_xnor); */
 /*  TEST(test_cvpi_image_logical_xnor_NOT); */
 /*  TEST(test_cvpi_image_logical_complement); */
 /*  TEST(test_cvpi_image_logical_complement_NOT); */
 /*  TEST(test_cvpi_image_logical_complement_NOT); */
 /*  TEST(test_cvpi_image_logical_complement_inv_NOT); */

 /*  TEST(test_cvpi_image_dilate); */
 /*  TEST(test_cvpi_image_dilate_NOT); */
 /*  TEST(test_cvpi_image_erode); */
 /*  TEST(test_cvpi_image_erode_NOT); */
 /*  TEST(test_cvpi_image_hit_miss); */
 /*  TEST(test_cvpi_image_hit_miss_NOT); */
 /*  TEST(test_cvpi_image_thin); */
 /*  TEST(test_cvpi_image_thin_NOT); */
 /*  TEST(test_cvpi_image_thicken); */
 /*  TEST(test_cvpi_image_thicken_NOT); */

 /*  TEST(test_cvpi_channel_max_alpha); */
 /*  TEST(test_cvpi_channel_max_red); */
 /*  TEST(test_cvpi_channel_max_green); */
 /*  TEST(test_cvpi_channel_max_blue); */
 /*  TEST(test_cvpi_channel_min_alpha); */
 /*  TEST(test_cvpi_channel_min_red); */
 /*  TEST(test_cvpi_channel_min_green); */
 /*  TEST(test_cvpi_channel_min_blue); */
 /*  TEST(test_cvpi_channel_max_min_alpha); */
 /*  TEST(test_cvpi_channel_max_min_red); */
 /*  TEST(test_cvpi_channel_max_min_green); */
 /*  TEST(test_cvpi_channel_max_min_blue); */

 /*  TEST(test_cvpi_channel_histogram_alpha); */
 /*  TEST(test_cvpi_channel_histogram_red); */
 /*  TEST(test_cvpi_channel_histogram_green); */
 /*  TEST(test_cvpi_channel_histogram_blue); */

 /*  TEST(test_cvpi_color_channels_histogram); */
 /*  TEST(test_cvpi_image_histogram); */

 /*  TEST(test_cvpi_channel_cumulative_distribution_alpha); */
 /*  TEST(test_cvpi_channel_cumulative_distribution_red); */
 /*  TEST(test_cvpi_channel_cumulative_distribution_green); */
 /*  TEST(test_cvpi_channel_cumulative_distribution_blue); */

 /*  TEST(test_cvpi_color_channels_cumulative_distribution); */
 /*  TEST(test_cvpi_image_cumulative_distribution); */

 /*  TEST(test_cvpi_image_coordinate_table_zero); */
 /*  TEST(test_cvpi_image_coordinate_table_some); */
 /*  TEST(test_cvpi_image_coordinate_table_all); */
 /*  TEST(test_cvpi_sobel); */
 /*  TEST(test_cvpi_scharr); */
 /*  TEST(test_cvpi_prewitt); */
 /*  TEST(test_cvpi_roberts_cross); */

 /*  TEST(test_cvpi_invert_colors); */
 /*  TEST(test_cvpi_pixel_average); */
 /*  TEST(test_cvpi_pixel_color_average); */
 /*  TEST(test_cvpi_avuy2ayuv); */

 /*  TEST(test_cvpi_image2argb); */

 /*  TEST(test_cvpi_image_rgba_to_binary); */
 /*  TEST(test_overflow_behavior); */
 /*  TEST(test_negative_behavior); */

  printf("test finished\n");
 TAKEDOWN:
  if(instance != NULL) {
    cvpi_egl_wrapper_destroy(instance);
  }

  return RETURN_VALUE;
#undef TAKEDOWN;
}
