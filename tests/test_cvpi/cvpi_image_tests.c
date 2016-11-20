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

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
/* bcmp */

#include <sys/types.h>

#include <dirent.h>
/* opendir */

#include <time.h>

#ifndef CVPI_IMAGE_TESTS
#include "cvpi_image_tests.h"
#endif

#ifndef CVPI
#include "cvpi.h"
#endif

#ifndef CVPI_EGL_SURFACE_FUNCTIONS
#include "cvpi_egl_surface_functions.h"
#endif

#ifndef CVPI_IMAGE_FUNCTIONS
#include "cvpi_image_functions.h"
#endif

#ifndef CVPI_BASE
#include "cvpi_base.h"
#endif

#define IMAGES_CORRECT_DIR "./test_images_correct/"
#define IMAGES_TMP_DIR "./test_images_tmp/"

#define heap_error_check(pointer)\
  do {\
  if(pointer == NULL) {\
  cvpi_log_3("heap error, errno = ", __func__, __LINE__, errno);\
  BADSTATE = 1;\
  goto TAKEDOWN;\
  }\
  } while(0)

#define seek_error_check(seeker)\
  do {\
  int val = seeker;\
  if(val == -1) {\
  cvpi_log_3("fseek error, errno = ", __func__, __LINE__, errno);\
  BADSTATE = 1;\
  goto TAKEDOWN;\
  }} while(0)

//    fprintf(CVPI_LOG_FILE, "%s:%d: vgError:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
#define cvpi_vg_error_check()\
  do {\
  VGErrorCode error = vgGetError();\
  if(error != VG_NO_ERROR) {\
    cvpi_log_2("", __func__, __LINE__, cvpi_vg_error_string(error)); \
    BADSTATE = 1;\
    goto TAKEDOWN;\
  }\
  } while(0)

#define cvpi_image_error_check(image)\
  do {\
  if((image) == VG_INVALID_HANDLE) {\
  cvpi_log_1("", __func__, __LINE__); \
  BADSTATE = 1;\
  goto TAKEDOWN;\
  }\
  } while(0)

/* use inside TAKEDOWN */
#define cvpi_vg_error_takedown()\
  do {\
  VGErrorCode error = vgGetError();\
  if(error != VG_NO_ERROR) {\
    cvpi_log_2("", __func__, __LINE__, cvpi_vg_error_string(error)); \
    BADSTATE = 1;\
  }\
  } while(0)

#define vgDestroyImageSafe(image)do{if(image != VG_INVALID_HANDLE){vgDestroyImage(image); image=VG_INVALID_HANDLE;}}while(0)

#define freeSafe(pointer)\
      do {\
      if(pointer != NULL) {\
      free(pointer);\
      pointer = NULL;\
      }\
      } while(0)
#define fcloseSafeTakedown(pointer)\
      do {			   \
      if(pointer != NULL) {\
      int ret = fclose(pointer);		\
      if(ret) {								\
      cvpi_log_3("fclose error, errno = ", __func__, __LINE__, errno);\
      }						\
      pointer = NULL;				\
      }} while(0)

#define fcloseSafeCheck(pointer)\
      do {			   \
      if(pointer != NULL) {\
      int ret = fclose(pointer);		\
      if(ret) {								\
      cvpi_log_3("fclose error, errno = ", __func__, __LINE__, errno);\
      BADSTATE = 1;						      \
      goto TAKEDOWN;				\
      }						\
      pointer = NULL;				\
      }} while(0)

static CVPI_BOOL test_endianness_channel(TestImage** data, int channel) {
#define TAKEDOWN test_endianness_channel_takedown
  CVPI_BOOL return_value = CVPI_FALSE;
  int BADSTATE = 0;
  
  VGImage input_image = VG_INVALID_HANDLE;
  VGImage output_image = VG_INVALID_HANDLE;

  input_image = vgCreateImage(CVPI_COLOR_SPACE, data[0]->width,
			      data[0]->height,VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();
  output_image = vgCreateImage(CVPI_COLOR_SPACE, data[1]->width,
			       data[1]->height,VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();

  vgImageSubData(input_image, data[0]->data, data[0]->stride, CVPI_COLOR_SPACE, 0, 0, data[0]->width, data[0]->height);
  cvpi_vg_error_check();

  switch (channel) {
  case cvpi_pixel_red:
    vgColorMatrix(output_image, input_image, cvpi_channel_red);
    break;
  case cvpi_pixel_green:
    vgColorMatrix(output_image, input_image, cvpi_channel_green);
    break;
  case cvpi_pixel_blue:
    vgColorMatrix(output_image, input_image, cvpi_channel_blue);
    break;
  case cvpi_pixel_alpha:
    vgColorMatrix(output_image, input_image, cvpi_channel_alpha);
    break;
  default:
    fprintf(stderr, "%s: Invalid channel", __func__);
    goto TAKEDOWN;
  }

  cvpi_vg_error_check();

  vgGetImageSubData(output_image, data[1]->data, data[1]->stride, CVPI_COLOR_SPACE, 0, 0, data[1]->width, data[1]->height);
  cvpi_vg_error_check();
  
  vgFinish();
  cvpi_vg_error_check();

  return_value = CVPI_TRUE;

 TAKEDOWN:
  vgDestroyImageSafe(input_image);
  cvpi_vg_error_takedown();

  vgDestroyImageSafe(output_image);
  cvpi_vg_error_takedown();

  return return_value;

#undef TAKEDOWN
}

CVPI_BOOL test_endianness_channel_red(TestImage** data) {
  return test_endianness_channel(data, cvpi_pixel_red);
}

CVPI_BOOL test_endianness_channel_green(TestImage** data) {
  return test_endianness_channel(data, cvpi_pixel_green);
}

CVPI_BOOL test_endianness_channel_blue(TestImage** data) {
  return test_endianness_channel(data, cvpi_pixel_blue);
}

CVPI_BOOL test_endianness_channel_alpha(TestImage** data) {
  return test_endianness_channel(data, cvpi_pixel_alpha);
}

CVPI_BOOL test_cvpi_pixel(void) {
#define TAKEDOWN test_cvpi_pixel_takedown
  int BADSTATE = 0;
  
  char width = 1;
  char height = 1;
  char* data = NULL;
  VGImage image = VG_INVALID_HANDLE;
  CVPI_BOOL return_value = CVPI_FALSE;
  
  data = malloc(width*height*CVPI_PIXEL_BYTES);
  heap_error_check(data);
  
  image = vgCreateImagePainted(CVPI_COLOR_SPACE,width, height,VG_IMAGE_QUALITY_NONANTIALIASED,
				       4,3,2,1);
  cvpi_vg_error_check();
  
  cvpi_pixel pixel;
  pixel.channel[cvpi_pixel_red] = 4;
  pixel.channel[cvpi_pixel_green] = 3;
  pixel.channel[cvpi_pixel_blue] = 2;
  pixel.channel[cvpi_pixel_alpha] = 1;
  
  vgGetImageSubData(image, (void*)data,
		    CVPI_PIXEL_BYTES * width, CVPI_COLOR_SPACE, 0, 0, width, height);
  cvpi_vg_error_check();
  vgFinish();
  cvpi_vg_error_check();
  
  int cmp = memcmp((void*)data, &pixel, width*height*CVPI_PIXEL_BYTES);
  if(cmp == 0) {
    return_value = CVPI_TRUE; 
  } else {
    fprintf(CVPI_LOG_FILE, "%s: cvpi_pixel: %d %d %d %d | VGImage: %d %d %d %d.\n", __func__,
	    pixel.channel[cvpi_pixel_red], pixel.channel[cvpi_pixel_green], 
	    pixel.channel[cvpi_pixel_blue], pixel.channel[cvpi_pixel_alpha], 
	    data[0], data[1], data[2], data[3]);
  }

  TAKEDOWN:

  freeSafe(data);
  vgDestroyImageSafe(image);
  cvpi_vg_error_takedown();

  if(BADSTATE) {
    return_value = CVPI_FALSE;
  }

  return return_value;
#undef TAKEDOWN
}

CVPI_BOOL test_cvpi_yuyv2yuva(TestImage** data) {
#define TAKEDOWN test_cvpi_yuyv2yuva_takedown
  int BADSTATE = 0;
  
  const int INPUT_WIDTH = 640;
  const int INPUT_HEIGHT = 960;
  const int OUTPUT_WIDTH = 1280;
  const int OUTPUT_HEIGHT = 960;
  CVPI_BOOL return_value = CVPI_FALSE;

  char* yuyv2yuva_input = IMAGES_CORRECT_DIR "mark1_black.yuv";
  char* yuyv2yuva_correct_result = IMAGES_CORRECT_DIR "mark1_black_yuva.yuv";
  char* yuyv2yuva_tmp_result = IMAGES_TMP_DIR "mark1_black_yuva.yuv";

  FILE* input = NULL;
  FILE* correct = NULL;
  FILE* output = NULL;
  void* image_in = NULL;
  void* image_out = NULL;
  void* image_correct = NULL;
  VGImage input_image = VG_INVALID_HANDLE;
  VGImage output_image = VG_INVALID_HANDLE;
  
  input = fopen(yuyv2yuva_input, "r");
  heap_error_check(input);

  correct = fopen(yuyv2yuva_correct_result, "r");
  heap_error_check(correct);

  output = fopen(yuyv2yuva_tmp_result, "w");
  heap_error_check(output);
  
  seek_error_check(fseek(input, 0L, SEEK_END));
  int input_size = ftell(input);		/* input file size */
  seek_error_check(input_size);
  seek_error_check(fseek(input, 0L, SEEK_SET));

  seek_error_check(fseek(correct, 0L, SEEK_END));
  int correct_size = ftell(correct);		/* correct file size */
  seek_error_check(correct_size);
  seek_error_check(fseek(correct, 0L, SEEK_SET));

  if((input_size*2 != correct_size
      && correct_size != CVPI_PIXEL_BYTES * OUTPUT_WIDTH * OUTPUT_HEIGHT)) {
    fprintf(CVPI_LOG_FILE, "%s: Input file and correct file size mismatch. %d and %d\n",
	    __func__, input_size*2, correct_size);
    BADSTATE = 1;
    goto TAKEDOWN;
  }

  image_in = malloc(input_size);
  heap_error_check(image_in);

  image_out = malloc(2*input_size);
  heap_error_check(image_out);

  image_correct = malloc(correct_size);

  if(input_size != fread(image_in, 1, input_size, input)) {
    fprintf(CVPI_LOG_FILE, "%s: Reading the input image from memory failed.\n",__func__);
    BADSTATE = 1;
    goto TAKEDOWN;
  }
  if(correct_size != fread(image_correct, 1, correct_size, correct)) {
    fprintf(CVPI_LOG_FILE, "%s: Reading the correct image from memory failed.\n",__func__);
    BADSTATE = 1;
    goto TAKEDOWN;
  }

  input_image = vgCreateImage(CVPI_COLOR_SPACE,INPUT_WIDTH,
			      INPUT_HEIGHT,VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();
  vgFinish();
  cvpi_vg_error_check();

  vgImageSubData(input_image, image_in, INPUT_WIDTH*CVPI_PIXEL_BYTES,
		 CVPI_COLOR_SPACE, 0, 0, INPUT_WIDTH, INPUT_HEIGHT);
  cvpi_vg_error_check();
  vgFinish();
  cvpi_vg_error_check();
  
  output_image = cvpi_yuyv2yuva(input_image);
  cvpi_image_error_check(output_image);
  
  vgGetImageSubData(output_image, image_out, CVPI_PIXEL_BYTES*OUTPUT_WIDTH,
		    CVPI_COLOR_SPACE, 0, 0, OUTPUT_WIDTH, OUTPUT_HEIGHT);
  cvpi_vg_error_check();
  vgFinish();
  cvpi_vg_error_check();

  if(!memcmp(image_out, image_correct, correct_size)) {
    return_value = CVPI_TRUE;
  } else {
    size_t written = fwrite(image_out, CVPI_PIXEL_BYTES * OUTPUT_WIDTH, OUTPUT_HEIGHT, output);
    int flushed = fflush(output);
    unsigned long fwrite_size = CVPI_PIXEL_BYTES * OUTPUT_WIDTH * OUTPUT_HEIGHT;
    if(written != fwrite_size || flushed != 0) {
      if(written != fwrite_size) {
	fprintf(CVPI_LOG_FILE, "%s: Error writing file: fwrite size discrepancy\nexpected:%d\nreturned:%d.\n",
		__func__, fwrite_size, written);
	BADSTATE = 1;
      } else {
	fprintf(CVPI_LOG_FILE, "%s: Error writing file: fflush\nerrno = %d\nreturned = %d\n",
		__func__, errno, flushed);
	BADSTATE = 1;
      }
    }
  }

  TAKEDOWN:
  
  vgDestroyImage(output_image);
  cvpi_vg_error_takedown();
  vgDestroyImage(input_image);
  cvpi_vg_error_takedown();
  freeSafe(image_in);
  
  freeSafe(image_out);
  freeSafe(image_correct);
  fcloseSafeTakedown(input);
  fcloseSafeTakedown(output);
  fcloseSafeTakedown(correct);

  if(BADSTATE) {
    return_value = CVPI_FALSE;
  }

  return return_value;
#undef TAKEDOWN
}

static CVPI_BOOL test_cvpi_image_add_common(int width, int height, 
					     VGfloat * image1_color, VGfloat * image2_color, 
					     VGshort a, VGshort b, VGfloat scale, VGfloat bias, 
					     VGfloat * image_correct_color, char* file_path) {
#define TAKEDOWN test_cvpi_image_add_common_takedown
int BADSTATE = 0;
 VGImage image0 = VG_INVALID_HANDLE;
 VGImage image1 = VG_INVALID_HANDLE;
 VGImage image2 = VG_INVALID_HANDLE;
 VGImage image_correct = VG_INVALID_HANDLE;
 VGImage image_out = VG_INVALID_HANDLE;

 void* added_data = NULL;
 void* correct_data = NULL;
 FILE* output = NULL;
 
 CVPI_BOOL return_value = CVPI_FALSE;
 
  /* Create two solid color images, add them, and check the value of each pixel. */

  image0 = vgCreateImage(CVPI_COLOR_SPACE, width, height,VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();

  image1 = vgCreateImage(CVPI_COLOR_SPACE, width, height,VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();

  image2 = vgCreateImage(CVPI_COLOR_SPACE, width, height,VG_IMAGE_QUALITY_NONANTIALIASED); 
  cvpi_vg_error_check();

  image_correct = vgCreateImage(CVPI_COLOR_SPACE, width, height,VG_IMAGE_QUALITY_NONANTIALIASED); 
  cvpi_vg_error_check();

  vgColorMatrixNormal(image1, image0, &image1_color[0]);
  cvpi_vg_error_check();
  vgColorMatrixNormal(image2, image0, &image2_color[0]);
  cvpi_vg_error_check();
  vgColorMatrixNormal(image_correct, image0, &image_correct_color[0]);
  cvpi_vg_error_check();
  vgFlush();
  cvpi_vg_error_check();
  
  image_out = cvpi_image_add(image1, image2, a, b, scale, bias);
  cvpi_image_error_check(image_out);

  added_data = malloc(width*height*CVPI_PIXEL_BYTES);
  heap_error_check(added_data);

  correct_data = malloc(width*height*CVPI_PIXEL_BYTES);
  heap_error_check(correct_data);

  vgGetImageSubData(image_out, added_data, CVPI_PIXEL_BYTES*width,
		    CVPI_COLOR_SPACE, 0, 0, width, height);
  cvpi_vg_error_check();
  
  vgGetImageSubData(image_correct, correct_data, CVPI_PIXEL_BYTES*width,
		    CVPI_COLOR_SPACE, 0, 0, width, height);
  cvpi_vg_error_check();

  vgDestroyImageSafe(image0);
  cvpi_vg_error_check();
  vgDestroyImageSafe(image1);
  cvpi_vg_error_check();
  vgDestroyImageSafe(image2);
  cvpi_vg_error_check();
  vgDestroyImageSafe(image_out);
  cvpi_vg_error_check();
  vgDestroyImageSafe(image_correct);
  cvpi_vg_error_check();

  int cmp = memcmp(added_data, correct_data, width*height*CVPI_PIXEL_BYTES);

  if(cmp == 0) {
    return_value = CVPI_TRUE;
  } else if(cmp > 0) {
    fprintf(CVPI_LOG_FILE, "%s: added_data is greater than the correct data.\n", __func__);
  } else {
    fprintf(CVPI_LOG_FILE, "%s: added_data is less than the correct data.\n", __func__);
  }
  if(strlen(file_path) > 0) {      
    output = fopen(file_path, "w");
    heap_error_check(output);

    unsigned long fwrite_size = width*height*CVPI_PIXEL_BYTES;
    size_t written = fwrite(added_data, fwrite_size, 1, output);
    int flushed = fflush(output);
    if(written != fwrite_size || flushed != 0) {
      if(written != fwrite_size) {
	fprintf(CVPI_LOG_FILE, "%s: Error writing file: fwrite size discrepancy\nexpected:%d\nreturned:%d.\n",
		__func__, fwrite_size, written);
      } else {
	fprintf(CVPI_LOG_FILE, "%s: Error writing file: fflush\nerrno = %d\nreturned = %d\n",
		__func__, errno, flushed);
      }
    }
  }

  TAKEDOWN:
  fcloseSafeTakedown(output);
  vgDestroyImageSafe(image0);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(image1);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(image2);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(image_out);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(image_correct);
  cvpi_vg_error_takedown();
  freeSafe(added_data);
  freeSafe(correct_data);

  if(BADSTATE) {
    return_value = CVPI_FALSE;
  }

  return return_value;
#undef TAKEDOWN
}

CVPI_BOOL test_cvpi_image_add_odd(void) {
  VGfloat image1_color[20] = {
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,

    1,1,1,1
  };

  VGfloat image2_color[20] = {
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,

    2,2,2,2
  };

  VGfloat image_correct_color[20] = {
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,

    3,3,3,3
  };

  return test_cvpi_image_add_common(640, 3, image1_color, image2_color, 1, 1, 1, 0, image_correct_color, IMAGES_TMP_DIR "test_cvpi_add_images_odd.yuv");
}

CVPI_BOOL test_cvpi_image_add_single(void) {
  VGfloat image1_color[20] = {
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,

    1,1,1,1
  };

  VGfloat image2_color[20] = {
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,

    2,2,2,2
  };

  VGfloat image_correct_color[20] = {
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,

    3,3,3,3
  };

  return test_cvpi_image_add_common(640, 1, image1_color, image2_color, 1, 1, 1, 0, image_correct_color, IMAGES_TMP_DIR "test_cvpi_add_images_single.yuv");
}

CVPI_BOOL test_cvpi_image_add_even(void) {
  VGfloat image1_color[20] = {
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,

    1,1,1,1
  };

  VGfloat image2_color[20] = {
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,

    2,2,2,2
  };

  VGfloat image_correct_color[20] = {
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,

    3,3,3,3
  };

  return test_cvpi_image_add_common(640, 2, image1_color, image2_color, 1, 1, 1, 0, image_correct_color, IMAGES_TMP_DIR "test_cvpi_add_images_even.yuv");
}

CVPI_BOOL test_cvpi_subtract_images_odd(void) {
  VGfloat image2_color[20] = {
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,

    1,1,1,1
  };

  VGfloat image_correct_color[20] = {
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,

    2,2,2,2
  };

  VGfloat image1_color[20] = {
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,

    3,3,3,3
  };

  return test_cvpi_image_add_common(640, 3, image1_color, image2_color, 1, -1, 1, 0, image_correct_color, IMAGES_TMP_DIR "test_cvpi_subtract_images_odd.yuv");
}

CVPI_BOOL test_cvpi_image_add_odd_scale_add(void) {
  VGfloat image2_color[20] = {
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,

    1,1,1,1
  };

  VGfloat image_correct_color[20] = {
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,

    37,37,37,37
  };

  VGfloat image1_color[20] = {
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,

    3,3,3,3
  };

  return test_cvpi_image_add_common(640, 3, image1_color, image2_color, 2, 1, 5, 2, image_correct_color, 
				    IMAGES_TMP_DIR "test_cvpi_add_images_odd_scale_add.yuv");
}
CVPI_BOOL test_cvpi_subtract_images_odd_scale_add(void) {
  VGfloat image2_color[20] = {
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,

    1,1,1,1
  };

  VGfloat image_correct_color[20] = {
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,

    7,7,7,7
  };

  VGfloat image1_color[20] = {
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,

    3,3,3,3
  };

  return test_cvpi_image_add_common(640, 3, image1_color, image2_color, 1, -2, 5, 2, image_correct_color, 
				    IMAGES_TMP_DIR "test_cvpi_subtract_images_odd_scale_add.yuv");
}

CVPI_BOOL test_cvpi_image_add_huge_even(void) {
  VGfloat image1_color[20] = {
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,

    1,1,1,1
  };

  VGfloat image2_color[20] = {
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,

    2,2,2,2
  };

  VGfloat image_correct_color[20] = {
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,

    3,3,3,3
  };

  return test_cvpi_image_add_common(EGL_CONFIG_MAX_WIDTH, EGL_CONFIG_MAX_HEIGHT, image1_color, image2_color, 1, 1, 1, 0, 
				    image_correct_color, IMAGES_TMP_DIR "test_cvpi_add_images_huge_even.yuv");
}
CVPI_BOOL test_cvpi_image_add_huge_odd(void) {
  VGfloat image1_color[20] = {
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,

    1,1,1,1
  };

  VGfloat image2_color[20] = {
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,

    2,2,2,2
  };

  VGfloat image_correct_color[20] = {
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,

    3,3,3,3
  };

  return test_cvpi_image_add_common(EGL_CONFIG_MAX_WIDTH-1, EGL_CONFIG_MAX_HEIGHT-1, image1_color, image2_color, 1, 1, 1, 0, 
				    image_correct_color, IMAGES_TMP_DIR "test_cvpi_add_images_huge_odd.yuv");
}

CVPI_BOOL test_cvpi_channel_add_RB(void) {
#define TAKEDOWN test_cvpi_add_channels_RB_takedown
  int BADSTATE = 0;

  VGfloat image_color[20] = {
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,

    0,0,0,0
  };

  VGfloat image_correct_color[20] = {
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,

    10,10,10,10
  };

  int width = 10;
  int height = 10;

  VGImage dummy = VG_INVALID_HANDLE;
  VGImage input_image = VG_INVALID_HANDLE;
  VGImage image_correct = VG_INVALID_HANDLE;
  VGImage first_image = VG_INVALID_HANDLE;
  VGImage second_image = VG_INVALID_HANDLE;
  void* added_data = NULL;
  void* correct_data = NULL;
  char* file_path = NULL;
  FILE* output = NULL;

  CVPI_BOOL return_value = CVPI_FALSE;
  
  dummy = vgCreateImage(CVPI_COLOR_SPACE, width, height,VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();
  input_image = vgCreateImage(CVPI_COLOR_SPACE, width, height,VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();

  image_correct = vgCreateImage(CVPI_COLOR_SPACE, width, height,VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();

  vgColorMatrixNormal(input_image, dummy, image_color);
  cvpi_vg_error_check();
  vgColorMatrixNormal(image_correct, dummy, image_correct_color);
  cvpi_vg_error_check();
  
  /* green and blue channels should contain 1's */
  first_image = cvpi_channel_add(input_image, VG_RED, VG_BLUE, 1, 1, 1, 
				 1, 1, 1, 1, 
				 VG_GREEN | VG_ALPHA);
  cvpi_image_error_check(first_image);

  /* all channels should contain 10's */
  second_image = cvpi_channel_add(first_image, VG_GREEN, VG_ALPHA, 2, 3, 2, 
				  0, 0, 0, 0,
				  VG_RED|VG_BLUE|VG_GREEN|VG_ALPHA);
  cvpi_image_error_check(second_image);

  VGint second_image_width = vgGetParameteri(second_image, VG_IMAGE_WIDTH);
  cvpi_vg_error_check();
  VGint second_image_height = vgGetParameteri(second_image, VG_IMAGE_HEIGHT);
  cvpi_vg_error_check();
  size_t image_size = width*height*CVPI_PIXEL_BYTES;
  if(image_size <= 0) {
    fprintf(CVPI_LOG_FILE, "%s:%d:malloc size = 0\n", __func__, __LINE__);
    BADSTATE = 1;
    goto TAKEDOWN;
  }
  added_data = malloc(image_size);
  heap_error_check(added_data);

  correct_data = malloc(image_size);
  heap_error_check(correct_data);

  vgGetImageSubData(second_image, added_data, CVPI_PIXEL_BYTES*width,
    CVPI_COLOR_SPACE, 0, 0, width, height);
  cvpi_vg_error_check();
  vgGetImageSubData(image_correct, correct_data, CVPI_PIXEL_BYTES*width,
    CVPI_COLOR_SPACE, 0, 0, width, height); 
  cvpi_vg_error_check();
  vgFinish();
  cvpi_vg_error_check();

  vgDestroyImageSafe(dummy);
  cvpi_vg_error_check();
  vgDestroyImageSafe(input_image);
  cvpi_vg_error_check();
  vgDestroyImageSafe(image_correct);
  cvpi_vg_error_check();
  vgDestroyImageSafe(first_image);
  cvpi_vg_error_check();
  vgDestroyImageSafe(second_image);
  cvpi_vg_error_check();
  vgFlush();
  cvpi_vg_error_check();
  
  int cmp = memcmp(added_data, correct_data, image_size);
  if(cmp == 0) {
    return_value = CVPI_TRUE;
  } else if(cmp > 0) {
    fprintf(CVPI_LOG_FILE, "%s: added_data is greater than the correct data.\n", __func__);
  } else {
    fprintf(CVPI_LOG_FILE, "%s: added_data is less than the correct data.\n", __func__);
  }
  char* file_name = "test_cvpi_channel_add_RB";

  int path_length = strlen(IMAGES_TMP_DIR) + strlen(file_name);
  if(path_length > 0) {
    file_path = malloc(sizeof(*file_path) * path_length);
    heap_error_check(file_path);
    
    int file_path_print = sprintf(file_path, "%s%s", IMAGES_TMP_DIR, file_name);
    if(file_path_print < 0) {
      fprintf(CVPI_LOG_FILE, "%s:%d:error writing file path: %d\n", __func__, __LINE__, file_path_print);
      return_value = CVPI_FALSE;
      BADSTATE = 1;
      goto TAKEDOWN;
    }

    output = fopen(file_path, "w");
    heap_error_check(output);

    unsigned long fwrite_size = width*height*CVPI_PIXEL_BYTES;
    size_t written = fwrite(added_data, fwrite_size, 1, output);
    int flushed = fflush(output);

    if(written != fwrite_size || flushed != 0) {
      if(written != fwrite_size) {
	fprintf(CVPI_LOG_FILE, "%s: Error writing file: fwrite size discrepancy\nexpected:%d\nreturned:%d.\n",
		__func__, fwrite_size, written);
      } else {
	fprintf(CVPI_LOG_FILE, "%s: Error writing file: fflush\nerrno = %d\nreturned = %d\n",
		__func__, errno, flushed);
      }
    }
  } else {
    fprintf(CVPI_LOG_FILE, "%s:%d: file path length = 0\n", __func__, __LINE__);
  }
 TAKEDOWN:
  fcloseSafeTakedown(output);

  freeSafe(file_path);
  freeSafe(added_data);
  freeSafe(correct_data);
  
  vgDestroyImageSafe(dummy);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(input_image);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(image_correct);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(first_image);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(second_image);
  cvpi_vg_error_takedown();

  if(BADSTATE) {
    return_value = CVPI_FALSE;
  }

  return return_value;
#undef TAKEDOWN
}
CVPI_BOOL test_cvpi_channel_add_RR(void) {
#define TAKEDOWN test_cvpi_channel_add_RR_takedown
  int BADSTATE = 0;

  VGfloat image_color[20] = {
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,

    0,0,0,0
  };

  VGfloat image_correct_color[20] = {
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,

    10,10,10,10
  };

  int width = 10;
  int height = 10;
  VGImage dummy = VG_INVALID_HANDLE;
  VGImage input_image = VG_INVALID_HANDLE;
  VGImage image_correct = VG_INVALID_HANDLE;
  VGImage first_image = VG_INVALID_HANDLE;
  VGImage second_image = VG_INVALID_HANDLE;
  void* added_data = NULL;
  void* correct_data = NULL;
  char* file_path = NULL;

  FILE* output = NULL;

  CVPI_BOOL return_value = CVPI_FALSE;
  dummy = vgCreateImage(CVPI_COLOR_SPACE, width, height,VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();

  input_image = vgCreateImage(CVPI_COLOR_SPACE, width, height,VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();
  image_correct = vgCreateImage(CVPI_COLOR_SPACE, width, height,VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();

  vgColorMatrixNormal(input_image, dummy, image_color);
  cvpi_vg_error_check();
  vgColorMatrixNormal(image_correct, dummy, image_correct_color);
  cvpi_vg_error_check();
  
  /* green and blue channels should contain 1's */
  first_image = cvpi_channel_add(input_image, VG_RED, VG_RED, 1, 1, 1, 
				 1, 1, 1, 1, VG_RED);
  cvpi_image_error_check(first_image);

  /* all channels should contain 10's */
  second_image = cvpi_channel_add(first_image, VG_RED, VG_RED, 2, 3, 2, 
				  0, 0, 0, 0,
				  VG_RED|VG_BLUE|VG_GREEN|VG_ALPHA);
  cvpi_image_error_check(second_image);
  size_t image_size = width*height*CVPI_PIXEL_BYTES;
  if(image_size <= 0) {
    fprintf(CVPI_LOG_FILE, "%s:%d:malloc size = 0\n", __func__, __LINE__);
    return_value = CVPI_FALSE;
    BADSTATE = 1;
    goto TAKEDOWN;
  }

  added_data = malloc(image_size);
  heap_error_check(added_data);
  
  correct_data = malloc(image_size);
  heap_error_check(correct_data);

  vgGetImageSubData(second_image, added_data, CVPI_PIXEL_BYTES*width,
		    CVPI_COLOR_SPACE, 0, 0, width, height);
  cvpi_vg_error_check();
  vgGetImageSubData(image_correct, correct_data, CVPI_PIXEL_BYTES*width,
		    CVPI_COLOR_SPACE, 0, 0, width, height);
  cvpi_vg_error_check();

  vgDestroyImageSafe(dummy);
  cvpi_vg_error_check();
  vgDestroyImageSafe(input_image);
  cvpi_vg_error_check();
  vgDestroyImageSafe(image_correct);
  cvpi_vg_error_check();
  vgDestroyImageSafe(first_image);
  cvpi_vg_error_check();
  vgDestroyImageSafe(second_image);
  cvpi_vg_error_check();

  vgFlush();
  cvpi_vg_error_check();

  int cmp = memcmp(added_data, correct_data, width*height*CVPI_PIXEL_BYTES);
  if(cmp == 0) {
    return_value = CVPI_TRUE;
  } else if(cmp > 0) {
    fprintf(CVPI_LOG_FILE, "%s: added_data is greater than the correct data.\n", __func__);
  } else {
    fprintf(CVPI_LOG_FILE, "%s: added_data is less than the correct data.\n", __func__);
  }

  char* file_name = "test_cvpi_channel_add_RR";

  int path_length = strlen(IMAGES_TMP_DIR) + strlen(file_name);
  if(path_length > 0) {
    file_path = malloc(sizeof(*file_path) * path_length);
    heap_error_check(file_path);

    int file_path_print = sprintf(file_path, "%s%s", IMAGES_TMP_DIR, file_name);
    if(file_path_print < 0) {
      fprintf(CVPI_LOG_FILE, "%s:%d:error writing file path: %d\n", __func__, __LINE__, file_path_print);
      BADSTATE = 1;
      goto TAKEDOWN;
    }

    output = fopen(file_path, "w");
    heap_error_check(output);

    unsigned long fwrite_size = width*height*CVPI_PIXEL_BYTES;
    size_t written = fwrite(added_data, fwrite_size, 1, output);
    int flushed = fflush(output);
    if(written != fwrite_size || flushed != 0) {
      if(written != fwrite_size) {
	fprintf(CVPI_LOG_FILE, "%s: Error writing file: fwrite size discrepancy\nexpected:%d\nreturned:%d.\n",
		__func__, fwrite_size, written);
      } else {
	fprintf(CVPI_LOG_FILE, "%s: Error writing file: fflush\nerrno = %d\nreturned = %d\n",
		__func__, errno, flushed);
      }
    }
  } else {
    fprintf(CVPI_LOG_FILE, "%s:%d: file path length = 0\n", __func__, __LINE__);
  }

 TAKEDOWN:
  fcloseSafeTakedown(output);
  freeSafe(file_path);
  freeSafe(added_data);
  freeSafe(correct_data);

  vgDestroyImageSafe(dummy);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(input_image);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(image_correct);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(first_image);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(second_image);
  cvpi_vg_error_takedown();

  if(BADSTATE) {
    return_value = CVPI_FALSE;
  }
  
  return return_value;
#undef TAKEDOWN
}

CVPI_BOOL test_cvpi_color_channels_add(void) {
#define TAKEDOWN test_cvpi_color_channels_add_takedown
  int BADSTATE = 0;
  VGfloat image_color[20] = {
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,

    0,0,0,0
  };

  VGfloat image_correct_color[20] = {
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,

    1,7,1,7
  };

  int width = 10;
  int height = 10;
  CVPI_BOOL return_value = CVPI_FALSE;
  VGImage dummy = VG_INVALID_HANDLE;
  VGImage input_image = VG_INVALID_HANDLE;
  VGImage image_correct = VG_INVALID_HANDLE;
  VGImage first_image = VG_INVALID_HANDLE;
  VGImage second_image = VG_INVALID_HANDLE;
  void* added_data = NULL;
  void* correct_data = NULL;
  FILE* output = NULL;
  char* file_path = NULL;
  
  dummy = vgCreateImage(CVPI_COLOR_SPACE, width, height,VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();

  input_image = vgCreateImage(CVPI_COLOR_SPACE, width, height,VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();

  image_correct = vgCreateImage(CVPI_COLOR_SPACE, width, height,VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();

  vgColorMatrixNormal(input_image, dummy, image_color);
  cvpi_vg_error_check();
  vgColorMatrixNormal(image_correct, dummy, image_correct_color);
  cvpi_vg_error_check();
  
  /* all channels should contain 1's */
  first_image = cvpi_color_channels_add(input_image, 3, 2, 1, 
					1, 1, 1,
					VG_RED|VG_BLUE|VG_GREEN|VG_ALPHA);
  cvpi_image_error_check(first_image);

  /* green and blue channels should contain 7's and red and blue contain 1's */
  second_image = cvpi_color_channels_add(first_image, 2, 3, 2, 
					 0, 0, 0,
					 VG_GREEN | VG_ALPHA);
  cvpi_image_error_check(second_image);

  size_t image_size = width*height*CVPI_PIXEL_BYTES;
  if(image_size <= 0) {
    fprintf(CVPI_LOG_FILE, "%s:%d:malloc size = 0\n", __func__, __LINE__);
    return_value = CVPI_FALSE;
    BADSTATE = 1;
    goto TAKEDOWN;
  }
  added_data = malloc(image_size);
  heap_error_check(added_data);

  correct_data = malloc(image_size);
  heap_error_check(correct_data);
  
  vgGetImageSubData(second_image, added_data, CVPI_PIXEL_BYTES*width,
		    CVPI_COLOR_SPACE, 0, 0, width, height);
  cvpi_vg_error_check();
  vgGetImageSubData(image_correct, correct_data, CVPI_PIXEL_BYTES*width,
		    CVPI_COLOR_SPACE, 0, 0, width, height);
  cvpi_vg_error_check();

  vgDestroyImageSafe(dummy);
  cvpi_vg_error_check();
  vgDestroyImageSafe(input_image);
  cvpi_vg_error_check();
  vgDestroyImageSafe(image_correct);
  cvpi_vg_error_check();
  vgDestroyImageSafe(first_image);
  cvpi_vg_error_check();
  vgDestroyImageSafe(second_image);
  cvpi_vg_error_check();

  vgFlush();
  cvpi_vg_error_check();

  int cmp = memcmp(added_data, correct_data, width*height*CVPI_PIXEL_BYTES);
  if(cmp == 0) {
    return_value = CVPI_TRUE;
  } else if(cmp > 0) {
    fprintf(stderr, "%s: added_data is greater than the correct data.\n", __func__);
  } else {
    fprintf(stderr, "%s: added_data is less than the correct data.\n", __func__);
  }

  const char* file_name = __func__;

  int path_length = strlen(IMAGES_TMP_DIR) + strlen(file_name);
  if(path_length > 0) {
    file_path = malloc(sizeof(*file_path) * path_length);
    heap_error_check(file_path);
    
    int file_path_print = sprintf(file_path, "%s%s", IMAGES_TMP_DIR, file_name);
    if(file_path_print < 0) {
      fprintf(CVPI_LOG_FILE, "%s:%d:error writing file path: %d\n", __func__, __LINE__, file_path_print);
      return_value = CVPI_FALSE;
      BADSTATE = 1;
      goto TAKEDOWN;
    }

    output = fopen(file_path, "w");
    heap_error_check(output);

    unsigned long fwrite_size = width*height*CVPI_PIXEL_BYTES;
    size_t written  = fwrite(added_data, fwrite_size, 1, output);
    int flushed = fflush(output);
    if(written != fwrite_size || flushed != 0) {
      if(written != fwrite_size) {
	fprintf(CVPI_LOG_FILE, "%s: Error writing file: fwrite size discrepancy\nexpected:%d\nreturned:%d.\n",
		__func__, fwrite_size, written);
      } else {
	fprintf(CVPI_LOG_FILE, "%s: Error writing file: fflush\nerrno = %d\nreturned = %d\n",
		__func__, errno, flushed);
      }
    }
  } else {
    fprintf(CVPI_LOG_FILE, "%s:%d: file path length = 0\n", __func__, __LINE__);
  }

  TAKEDOWN:
  fcloseSafeTakedown(output);
  freeSafe(file_path);
  freeSafe(added_data);
  freeSafe(correct_data);
  
  vgDestroyImageSafe(dummy);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(input_image);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(image_correct);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(first_image);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(second_image);
  cvpi_vg_error_takedown();

  if(BADSTATE) {
    return_value = CVPI_FALSE;
  }

  return return_value;
#undef TAKEDOWN
}

CVPI_BOOL test_cvpi_all_channels_add(void) {
#define TAKEDOWN test_cvpi_all_channels_add_takedown
  int BADSTATE = 0;
  VGfloat image_color[20] = {
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,

    0,0,0,0
  };

  VGfloat image_correct_color[20] = {
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,

    1,8,1,8
  };

  int width = 10;
  int height = 10;
  CVPI_BOOL return_value = CVPI_FALSE;
  VGImage dummy = VG_INVALID_HANDLE;
  VGImage input_image = VG_INVALID_HANDLE;
  VGImage image_correct = VG_INVALID_HANDLE;
  VGImage first_image = VG_INVALID_HANDLE;
  VGImage second_image = VG_INVALID_HANDLE;
  void* added_data = NULL;
  void* correct_data = NULL;
  FILE* output = NULL;
  
  dummy = vgCreateImage(CVPI_COLOR_SPACE, width, height,VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();

  input_image = vgCreateImage(CVPI_COLOR_SPACE, width, height,VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();

  image_correct = vgCreateImage(CVPI_COLOR_SPACE, width, height,VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();

  vgColorMatrixNormal(input_image, dummy, image_color);
  cvpi_vg_error_check();
  vgColorMatrixNormal(image_correct, dummy, image_correct_color);
  cvpi_vg_error_check();
  
  /* all channels should contain 1's */
  first_image = cvpi_all_channels_add(input_image, 
				      3, 2, 1, 1, 
				      1, 1, 1, 1,
				      VG_RED|VG_BLUE|VG_GREEN|VG_ALPHA);
  cvpi_image_error_check(first_image);

  /* green and blue channels should contain 8's and red and blue contain 1's */
  second_image = cvpi_all_channels_add(first_image, 
				       2, 3, 2, 1, 
				       0, 0, 0, 0,
				       VG_GREEN | VG_ALPHA);
  cvpi_image_error_check(second_image);

  size_t image_size = width*height*CVPI_PIXEL_BYTES;
  if(image_size <= 0) {
    fprintf(CVPI_LOG_FILE, "%s:%d:malloc size = 0\n", __func__, __LINE__);
    return_value = CVPI_FALSE;
    BADSTATE = 1;
    goto TAKEDOWN;
  }
  added_data = malloc(image_size);
  heap_error_check(added_data);
  
  correct_data = malloc(image_size);
  heap_error_check(correct_data);

  vgGetImageSubData(second_image, added_data, CVPI_PIXEL_BYTES*width,
		    CVPI_COLOR_SPACE, 0, 0, width, height);
  cvpi_vg_error_check();
  vgGetImageSubData(image_correct, correct_data, CVPI_PIXEL_BYTES*width,
		    CVPI_COLOR_SPACE, 0, 0, width, height);
  cvpi_vg_error_check();

  vgDestroyImageSafe(dummy);
  cvpi_vg_error_check();
  vgDestroyImageSafe(input_image);
  cvpi_vg_error_check();
  vgDestroyImageSafe(image_correct);
  cvpi_vg_error_check();
  vgDestroyImageSafe(first_image);
  cvpi_vg_error_check();
  vgDestroyImageSafe(second_image);
  cvpi_vg_error_check();
  vgFlush();
  cvpi_vg_error_check();

  int cmp = memcmp(added_data, correct_data, width*height*CVPI_PIXEL_BYTES);
  if(cmp == 0) {
    return_value = CVPI_TRUE;
  } else if(cmp > 0) {
    fprintf(stderr, "%s: added_data is greater than the correct data.\n", __func__);
  } else {
    fprintf(stderr, "%s: added_data is less than the correct data.\n", __func__);
  }

  const char* file_path = IMAGES_TMP_DIR "test_cvpi_all_channels_add";
  int path_length = strlen(file_path);
  if(path_length > 0) {

    output = fopen(file_path, "w");
    heap_error_check(output);

    unsigned long fwrite_size = width*height*CVPI_PIXEL_BYTES;
    size_t written = fwrite(added_data, fwrite_size, 1, output);
    int flushed = fflush(output);
    if(written != fwrite_size || flushed != 0) {
      if(written != fwrite_size) {
	fprintf(CVPI_LOG_FILE, "%s: Error writing file: fwrite size discrepancy\nexpected:%d\nreturned:%d.\n",
		__func__, fwrite_size, written);
      } else {
	fprintf(CVPI_LOG_FILE, "%s: Error writing file: fflush\nerrno = %d\nreturned = %d\n",
		__func__, errno, flushed);
      }
    }
  } else {
    fprintf(CVPI_LOG_FILE, "%s:%d: file path length = 0\n", __func__, __LINE__);
  }

  TAKEDOWN:
  vgDestroyImageSafe(dummy);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(input_image);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(image_correct);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(first_image);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(second_image);
  cvpi_vg_error_takedown();
  vgFlush();
  cvpi_vg_error_check();

  freeSafe(added_data);
  freeSafe(correct_data);  
  //freeSafe(file_path);
  /* SEGFAULT */
/* Program received signal SIGSEGV, Segmentation fault. */
/* malloc_consolidate (av=0xb6eef250) at malloc.c:5190 */
/* 5190	malloc.c: No such file or directory. */
  fcloseSafeTakedown(output);

  if(BADSTATE) {
    return_value = CVPI_FALSE;
  }
  
  return return_value;
#undef TAKEDOWN
}

CVPI_BOOL test_cvpi_image_combine_channelwise(void) {
#define TAKEDOWN test_cvpi_image_combine_channelwise_takedown
  int BADSTATE = 0;
  VGfloat image1_color[20] = {
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,

    0,0,0,0
  };

  VGfloat image2_color[20] = {
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,

    1,1,1,1
  };

  VGfloat image_correct_color[20] = {
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,

    0,0,1,1
  };

  int width = 10;
  int height = 10;
  VGImage dummy = VG_INVALID_HANDLE;
  VGImage image1 = VG_INVALID_HANDLE;
  VGImage image2 = VG_INVALID_HANDLE;
  VGImage image_correct = VG_INVALID_HANDLE;
  VGImage output_image = VG_INVALID_HANDLE;
  void* added_data = NULL;
  void* correct_data = NULL;
  char* file_path = NULL;
  FILE* output = NULL;
  
  CVPI_BOOL return_value = CVPI_FALSE;
  
  dummy = vgCreateImage(CVPI_COLOR_SPACE, width, height,VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();

  image1 = vgCreateImage(CVPI_COLOR_SPACE, width, height,VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();

  image2 = vgCreateImage(CVPI_COLOR_SPACE, width, height,VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();

  image_correct = vgCreateImage(CVPI_COLOR_SPACE, width, height,VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();

  vgColorMatrixNormal(image1, dummy, image1_color);
  cvpi_vg_error_check();
  vgColorMatrixNormal(image2, dummy, image2_color);
  cvpi_vg_error_check();
  vgColorMatrixNormal(image_correct, dummy, image_correct_color);
  cvpi_vg_error_check();
  
  output_image = cvpi_image_combine_channelwise(image1, image2, VG_RED|VG_GREEN);
  cvpi_image_error_check(output_image);
  
  size_t image_size = width*height*CVPI_PIXEL_BYTES;
  if(image_size <= 0) {
    fprintf(CVPI_LOG_FILE, "%s:%d:malloc size = 0\n", __func__, __LINE__);

    return_value = CVPI_FALSE;
    BADSTATE = 1;
    goto TAKEDOWN;
  }
  added_data = malloc(image_size);
  heap_error_check(added_data);

  correct_data = malloc(image_size);
  heap_error_check(correct_data);

  vgGetImageSubData(output_image, added_data, CVPI_PIXEL_BYTES*width,
		    CVPI_COLOR_SPACE, 0, 0, width, height);
  cvpi_vg_error_check();
  vgGetImageSubData(image_correct, correct_data, CVPI_PIXEL_BYTES*width,
		    CVPI_COLOR_SPACE, 0, 0, width, height);
  cvpi_vg_error_check();

  vgDestroyImageSafe(dummy);
  cvpi_vg_error_check();
  vgDestroyImageSafe(image_correct);
  cvpi_vg_error_check();
  vgDestroyImageSafe(image1);
  cvpi_vg_error_check();
  vgDestroyImageSafe(image2);
  cvpi_vg_error_check();
  vgDestroyImageSafe(output_image);
  cvpi_vg_error_check();

  vgFlush();
  cvpi_vg_error_check();

  int cmp = memcmp(added_data, correct_data, image_size);
  if(cmp == 0) {
    return_value = CVPI_TRUE;
  } else if(cmp > 0) {
    fprintf(stderr, "%s: added_data is greater than the correct data.\n", __func__);
  } else {
    fprintf(stderr, "%s: added_data is less than the correct data.\n", __func__);
  }

  const char* file_name = __func__;

  int path_length = strlen(IMAGES_TMP_DIR) + strlen(file_name);
  if(path_length > 0) {
    file_path = malloc(sizeof(*file_path) * path_length);
    heap_error_check(file_path);

    int file_path_print = sprintf(file_path, "%s%s", IMAGES_TMP_DIR, file_name);
    if(file_path_print < 0) {
      fprintf(CVPI_LOG_FILE, "%s:%d:error writing file path: %d\n", __func__, __LINE__, file_path_print);
      return_value = CVPI_FALSE;
      BADSTATE = 1;
      goto TAKEDOWN;
    }
    output = fopen(file_path, "w");
    heap_error_check(output);

    unsigned long fwrite_size = width*height*CVPI_PIXEL_BYTES;
    size_t written = fwrite(added_data, fwrite_size, 1, output);
    int flushed = fflush(output);
    if(written != fwrite_size || flushed != 0) {
      if(written != fwrite_size) {
	fprintf(CVPI_LOG_FILE, "%s: Error writing file: fwrite size discrepancy\nexpected:%d\nreturned:%d.\n",
		__func__, fwrite_size, written);
      } else {
	fprintf(CVPI_LOG_FILE, "%s: Error writing file: fflush\nerrno = %d\nreturned = %d\n",
		__func__, errno, flushed);
      }
    }
  } else {
    fprintf(CVPI_LOG_FILE, "%s:%d: file path length = 0\n", __func__, __LINE__);
  }
  TAKEDOWN:
  fcloseSafeTakedown(output);

  freeSafe(file_path);
  freeSafe(added_data);
  freeSafe(correct_data);
  
  vgDestroyImageSafe(dummy);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(image_correct);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(image1);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(image2);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(output_image);
  cvpi_vg_error_takedown();

  if(BADSTATE) {
    return_value = CVPI_FALSE;
  }

  return return_value;
#undef TAKEDOWN
}

static CVPI_BOOL test_cvpi_channel_threshold_common(char* input_file_name, char* output_file_name,
						    int width, int height, 
						    VGImageChannel channel, VGubyte bound1, VGubyte bound2, 
						    VGubyte fill, CVPI_BOOL invert) {
#define TAKEDOWN test_cvpi_channel_threshold_common_takedown
  int BADSTATE = 0;
  CVPI_BOOL return_value = CVPI_TRUE;
  FILE* input_file = NULL;
  FILE* output_file = NULL;
  char* input_path = NULL;
  void* input_data = NULL;
  char* output_path = NULL;
  void* thresholded_data = NULL;
  
  VGImage image = VG_INVALID_HANDLE;
  VGImage thresholded = VG_INVALID_HANDLE;

  size_t path_size = sizeof(*input_file_name) * (strlen(IMAGES_CORRECT_DIR) + strlen(input_file_name));
  if(path_size <= 0) {
    fprintf(CVPI_LOG_FILE, "%s:%d:malloc size = 0\n", __func__, __LINE__);
    return_value = CVPI_FALSE;
    BADSTATE = 1;
    goto TAKEDOWN;
  }
  input_path = malloc(path_size);
  heap_error_check(input_path);

  int file_path_print = sprintf(input_path, "%s%s", IMAGES_CORRECT_DIR, input_file_name);
  if(file_path_print < 0) {
    fprintf(CVPI_LOG_FILE, "%s:%d:error writing file path: %d\n", __func__, __LINE__, file_path_print);
    return_value = CVPI_FALSE;
    BADSTATE = 1;
    goto TAKEDOWN;
  }

  input_file = fopen(input_path, "r");
  heap_error_check(input_file);

  seek_error_check(fseek(input_file, 0L, SEEK_END));
  unsigned long input_file_size = ftell(input_file);
  seek_error_check(input_file_size);
  seek_error_check(fseek(input_file, 0L, SEEK_SET));

  if(input_file_size <= 0) {
    fprintf(CVPI_LOG_FILE, "%s:%d:malloc size = 0\n", __func__, __LINE__);
    return_value = CVPI_FALSE;
    BADSTATE = 1;
    goto TAKEDOWN;
  }

  input_data = malloc(input_file_size);
  heap_error_check(input_data);

  if(input_file_size != fread(input_data, 1, input_file_size, input_file)) {
    fprintf(stderr, "%s:%d: Reading the input image from memory failed.\n",__func__, __LINE__);
    return_value = CVPI_FALSE;
    BADSTATE = 1;
    goto TAKEDOWN;
  }

  size_t output_path_size = sizeof(*output_file_name) * (strlen(IMAGES_TMP_DIR) + strlen(output_file_name));
  if(output_path_size <= 0) {
    fprintf(CVPI_LOG_FILE, "%s:%d:malloc size = 0\n", __func__, __LINE__);
    return_value = CVPI_FALSE;
    BADSTATE = 1;
    goto TAKEDOWN;
  }
  output_path = malloc(output_path_size);
  heap_error_check(output_path);

  file_path_print = sprintf(output_path, "%s%s", IMAGES_TMP_DIR, output_file_name);
  if(file_path_print < 0) {
    fprintf(CVPI_LOG_FILE, "%s:%d:error writing file path: %d\n", __func__, __LINE__, file_path_print);
    return_value = CVPI_FALSE;
    BADSTATE = 1;
    goto TAKEDOWN;
  }

  image = vgCreateImage(CVPI_COLOR_SPACE, width, height,VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();

  vgImageSubData(image, input_data, CVPI_PIXEL_BYTES*width, CVPI_COLOR_SPACE, 0, 0, width, height);
  cvpi_vg_error_check();
  thresholded = cvpi_channel_threshold(image, channel, bound1, bound2, fill, invert);
  cvpi_image_error_check(thresholded);

  vgDestroyImageSafe(image);
  cvpi_vg_error_check();
  size_t output_size = width*height*CVPI_PIXEL_BYTES;
  if(output_size <= 0) {
    fprintf(CVPI_LOG_FILE, "%s:%d:malloc size = 0\n", __func__, __LINE__);
    return_value = CVPI_FALSE;
    BADSTATE = 1;
    goto TAKEDOWN;
  }
  thresholded_data = malloc(output_size);
  heap_error_check(thresholded_data);

  vgGetImageSubData(thresholded, thresholded_data, width*CVPI_PIXEL_BYTES,
		    CVPI_COLOR_SPACE, 0, 0, width, height);
  cvpi_vg_error_check();
  
  vgDestroyImageSafe(thresholded);
  cvpi_vg_error_check();
  vgFlush();
  cvpi_vg_error_check();
  
  output_file = fopen(output_path, "w");
  heap_error_check(output_file);

  //cvpi_pbm_header_write(output_file, width, height);
  size_t written = fwrite(thresholded_data, output_size, 1, output_file);
  int flushed = fflush(output_file);
  if(written != output_size || flushed != 0) {
    if(written != output_size) {
      fprintf(CVPI_LOG_FILE, "%s: Error writing file: fwrite size discrepancy\nexpected:%d\nreturned:%d.\n",
	      __func__, output_size, written);
    } else {
      fprintf(CVPI_LOG_FILE, "%s: Error writing file: fflush\nerrno = %d\nreturned = %d\n",
	      __func__, errno, flushed);
    }
  }
  TAKEDOWN:
  freeSafe(thresholded_data);
  freeSafe(input_data);
  freeSafe(input_path);
  freeSafe(output_path);
  fcloseSafeTakedown(input_file);  
  fcloseSafeTakedown(output_file);

  vgDestroyImageSafe(image);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(thresholded);
  cvpi_vg_error_takedown();

  if(BADSTATE) {
    return_value = CVPI_FALSE;
  }

  return return_value;
#undef TAKEDOWN
}

CVPI_BOOL test_cvpi_channel_threshold(void) {
  const int width = 2048;
  const int height = 2048;

  return test_cvpi_channel_threshold_common("test_threshold_data", "test_cvpi_channel_threshold.yuv",
					    width, height, VG_BLUE, 0x64, 0xc8, 0, VG_FALSE);
}
CVPI_BOOL test_cvpi_channel_threshold_inv(void) {
  const int width = 2048;
  const int height = 2048;

  return test_cvpi_channel_threshold_common("test_threshold_data", "test_cvpi_channel_threshold_inv.yuv",
					    width, height, VG_BLUE, 0x64, 0xc8, 0, VG_TRUE);
}
CVPI_BOOL test_cvpi_channel_threshold_same(void) {
  const int width = 2048;
  const int height = 2048;

  return test_cvpi_channel_threshold_common("test_threshold_data", "test_cvpi_channel_threshold_same.yuv",
					    width, height, VG_BLUE, 0x64, 0x64, 0, VG_FALSE);
}
CVPI_BOOL test_cvpi_channel_threshold_inv_same(void) {
  const int width = 2048;
  const int height = 2048;

  return test_cvpi_channel_threshold_common("test_threshold_data", "test_cvpi_channel_threshold_inv_same.yuv",
					    width, height, VG_BLUE, 0x64, 0x64, 0, VG_TRUE);
}

static CVPI_BOOL test_cvpi_image_threshold_common(const char* input_file_name,
						  const char* output_file_name,
						  const int width, const int height, 
						  const VGubyte* bound1, const VGubyte* bound2, 
						  const VGubyte* fill, const CVPI_BOOL* invert, 
						  const CVPI_BOOL dependent) {
#define TAKEDOWN test_cvpi_image_threshold_common_takedown
  int BADSTATE = 0;

  CVPI_BOOL return_value = CVPI_TRUE;

  FILE* output_file = NULL;
  FILE* input_file = NULL;
  char* input_path = NULL;
  void* thresholded_data = NULL;
  void* input_data = NULL;
  char* output_path = NULL;
  VGImage image = VG_INVALID_HANDLE;
  VGImage thresholded = VG_INVALID_HANDLE;

  size_t input_path_size = sizeof(*input_file_name) * (strlen(IMAGES_CORRECT_DIR) + strlen(input_file_name));
  if(input_path_size <= 0) {
    fprintf(CVPI_LOG_FILE, "%s:%d:malloc size = 0\n", __func__, __LINE__);
    return_value = CVPI_FALSE;
    BADSTATE = 1;
    goto TAKEDOWN;
  }
  input_path = malloc(input_path_size);
  heap_error_check(input_path);
  
  int input_path_print = sprintf(input_path, "%s%s", IMAGES_CORRECT_DIR, input_file_name);
  if(input_path_print < 0) {
    fprintf(CVPI_LOG_FILE, "%s:%d:error writing file path: %d\n", __func__, __LINE__, input_path);
    BADSTATE = 1;
    goto TAKEDOWN;
  }
  input_file = fopen(input_path, "r");
  if(input_file == NULL) {
    fprintf(CVPI_LOG_FILE, "%s:%d: Error opening %s: errno = %d\n",
	    __func__, __LINE__, input_path, errno);
    BADSTATE = 1;
    goto TAKEDOWN;
  }

  seek_error_check(fseek(input_file, 0L, SEEK_END));
  unsigned long input_file_size = ftell(input_file);
  seek_error_check(input_file_size);
  seek_error_check(fseek(input_file, 0L, SEEK_SET));

  if(input_file_size <= 0) {
    fprintf(CVPI_LOG_FILE, "%s:%d:malloc size = 0\n", __func__, __LINE__);
    BADSTATE = 1;
    goto TAKEDOWN;
  }
  input_data = malloc(input_file_size);
  heap_error_check(input_data);

  if(input_file_size != fread(input_data, 1, input_file_size, input_file)) {
    fprintf(CVPI_LOG_FILE, "%s: Reading the input image from memory failed.\n",__func__);
    BADSTATE = 1;
    goto TAKEDOWN;
  }
  size_t output_path_size = sizeof(*output_file_name) * (strlen(IMAGES_TMP_DIR) + strlen(output_file_name));
  if(output_path_size <= 0) {
    fprintf(CVPI_LOG_FILE, "%s:%d:malloc size = 0\n", __func__, __LINE__);
    BADSTATE = 1;
    goto TAKEDOWN;
  }
  output_path = malloc(output_path_size);
  heap_error_check(output_path);
  
  int file_path_print = sprintf(output_path, "%s%s", IMAGES_TMP_DIR, output_file_name);
  if(file_path_print < 0) {
    fprintf(CVPI_LOG_FILE, "%s:%d:error writing file path: %d\n", __func__, __LINE__, file_path_print);
    BADSTATE = 1;
    goto TAKEDOWN;
  }
  image = vgCreateImage(CVPI_COLOR_SPACE, width, height,VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();

  vgImageSubData(image, input_data, CVPI_PIXEL_BYTES*width, CVPI_COLOR_SPACE, 0, 0, width, height);
  cvpi_vg_error_check();

  thresholded = cvpi_image_threshold(image, bound1, bound2, fill, invert, dependent);
  cvpi_image_error_check(thresholded);

  vgDestroyImageSafe(image);
  cvpi_vg_error_check();

  thresholded_data = malloc(height*width*CVPI_PIXEL_BYTES);
  heap_error_check(thresholded_data);
  
  vgGetImageSubData(thresholded, thresholded_data, width*CVPI_PIXEL_BYTES,
		    CVPI_COLOR_SPACE, 0, 0, width, height);
  cvpi_vg_error_check();

  vgDestroyImageSafe(thresholded);
  cvpi_vg_error_check();
  vgFlush();
  cvpi_vg_error_check();

  output_file = fopen(output_path, "w");
  heap_error_check(output_file);
  size_t output_size = width*height*CVPI_PIXEL_BYTES;
  size_t written = fwrite(thresholded_data, output_size, 1, output_file);
  int flushed = fflush(output_file);

  if(written != output_size || flushed != 0) {
    if(written != output_size) {
      fprintf(CVPI_LOG_FILE, "%s: Error writing file: fwrite size discrepancy\nexpected:%d\nreturned:%d.\n",
	      __func__, output_size, written);
    } else {
      fprintf(CVPI_LOG_FILE, "%s: Error writing file: fflush\nerrno = %d\nreturned = %d\n",
	      __func__, errno, flushed);
    }
  }

  TAKEDOWN:
  
  freeSafe(input_path);
  freeSafe(output_path);
  freeSafe(thresholded_data);
  freeSafe(input_data);
  fcloseSafeTakedown(input_file);  
  fcloseSafeTakedown(output_file);

  vgDestroyImageSafe(image);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(thresholded);
  cvpi_vg_error_takedown();

  if(BADSTATE) {
    return_value = CVPI_FALSE;
  }

  return return_value;
#undef TAKEDOWN
}

CVPI_BOOL test_cvpi_image_threshold(void) {
  const int width = 2048;
  const int height = 2048;
  const VGubyte lower_bound[4] = {0x64, 0x64, 0x64, 0x64};
  const VGubyte upper_bound[4] = {0xc8, 0xc8, 0xc8, 0xc8};
  const VGubyte fill[4] = {0,0,0,0};
  const CVPI_BOOL array[4] = {CVPI_FALSE, CVPI_FALSE, CVPI_FALSE, CVPI_FALSE};
  return test_cvpi_image_threshold_common("test_threshold_data", "test_cvpi_image_threshold.yuv",
					  width, height, lower_bound, upper_bound, fill, array, CVPI_FALSE);
}
CVPI_BOOL test_cvpi_image_threshold_inv(void) {
  const int width = 2048;
  const int height = 2048;
  const VGubyte lower_bound[4] = {0x64, 0x64, 0x64, 0x64};
  const VGubyte upper_bound[4] = {0xc8, 0xc8, 0xc8, 0xc8};
  const VGubyte fill[4] = {0,0,0,0};
  const CVPI_BOOL array[4] = {CVPI_TRUE, CVPI_TRUE, CVPI_TRUE, CVPI_TRUE};
  return test_cvpi_image_threshold_common("test_threshold_data", "test_cvpi_image_threshold_inv.yuv",
					  width, height, lower_bound, upper_bound, fill, array, CVPI_FALSE);
}
CVPI_BOOL test_cvpi_image_threshold_dependent(void) {
  const int width = 2048;
  const int height = 2048;
  const VGubyte lower_bound[4] = {0x64, 0x64, 0x64, 0x64};
  const VGubyte upper_bound[4] = {0xc8, 0xc8, 0xc8, 0xc8};
  const VGubyte fill[4] = {0,0,0,0};
  const CVPI_BOOL array[4] = {CVPI_FALSE, CVPI_FALSE, CVPI_FALSE, CVPI_FALSE};
  return test_cvpi_image_threshold_common("test_threshold_data", "test_cvpi_image_threshold_dependent.yuv",
					  width, height, lower_bound, upper_bound, fill, array, CVPI_TRUE);
}
CVPI_BOOL test_cvpi_image_threshold_inv_dependent(void) {
  const int width = 2048;
  const int height = 2048;
  const VGubyte lower_bound[4] = {0x64, 0x64, 0x64, 0x64};
  const VGubyte upper_bound[4] = {0xc8, 0xc8, 0xc8, 0xc8};
  const VGubyte fill[4] = {0,0,0,0};
  const CVPI_BOOL array[4] = {CVPI_TRUE, CVPI_TRUE, CVPI_TRUE, CVPI_TRUE};
  return test_cvpi_image_threshold_common("test_threshold_data", "test_cvpi_image_threshold_inv_dependent.yuv",
					  width, height, lower_bound, upper_bound, fill, array, CVPI_TRUE);
}

/* cvpi_image_threshold_sector */
static CVPI_BOOL test_cvpi_image_threshold_sector_common(const char* input_file_name,
							 const char* output_file_name,
							 const int width, const int height, 
							 const unsigned int sector_width, 
							 const unsigned int sector_height,
							 const VGubyte* fill, const CVPI_BOOL* invert,
							 const CVPI_BOOL dependent,
							 const VGImage (*statistic)(const VGImage image)) {
#define TAKEDOWN test_cvpi_image_threshold_sector_common_takedown
  int BADSTATE = 0;

  CVPI_BOOL return_value = CVPI_TRUE;

  FILE* output_file = NULL;
  FILE* input_file = NULL;
  char* input_path = NULL;
  void* input_data = NULL;
  char* output_path = NULL;
  void* thresholded_data = NULL;
  
  VGImage image = VG_INVALID_HANDLE;
  VGImage thresholded = VG_INVALID_HANDLE;

  
  if(width <= 0 || height <= 0 || sector_height <= 0 || sector_width <= 0
     || fill == NULL || invert == NULL || statistic == NULL || 
     input_file_name == NULL || output_file_name == NULL) {
    fprintf(stderr, "%s: invalid parameter\n", __func__);
    BADSTATE = 1;
    goto TAKEDOWN;
  }
  if(strlen(IMAGES_CORRECT_DIR) <= 0 || strlen(input_file_name) <= 0) {
    fprintf(stderr, "%s: invalid file name: %s\n", __func__, input_file_name);
    BADSTATE = 1;
    goto TAKEDOWN;
  }
  if(strlen(IMAGES_TMP_DIR) <= 0 || strlen(input_file_name) <= 0) {
    fprintf(stderr, "%s: invalid file name: %s\n", __func__, output_file_name);
    BADSTATE = 1;
    goto TAKEDOWN;
  }
  input_path = malloc(sizeof(*input_path) * (strlen(IMAGES_CORRECT_DIR) + strlen(input_file_name)));
  heap_error_check(input_path);
  sprintf(input_path, "%s%s", IMAGES_CORRECT_DIR, input_file_name);
  input_file = fopen(input_path, "r");
  heap_error_check(input_file);

  seek_error_check(fseek(input_file, 0L, SEEK_END));
  unsigned long input_file_size = ftell(input_file);
  seek_error_check(input_file_size);
  seek_error_check(fseek(input_file, 0L, SEEK_SET));
  
  if(input_file_size <= 0) {
    fprintf(stderr, "%s: Input file (%s) size is 0\n", __func__, input_file_name);
    BADSTATE = 1;
    goto TAKEDOWN;
  }
  input_data = malloc(input_file_size);
  heap_error_check(input_data);

  output_path = malloc(sizeof(*output_path) * (strlen(IMAGES_TMP_DIR) + strlen(output_file_name)));
  heap_error_check(output_path);

  sprintf(output_path, "%s%s", IMAGES_TMP_DIR, output_file_name);

  image = vgCreateImage(CVPI_COLOR_SPACE, width, height,VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();
  vgImageSubData(image, input_data, CVPI_PIXEL_BYTES*width, CVPI_COLOR_SPACE, 0, 0, width, height);
  cvpi_vg_error_check();
  thresholded = cvpi_image_threshold_sector(image, sector_width, sector_height, 
						    fill, invert, dependent, statistic);
  cvpi_image_error_check(thresholded);

  vgDestroyImageSafe(image);
  cvpi_vg_error_check();

  unsigned long fwrite_size = width*height*CVPI_PIXEL_BYTES;
  if(fwrite_size <= 0) {
    fprintf(CVPI_LOG_FILE, "%s:%d:malloc size = 0\n", __func__, __LINE__);
    BADSTATE = 1;
    goto TAKEDOWN;
  }
  thresholded_data = malloc(fwrite_size);
  heap_error_check(thresholded_data);
  
  vgGetImageSubData(thresholded, thresholded_data, width*CVPI_PIXEL_BYTES,
		    CVPI_COLOR_SPACE, 0, 0, width, height);
  cvpi_vg_error_check();

  output_file = fopen(output_path, "w");
  heap_error_check(output_file);
  
  size_t written = fwrite(thresholded_data, fwrite_size, 1, output_file);
  int flushed = fflush(output_file);

  if(written != fwrite_size || flushed != 0) {
    if(written != fwrite_size) {
      fprintf(CVPI_LOG_FILE, "%s: Error writing file: fwrite size discrepancy\nexpected:%d\nreturned:%d.\n",
	      __func__, fwrite_size, written);
    } else {
      fprintf(CVPI_LOG_FILE, "%s: Error writing file: fflush\nerrno = %d\nreturned = %d\n",
	      __func__, errno, flushed);
    }
  }

  TAKEDOWN:
  
  fcloseSafeTakedown(output_file);  
  fcloseSafeTakedown(input_file);
  cvpi_vg_error_takedown();
  vgFlush();
  cvpi_vg_error_takedown();
  freeSafe(input_data);
  freeSafe(input_path);
  freeSafe(output_path);
  freeSafe(thresholded_data);
  vgDestroyImageSafe(thresholded);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(image);
  cvpi_vg_error_takedown();

  if(BADSTATE) {
    return_value = CVPI_FALSE;
  }

  return return_value;
#undef TAKEDOWN
}
CVPI_BOOL test_cvpi_image_threshold_sector(void) {
  const int width = 2048;
  const int height = 2048;
  const VGubyte fill[4] = {0,0,0,0};
  const CVPI_BOOL array[4] = {CVPI_FALSE, CVPI_FALSE, CVPI_FALSE, CVPI_FALSE};
  const VGImage (*statistic)(const VGImage image);
  statistic = cvpi_image_mean_arithmetic;

  return test_cvpi_image_threshold_sector_common("test_threshold_data", "test_cvpi_image_threshold_sector.yuv",
						 width, height, 10, 10, fill, array, CVPI_FALSE, statistic);
}
CVPI_BOOL test_cvpi_image_threshold_sector_inv(void) {
  const int width = 2048;
  const int height = 2048;
  const VGubyte fill[4] = {0,0,0,0};
  const CVPI_BOOL array[4] = {CVPI_TRUE, CVPI_TRUE, CVPI_TRUE, CVPI_TRUE};
  const VGImage (*statistic)(const VGImage image);
  statistic = cvpi_image_mean_arithmetic;

  return test_cvpi_image_threshold_sector_common("test_threshold_data", "test_cvpi_image_threshold_sector.yuv",
						 width, height, 10, 10, fill, array, CVPI_FALSE, statistic);
}
CVPI_BOOL test_cvpi_image_threshold_sector_dependent(void) {
  const int width = 2048;
  const int height = 2048;
  const VGubyte fill[4] = {0,0,0,0};
  const CVPI_BOOL array[4] = {CVPI_FALSE, CVPI_FALSE, CVPI_FALSE, CVPI_FALSE};
  const VGImage (*statistic)(const VGImage image);
  statistic = cvpi_image_mean_arithmetic;

  return test_cvpi_image_threshold_sector_common("test_threshold_data", "test_cvpi_image_threshold_sector.yuv",
						 width, height, 10, 10, fill, array, CVPI_TRUE, statistic);
}
CVPI_BOOL test_cvpi_image_threshold_sector_inv_dependent(void) {
  const int width = 2048;
  const int height = 2048;
  const VGubyte fill[4] = {0,0,0,0};
  const CVPI_BOOL array[4] = {CVPI_TRUE, CVPI_TRUE, CVPI_TRUE, CVPI_TRUE};
  const VGImage (*statistic)(const VGImage image);
  statistic = cvpi_image_mean_arithmetic;

  return test_cvpi_image_threshold_sector_common("test_threshold_data", "test_cvpi_image_threshold_sector.yuv",
						 width, height, 10, 10, fill, array, CVPI_TRUE, statistic);
}

/* cvpi_image_mask_channel */
static CVPI_BOOL test_cvpi_image_mask_channel_common(char* output_file_name,
						     VGImageChannel maskChannel, CVPI_BOOL add) {
#define TAKEDOWN test_cvpi_image_mask_channel_common_takedown
  int BADSTATE = 0;

  CVPI_BOOL return_value = CVPI_TRUE;

  char* output_path = NULL;

  FILE* output_file = NULL;
  
  VGImage input_image = VG_INVALID_HANDLE;
  VGImage output_image = VG_INVALID_HANDLE;
  
  cvpi_pixel image_data[2];
  image_data[0].channel[cvpi_pixel_red] = 3;
  image_data[0].channel[cvpi_pixel_green] = 2;
  image_data[0].channel[cvpi_pixel_blue] = 1;
  image_data[0].channel[cvpi_pixel_alpha] = 0;
  image_data[1].channel[cvpi_pixel_red] = 3;
  image_data[1].channel[cvpi_pixel_green] = 2;
  image_data[1].channel[cvpi_pixel_blue] = 1;
  image_data[1].channel[cvpi_pixel_alpha] = ~0;

  input_image = vgCreateImage(CVPI_COLOR_SPACE, 2, 1, VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();
  
  vgImageSubData(input_image, image_data, 2*CVPI_PIXEL_BYTES, CVPI_COLOR_SPACE, 0,0, 2, 1);
  cvpi_vg_error_check();

  output_image = cvpi_image_mask_channel(input_image, maskChannel, add);
  cvpi_image_error_check(output_image);
  
  vgDestroyImageSafe(input_image);
  cvpi_vg_error_check();
  vgGetImageSubData(output_image, image_data, 2*CVPI_PIXEL_BYTES, CVPI_COLOR_SPACE, 0, 0, 2, 1);
  cvpi_vg_error_check();

  size_t output_path_length = sizeof(*output_file_name) * (strlen(IMAGES_TMP_DIR) + strlen(output_file_name));
  output_path = malloc(output_path_length);
  heap_error_check(output_path);

  int file_path_print = sprintf(output_path, "%s%s", IMAGES_TMP_DIR, output_file_name);
  if(file_path_print < 0) {
    fprintf(CVPI_LOG_FILE, "%s:%d:error writing file path: %d\n", __func__, __LINE__, file_path_print);
    BADSTATE = 1;
    goto TAKEDOWN;
  }
  output_file = fopen(output_path, "w");
  heap_error_check(output_file);

  int fwrite_size = 2*CVPI_PIXEL_BYTES;
  size_t written = fwrite(image_data, fwrite_size, 1, output_file);
  int flushed = fflush(output_file);
  if(written != fwrite_size || flushed != 0) {
    if(written != fwrite_size) {
      fprintf(CVPI_LOG_FILE, "%s: Error writing file: fwrite size discrepancy\nexpected:%d\nreturned:%d.\n",
	      __func__, fwrite_size, written);
    } else {
      fprintf(CVPI_LOG_FILE, "%s: Error writing file: fflush\nerrno = %d\nreturned = %d\n",
	      __func__, errno, flushed);
    }
  }
  TAKEDOWN:
  
  fcloseSafeTakedown(output_file);
  freeSafe(output_path);
  vgDestroyImageSafe(input_image);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(output_image);
  cvpi_vg_error_takedown();
  if(BADSTATE) {
    return_value = CVPI_FALSE;
  }
  
  return return_value;  
#undef TAKEDOWN
}
CVPI_BOOL test_cvpi_image_mask_channel_add(void) {
  return test_cvpi_image_mask_channel_common("test_cvpi_image_mask_channel_add", VG_ALPHA, CVPI_TRUE);
}
CVPI_BOOL test_cvpi_image_mask_channel_subtract(void) {
  return test_cvpi_image_mask_channel_common("test_cvpi_image_mask_channel_subtract", VG_ALPHA, CVPI_FALSE);
}

CVPI_BOOL test_cvpi_image_rgba_to_binary(void) {
#define TAKEDOWN test_cvpi_image_rgba_to_binary_takedown
  int BADSTATE = 0;

  CVPI_BOOL return_value = CVPI_TRUE;
  
  const int width = 1280;
  const int height = 960;

  FILE* input_file = NULL;
  FILE* output_file = NULL;

  void* input_data = NULL;
  void* bw_data = NULL;

  VGImage image = VG_INVALID_HANDLE;
  VGImage bw = VG_INVALID_HANDLE;
  
  char* input_path = IMAGES_CORRECT_DIR "mark1_black_yuva.yuv";
  input_file = fopen(input_path, "r");
  heap_error_check(input_file);

  seek_error_check(fseek(input_file, 0L, SEEK_END));
  unsigned long input_file_size = ftell(input_file);
  seek_error_check(input_file_size);
  seek_error_check(fseek(input_file, 0L, SEEK_SET));
  input_data = malloc(input_file_size);
  heap_error_check(input_data);

  char* output_path = IMAGES_TMP_DIR "mark1_black_bw.pbm";
  image = vgCreateImage(CVPI_COLOR_SPACE, width, height,VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();

  vgImageSubData(image, input_data, CVPI_PIXEL_BYTES*width, CVPI_COLOR_SPACE, 0, 0, width, height);
  cvpi_vg_error_check();

  bw = cvpi_image_rgba2bw(image, VG_BLUE, CVPI_TRUE);
  cvpi_image_error_check(bw);
  vgDestroyImageSafe(image);
  cvpi_vg_error_check();
  
  bw_data = malloc(height*width/8);
  heap_error_check(bw_data);
  
  vgGetImageSubData(bw, bw_data, width/8, VG_BW_1, 0, 0, width, height);
  cvpi_vg_error_check();
  vgFinish();
  cvpi_vg_error_check();
  vgDestroyImageSafe(bw);
  cvpi_vg_error_check();
  vgFlush();
  cvpi_vg_error_check();

  output_file = fopen(output_path, "w");
  heap_error_check(output_file);
  
  cvpi_pbm_header_write(output_file, width, height);

  size_t fwrite_size = width*height/8;
  size_t written = fwrite(bw_data, fwrite_size, 1, output_file);
  int flushed = fflush(output_file);

  if(written != fwrite_size || flushed != 0) {
    if(written != fwrite_size) {
      fprintf(CVPI_LOG_FILE, "%s: Error writing file: fwrite size discrepancy\nexpected:%d\nreturned:%d.\n",
	      __func__, fwrite_size, written);
    } else {
      fprintf(CVPI_LOG_FILE, "%s: Error writing file: fflush\nerrno = %d\nreturned = %d\n",
	      __func__, errno, flushed);
    }
  }

  TAKEDOWN:
  
  freeSafe(bw_data);
  freeSafe(input_data);
  fcloseSafeTakedown(input_file);  
  fcloseSafeTakedown(output_file);

  vgDestroyImageSafe(image);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(bw);
  cvpi_vg_error_takedown();
  
  if(BADSTATE) {
    return_value = CVPI_FALSE;
  }

  return return_value;
#undef TAKEDOWN
}

static CVPI_BOOL test_cvpi_image_logical_common(
VGImage(*logical_function)(VGImage image1, VGImage image2, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true), 
cvpi_pixel* correct, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true) {
#define TAKEDOWN test_cvpi_image_logical_common_takedown
  int BADSTATE = 0;

  CVPI_BOOL return_value = CVPI_FALSE;

  VGImage image1 = VG_INVALID_HANDLE;
  VGImage image2 = VG_INVALID_HANDLE;
  VGImage output = VG_INVALID_HANDLE;

  int width = 2;
  int height = 2;

  cvpi_pixel input1[4];
  /* 0101 */  
  input1[0].all = 0;
  input1[1].all = ~0;
  input1[2].all = 0;
  input1[3].all = ~0;

  cvpi_pixel input2[4];
  /* 0011 */
  input2[0].all = 0;
  input2[1].all = 0;
  input2[2].all = ~0;
  input2[3].all = ~0;

  image1 = vgCreateImage(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();
  image2 = vgCreateImage(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();
  vgImageSubData(image1, input1, CVPI_PIXEL_BYTES*width, CVPI_COLOR_SPACE, 0, 0, width, height);
  cvpi_vg_error_check();
  vgImageSubData(image2, input2, CVPI_PIXEL_BYTES*width, CVPI_COLOR_SPACE, 0, 0, width, height);
  cvpi_vg_error_check();
  vgFinish();
  cvpi_vg_error_check();
  
  output = logical_function(image1, image2, t_c, f_c, nonzero_true);
  cvpi_image_error_check(output);  
  vgGetImageSubData(output, input1, width*CVPI_PIXEL_BYTES, CVPI_COLOR_SPACE, 0, 0, width, height);
  cvpi_vg_error_check();
  vgFinish();
  cvpi_vg_error_check();
  vgDestroyImageSafe(image1);
  cvpi_vg_error_check();
  vgDestroyImageSafe(image2);
  cvpi_vg_error_check();
  vgDestroyImageSafe(output);
  cvpi_vg_error_check();
  vgFlush();
  cvpi_vg_error_check();
  
  int cmp = memcmp(input1, correct, width*height*CVPI_PIXEL_BYTES);

  if(cmp == 0) {
    return_value = CVPI_TRUE;
  } else if(cmp > 0) {
    fprintf(stderr, "%s: added_data is greater than the correct data.\n", __func__);
  } else {
    fprintf(stderr, "%s: added_data is less than the correct data.\n", __func__);
  }
  fprintf(stderr, "%08x %08x %08x %08x\n", input1[0].all, input1[1].all, input1[2].all, input1[3].all);

  TAKEDOWN:

  vgDestroyImageSafe(image1);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(image2);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(output);
  cvpi_vg_error_takedown();
  
  if(BADSTATE) {
    return_value = CVPI_FALSE;
  }
  
  return return_value;
#undef TAKEDOWN
}

/* cvpi_image_logical_and */
CVPI_BOOL test_cvpi_image_logical_and(void) {
  VGImage(*logical_function)(VGImage image1, VGImage image2, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);
  logical_function = cvpi_image_logical_and;

  VGubyte t_c = 3;
  VGubyte f_c = 2;
  /* 
| 0 | 0 | 2 |
| 1 | 0 | 2 |
| 0 | 1 | 2 |
| 1 | 1 | 3 |
   */
  cvpi_pixel correct[4];
  correct[0].all = 0x02020202;
  correct[1].all = 0x02020202;
  correct[2].all = 0x02020202;
  correct[3].all = 0x03030303;

  return test_cvpi_image_logical_common(logical_function, correct, t_c, f_c, CVPI_TRUE);
}

CVPI_BOOL test_cvpi_image_logical_and_NOT(void) {
  VGImage(*logical_function)(VGImage image1, VGImage image2, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);
  logical_function = cvpi_image_logical_and;

  VGubyte t_c = 3;
  VGubyte f_c = 2;

  /* 
| 0 | 0 | 3 |
| 1 | 0 | 2 |
| 0 | 1 | 2 |
| 1 | 1 | 2 |
   */
  cvpi_pixel correct[4];
  correct[0].all = 0x03030303;
  correct[1].all = 0x02020202;
  correct[2].all = 0x02020202;
  correct[3].all = 0x02020202;

  return test_cvpi_image_logical_common(logical_function, correct, t_c, f_c, CVPI_FALSE);
}

/* cvpi_image_logical_nand */
CVPI_BOOL test_cvpi_image_logical_nand(void) {
  VGImage(*logical_function)(VGImage image1, VGImage image2, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);
  logical_function = cvpi_image_logical_nand;

  VGubyte t_c = 3;
  VGubyte f_c = 2;

  /* 
| 0 | 0 | 3 |
| 1 | 0 | 3 |
| 0 | 1 | 3 |
| 1 | 1 | 2 |
   */

  cvpi_pixel correct[4];
  correct[0].all = 0x03030303;
  correct[1].all = 0x03030303;
  correct[2].all = 0x03030303;
  correct[3].all = 0x02020202;

  return test_cvpi_image_logical_common(logical_function, correct, t_c, f_c, CVPI_TRUE);
}

CVPI_BOOL test_cvpi_image_logical_nand_NOT(void) {
  VGImage(*logical_function)(VGImage image1, VGImage image2, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);
  logical_function = cvpi_image_logical_nand;

  VGubyte t_c = 3;
  VGubyte f_c = 2;

  /* 
| 0 | 0 | 2 |
| 1 | 0 | 3 |
| 0 | 1 | 3 |
| 1 | 1 | 3 |
   */

  cvpi_pixel correct[4];
  correct[0].all = 0x02020202;
  correct[1].all = 0x03030303;
  correct[2].all = 0x03030303;
  correct[3].all = 0x03030303;

  return test_cvpi_image_logical_common(logical_function, correct, t_c, f_c, CVPI_FALSE);
}

/* cvpi_image_logical_or */
CVPI_BOOL test_cvpi_image_logical_or(void) {
  VGImage(*logical_function)(VGImage image1, VGImage image2, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);
  logical_function = cvpi_image_logical_or;

  VGubyte t_c = 3;
  VGubyte f_c = 2;

  /* 
| 0 | 0 | 2 |
| 1 | 0 | 3 |
| 0 | 1 | 3 |
| 1 | 1 | 3 |
   */

  cvpi_pixel correct[4];
  correct[0].all = 0x02020202;
  correct[1].all = 0x03030303;
  correct[2].all = 0x03030303;
  correct[3].all = 0x03030303;

  return test_cvpi_image_logical_common(logical_function, correct, t_c, f_c, CVPI_TRUE);
}

CVPI_BOOL test_cvpi_image_logical_or_NOT(void) {
  VGImage(*logical_function)(VGImage image1, VGImage image2, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);
  logical_function = cvpi_image_logical_or;

  VGubyte t_c = 3;
  VGubyte f_c = 2;

  /* 
| 0 | 0 | 3 |
| 1 | 0 | 3 |
| 0 | 1 | 3 |
| 1 | 1 | 2 |
   */

  cvpi_pixel correct[4];
  correct[0].all = 0x03030303;
  correct[1].all = 0x03030303;
  correct[2].all = 0x03030303;
  correct[3].all = 0x02020202;

  return test_cvpi_image_logical_common(logical_function, correct, t_c, f_c, CVPI_FALSE);
}

/* cvpi_image_logical_nor */
CVPI_BOOL test_cvpi_image_logical_nor(void) {
  VGImage(*logical_function)(VGImage image1, VGImage image2, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);
  logical_function = cvpi_image_logical_nor;

  VGubyte t_c = 3;
  VGubyte f_c = 2;

  /* 
| 0 | 0 | 3 |
| 1 | 0 | 2 |
| 0 | 1 | 2 |
| 1 | 1 | 2 |
   */

  cvpi_pixel correct[4];
  correct[0].all = 0x03030303;
  correct[1].all = 0x02020202;
  correct[2].all = 0x02020202;
  correct[3].all = 0x02020202;

  return test_cvpi_image_logical_common(logical_function, correct, t_c, f_c, CVPI_TRUE);
}

CVPI_BOOL test_cvpi_image_logical_nor_NOT(void) {
  VGImage(*logical_function)(VGImage image1, VGImage image2, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);
  logical_function = cvpi_image_logical_nor;

  VGubyte t_c = 3;
  VGubyte f_c = 2;

  /* 
| 0 | 0 | 2 |
| 1 | 0 | 2 |
| 0 | 1 | 2 |
| 1 | 1 | 3 |
   */

  cvpi_pixel correct[4];
  correct[0].all = 0x02020202;
  correct[1].all = 0x02020202;
  correct[2].all = 0x02020202;
  correct[3].all = 0x03030303;

  return test_cvpi_image_logical_common(logical_function, correct, t_c, f_c, CVPI_FALSE);
}

/* cvpi_image_logical_xor */
CVPI_BOOL test_cvpi_image_logical_xor(void) {
  VGImage(*logical_function)(VGImage image1, VGImage image2, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);
  logical_function = cvpi_image_logical_xor;

  VGubyte t_c = 3;
  VGubyte f_c = 2;

  /* 
| 0 | 0 | 2 |
| 1 | 0 | 3 |
| 0 | 1 | 3 |
| 1 | 1 | 2 |
   */

  cvpi_pixel correct[4];
  correct[0].all = 0x02020202;
  correct[1].all = 0x03030303;
  correct[2].all = 0x03030303;
  correct[3].all = 0x02020202;

  return test_cvpi_image_logical_common(logical_function, correct, t_c, f_c, CVPI_TRUE);
}
CVPI_BOOL test_cvpi_image_logical_xor_NOT(void) {
  VGImage(*logical_function)(VGImage image1, VGImage image2, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);
  logical_function = cvpi_image_logical_xor;

  VGubyte t_c = 3;
  VGubyte f_c = 2;

  /* 
| 0 | 0 | 2 |
| 1 | 0 | 3 |
| 0 | 1 | 3 |
| 1 | 1 | 2 |
   */

  cvpi_pixel correct[4];
  correct[0].all = 0x02020202;
  correct[1].all = 0x03030303;
  correct[2].all = 0x03030303;
  correct[3].all = 0x02020202;

  return test_cvpi_image_logical_common(logical_function, correct, t_c, f_c, CVPI_FALSE);
}
/* cvpi_image_logical_xnor */
CVPI_BOOL test_cvpi_image_logical_xnor(void) {
  VGImage(*logical_function)(VGImage image1, VGImage image2, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);
  logical_function = cvpi_image_logical_xnor;

  VGubyte t_c = 3;
  VGubyte f_c = 2;

  /* 
| 0 | 0 | 3 |
| 1 | 0 | 2 |
| 0 | 1 | 2 |
| 1 | 1 | 3 |
   */

  cvpi_pixel correct[4];
  correct[0].all = 0x03030303;
  correct[1].all = 0x02020202;
  correct[2].all = 0x02020202;
  correct[3].all = 0x03030303;

  return test_cvpi_image_logical_common(logical_function, correct, t_c, f_c, CVPI_TRUE);
}

CVPI_BOOL test_cvpi_image_logical_xnor_NOT(void) {
  VGImage(*logical_function)(VGImage image1, VGImage image2, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);
  logical_function = cvpi_image_logical_xnor;

  VGubyte t_c = 3;
  VGubyte f_c = 2;

  /* 
| 0 | 0 | 3 |
| 1 | 0 | 2 |
| 0 | 1 | 2 |
| 1 | 1 | 3 |
   */

  cvpi_pixel correct[4];
  correct[0].all = 0x03030303;
  correct[1].all = 0x02020202;
  correct[2].all = 0x02020202;
  correct[3].all = 0x03030303;

  return test_cvpi_image_logical_common(logical_function, correct, t_c, f_c, CVPI_FALSE);
}
/* cvpi_image_logical_complement */
CVPI_BOOL test_cvpi_image_logical_complement(void) {
  VGImage(*logical_function)(VGImage image1, VGImage image2, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);
  logical_function = cvpi_image_logical_complement;

  VGubyte t_c = 3;
  VGubyte f_c = 2;

  /* 
| 0 | 0 | 2 |
| 1 | 0 | 3 |
| 0 | 1 | 2 |
| 1 | 1 | 2 |
   */

  cvpi_pixel correct[4];
  correct[0].all = 0x02020202;
  correct[1].all = 0x03030303;
  correct[2].all = 0x02020202;
  correct[3].all = 0x02020202;

  return test_cvpi_image_logical_common(logical_function, correct, t_c, f_c, CVPI_TRUE);
}

CVPI_BOOL test_cvpi_image_logical_complement_NOT(void) {
  VGImage(*logical_function)(VGImage image1, VGImage image2, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);
  logical_function = cvpi_image_logical_complement;

  VGubyte t_c = 3;
  VGubyte f_c = 2;

  /* 
| 0 | 0 | 2 |
| 1 | 0 | 2 |
| 0 | 1 | 3 |
| 1 | 1 | 2 |
   */

  cvpi_pixel correct[4];
  correct[0].all = 0x02020202;
  correct[1].all = 0x02020202;
  correct[2].all = 0x03030303;
  correct[3].all = 0x02020202;

  return test_cvpi_image_logical_common(logical_function, correct, t_c, f_c, CVPI_FALSE);
}

/* cvpi_image_logical_complement_inv */
CVPI_BOOL test_cvpi_image_logical_complement_inv(void) {
  VGImage(*logical_function)(VGImage image1, VGImage image2, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);
  logical_function = cvpi_image_logical_complement_inv;

  VGubyte t_c = 3;
  VGubyte f_c = 2;

  /* 
| 0 | 0 | 3 |
| 1 | 0 | 2 |
| 0 | 1 | 3 |
| 1 | 1 | 3 |
   */

  cvpi_pixel correct[4];
  correct[0].all = 0x03030303;
  correct[1].all = 0x02020202;
  correct[2].all = 0x03030303;
  correct[3].all = 0x03030303;

  return test_cvpi_image_logical_common(logical_function, correct, t_c, f_c, CVPI_TRUE);
}

CVPI_BOOL test_cvpi_image_logical_complement_inv_NOT(void) {
  VGImage(*logical_function)(VGImage image1, VGImage image2, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);
  logical_function = cvpi_image_logical_complement_inv;

  VGubyte t_c = 3;
  VGubyte f_c = 2;

  /* 
| 0 | 0 | 3 |
| 1 | 0 | 3 |
| 0 | 1 | 2 |
| 1 | 1 | 3 |
   */

  cvpi_pixel correct[4];
  correct[0].all = 0x03030303;
  correct[1].all = 0x03030303;
  correct[2].all = 0x02020202;
  correct[3].all = 0x03030303;

  return test_cvpi_image_logical_common(logical_function, correct, t_c, f_c, CVPI_FALSE);
}

/* 6x4 binary image 
| 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| 0 | 0 | 0 | 1 | 1 | 0 | 0 | 0 |
| 0 | 0 | 0 | 1 | 1 | 0 | 0 | 0 |
| 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
 */

const uint32_t test_cvpi_image_morphology_image_1 = 0b00000000000110000001100000000000;
const uint32_t test_cvpi_image_morphology_image_2 = 0b11111111111001111110011111111111;

/* 6x4 binary image 
| 0 | 0 | 1 | 1 | 1 | 1 | 0 | 0 |
| 0 | 0 | 1 | 1 | 1 | 1 | 0 | 0 |
| 0 | 0 | 1 | 1 | 1 | 1 | 0 | 0 |
| 0 | 0 | 1 | 1 | 1 | 1 | 0 | 0 |
 */

const uint32_t test_cvpi_image_morphology_image_3 = 0b00111100001111000011110000111100;
const uint32_t test_cvpi_image_morphology_image_4 = 0b11000011110000111100001111000011;

static void test_cvpi_image_morphology_print(uint32_t image) {
  int i = 0;
  int j = 0;

  for(; i < 4; ++i) {
    for(j = 0; j < 8; ++j) {
      if(image & 1) {
        printf("1");
      } else {
        printf("0");
      }

      image >>= 1;
    }
    printf("\n");
  }
  printf("\n\n");
}

static CVPI_BOOL test_cvpi_image_morphology_common(uint32_t data, VGImage (*morph)(VGImage image, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true), VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true) {
#define TAKEDOWN test_cvpi_image_morphology_common_takedown
  int BADSTATE = 0;

  CVPI_BOOL return_value = CVPI_TRUE;
  
  int width = 8;
  int height = 4;

  VGImage output_image = VG_INVALID_HANDLE;
  VGImage input_image = VG_INVALID_HANDLE;
  VGImage intermediate_image = VG_INVALID_HANDLE;
  
  output_image = vgCreateImage(VG_BW_1, width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();
  input_image = vgCreateImage(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();
  vgImageSubData(input_image, &data, 1, VG_BW_1, 0, 0, width, height);
  cvpi_vg_error_check();
  vgFinish();
  cvpi_vg_error_check();
  
  intermediate_image = morph(input_image, t_c, f_c, nonzero_true);
  cvpi_image_error_check(intermediate_image);

  vgCopyImage(output_image, 0, 0, intermediate_image, 0, 0, width, height, VG_FALSE);
  cvpi_vg_error_check();
  
  uint32_t result;

  vgGetImageSubData(output_image, &result, 1, VG_BW_1, 0, 0, width, height);
  cvpi_vg_error_check();
  vgFinish();
  cvpi_vg_error_check();
  
  test_cvpi_image_morphology_print(data);
  test_cvpi_image_morphology_print(result);
  TAKEDOWN:
  vgDestroyImageSafe(output_image);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(input_image);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(intermediate_image);
  cvpi_vg_error_takedown();
  
  if(BADSTATE) {
    return_value = CVPI_FALSE;
  }
  return return_value;
#undef TAKEDOWN
}

/* cvpi_image_dilate */
CVPI_BOOL test_cvpi_image_dilate(void) {
  VGImage (*morph)(VGImage image, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);
  morph = cvpi_image_dilate;
  return test_cvpi_image_morphology_common(test_cvpi_image_morphology_image_1, morph, 255, 0, CVPI_TRUE);
}
CVPI_BOOL test_cvpi_image_dilate_NOT(void) {
  VGImage (*morph)(VGImage image, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);
  morph = cvpi_image_dilate;
  return test_cvpi_image_morphology_common(test_cvpi_image_morphology_image_2, morph, 0, 255, CVPI_FALSE);
}
/* cvpi_image_erode */
CVPI_BOOL test_cvpi_image_erode(void) {
  VGImage (*morph)(VGImage image, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);
  morph = cvpi_image_erode;
  return test_cvpi_image_morphology_common(test_cvpi_image_morphology_image_3, morph, 255, 0, CVPI_TRUE);
}
CVPI_BOOL test_cvpi_image_erode_NOT(void) {
  VGImage (*morph)(VGImage image, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);
  morph = cvpi_image_erode;
  return test_cvpi_image_morphology_common(test_cvpi_image_morphology_image_4, morph, 0, 255, CVPI_FALSE);
}
/* cvpi_image_hit_miss */
CVPI_BOOL test_cvpi_image_hit_miss(void) {
  VGImage (*morph)(VGImage image, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);
  morph = cvpi_image_hit_miss;
  return test_cvpi_image_morphology_common(test_cvpi_image_morphology_image_4, morph, 255, 0, CVPI_TRUE);
}
CVPI_BOOL test_cvpi_image_hit_miss_NOT(void) {
  VGImage (*morph)(VGImage image, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);
  morph = cvpi_image_hit_miss;
  return test_cvpi_image_morphology_common(test_cvpi_image_morphology_image_4, morph, 0, 255, CVPI_FALSE);
}
/* cvpi_image_thin */
CVPI_BOOL test_cvpi_image_thin(void) {
  VGImage (*morph)(VGImage image, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);
  morph = cvpi_image_thin;
  return test_cvpi_image_morphology_common(test_cvpi_image_morphology_image_3, morph, 255, 0, CVPI_TRUE);
}
CVPI_BOOL test_cvpi_image_thin_NOT(void) {
  VGImage (*morph)(VGImage image, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);
  morph = cvpi_image_thin;
  return test_cvpi_image_morphology_common(test_cvpi_image_morphology_image_4, morph, 0, 255, CVPI_FALSE);
}

/* cvpi_image_thicken */
CVPI_BOOL test_cvpi_image_thicken(void) {
  VGImage (*morph)(VGImage image, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);
  morph = cvpi_image_thicken;
  return test_cvpi_image_morphology_common(test_cvpi_image_morphology_image_1, morph, 255, 0, CVPI_TRUE);
}
CVPI_BOOL test_cvpi_image_thicken_NOT(void) {
  VGImage (*morph)(VGImage image, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);
  morph = cvpi_image_thicken;
  return test_cvpi_image_morphology_common(test_cvpi_image_morphology_image_2, morph, 0, 255, CVPI_FALSE);
}

enum test_cvpi_channel_max_min_function {max_function, min_function, max_min_function};

static CVPI_BOOL test_cvpi_channel_max_min_common(enum test_cvpi_channel_max_min_function func, 
						  VGImageChannel channel) {
#define TAKEDOWN test_cvpi_channel_max_min_common_takedown
  int BADSTATE = 0;

  CVPI_BOOL return_value = CVPI_TRUE;

  VGImage input_image = VG_INVALID_HANDLE;
  
  int width = 3;
  int height = 1;

  cvpi_pixel data[3];
  /* 
| data | R | G | B | A |
|    0 | 0 | 1 | 2 | 6 |
|    1 | 7 | 2 | 3 | 4 |
|    2 | 2 | 4 | 5 | 3 |
|------+---+---+---+---|
|  max | 7 | 4 | 5 | 6 |
|  min | 0 | 1 | 2 | 3 |
  */
  data[0].channel[cvpi_pixel_red] = 0;
  data[0].channel[cvpi_pixel_green] = 1;
  data[0].channel[cvpi_pixel_blue] = 2;
  data[0].channel[cvpi_pixel_alpha] = 6;

  data[1].channel[cvpi_pixel_red] = 7;
  data[1].channel[cvpi_pixel_green] = 2;
  data[1].channel[cvpi_pixel_blue] = 3;
  data[1].channel[cvpi_pixel_alpha] = 4;

  data[2].channel[cvpi_pixel_red] = 2;
  data[2].channel[cvpi_pixel_green] = 4;
  data[2].channel[cvpi_pixel_blue] = 5;
  data[2].channel[cvpi_pixel_alpha] = 3;

  input_image = vgCreateImage(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();

  vgImageSubData(input_image, data, width*CVPI_PIXEL_BYTES, CVPI_COLOR_SPACE, 0, 0, width, height);
  cvpi_vg_error_check();
  
  vgFinish();
  cvpi_vg_error_check();
  
  VGubyte max = 99;
  VGubyte min = 99;
  cvpi_max_min max_min;
  if(func == max_function) {
    max = cvpi_channel_max(input_image, channel);
  } else if(func == min_function) {
    min = cvpi_channel_min(input_image, channel);
  } else {
    max_min = cvpi_channel_max_min(input_image, channel);
    if(max_min.all == ~0) {
      cvpi_log_1("max_min returned ~0", __func__, __LINE__);
      BADSTATE = 1;
      goto TAKEDOWN;
    } else {
      max = max_min.max_min[0];
      min = max_min.max_min[1];
    }
  }
  vgFinish();

  CVPI_BOOL return_value_max, return_value_min;

  switch(channel) {
  case VG_RED:
    return_value_max = 7 == max ? CVPI_TRUE : CVPI_FALSE;
    return_value_min = 0 == min ? CVPI_TRUE : CVPI_FALSE;
    break;
  case VG_GREEN:
    return_value_max = 4 == max ? CVPI_TRUE : CVPI_FALSE;
    return_value_min = 1 == min ? CVPI_TRUE : CVPI_FALSE;
    break;
  case VG_BLUE:
    return_value_max = 5 == max ? CVPI_TRUE : CVPI_FALSE;
    return_value_min = 2 == min ? CVPI_TRUE : CVPI_FALSE;
    break;
  case VG_ALPHA:
    return_value_max = 6 == max ? CVPI_TRUE : CVPI_FALSE;
    return_value_min = 3 == min ? CVPI_TRUE : CVPI_FALSE;
    break;
  default:
    fprintf(stderr, "%s: Bad channel %d\n", __func__, channel);
    BADSTATE = 1;
    goto TAKEDOWN;
  }

  if(func == max_function) {
    return_value = return_value_max;
  } else if(func == min_function) {
    return_value = return_value_min;
  } else {
    return_value = CVPI_TRUE_TEST(return_value_max) && CVPI_TRUE_TEST(return_value_min);
  }
  TAKEDOWN:
  vgDestroyImageSafe(input_image);
  cvpi_vg_error_takedown();

  if(BADSTATE) {
    return_value = CVPI_FALSE;
  }
  
  return return_value;
#undef TAKEDOWN
}

CVPI_BOOL test_cvpi_channel_max_alpha(void) {
  return test_cvpi_channel_max_min_common(max_function, VG_ALPHA);
}
CVPI_BOOL test_cvpi_channel_max_red(void) {
  return test_cvpi_channel_max_min_common(max_function, VG_RED);
}
CVPI_BOOL test_cvpi_channel_max_green(void) {
  return test_cvpi_channel_max_min_common(max_function, VG_GREEN);
}
CVPI_BOOL test_cvpi_channel_max_blue(void) {
  return test_cvpi_channel_max_min_common(max_function, VG_BLUE);
}

CVPI_BOOL test_cvpi_channel_min_alpha(void) {
  return test_cvpi_channel_max_min_common(min_function, VG_ALPHA);
}
CVPI_BOOL test_cvpi_channel_min_red(void) {
  return test_cvpi_channel_max_min_common(min_function, VG_RED);
}
CVPI_BOOL test_cvpi_channel_min_green(void) {
  return test_cvpi_channel_max_min_common(min_function, VG_GREEN);
}
CVPI_BOOL test_cvpi_channel_min_blue(void) {
  return test_cvpi_channel_max_min_common(min_function, VG_BLUE);
}

CVPI_BOOL test_cvpi_channel_max_min_alpha(void) {
  return test_cvpi_channel_max_min_common(max_min_function, VG_ALPHA);
}
CVPI_BOOL test_cvpi_channel_max_min_red(void) {
  return test_cvpi_channel_max_min_common(max_min_function, VG_RED);
}
CVPI_BOOL test_cvpi_channel_max_min_green(void) {
  return test_cvpi_channel_max_min_common(max_min_function, VG_GREEN);
}
CVPI_BOOL test_cvpi_channel_max_min_blue(void) {
  return test_cvpi_channel_max_min_common(max_min_function, VG_BLUE);
}

static CVPI_BOOL test_cvpi_channel_histogram_common(VGImageChannel channel) {
#define TAKEDOWN test_cvpi_channel_histogram_common_takedown
  int BADSTATE = 0; 

  CVPI_BOOL return_value = CVPI_TRUE;

  VGImage input_image = VG_INVALID_HANDLE;

  unsigned int* histogram = NULL;

  FILE* output = NULL;

  cvpi_pixel data[256];
  int i = 0;
  for(; i < 256; ++i) {
    data[i].channel[cvpi_pixel_red] = channel == VG_RED ? i : 0;
    data[i].channel[cvpi_pixel_green] = channel == VG_GREEN ? i : 0;
    data[i].channel[cvpi_pixel_blue] = channel == VG_BLUE ? i : 0;
    data[i].channel[cvpi_pixel_alpha] = channel == VG_ALPHA ? i : 0;
  }

  int width = 128;
  int height = 2;

  input_image = vgCreateImage(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();
  vgImageSubData(input_image, &data, CVPI_PIXEL_BYTES * width, CVPI_COLOR_SPACE, 0, 0, width, height);
  cvpi_vg_error_check();
  
  histogram = cvpi_channel_histogram(input_image, channel);
  heap_error_check(histogram);

  char* error_file_name;
  int name_length = strlen(IMAGES_TMP_DIR) + strlen("test_cvpi_channel_histogram_n");

  switch(channel) {
  case VG_RED:
    error_file_name = IMAGES_TMP_DIR "test_cvpi_channel_histogram_r";
    break;
  case VG_GREEN:
    error_file_name = IMAGES_TMP_DIR "test_cvpi_channel_histogram_g";
    break;
  case VG_BLUE:
    error_file_name = IMAGES_TMP_DIR "test_cvpi_channel_histogram_b";
    break;
  case VG_ALPHA:
    error_file_name = IMAGES_TMP_DIR "test_cvpi_channel_histogram_a";
    break;
  default:
    fprintf(stderr, "%s: Bad channel %d\n", __func__, channel);
    BADSTATE = 1;
    goto TAKEDOWN;
  }

  for(i = 0; i < 256; ++i) {
    if(histogram[i] != 1) {
      return_value = CVPI_FALSE;
    }
  }

  output = fopen(error_file_name, "w");
  for(i = 0; i < 256; ++i) {
    fprintf(output, "%d ", histogram[i]);
  }

  TAKEDOWN:
  fcloseSafeTakedown(output);
  freeSafe(histogram);
  vgDestroyImageSafe(input_image);
  cvpi_vg_error_takedown();

  if(BADSTATE) {
    return_value = CVPI_FALSE;
  }

  return return_value;
#undef TAKEDOWN
}

CVPI_BOOL test_cvpi_channel_histogram_alpha(void) {
  return test_cvpi_channel_histogram_common(VG_ALPHA);
}
CVPI_BOOL test_cvpi_channel_histogram_red(void) {
  return test_cvpi_channel_histogram_common(VG_RED);
}
CVPI_BOOL test_cvpi_channel_histogram_green(void) {
  return test_cvpi_channel_histogram_common(VG_GREEN);
}
CVPI_BOOL test_cvpi_channel_histogram_blue(void) {
  return test_cvpi_channel_histogram_common(VG_BLUE);
}

CVPI_BOOL test_cvpi_color_channels_histogram(void) {
#define test_cvpi_color_channels_histogram_takedown
  int BADSTATE = 0;

  VGImage input_image = VG_INVALID_HANDLE;

  unsigned int* histogram = NULL;
  FILE* output = NULL;
  
  CVPI_BOOL return_value = CVPI_TRUE;
  cvpi_pixel data[256];
  int i = 0;
  for(; i < 256; ++i) {
    data[i].channel[cvpi_pixel_red] = 1;
    data[i].channel[cvpi_pixel_green] = 2;
    data[i].channel[cvpi_pixel_blue] = 3;
    data[i].channel[cvpi_pixel_alpha] = 0;
  }

  int width = 128;
  int height = 2;

  input_image = vgCreateImage(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();
  vgImageSubData(input_image, &data, CVPI_PIXEL_BYTES * width, CVPI_COLOR_SPACE, 0, 0, width, height);
  cvpi_vg_error_check();
  
  histogram = cvpi_color_channels_histogram(input_image);
  heap_error_check(histogram);

  char* error_file_name = IMAGES_TMP_DIR "test_cvpi_color_channels_histogram";

  for(i = 0; i < 768; ++i) {
    if(histogram[i] != 0) {
      if((i == 1 && histogram[i] == 256) ||
	 (i == 258 && histogram[i] == 256) ||
	 (i == 515 && histogram[i] == 256)) {
	continue;
      }

      return_value = CVPI_FALSE;
    }
  }

  output = fopen(error_file_name, "w");
  for(i = 0; i < 256; ++i) {
    fprintf(output, "%d ", histogram[i]);
  }
  fprintf(output, "\n\n");
  for(; i < 512; ++i) {
    fprintf(output, "%d ", histogram[i]);
  }
  fprintf(output, "\n\n");
  for(; i < 768; ++i) {
    fprintf(output, "%d ", histogram[i]);
  }

  TAKEDOWN:
  fcloseSafeTakedown(output);

  freeSafe(histogram);
  vgDestroyImageSafe(input_image);
  cvpi_vg_error_takedown();

  if(BADSTATE) {
    return_value = CVPI_FALSE;
  }
  
  return return_value;
#undef TAKEDOWN
}

CVPI_BOOL test_cvpi_image_histogram(void) {
#define TAKEDOWN test_cvpi_image_histogram_takedown
  int BADSTATE = 0;

  VGImage input_image = VG_INVALID_HANDLE;
  unsigned int* histogram = NULL;
  FILE* output = NULL;

  CVPI_BOOL return_value = CVPI_TRUE;  
  cvpi_pixel data[256];
  int i = 0;
  for(; i < 256; ++i) {
    data[i].channel[cvpi_pixel_red] = 1;
    data[i].channel[cvpi_pixel_green] = 2;
    data[i].channel[cvpi_pixel_blue] = 3;
    data[i].channel[cvpi_pixel_alpha] = 0;
  }

  int width = 128;
  int height = 2;

  input_image = vgCreateImage(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();
  vgImageSubData(input_image, &data, CVPI_PIXEL_BYTES * width, CVPI_COLOR_SPACE, 0, 0, width, height);
  cvpi_vg_error_check();
  
  histogram = cvpi_image_histogram(input_image);
  heap_error_check(histogram);

  char* error_file_name = IMAGES_TMP_DIR "test_cvpi_image_histogram";

  for(i = 0; i < 1024; ++i) {
    if(histogram[i] != 0) {
      if((i == 1 && histogram[i] == 256) ||
	 (i == 258 && histogram[i] == 256) ||
	 (i == 515 && histogram[i] == 256) ||
	 (i == 768 && histogram[i] == 256)) {
	continue;
      }

      return_value = CVPI_FALSE;
    }
  }

  output = fopen(error_file_name, "w");
  for(i = 0; i < 256; ++i) {
    fprintf(output, "%d ", histogram[i]);
  }
  fprintf(output, "\n\n");
  for(; i < 512; ++i) {
    fprintf(output, "%d ", histogram[i]);
  }
  fprintf(output, "\n\n");
  for(; i < 768; ++i) {
    fprintf(output, "%d ", histogram[i]);
  }
  fprintf(output, "\n\n");
  for(; i < 1024; ++i) {
    fprintf(output, "%d ", histogram[i]);
  }
  TAKEDOWN:
  fcloseSafeTakedown(output);

  freeSafe(histogram);
  vgDestroyImageSafe(input_image);
  cvpi_vg_error_takedown();

  if(BADSTATE) {
    return_value = CVPI_FALSE;
  }
  return return_value;
#undef TAKEDOWN
}

static CVPI_BOOL test_cvpi_channel_cumulative_distribution_common(VGImageChannel channel) {
#define test_cvpi_channel_cumulative_distribution_common_takedown
  int BADSTATE = 0;

  CVPI_BOOL return_value = CVPI_TRUE;
  VGImage input_image = VG_INVALID_HANDLE;
  unsigned int* histogram = NULL;
  VGubyte* cd = NULL;
  FILE* output = NULL;
    
  cvpi_pixel data[256];
  int i = 0;
  for(; i < 256; ++i) {
    data[i].channel[cvpi_pixel_red] = channel == VG_RED ? i : 0;
    data[i].channel[cvpi_pixel_green] = channel == VG_GREEN ? i : 0;
    data[i].channel[cvpi_pixel_blue] = channel == VG_BLUE ? i : 0;
    data[i].channel[cvpi_pixel_alpha] = channel == VG_ALPHA ? i : 0;
  }

  int width = 128;
  int height = 2;

  input_image = vgCreateImage(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();
  vgImageSubData(input_image, &data, CVPI_PIXEL_BYTES * width, CVPI_COLOR_SPACE, 0, 0, width, height);
  cvpi_vg_error_check();
  
  histogram = cvpi_channel_histogram(input_image, channel);
  heap_error_check(histogram);
  cd = cvpi_channel_cumulative_distribution(histogram, width, height);
  heap_error_check(cd);
  
  char* error_file_name;
  int name_length = strlen(IMAGES_TMP_DIR) + strlen("test_cvpi_channel_histogram_n");

  switch(channel) {
  case VG_RED:
    error_file_name = IMAGES_TMP_DIR "test_cvpi_channel_cumulative_distribution_r";
    break;
  case VG_GREEN:
    error_file_name = IMAGES_TMP_DIR "test_cvpi_channel_cumulative_distribution_g";
    break;
  case VG_BLUE:
    error_file_name = IMAGES_TMP_DIR "test_cvpi_channel_cumulative_distribution_b";
    break;
  case VG_ALPHA:
    error_file_name = IMAGES_TMP_DIR "test_cvpi_channel_cumulative_distribution_a";
    break;
  default:
    fprintf(stderr, "%s: Bad channel %d\n", __func__, channel);
    BADSTATE = 1;
    goto TAKEDOWN;
  }

  /* for(i = 0; i < 256; ++i) { */
  /*   if(histogram[i] != 1) { */
  /*     return_value = CVPI_FALSE; */
  /*   } */
  /* } */

  output = fopen(error_file_name, "w");
  heap_error_check(output);
  for(i = 0; i < 256; ++i) {
    fprintf(output, "%f ", cd[i]);
  }
  TAKEDOWN:
  fcloseSafeTakedown(output);
  
  vgDestroyImageSafe(input_image);
  cvpi_vg_error_takedown();
    
  freeSafe(histogram);
  freeSafe(cd);

  if(BADSTATE) {
    return_value = CVPI_FALSE;
  }
  return return_value;
#undef TAKEDOWN
}

CVPI_BOOL test_cvpi_channel_cumulative_distribution_alpha(void) {
  return test_cvpi_channel_cumulative_distribution_common(VG_ALPHA);
}
CVPI_BOOL test_cvpi_channel_cumulative_distribution_red(void) {
  return test_cvpi_channel_cumulative_distribution_common(VG_RED);
}
CVPI_BOOL test_cvpi_channel_cumulative_distribution_green(void) {
  return test_cvpi_channel_cumulative_distribution_common(VG_GREEN);
}
CVPI_BOOL test_cvpi_channel_cumulative_distribution_blue(void) {
  return test_cvpi_channel_cumulative_distribution_common(VG_BLUE);
}

CVPI_BOOL test_cvpi_color_channels_cumulative_distribution(void) {
#define TAKEDOWN test_cvpi_color_channels_cumulative_distribution_takedown
  int BADSTATE = 0;

  VGImage input_image = VG_INVALID_HANDLE;
  unsigned int* histogram = NULL;
  VGubyte* cd = NULL;
  FILE* output = NULL;

  CVPI_BOOL return_value = CVPI_TRUE;
  cvpi_pixel data[256];
  int i = 0;
  for(; i < 256; ++i) {
    data[i].channel[cvpi_pixel_red] = 1;
    data[i].channel[cvpi_pixel_green] = 2;
    data[i].channel[cvpi_pixel_blue] = 3;
    data[i].channel[cvpi_pixel_alpha] = 0;
  }

  int width = 128;
  int height = 2;

  input_image = vgCreateImage(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();
  vgImageSubData(input_image, &data, CVPI_PIXEL_BYTES * width, CVPI_COLOR_SPACE, 0, 0, width, height);
  cvpi_vg_error_check();
  
  histogram = cvpi_color_channels_histogram(input_image);
  heap_error_check(histogram);
  cd = cvpi_color_channels_cumulative_distribution(histogram, width, height);
  heap_error_check(cd);

  char* error_file_name = IMAGES_TMP_DIR "test_cvpi_color_channels_cumulative_distribution";

  /* for(i = 0; i < 768; ++i) { */
  /*   if(histogram[i] != 0) { */
  /*     if((i == 1 && histogram[i] == 256) || */
  /* 	 (i == 258 && histogram[i] == 256) || */
  /* 	 (i == 515 && histogram[i] == 256)) { */
  /* 	continue; */
  /*     } */

  /*     return_value = CVPI_FALSE; */
  /*   } */
  /* } */

  output = fopen(error_file_name, "w");
  heap_error_check(output);
  for(i = 0; i < 256; ++i) {
    fprintf(output, "%f ", cd[i]);
  }
  fprintf(output, "\n\n");
  for(; i < 512; ++i) {
    fprintf(output, "%f ", cd[i]);
  }
  fprintf(output, "\n\n");
  for(; i < 768; ++i) {
    fprintf(output, "%f ", cd[i]);
  }
  TAKEDOWN:
  fcloseSafeTakedown(output);
  vgDestroyImageSafe(input_image);
  cvpi_vg_error_takedown();
  freeSafe(histogram);
  freeSafe(cd);

  if(BADSTATE) {
    return_value = CVPI_FALSE;
  }
  return return_value;
#undef TAKEDOWN
}

CVPI_BOOL test_cvpi_image_cumulative_distribution(void) {
#define TAKEDOWN test_cvpi_image_cumulative_distribution_takedown

  int BADSTATE = 0;

  VGImage input_image = VG_INVALID_HANDLE;
  unsigned int* histogram = NULL;
  VGubyte* cd = NULL;
  FILE* output = NULL;

  CVPI_BOOL return_value = CVPI_TRUE;

  cvpi_pixel data[256];
  int i = 0;
  for(; i < 256; ++i) {
    data[i].channel[cvpi_pixel_red] = 1;
    data[i].channel[cvpi_pixel_green] = 2;
    data[i].channel[cvpi_pixel_blue] = 3;
    data[i].channel[cvpi_pixel_alpha] = 0;
  }

  int width = 128;
  int height = 2;

  input_image = vgCreateImage(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();
  vgImageSubData(input_image, &data, CVPI_PIXEL_BYTES * width, CVPI_COLOR_SPACE, 0, 0, width, height);
  cvpi_vg_error_check();
  
  histogram = cvpi_image_histogram(input_image);
  heap_error_check(histogram);
  cd = cvpi_image_cumulative_distribution(histogram, width, height);
  heap_error_check(cd);
  char* error_file_name = IMAGES_TMP_DIR "test_cvpi_image_cumulative_distribution";

  /* for(i = 0; i < 1024; ++i) { */
  /*   if(histogram[i] != 0) { */
  /*     if((i == 1 && histogram[i] == 256) || */
  /* 	 (i == 258 && histogram[i] == 256) || */
  /* 	 (i == 515 && histogram[i] == 256) || */
  /* 	 (i == 768 && histogram[i] == 256)) { */
  /* 	continue; */
  /*     } */

  /*     return_value = CVPI_FALSE; */
  /*   } */
  /* } */

  output = fopen(error_file_name, "w");
  heap_error_check(output);
  for(i = 0; i < 256; ++i) {
    fprintf(output, "%f ", cd[i]);
  }
  fprintf(output, "\n\n");
  for(; i < 512; ++i) {
    fprintf(output, "%f ", cd[i]);
  }
  fprintf(output, "\n\n");
  for(; i < 768; ++i) {
    fprintf(output, "%f ", cd[i]);
  }
  fprintf(output, "\n\n");
  for(; i < 1024; ++i) {
    fprintf(output, "%f ", cd[i]);
  }

  TAKEDOWN:
  fcloseSafeTakedown(output);
  vgDestroyImageSafe(input_image);
  cvpi_vg_error_takedown();
  freeSafe(histogram);
  freeSafe(cd);
  if(BADSTATE) {
    return_value = CVPI_FALSE;
  }
  return return_value;
#undef TAKEDOWN
}

/* cvpi_image_coordinate_table */
CVPI_BOOL test_cvpi_image_coordinate_table_zero(void) {
#define TAKEDOWN test_cvpi_image_coordinate_table_zero_takedown
  int BADSTATE = 0;

  cvpi_pixel* data = NULL;
  VGImage image = VG_INVALID_HANDLE;
  cvpi_coordinate_table* table = NULL;

  CVPI_BOOL return_value = CVPI_FALSE;
    
  int width = 4;
  int height = 1;
  data = calloc(4, sizeof(*data));
  heap_error_check(data);
  image = vgCreateImage(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();
  vgImageSubData(image, data, CVPI_PIXEL_BYTES * width, CVPI_COLOR_SPACE, 0, 0, width, height);  
  cvpi_vg_error_check();
  
  table = cvpi_image_coordinate_table(image,VG_RED|VG_GREEN|VG_BLUE|VG_ALPHA, CVPI_FALSE);
  heap_error_check(table);

  if(table->length == 0) {
    return_value = CVPI_TRUE;
  } else {
    fprintf(stderr, "%s: incorrect length: %l\n", __func__, table->length);
  }
  TAKEDOWN:
  vgDestroyImageSafe(image);
  cvpi_vg_error_takedown();

  freeSafe(data);
  if(table != NULL) {
    cvpi_image_coordinate_table_destroy(table);
  }
  if(BADSTATE) {
    return_value = CVPI_FALSE;
  }
  return return_value;
#undef TAKEDOWN
}

CVPI_BOOL test_cvpi_image_coordinate_table_some(void) {
#define TAKEDOWN test_cvpi_image_coordinate_table_some_takedown
  int BADSTATE = 0;

  cvpi_pixel* data = NULL;
  VGImage image = VG_INVALID_HANDLE;
  cvpi_coordinate_table* table = NULL;

  CVPI_BOOL return_value = CVPI_FALSE;
    
  int width = 4;
  int height = 1;
  data = calloc(4, sizeof(*data));
  heap_error_check(data);
  data[0].channel[cvpi_pixel_blue]= 1;
  data[2].channel[cvpi_pixel_red] = 1;
  data[3].channel[cvpi_pixel_alpha] = 1; /* should be ignored */

  image = vgCreateImage(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();
  vgImageSubData(image, data, CVPI_PIXEL_BYTES * width, CVPI_COLOR_SPACE, 0, 0, width, height);  
  cvpi_vg_error_check();
  
  table = cvpi_image_coordinate_table(image,VG_RED|VG_GREEN|VG_BLUE, CVPI_FALSE);
  heap_error_check(table);

  if(table->length == 2 && 
     table->coordinates[0].xy[0] == 0 && table->coordinates[0].xy[1] == 0 &&
     table->coordinates[1].xy[0] == 2 && table->coordinates[1].xy[1] == 0) {
    return_value = CVPI_TRUE;
  } else {
    fprintf(stderr, "%s: length %l, %d %d %d %d %d %d %d %d\n", __func__, table->length, 
	    table->coordinates[0].xy[0], table->coordinates[0].xy[1],
	    table->coordinates[1].xy[0], table->coordinates[1].xy[1],
	    table->coordinates[2].xy[0], table->coordinates[2].xy[1],
	    table->coordinates[3].xy[0], table->coordinates[3].xy[1]);
  }
  TAKEDOWN:
  vgDestroyImageSafe(image);
  cvpi_vg_error_takedown();

  freeSafe(data);
  if(table != NULL) {
    cvpi_image_coordinate_table_destroy(table);
  }
  if(BADSTATE) {
    return_value = CVPI_FALSE;
  }
  return return_value;
#undef TAKEDOWN
}

CVPI_BOOL test_cvpi_image_coordinate_table_all(void) {
#define TAKEDOWN test_cvpi_image_coordinate_table_all_takedown
  int BADSTATE = 0;
  CVPI_BOOL return_value = CVPI_FALSE;

  cvpi_pixel* data = NULL;
  VGImage image = VG_INVALID_HANDLE;
  cvpi_coordinate_table* table = NULL;
  
  int width = 2;
  int height = 2;
  data = calloc(4, sizeof(*data));
  heap_error_check(data);
  data[0].channel[cvpi_pixel_red] = 1;
  data[1].channel[cvpi_pixel_green] = 1;
  data[2].channel[cvpi_pixel_blue] = 1;
  data[3].channel[cvpi_pixel_alpha] = 1;

  image = vgCreateImage(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();
  vgImageSubData(image, data, CVPI_PIXEL_BYTES * width, CVPI_COLOR_SPACE, 0, 0, width, height);  
  cvpi_vg_error_check();
  
  table = cvpi_image_coordinate_table(image,VG_RED|VG_GREEN|VG_BLUE|VG_ALPHA, CVPI_FALSE);
  heap_error_check(table);
  
  if(table->length == 4 &&
     table->coordinates[0].xy[0] == 0 && table->coordinates[0].xy[1] == 0 &&
     table->coordinates[1].xy[0] == 1 && table->coordinates[1].xy[1] == 0 &&
     table->coordinates[2].xy[0] == 0 && table->coordinates[2].xy[1] == 1 &&
     table->coordinates[3].xy[0] == 1 && table->coordinates[3].xy[1] == 1) {
    return_value = CVPI_TRUE;
  } else {
    fprintf(stderr, "%s: length: %l, %d %d %d %d %d %d %d %d\n", __func__, table->length,
	    table->coordinates[0].xy[0], table->coordinates[0].xy[1],
	    table->coordinates[1].xy[0], table->coordinates[1].xy[1],
	    table->coordinates[2].xy[0], table->coordinates[2].xy[1],
	    table->coordinates[3].xy[0], table->coordinates[3].xy[1]);
  }
  TAKEDOWN:
  vgDestroyImageSafe(image);
  cvpi_vg_error_takedown();

  freeSafe(data);
  if(table != NULL) {
    cvpi_image_coordinate_table_destroy(table);
  }
  if(BADSTATE) {
    return_value = CVPI_FALSE;
  }
  return return_value;
#undef TAKEDOWN
}

/* filters */
CVPI_BOOL test_filter_common(const VGshort* filter_x, const VGshort* filter_y, int f_width, int f_height,
			     const char* output_image_path) {
#define TAKEDOWN test_filter_common_takedown
  int BADSTATE = 0;

  CVPI_BOOL return_value = CVPI_TRUE;

  FILE* input_file = NULL;
  FILE* output_file = NULL;
  void* data = NULL;

  VGImage input_image = VG_INVALID_HANDLE;
  VGImage morphed_image_x = VG_INVALID_HANDLE;
  VGImage morphed_image_y = VG_INVALID_HANDLE;
  VGImage magnitude = VG_INVALID_HANDLE;

  const int width = 1280;
  const int height = 960;

  char* input_image_path = IMAGES_CORRECT_DIR "mark1_black_yuva.yuv";

  input_file = fopen(input_image_path, "r");
  heap_error_check(input_file);
  output_file = fopen(output_image_path, "w");
  heap_error_check(output_file);

  seek_error_check(fseek(input_file, 0L, SEEK_END));
  int input_size = ftell(input_file);		/* input file size */
  seek_error_check(input_size);
  seek_error_check(fseek(input_file, 0L, SEEK_SET));

  data = malloc(input_size);
  heap_error_check(data);

  if(input_size != fread(data, 1, input_size, input_file)) {
    fprintf(CVPI_LOG_FILE, "%s: Reading the input image from memory failed.\n",__func__);
    BADSTATE = 1;
    goto TAKEDOWN;
  }

  input_image = vgCreateImage(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();
  morphed_image_x = vgCreateImage(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();
  morphed_image_y = vgCreateImage(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();

  vgImageSubData(input_image, data, width*CVPI_PIXEL_BYTES, CVPI_COLOR_SPACE, 0, 0, width, height);
  cvpi_vg_error_check();
  
  vgConvolve(morphed_image_x, input_image, f_width, f_height, 0, 0, filter_x, 1, 0, VG_TILE_FILL);
  cvpi_vg_error_check();
  vgConvolve(morphed_image_y, input_image, f_width, f_height, 0, 0, filter_y, 1, 0, VG_TILE_FILL);
  cvpi_vg_error_check();
  vgFinish();
  cvpi_vg_error_check();
  magnitude = cvpi_image_magnitude(morphed_image_x, morphed_image_y, cvpi_round);
  cvpi_image_error_check(magnitude);
  
  vgGetImageSubData(magnitude, data, width*CVPI_PIXEL_BYTES, CVPI_COLOR_SPACE, 0, 0, width, height);
  cvpi_vg_error_check();
  vgFinish();
  cvpi_vg_error_check();
  
  cvpi_bmp_header_alloc_write(output_file, width, height, 100, cvpi_bmp_sARGB_8888);
  size_t written = fwrite(data, 1, input_size, output_file);
  int flushed = fflush(output_file);

  if(written != input_size || flushed != 0) {
    if(written != input_size) {
      fprintf(CVPI_LOG_FILE, "%s: Error writing file: fwrite size discrepancy\nexpected:%d\nreturned:%d.\n",
	      __func__, input_size, written);
    } else {
      fprintf(CVPI_LOG_FILE, "%s: Error writing file: fflush\nerrno = %d\nreturned = %d\n",
	      __func__, errno, flushed);
    }
  }

  TAKEDOWN:
  fcloseSafeTakedown(output_file);
  fcloseSafeTakedown(input_file);
  freeSafe(data);
  vgDestroyImageSafe(magnitude);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(morphed_image_y);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(morphed_image_x);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(input_image);
  cvpi_vg_error_takedown();
  if(BADSTATE) {
    return_value = CVPI_FALSE;
  }
  return CVPI_TRUE;
#undef TAKEDOWN
}

CVPI_BOOL test_cvpi_sobel(void) {
  char* output_image_path = IMAGES_TMP_DIR "filter_sobel.bmp";
  return test_filter_common(cvpi_filter_sobel_x, cvpi_filter_sobel_y, 3, 3, output_image_path);
}
CVPI_BOOL test_cvpi_scharr(void) {
  char* output_image_path = IMAGES_TMP_DIR "filter_scharr.bmp";
  return test_filter_common(cvpi_filter_scharr_x, cvpi_filter_scharr_y, 3, 3, output_image_path);
}
CVPI_BOOL test_cvpi_prewitt(void) {
  char* output_image_path = IMAGES_TMP_DIR "filter_prewitt.bmp";
  return test_filter_common(cvpi_filter_prewitt_x, cvpi_filter_prewitt_y, 3, 3, output_image_path);
}
CVPI_BOOL test_cvpi_roberts_cross(void) {
  char* output_image_path = IMAGES_TMP_DIR "filter_roberts_cross.bmp";
  return test_filter_common(cvpi_filter_roberts_cross_x, cvpi_filter_roberts_cross_y, 2, 2, output_image_path);
}

CVPI_BOOL test_cvpi_invert_colors(void) {
#define TAKEDOWN test_cvpi_invert_colors_takedown
  int BADSTATE = 0;
  CVPI_BOOL return_value = CVPI_FALSE;

  FILE* output_file_lt = NULL;
  FILE* output_file = NULL;
  FILE* input_file = NULL;

  void* data = NULL;
  void* data2 = NULL;

  VGImage input_image = VG_INVALID_HANDLE;
  VGImage inv_image = VG_INVALID_HANDLE;
  VGImage inv_image_lt = VG_INVALID_HANDLE;
  
  const int width = 1280;
  const int height = 960;

  char* input_image_path = IMAGES_CORRECT_DIR "mark1_black_yuva.yuv";
  char* output_image_path = IMAGES_TMP_DIR "mark1_black_yuva_inv.bmp";
  char* output_image_path_lt = IMAGES_TMP_DIR "mark1_black_yuva_inv_lt.bmp";

  input_file = fopen(input_image_path, "r");
  heap_error_check(input_file);
  output_file = fopen(output_image_path, "w");
  heap_error_check(output_file);
  output_file_lt = fopen(output_image_path_lt, "w");
  heap_error_check(output_file_lt);

  seek_error_check(fseek(input_file, 0L, SEEK_END));
  int input_size = ftell(input_file);		/* input file size */
  seek_error_check(input_size);
  seek_error_check(fseek(input_file, 0L, SEEK_SET));

  data = malloc(input_size);
  heap_error_check(data);
  data2 = malloc(input_size);
  heap_error_check(data2);

  if(input_size != fread(data, 1, input_size, input_file)) {
    fprintf(CVPI_LOG_FILE, "%s: Reading the input image from memory failed.\n",__func__);
    BADSTATE = 1;
    goto TAKEDOWN;
  }

  input_image = vgCreateImage(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();
  inv_image = vgCreateImage(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();
  inv_image_lt = vgCreateImage(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();
  
  vgImageSubData(input_image, data, width*CVPI_PIXEL_BYTES, CVPI_COLOR_SPACE, 0, 0, width, height);
  cvpi_vg_error_check();
  
  vgColorMatrix(inv_image, input_image, cvpi_invert_colors);
  cvpi_vg_error_check();
  vgFinish();
  cvpi_vg_error_check();
  
  vgLookup(inv_image_lt, input_image, cvpi_inversion_array, cvpi_inversion_array, cvpi_inversion_array, cvpi_identity_array, VG_FALSE, VG_FALSE);
  cvpi_vg_error_check();
  vgFinish();
  cvpi_vg_error_check();

  vgGetImageSubData(inv_image, data, width*CVPI_PIXEL_BYTES, CVPI_COLOR_SPACE, 0, 0, width, height);
  cvpi_vg_error_check();
  vgGetImageSubData(inv_image_lt, data2, width*CVPI_PIXEL_BYTES, CVPI_COLOR_SPACE, 0, 0, width, height);
  cvpi_vg_error_check();
  vgFinish();
  cvpi_vg_error_check();

  int cmp = memcmp(data, data2, width*height*CVPI_PIXEL_BYTES);

  if(cmp == 0) {
    return_value = CVPI_TRUE;
  } else if(cmp > 0) {
    fprintf(stderr, "%s: added_data is greater than the correct data.\n", __func__);
  } else {
    fprintf(stderr, "%s: added_data is less than the correct data.\n", __func__);
  }

  cvpi_bmp_header_alloc_write(output_file, width, height, 100, cvpi_bmp_sARGB_8888);
  size_t written = fwrite(data, 1, input_size, output_file);
  int flushed = fflush(output_file);

  if(written != input_size || flushed != 0) {
    if(written != input_size) {
      fprintf(CVPI_LOG_FILE, "%s: Error writing file: fwrite size discrepancy\nexpected:%d\nreturned:%d.\n",
	      __func__, input_size, written);
    } else {
      fprintf(CVPI_LOG_FILE, "%s: Error writing file: fflush\nerrno = %d\nreturned = %d\n",
	      __func__, errno, flushed);
    }
  }

  cvpi_bmp_header_alloc_write(output_file_lt, width, height, 100, cvpi_bmp_sARGB_8888);
  written = fwrite(data2, 1, input_size, output_file_lt);
  flushed = fflush(output_file_lt);

  if(written != input_size || flushed != 0) {
    if(written != input_size) {
      fprintf(CVPI_LOG_FILE, "%s: Error writing file: fwrite size discrepancy\nexpected:%d\nreturned:%d.\n",
	      __func__, input_size, written);
    } else {
      fprintf(CVPI_LOG_FILE, "%s: Error writing file: fflush\nerrno = %d\nreturned = %d\n",
	      __func__, errno, flushed);
    }
  }

  TAKEDOWN:
  fcloseSafeTakedown(output_file);
  fcloseSafeTakedown(output_file_lt);
  fcloseSafeTakedown(input_file);
  freeSafe(data);
  freeSafe(data2);
  
  vgDestroyImageSafe(inv_image);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(inv_image_lt);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(input_image);
  cvpi_vg_error_takedown();

  if(BADSTATE) {
    return_value = CVPI_FALSE;
  }

  return return_value;
#undef TAKEDOWN
}
CVPI_BOOL test_cvpi_pixel_average(void) {
#define TAKEDOWN test_cvpi_pixel_average_takedown
  int BADSTATE = 0;
  int width = 10;
  int height = 10;

  VGImage correct = VG_INVALID_HANDLE;
  VGImage input = VG_INVALID_HANDLE;
  VGImage output = VG_INVALID_HANDLE;

  void* correct_output = NULL;
  void* test_output = NULL;

  CVPI_BOOL return_value = CVPI_FALSE;
  
  correct = vgCreateImagePainted(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED, 
					 7, 7, 7, 7);
  cvpi_vg_error_check();
  input = vgCreateImagePainted(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED,
				       1, 3, 8, 16);
  cvpi_vg_error_check();
  output = vgCreateImage(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();
  vgColorMatrix(output, input,
		cvpi_pixel_average);
  cvpi_vg_error_check();
  vgFinish();
  cvpi_vg_error_check();
  
  correct_output = malloc(CVPI_PIXEL_BYTES * width * height);
  heap_error_check(correct_output);
  test_output = malloc(CVPI_PIXEL_BYTES * width * height);
  heap_error_check(test_output);
  
  vgGetImageSubData(output, test_output, width*CVPI_PIXEL_BYTES, CVPI_COLOR_SPACE, 0, 0, width, height);
  cvpi_vg_error_check();
  vgGetImageSubData(correct, correct_output, width*CVPI_PIXEL_BYTES,
		    CVPI_COLOR_SPACE, 0, 0, width, height);
  cvpi_vg_error_check();

  int cmp = memcmp(test_output, correct_output, width*height*CVPI_PIXEL_BYTES);
  if(cmp == 0) {
    return_value = CVPI_TRUE;
  } else if(cmp > 0) {
    fprintf(stderr, "%s: added_data is greater than the correct data.\n", __func__);
  } else {
    fprintf(stderr, "%s: added_data is less than the correct data.\n", __func__);
  }

  TAKEDOWN:
  freeSafe(correct_output);
  freeSafe(test_output);

  vgDestroyImageSafe(correct);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(input);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(output);
  cvpi_vg_error_takedown();
  
  if(BADSTATE) {
    return_value = CVPI_FALSE;
  }
  return return_value;
#undef TAKEDOWN
}
CVPI_BOOL test_cvpi_pixel_color_average(void) {
#define TAKEDOWN test_cvpi_pixel_color_average_takedown
  int BADSTATE = 0;
  int width = 10;
  int height = 10;

  VGImage correct = VG_INVALID_HANDLE;
  VGImage input = VG_INVALID_HANDLE;
  VGImage output= VG_INVALID_HANDLE;
  void* correct_output = NULL;
  void* test_output = NULL;

  CVPI_BOOL return_value = CVPI_FALSE;
  
  correct = vgCreateImagePainted(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED, 
					 3, 3, 3, 255);
  cvpi_vg_error_check();
  input = vgCreateImagePainted(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED,
				       1, 3, 5, 255);
  cvpi_vg_error_check();
  output = vgCreateImage(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();
  vgColorMatrix(output, input,
		cvpi_pixel_color_average);
  cvpi_vg_error_check();
  vgFinish();
  cvpi_vg_error_check();
  correct_output = malloc(CVPI_PIXEL_BYTES * width * height);
  heap_error_check(correct_output);
  test_output = malloc(CVPI_PIXEL_BYTES * width * height);
  heap_error_check(test_output);

  vgGetImageSubData(output, test_output, width*CVPI_PIXEL_BYTES, CVPI_COLOR_SPACE, 0, 0, width, height);
  cvpi_vg_error_check();
  vgGetImageSubData(correct, correct_output, width*CVPI_PIXEL_BYTES,
		    CVPI_COLOR_SPACE, 0, 0, width, height);
  cvpi_vg_error_check();

  int cmp = memcmp(test_output, correct_output, width*height*CVPI_PIXEL_BYTES);

  if(cmp == 0) {
    return_value = CVPI_TRUE;
  } else if(cmp > 0) {
    fprintf(stderr, "%s: added_data is greater than the correct data.\n", __func__);
  } else {
    fprintf(stderr, "%s: added_data is less than the correct data.\n", __func__);
  }

  TAKEDOWN:
  freeSafe(correct_output);
  freeSafe(test_output);

  vgDestroyImageSafe(correct);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(input);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(output);
  cvpi_vg_error_takedown();
  if(BADSTATE) {
    return_value = CVPI_FALSE;
  }
  return return_value;
#undef TAKEDOWN
}

CVPI_BOOL test_cvpi_avuy2ayuv(void) {
#define test_cvpi_avuy2ayuv_takedown
  int BADSTATE = 0;

  int width = 10;
  int height = 10;

  VGImage correct = VG_INVALID_HANDLE;
  VGImage input = VG_INVALID_HANDLE;
  VGImage output = VG_INVALID_HANDLE;

  void* correct_output = NULL;
  void* test_output = NULL;

  FILE* output_file = NULL;

  CVPI_BOOL return_value = CVPI_FALSE;
  /* 0x004696 */
  correct = vgCreateImagePainted(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED, 
					 0, 70, 150, 255);
  cvpi_vg_error_check();
  input = vgCreateImagePainted(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED,
				       150, 70, 0, 255);
  cvpi_vg_error_check();
  output = vgCreateImage(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();

  vgColorMatrix(output, input,
		cvpi_avuy2ayuv);
  cvpi_vg_error_check();
  vgFinish();
  cvpi_vg_error_check();
  correct_output = malloc(CVPI_PIXEL_BYTES * width * height);
  heap_error_check(correct_output);
  test_output = malloc(CVPI_PIXEL_BYTES * width * height);
  heap_error_check(test_output);

  vgGetImageSubData(output, test_output, width*CVPI_PIXEL_BYTES, CVPI_COLOR_SPACE, 0, 0, width, height);
  cvpi_vg_error_check();
  vgGetImageSubData(correct, correct_output, width*CVPI_PIXEL_BYTES,
		    CVPI_COLOR_SPACE, 0, 0, width, height);
  cvpi_vg_error_check();

  int cmp = memcmp(test_output, correct_output, width*height*CVPI_PIXEL_BYTES);

  if(cmp == 0) {
    return_value = CVPI_TRUE;
  } else if(cmp > 0) {
    fprintf(stderr, "%s: added_data is greater than the correct data.\n", __func__);
  } else {
    fprintf(stderr, "%s: added_data is less than the correct data.\n", __func__);
  }

  /* output color should be 0xRRGGBB = 0x004696 */
  char* output_image_path = IMAGES_TMP_DIR "test_cvpi_avuy2ayuv.bmp";
  output_file = fopen(output_image_path, "w");
  cvpi_bmp_header_alloc_write(output_file, width, height, 100, cvpi_bmp_sARGB_8888);
  size_t fwrite_size = CVPI_PIXEL_BYTES * width * height;
  size_t written = fwrite(test_output, 1, fwrite_size, output_file);
  int flushed = fflush(output_file);

  if(written != fwrite_size || flushed != 0) {
    if(written != fwrite_size) {
      fprintf(CVPI_LOG_FILE, "%s: Error writing file: fwrite size discrepancy\nexpected:%d\nreturned:%d.\n",
	      __func__, fwrite_size, written);
    } else {
      fprintf(CVPI_LOG_FILE, "%s: Error writing file: fflush\nerrno = %d\nreturned = %d\n",
	      __func__, errno, flushed);
    }
  }

  TAKEDOWN:

  fcloseSafeTakedown(output_file);
  freeSafe(correct_output);
  freeSafe(test_output);

  vgDestroyImageSafe(correct);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(input);
  cvpi_vg_error_takedown();
  vgDestroyImageSafe(output);
  cvpi_vg_error_takedown();
  if(BADSTATE) {
    return_value = CVPI_FALSE;
  }
  return return_value;
#undef TAKEDOWN
}

/* color conversion */
CVPI_BOOL test_cvpi_avuy2argb(void) {
#define TAKEDOWN test_cvpi_avuy2argb_takedown
  int BADSTATE = 0;

  CVPI_BOOL return_value = CVPI_TRUE;

  FILE* input_file = NULL;
  FILE* output_file = NULL;

  cvpi_pixel* input_data = NULL;
  cvpi_pixel* output_data = NULL;
  
  const int width = 1280;
  const int height = 960;

  char* input_image_path = IMAGES_CORRECT_DIR "mark1_black_yuva.yuv";
  char* output_image_path = IMAGES_TMP_DIR "mark1_black_avuy2argb.bmp";

  input_file = fopen(input_image_path, "r");
  heap_error_check(input_file);
  output_file = fopen(output_image_path, "w");
  heap_error_check(output_file);
  
  seek_error_check(fseek(input_file, 0L, SEEK_END));
  int input_size = ftell(input_file);		/* input file size */
  seek_error_check(input_size);
  seek_error_check(fseek(input_file, 0L, SEEK_SET));

  input_data = malloc(input_size);
  heap_error_check(input_data);
  output_data = malloc(input_size);
  heap_error_check(output_data);

  if(input_size != fread(input_data, 1, input_size, input_file)) {
    fprintf(CVPI_LOG_FILE, "%s: Reading the input image from memory failed.\n",__func__);
    BADSTATE = 1;
    goto TAKEDOWN;
  }

  if(CVPI_FALSE_TEST(cvpi_avuy2argb(input_data, output_data, width, height))) {
    fprintf(CVPI_LOG_FILE, "%s: input_data or output_data is NULL.\n",__func__);
    BADSTATE = 1;
    goto TAKEDOWN;
  }

  cvpi_bmp_header_alloc_write(output_file, width, height, 100, cvpi_bmp_sARGB_8888);
  size_t written = fwrite(output_data, 1, input_size, output_file);
  int flushed = fflush(output_file);
  if(written != input_size || flushed != 0) {
    if(written != input_size) {
      fprintf(CVPI_LOG_FILE, "%s: Error writing file: fwrite size discrepancy\nexpected:%d\nreturned:%d.\n",
	      __func__, input_size, written);
    } else {
      fprintf(CVPI_LOG_FILE, "%s: Error writing file: fflush\nerrno = %d\nreturned = %d\n",
	      __func__, errno, flushed);
    }
  }

  TAKEDOWN:
  fcloseSafeTakedown(output_file);
  fcloseSafeTakedown(input_file);
  freeSafe(input_data);
  freeSafe(output_data);
  if(BADSTATE) {
    return_value = CVPI_FALSE;
  }
  return return_value;
#undef TAKEDOWN
}

CVPI_BOOL test_cvpi_image2argb(void) {
#define TAKEDOWN test_cvpi_image2argb_takedown
  int BADSTATE = 0;

  CVPI_BOOL return_value = CVPI_TRUE;

  FILE* input = NULL;
  FILE* output = NULL;

  void* image_in = NULL;

  cvpi_pixel* image_out = NULL;

  VGImage input_image = VG_INVALID_HANDLE;

  char* input_image_path = IMAGES_CORRECT_DIR "mark1_black_yuva.yuv";  
  char* output_image_path = IMAGES_TMP_DIR "mark1_black_argb.bmp";
  const int width = 1280;
  const int height = 960;

  input = fopen(input_image_path, "r");
  heap_error_check(input);
  output = fopen(output_image_path, "w");
  heap_error_check(output);

  seek_error_check(fseek(input, 0L, SEEK_END));
  int input_size = ftell(input);		/* input file size */
  seek_error_check(input_size);
  seek_error_check(fseek(input, 0L, SEEK_SET));

  image_in = malloc(input_size);
  heap_error_check(image_in);

  if(input_size != fread(image_in, 1, input_size, input)) {
    fprintf(stderr, "%s: Reading the input image from memory failed.\n",__func__);
    BADSTATE = 1;
    goto TAKEDOWN;
  }

  input_image = vgCreateImage(CVPI_COLOR_SPACE,width, height,VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();
  vgImageSubData(input_image, image_in, width*CVPI_PIXEL_BYTES, CVPI_COLOR_SPACE, 0, 0, width, height);
  cvpi_vg_error_check();
  vgFinish();
  cvpi_vg_error_check();

  image_out = cvpi_image2argb(input_image);
  heap_error_check(image_out);

  cvpi_bmp_header_alloc_write(output, width, height, 100, cvpi_bmp_sARGB_8888);
  size_t written = fwrite(image_out, 1, input_size, output);
  int flushed = fflush(output);
  if(written != input_size || flushed != 0) {
    if(written != input_size) {
      fprintf(CVPI_LOG_FILE, "%s: Error writing file: fwrite size discrepancy\nexpected:%d\nreturned:%d.\n",
	      __func__, input_size, written);
    } else {
      fprintf(CVPI_LOG_FILE, "%s: Error writing file: fflush\nerrno = %d\nreturned = %d\n",
	      __func__, errno, flushed);
    }
  }
  TAKEDOWN:
  freeSafe(image_out);
  freeSafe(image_in);
  vgDestroyImageSafe(input_image);
  cvpi_vg_error_takedown();
  fcloseSafeTakedown(input);
  fcloseSafeTakedown(output);

  if(BADSTATE) {
    return_value = CVPI_FALSE;
  }

  return return_value;
#undef TAKEDOWN
}

CVPI_BOOL test_overflow_behavior(void) {
  VGfloat image_correct[20] = {
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,

    255,255,255,255
  };

  VGfloat image1_color[20] = {
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,

    254,254,254,254
  };

  VGfloat image2_color[20] = {
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,

    10,10,10,10
  };

  return test_cvpi_image_add_common(1, 1, image1_color, image2_color, 1, 1, 1, 0, image_correct, IMAGES_TMP_DIR "overflow.yuv");
}

CVPI_BOOL test_negative_behavior(void) {
  VGfloat image_correct[20] = {
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,

    0,0,0,0
  };

  VGfloat image1_color[20] = {
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,

    1,1,1,1
  };

  VGfloat image2_color[20] = {
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,

    2,2,2,2
  };

  return test_cvpi_image_add_common(1, 1, image1_color, image2_color, 1, -1, 1, 0, image_correct, IMAGES_TMP_DIR "negative.yuv");
}

CVPI_BOOL test_vgConvolveNoShift(void) {
#define TAKEDOWN test_vgConvolveNoShift_takedown
  int BADSTATE = 0;

  VGImage dst = VG_INVALID_HANDLE;
  VGImage src = VG_INVALID_HANDLE;
  void* test_data = NULL;
  void* convolved_data = NULL;
  FILE* test_file = NULL;
  VGint kernelWidth = 5;
  VGint kernelHeight = 5;

  CVPI_BOOL return_value = CVPI_TRUE;

  const VGshort kernel[25] = {0, 0, 0, 0, 0,
  			      0, 0, 0, 0, 0,
  			      0, 0, 1, 0, 0,
  			      0, 0, 0, 0, 0,
  			      0, 0, 0, 0, 0};

  VGfloat scale = 1;
  VGfloat bias = 0;
  VGTilingMode tilingMode = VG_TILE_FILL;

  char* test_image_file_path = IMAGES_CORRECT_DIR "mark1_black_yuva.yuv";
  const int WIDTH = 1280;
  const int HEIGHT = 960;

  test_file = fopen(test_image_file_path, "r");
  heap_error_check(test_file);

  seek_error_check(fseek(test_file, 0L, SEEK_END));
  int data_size = ftell(test_file);		/* correct file size */
  seek_error_check(data_size);
  seek_error_check(fseek(test_file, 0L, SEEK_SET));

  test_data = malloc(data_size);
  heap_error_check(test_data);

  convolved_data = malloc(data_size);
  heap_error_check(convolved_data);

  if(data_size != fread(test_data, 1, data_size, test_file)) {
    fprintf(CVPI_LOG_FILE, "%s: Reading the image from memory failed.\n",__func__);
    BADSTATE = 1;
    goto TAKEDOWN;
  }

  dst = vgCreateImage(CVPI_COLOR_SPACE, WIDTH, HEIGHT,VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();

  src = vgCreateImage(CVPI_COLOR_SPACE, WIDTH, HEIGHT,VG_IMAGE_QUALITY_NONANTIALIASED);
  cvpi_vg_error_check();

  vgImageSubData(src, test_data, WIDTH*CVPI_PIXEL_BYTES,
		 CVPI_COLOR_SPACE, 0, 0, WIDTH, HEIGHT);
  cvpi_vg_error_check();

  vgConvolveNoShift(dst, src,
  		    kernelWidth, kernelHeight,
  		    kernel,
  		    scale,
  		    bias,
  		    tilingMode);
  cvpi_vg_error_check();

  vgFinish();
  cvpi_vg_error_check();

  vgGetImageSubData(dst, convolved_data, WIDTH*CVPI_PIXEL_BYTES,
		    CVPI_COLOR_SPACE, 0, 0, WIDTH, HEIGHT);
  cvpi_vg_error_check();

  vgFinish();
  cvpi_vg_error_check();

  if(memcmp(convolved_data, test_data, data_size)) {
    return_value = CVPI_FALSE;
  }
 TAKEDOWN:

  vgDestroyImageSafe(dst);
  cvpi_vg_error_takedown();

  vgDestroyImageSafe(src);
  cvpi_vg_error_takedown();

  freeSafe(test_data);
  freeSafe(convolved_data);
  fcloseSafeTakedown(test_file);

  if(BADSTATE == 1) {
    return_value = CVPI_FALSE;
  }

  return return_value;
}

CVPI_BOOL test_cvpi_bmp_header_alloc_write(void) {
  FILE* header_file = fopen(IMAGES_TMP_DIR "bmp_header.bmp", "w");
  if(header_file == NULL) {
    fprintf(stderr, "unable to open file\n");
    return CVPI_FALSE;
  }

  CVPI_BOOL return_value = cvpi_bmp_header_alloc_write(header_file, 1, 2, 3, CVPI_BMP_DEFAULT);

  fcloseSafeTakedown(header_file);

  return return_value;
}
