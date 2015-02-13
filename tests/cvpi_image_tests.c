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

#define IMAGES_CORRECT_DIR "./test_images_correct/"
#define IMAGES_TMP_DIR "./test_images_tmp/"

#define PRINT_MALLOC_ERROR fprintf(cvpi_log_file, "%s:%d: Error malloc returned NULL: errno = %d\n", __func__, __LINE__, errno)

VGErrorCode error;

CVPI_BOOL test_cvpi_pixel(void) { 
  char width = 1;
  char height = 1;
  char* data = malloc(width*height*CVPI_PIXEL_BYTES);
  if(data == NULL) {
    PRINT_MALLOC_ERROR;
    return CVPI_FALSE;
  }
  
  VGImage image = vgCreateImagePainted(CVPI_COLOR_SPACE,width, height,VG_IMAGE_QUALITY_NONANTIALIASED,
				       4,3,2,1);
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    fprintf(cvpi_log_file, "%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    return CVPI_FALSE;
  }
  
  cvpi_pixel pixel;
  pixel.channel[cvpi_pixel_red] = 4;
  pixel.channel[cvpi_pixel_green] = 3;
  pixel.channel[cvpi_pixel_blue] = 2;
  pixel.channel[cvpi_pixel_alpha] = 1;
  
  vgGetImageSubData(image, (void*)data,
		    CVPI_PIXEL_BYTES * width, CVPI_COLOR_SPACE, 0, 0, width, height);
  vgFinish();
  int cmp = memcmp((void*)data, &pixel, width*height*CVPI_PIXEL_BYTES);
  CVPI_BOOL return_value = CVPI_FALSE;
  if(cmp == 0) {
    return_value = CVPI_TRUE; 
  } else {
    fprintf(cvpi_log_file, "%s: cvpi_pixel: %d %d %d %d | VGImage: %d %d %d %d.\n", __func__,
	    pixel.channel[cvpi_pixel_red], pixel.channel[cvpi_pixel_green], 
	    pixel.channel[cvpi_pixel_blue], pixel.channel[cvpi_pixel_alpha], 
	    data[0], data[1], data[2], data[3]);
  }
  
  free(data);
  vgDestroyImage(image);
  vgFlush();

  return return_value;
}

CVPI_BOOL test_cvpi_yuyv2yuva() {
  const int INPUT_WIDTH = 640;
  const int INPUT_HEIGHT = 960;
  const int OUTPUT_WIDTH = 1280;
  const int OUTPUT_HEIGHT = 960;
  CVPI_BOOL return_value = CVPI_FALSE;

  char* yuyv2yuva_input = IMAGES_CORRECT_DIR "mark1_black.yuv";
  char* yuyv2yuva_correct_result = IMAGES_CORRECT_DIR "mark1_black_yuva.yuv";
  char* yuyv2yuva_tmp_result = IMAGES_TMP_DIR "mark1_black_yuva.yuv";

  FILE* input = fopen(yuyv2yuva_input, "r");
  if(input == NULL) {
    fprintf(cvpi_log_file, "%s: Error opening %s: errno = %d\n", __func__, yuyv2yuva_input, errno);
    return CVPI_FALSE;
  }

  FILE* correct = fopen(yuyv2yuva_correct_result, "r");
  if(input == NULL) {
    fprintf(cvpi_log_file, "%s: Error opening %s: errno = %d\n", __func__, yuyv2yuva_correct_result, errno);
    return CVPI_FALSE;
  }

  FILE* output = fopen(yuyv2yuva_tmp_result, "w");
  if(input == NULL) {
    fprintf(cvpi_log_file, "%s: Error opening %s: errno = %d\n", __func__, yuyv2yuva_tmp_result, errno);
    return CVPI_FALSE;
  }
  
  fseek(input, 0L, SEEK_END);
  int input_size = ftell(input);		/* input file size */
  fseek(input, 0L, SEEK_SET);

  fseek(correct, 0L, SEEK_END);
  int correct_size = ftell(correct);		/* correct file size */
  fseek(correct, 0L, SEEK_SET);

  if(input_size <= 0
     || correct_size <= 0
     || (input_size*2 != correct_size && correct_size != CVPI_PIXEL_BYTES * OUTPUT_WIDTH * OUTPUT_HEIGHT)) {
    fprintf(cvpi_log_file, "%s: Input file and correct file size mismatch. %d and %d\n",
	    __func__, input_size*2, correct_size);
    goto test_cvpi_yuyv2yuva_takedown;
  }

  void* image_in = malloc(input_size);
  if(image_in == NULL) {
    fprintf(cvpi_log_file, "%s: image_in malloc null\n", __func__);
    goto test_cvpi_yuyv2yuva_takedown;
  }

  void* image_out = malloc(2*input_size);
  if(image_out == NULL) {
    fprintf(cvpi_log_file, "%s: image_out malloc null\n", __func__);
    free(image_in);
    goto test_cvpi_yuyv2yuva_takedown;
  }

  void* image_correct = malloc(correct_size);
  if(image_correct == NULL) {
    fprintf(cvpi_log_file, "%s: image_correct malloc null\n", __func__);
    free(image_in);
    free(image_correct);
    goto test_cvpi_yuyv2yuva_takedown;
  }

  if(input_size != fread(image_in, 1, input_size, input)) {
    fprintf(cvpi_log_file, "%s: Reading the input image from memory failed.\n",__func__);
    goto test_cvpi_yuyv2yuva_takedown_free;
  }
  if(correct_size != fread(image_correct, 1, correct_size, correct)) {
    fprintf(cvpi_log_file, "%s: Reading the correct image from memory failed.\n",__func__);
    goto test_cvpi_yuyv2yuva_takedown_free;
  }

  VGImage input_image = vgCreateImage(CVPI_COLOR_SPACE,INPUT_WIDTH, INPUT_HEIGHT,VG_IMAGE_QUALITY_NONANTIALIASED);
  vgFinish();
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    fprintf(cvpi_log_file, "%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    goto test_cvpi_yuyv2yuva_takedown_free;
  }
  vgImageSubData(input_image, image_in, INPUT_WIDTH*CVPI_PIXEL_BYTES, CVPI_COLOR_SPACE, 0, 0, INPUT_WIDTH, INPUT_HEIGHT);
  vgFinish();
  VGImage output_image = cvpi_yuyv2yuva(input_image);
  vgFinish();
  if(output_image == VG_INVALID_HANDLE) {
    fprintf(cvpi_log_file, "%s:%d:Unable to convert\n", __func__, __LINE__);
  }
  
  vgGetImageSubData(output_image, image_out, CVPI_PIXEL_BYTES*OUTPUT_WIDTH, CVPI_COLOR_SPACE, 0, 0, OUTPUT_WIDTH, OUTPUT_HEIGHT);
  vgFinish();

  if(!memcmp(image_out, image_correct, correct_size)) {
    return_value = CVPI_TRUE;
  } else {
    size_t written = fwrite(image_out, CVPI_PIXEL_BYTES * OUTPUT_WIDTH, OUTPUT_HEIGHT, output);
    int flushed = fflush(output);
    unsigned long fwrite_size = CVPI_PIXEL_BYTES * OUTPUT_WIDTH * OUTPUT_HEIGHT;
    if(written != fwrite_size || flushed != 0) {
      if(written != fwrite_size) {
	fprintf(cvpi_log_file, "%s: Error writing file: fwrite size discrepancy\nexpected:%d\nreturned:%d.\n",
		__func__, fwrite_size, written);
      } else {
	fprintf(cvpi_log_file, "%s: Error writing file: fflush\nerrno = %d\nreturned = %d\n",
		__func__, errno, flushed);
      }
    }
  }

  vgDestroyImage(output_image);
  vgDestroyImage(input_image);
  vgFlush();

 test_cvpi_yuyv2yuva_takedown_free:
  free(image_in);
  free(image_out);
  free(image_correct);

 test_cvpi_yuyv2yuva_takedown:
  fclose(input);
  fclose(output);
  fclose(correct);

  return return_value;
}

static CVPI_BOOL test_cvpi_add_images_common(int width, int height, 
					     VGfloat * image1_color, VGfloat * image2_color, 
					     VGshort a, VGshort b, VGfloat scale, VGfloat bias, 
					     VGfloat * image_correct_color, char* file_name) {
  /* Create two solid color images, add them, and check the value of each pixel. */

  VGImage image0 = vgCreateImage(CVPI_COLOR_SPACE, width, height,VG_IMAGE_QUALITY_NONANTIALIASED);
  vgFinish();
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    fprintf(cvpi_log_file, "%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    return CVPI_FALSE;
  }
  VGImage image1 = vgCreateImage(CVPI_COLOR_SPACE, width, height,VG_IMAGE_QUALITY_NONANTIALIASED);
  vgFinish();
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    fprintf(cvpi_log_file, "%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    vgDestroyImage(image0);
    vgFlush();
    return CVPI_FALSE;
  }
  VGImage image2 = vgCreateImage(CVPI_COLOR_SPACE, width, height,VG_IMAGE_QUALITY_NONANTIALIASED); 
  vgFinish();
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    fprintf(cvpi_log_file, "%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    vgDestroyImage(image0);
    vgDestroyImage(image1);
    vgFlush();
    return CVPI_FALSE;
  }
  VGImage image_correct = vgCreateImage(CVPI_COLOR_SPACE, width, height,VG_IMAGE_QUALITY_NONANTIALIASED); 
  vgFinish();
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    fprintf(cvpi_log_file, "%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    vgDestroyImage(image0);
    vgDestroyImage(image1);
    vgDestroyImage(image2);
    vgFlush();
    return CVPI_FALSE;
  }

  vgColorMatrixNormal(image1, image0, &image1_color[0]);
  vgColorMatrixNormal(image2, image0, &image2_color[0]);
  vgColorMatrixNormal(image_correct, image0, &image_correct_color[0]);
  vgFlush();

  VGImage image_out = cvpi_add_images(image1, image2, a, b, scale, bias);
  vgFinsh();
  if(image_out == VG_INVALID_HANDLE) {
    fprintf(cvpi_log_file, "%s:%d:Unable to add images\n", __func__, __LINE__);
    vgDestroyImage(image0);
    vgDestroyImage(image1);
    vgDestroyImage(image2);
    vgDestroyImage(image_correct);
    vgFlush();
    return CVPI_FALSE;
  }

  void* added_data = malloc(width*height*CVPI_PIXEL_BYTES);
  if(added_data == NULL) {
    PRINT_MALLOC_ERROR;
    vgDestroyImage(image0);
    vgDestroyImage(image1);
    vgDestroyImage(image2);
    vgDestroyImage(image_correct);
    vgDestroyImage(image_out);
    vgFlush();
    return CVPI_FALSE;
  }
  void* correct_data = malloc(width*height*CVPI_PIXEL_BYTES);
  if(correct_data == NULL) {
    PRINT_MALLOC_ERROR;
    vgDestroyImage(image0);
    vgDestroyImage(image1);
    vgDestroyImage(image2);
    vgDestroyImage(image_correct);
    vgDestroyImage(image_out);
    vgFlush();
    free(added_data);
    return CVPI_FALSE;
  }

  vgGetImageSubData(image_out, added_data, CVPI_PIXEL_BYTES*width, CVPI_COLOR_SPACE, 0, 0, width, height);
  vgGetImageSubData(image_correct, correct_data, CVPI_PIXEL_BYTES*width, CVPI_COLOR_SPACE, 0, 0, width, height);
  unsigned long error = vgGetError();
  if(error != VG_NO_ERROR) {
    fprintf(cvpi_log_file, "%s: vgGetImageSubData %s\n", __func__, cvpi_vg_error_string(error));
  }
  vgFinish();

  vgDestroyImage(image0);
  vgDestroyImage(image1);
  vgDestroyImage(image2);
  vgDestroyImage(image_out);
  vgDestroyImage(image_correct);

  int cmp = memcmp(added_data, correct_data, width*height*CVPI_PIXEL_BYTES);
  CVPI_BOOL return_value = CVPI_FALSE;
  if(cmp == 0) {
    return_value = CVPI_TRUE;
  } else if(cmp > 0) {
    fprintf(cvpi_log_file, "%s: added_data is greater than the correct data.\n", __func__);
  } else {
    fprintf(cvpi_log_file, "%s: added_data is less than the correct data.\n", __func__);
  }
  if(strlen(file_name) > 0) {  
    int path_length = strlen(IMAGES_TMP_DIR) + strlen(file_name);
    int size = sizeof(*file_name) * path_length;
    if(size == 0) {
      free(added_data);
      fprintf(cvpi_log_file, "%s:%d:size = 0\n", __func__, __LINE__);
      return CVPI_FALSE;
    }
    char* file_path = malloc(size);
    if(file_path == NULL) {
      PRINT_MALLOC_ERROR;
      free(added_data);
      return CVPI_FALSE;
    }
    int snprintf_ret;
    if((snprintf_ret = snprintf(file_path,  size, "%s%s", IMAGES_TMP_DIR, file_name)) < 0) {
      fprintf(cvpi_log_file, "%s:%d: snprintf returned: %d\n", __func__, __LINE__, errno);
      free(added_data);
      return CVPI_FALSE;
    }
    
    FILE* output = fopen(file_path, "w");
    if(output == NULL) {
      PRINT_MALLOC_ERROR;
      free(added_data);
      free(file_path);
      return CVPI_FALSE;
    }
    unsigned long fwrite_size = width*height*CVPI_PIXEL_BYTES;
    size_t written = fwrite(added_data, fwrite_size, 1, output);
    int flushed = fflush(output);
    if(written != fwrite_size || flushed != 0) {
      if(written != fwrite_size) {
	fprintf(cvpi_log_file, "%s: Error writing file: fwrite size discrepancy\nexpected:%d\nreturned:%d.\n",
		__func__, fwrite_size, written);
      } else {
	fprintf(cvpi_log_file, "%s: Error writing file: fflush\nerrno = %d\nreturned = %d\n",
		__func__, errno, flushed);
      }
    }
    fclose(output);

    free(file_path);
  }
  free(added_data);

  return return_value;
}

CVPI_BOOL test_cvpi_add_images_odd(void) {
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

  return test_cvpi_add_images_common(640, 3, image1_color, image2_color, 1, 1, 1, 0, image_correct_color, "test_cvpi_add_images_odd.yuv");
}

CVPI_BOOL test_cvpi_add_images_single(void) {
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

  return test_cvpi_add_images_common(640, 1, image1_color, image2_color, 1, 1, 1, 0, image_correct_color, "test_cvpi_add_images_single.yuv");
}

CVPI_BOOL test_cvpi_add_images_even(void) {
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

  return test_cvpi_add_images_common(640, 2, image1_color, image2_color, 1, 1, 1, 0, image_correct_color, "test_cvpi_add_images_even.yuv");
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

  return test_cvpi_add_images_common(640, 3, image1_color, image2_color, 1, -1, 1, 0, image_correct_color, "test_cvpi_subtract_images_odd.yuv");
}

CVPI_BOOL test_cvpi_add_images_odd_scale_add(void) {
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

  return test_cvpi_add_images_common(640, 3, image1_color, image2_color, 2, 1, 5, 2, image_correct_color, "test_cvpi_add_images_odd_scale_add.yuv");
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

  return test_cvpi_add_images_common(640, 3, image1_color, image2_color, 1, -2, 5, 2, image_correct_color, "test_cvpi_subtract_images_odd_scale_add.yuv");
}

CVPI_BOOL test_cvpi_add_images_huge_even(void) {
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

  return test_cvpi_add_images_common(EGL_CONFIG_MAX_WIDTH, EGL_CONFIG_MAX_HEIGHT, image1_color, image2_color, 1, 1, 1, 0, image_correct_color, "test_cvpi_add_images_huge_even.yuv");
}
CVPI_BOOL test_cvpi_add_images_huge_odd(void) {
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

  return test_cvpi_add_images_common(EGL_CONFIG_MAX_WIDTH-1, EGL_CONFIG_MAX_HEIGHT-1, image1_color, image2_color, 1, 1, 1, 0, image_correct_color, "test_cvpi_add_images_huge_odd.yuv");
}

CVPI_BOOL test_cvpi_add_channels_RB(void) {

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

  VGImage dummy = vgCreateImage(CVPI_COLOR_SPACE, width, height,VG_IMAGE_QUALITY_NONANTIALIASED);
  vgFinish();
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    fprintf(cvpi_log_file, "%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    return CVPI_FALSE;
  }
  VGImage input_image = vgCreateImage(CVPI_COLOR_SPACE, width, height,VG_IMAGE_QUALITY_NONANTIALIASED);
  vgFinish();
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    fprintf(cvpi_log_file, "%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    vgDestroyImage(dummy);
    vgFlush();
    return CVPI_FALSE;
  }
  VGImage image_correct = vgCreateImage(CVPI_COLOR_SPACE, width, height,VG_IMAGE_QUALITY_NONANTIALIASED);
  vgFinish();
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    fprintf(cvpi_log_file, "%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    vgDestroyImage(dummy);
    vgDestroyImage(input_image);
    vgFlush();
    return CVPI_FALSE;
  }

  vgColorMatrixNormal(input_image, dummy, image_color);
  vgColorMatrixNormal(image_correct, dummy, image_correct_color);

  /* green and blue channels should contain 1's */
  VGImage first_image = cvpi_add_channels(input_image, VG_RED, VG_BLUE, 1, 1, 1, 1, VG_GREEN | VG_ALPHA);
  vgFinish();
  if(first_image == VG_INVALID_HANDLE) {
    fprintf(cvpi_log_file, "%s:%d:Unable to add channels\n", __func__, __LINE__);
    vgDestroyImage(dummy);
    vgDestroyImage(input_image);
    vgDestroyImage(image_correct);
    vgFlush();
    return CVPI_FALSE;
  }

  /* all channels should contain 10's */
  VGImage second_image = cvpi_add_channels(first_image, VG_GREEN, VG_ALPHA, 2, 3, 2, 0, 
					   VG_RED|VG_BLUE|VG_GREEN|VG_ALPHA);
  vgFinish();
  if(second_image == VG_INVALID_HANDLE) {
    fprintf(cvpi_log_file, "%s:%d:Unable to add channels\n", __func__, __LINE__);
    vgDestroyImage(dummy);
    vgDestroyImage(input_image);
    vgDestroyImage(image_correct);
    vgDestroyImage(first_image);
    vgFlush();
    return CVPI_FALSE;
  }

  VGint second_image_width = vgGetParameteri(second_image, VG_IMAGE_WIDTH);
  VGint second_image_height = vgGetParameteri(second_image, VG_IMAGE_HEIGHT);

  size_t image_size = width*height*CVPI_PIXEL_BYTES;
  if(image_size <= 0) {
    fprnitf(cvpi_log_file, "%s:%d:malloc size = 0\n", __func__, __LINE__);
    vgDestroyImage(dummy);
    vgDestroyImage(input_image);
    vgDestroyImage(image_correct);
    vgDestroyImage(first_image);
    vgDestroyImage(second_image);
    vgFlush();
    return CVPI_FALSE;
  }
  void* added_data = malloc(image_size);
  if(added_data == NULL) {
    PRINT_MALLOC_ERROR;
    vgDestroyImage(dummy);
    vgDestroyImage(input_image);
    vgDestroyImage(image_correct);
    vgDestroyImage(first_image);
    vgDestroyImage(second_image);
    vgFlush();
    return CVPI_FALSE;
  }
  void* correct_data = malloc(image_size);
  if(correct_data == NULL) {
    PRINT_MALLOC_ERROR;
    vgDestroyImage(dummy);
    vgDestroyImage(input_image);
    vgDestroyImage(image_correct);
    vgDestroyImage(first_image);
    vgDestroyImage(second_image);
    vgFlush();
    free(added_data);
    return CVPI_FALSE;
  }
  vgGetImageSubData(second_image, added_data, CVPI_PIXEL_BYTES*width, CVPI_COLOR_SPACE, 0, 0, width, height);
  vgGetImageSubData(image_correct, correct_data, CVPI_PIXEL_BYTES*width, CVPI_COLOR_SPACE, 0, 0, width, height); 

  vgFinish();

  vgDestroyImage(dummy);
  vgDestroyImage(input_image);
  vgDestroyImage(image_correct);
  vgDestroyImage(first_image);
  vgDestroyImage(second_image);

  vgFlush();

  int cmp = memcmp(added_data, correct_data, image_size);
  CVPI_BOOL return_value = CVPI_FALSE;
  if(cmp == 0) {
    return_value = CVPI_TRUE;
  } else if(cmp > 0) {
    fprintf(cvpi_log_file, "%s: added_data is greater than the correct data.\n", __func__);
  } else {
    fprintf(cvpi_log_file, "%s: added_data is less than the correct data.\n", __func__);
  }
  char* file_name = "test_cvpi_add_channels_RB";

  int path_length = strlen(IMAGES_TMP_DIR) + strlen(file_name);
  if(path_length > 0) {
    char* file_path = malloc(sizeof(*file_path) * path_length);
    if(file_path == NULL) {
      PRINT_MALLOC_ERROR;
      goto test_cvpi_add_channels_RB_free;
    }
    
    int file_path_print = sprintf(file_path, "%s%s", IMAGES_TMP_DIR, file_name);
    if(file_path_print < 0) {
      fprintf(cvpi_log_file, "%s:%d:error writing file path: %d\n", __func__, __LINE__, file_path_print);
      free(file_path);
      goto test_cvpi_add_channels_RB_free;
    }

    FILE* output = fopen(file_path, "w");
    if(output == NULL) {
      fprintf(cvpi_log_file, "%s: Error opening %s: errno = %d\n", __func__, file_path, errno);
      free(file_path);
      goto test_cvpi_add_channels_RB_free;
    }

    unsigned long fwrite_size = width*height*CVPI_PIXEL_BYTES;
    size_t written = fwrite(added_data, fwrite_size, 1, output);
    int flushed = fflush(output);

    if(written != fwrite_size || flushed != 0) {
      if(written != fwrite_size) {
	fprintf(cvpi_log_file, "%s: Error writing file: fwrite size discrepancy\nexpected:%d\nreturned:%d.\n",
		__func__, fwrite_size, written);
      } else {
	fprintf(cvpi_log_file, "%s: Error writing file: fflush\nerrno = %d\nreturned = %d\n",
		__func__, errno, flushed);
      }
    }

    fclose(output);

    free(file_path);
  } else {
    fprintf(cvpi_log_file, "%s:%d: file path length = 0\n", __func__, __LINE__);
  }
 test_cvpi_add_channels_RB_free:
  free(added_data);
  free(correct_data);

  return return_value;
}
CVPI_BOOL test_cvpi_add_channels_RR(void) {

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

  VGImage dummy = vgCreateImage(CVPI_COLOR_SPACE, width, height,VG_IMAGE_QUALITY_NONANTIALIASED);
  vgFinish();
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    fprintf(cvpi_log_file, "%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    return CVPI_FALSE;
  }
  VGImage input_image = vgCreateImage(CVPI_COLOR_SPACE, width, height,VG_IMAGE_QUALITY_NONANTIALIASED);
  vgFinish();
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    fprintf(cvpi_log_file, "%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    vgDestroyImage(dummy);
    vgFlush();
    return CVPI_FALSE;
  }
  VGImage image_correct = vgCreateImage(CVPI_COLOR_SPACE, width, height,VG_IMAGE_QUALITY_NONANTIALIASED);
  vgFinish();
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    fprintf(cvpi_log_file, "%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    vgDestroyImage(dummy);
    vgDestroyImage(input_image);
    vgFlush();
    return CVPI_FALSE;
  }

  vgColorMatrixNormal(input_image, dummy, image_color);
  vgColorMatrixNormal(image_correct, dummy, image_correct_color);

  /* green and blue channels should contain 1's */
  VGImage first_image = cvpi_add_channels(input_image, VG_RED, VG_RED, 1, 1, 1, 1, VG_RED);
  vgFinish();
  if(first_image == VG_INVALID_HANDLE) {
    fprintf(cvpi_log_file, "%s:%d:Unable to add channels\n", __func__, __LINE__);
    vgDestroyImage(dummy);
    vgDestroyImage(input_image);
    vgDestroyImage(image_correct);
    vgFlush();
    return CVPI_FALSE;
  }

  /* all channels should contain 10's */
  VGImage second_image = cvpi_add_channels(first_image, VG_RED, VG_RED, 2, 3, 2, 0, 
					   VG_RED|VG_BLUE|VG_GREEN|VG_ALPHA);
  vgFinish();
  if(second_image == VG_INVALID_HANDLE) {
    fprintf(cvpi_log_file, "%s:%d:Unable to add channels\n", __func__, __LINE__);
    vgDestroyImage(dummy);
    vgDestroyImage(input_image);
    vgDestroyImage(image_correct);
    vgDestroyImage(first_image);
    vgFlush();
    return CVPI_FALSE;
  }

  size_t image_size = width*height*CVPI_PIXEL_BYTES;
  if(image_size <= 0) {
    fprnitf(cvpi_log_file, "%s:%d:malloc size = 0\n", __func__, __LINE__);
    vgDestroyImage(dummy);
    vgDestroyImage(input_image);
    vgDestroyImage(image_correct);
    vgDestroyImage(first_image);
    vgDestroyImage(second_image);
    vgFlush();
    return CVPI_FALSE;
  }

  void* added_data = malloc(image_size);
  if(added_data == NULL) {
    PRINT_MALLOC_ERROR;
    vgDestroyImage(dummy);
    vgDestroyImage(input_image);
    vgDestroyImage(image_correct);
    vgDestroyImage(first_image);
    vgDestroyImage(second_image);
    vgFlush();
    return CVPI_FALSE;
  }
  void* correct_data = malloc(image_size);
  if(correct_data == NULL) {
    PRINT_MALLOC_ERROR;
    vgDestroyImage(dummy);
    vgDestroyImage(input_image);
    vgDestroyImage(image_correct);
    vgDestroyImage(first_image);
    vgDestroyImage(second_image);
    vgFlush();
    free(added_data);
    return CVPI_FALSE;
  }

  vgGetImageSubData(second_image, added_data, CVPI_PIXEL_BYTES*width, CVPI_COLOR_SPACE, 0, 0, width, height);
  vgGetImageSubData(image_correct, correct_data, CVPI_PIXEL_BYTES*width, CVPI_COLOR_SPACE, 0, 0, width, height); 

  vgFinish();

  vgDestroyImage(dummy);
  vgDestroyImage(input_image);
  vgDestroyImage(image_correct);
  vgDestroyImage(first_image);
  vgDestroyImage(second_image);

  vgFlush();

  int cmp = memcmp(added_data, correct_data, width*height*CVPI_PIXEL_BYTES);
  CVPI_BOOL return_value = CVPI_FALSE;
  if(cmp == 0) {
    return_value = CVPI_TRUE;
  } else if(cmp > 0) {
    fprintf(cvpi_log_file, "%s: added_data is greater than the correct data.\n", __func__);
  } else {
    fprintf(cvpi_log_file, "%s: added_data is less than the correct data.\n", __func__);
  }

  char* file_name = "test_cvpi_add_channels_RR";

  int path_length = strlen(IMAGES_TMP_DIR) + strlen(file_name);
  if(path_length > 0) {
    char* file_path = malloc(sizeof(*file_path) * path_length);
    if(file_path == NULL) {
      PRINT_MALLOC_ERROR;
      goto test_cvpi_add_channels_RR_free;
    }

    int file_path_print = sprintf(file_path, "%s%s", IMAGES_TMP_DIR, file_name);
    if(file_path_print < 0) {
      fprintf(cvpi_log_file, "%s:%d:error writing file path: %d\n", __func__, __LINE__, file_path_print);
      free(file_path);
      goto test_cvpi_add_channels_RR_free;
    }

    FILE* output = fopen(file_path, "w");
    if(output == NULL) {
      fprintf(cvpi_log_file, "%s: Error opening %s: errno = %d\n", __func__, file_path, errno);
      free(file_path);
      goto test_cvpi_add_channels_RR_free;
    } 
    unsigned long fwrite_size = width*height*CVPI_PIXEL_BYTES;
    size_t written = fwrite(added_data, fwrite_size, 1, output);
    int flushed = fflush(output);
    if(written != fwrite_size || flushed != 0) {
      if(written != fwrite_size) {
	fprintf(cvpi_log_file, "%s: Error writing file: fwrite size discrepancy\nexpected:%d\nreturned:%d.\n",
		__func__, fwrite_size, written);
      } else {
	fprintf(cvpi_log_file, "%s: Error writing file: fflush\nerrno = %d\nreturned = %d\n",
		__func__, errno, flushed);
      }
    }

    fclose(output);
    free(file_path);
  } else {
    fprintf(cvpi_log_file, "%s:%d: file path length = 0\n", __func__, __LINE__);
  }
 test_cvpi_add_channels_RR_free:
  free(added_data);
  free(correct_data);

  return return_value;
}

CVPI_BOOL test_cvpi_add_channels_color(void) {

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

  VGImage dummy = vgCreateImage(CVPI_COLOR_SPACE, width, height,VG_IMAGE_QUALITY_NONANTIALIASED);
  vgFinish();
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    fprintf(cvpi_log_file, "%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    return CVPI_FALSE;
  }
  VGImage input_image = vgCreateImage(CVPI_COLOR_SPACE, width, height,VG_IMAGE_QUALITY_NONANTIALIASED);
  vgFinish();
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    fprintf(cvpi_log_file, "%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    vgDestroyImage(dummy);
    vgFlush();
    return CVPI_FALSE;
  }
  VGImage image_correct = vgCreateImage(CVPI_COLOR_SPACE, width, height,VG_IMAGE_QUALITY_NONANTIALIASED);
  vgFinish();
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    fprintf(cvpi_log_file, "%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    vgDestroyImage(dummy);
    vgDestroyImage(input_image);
    vgFlush();
    return CVPI_FALSE;
  }
  vgColorMatrixNormal(input_image, dummy, image_color);
  vgColorMatrixNormal(image_correct, dummy, image_correct_color);

  /* all channels should contain 1's */
  VGImage first_image = cvpi_add_channels_color(input_image, 3, 2, 1, 1, VG_RED|VG_BLUE|VG_GREEN|VG_ALPHA);
  vgFinish();
  if(first_image == VG_INVALID_HANDLE) {
    fprintf(cvpi_log_file, "%s:%d:Unable to add channels\n", __func__, __LINE__);
    vgDestroyImage(dummy);
    vgDestroyImage(input_image);
    vgDestroyImage(image_correct);
    vgFlush();
    return CVPI_FALSE;
  }
  /* green and blue channels should contain 7's and red and blue contain 1's */
  VGImage second_image = cvpi_add_channels_color(first_image, 2, 3, 2, 0, VG_GREEN | VG_ALPHA);
  vgFinish();
  if(second_image == VG_INVALID_HANDLE) {
    fprintf(cvpi_log_file, "%s:%d:Unable to add channels\n", __func__, __LINE__);
    vgDestroyImage(dummy);
    vgDestroyImage(input_image);
    vgDestroyImage(image_correct);
    vgDestroyImage(first_image);
    vgFlush();
    return CVPI_FALSE;
  }
  size_t image_size = width*height*CVPI_PIXEL_BYTES;
  if(image_size <= 0) {
    fprnitf(cvpi_log_file, "%s:%d:malloc size = 0\n", __func__, __LINE__);
    vgDestroyImage(dummy);
    vgDestroyImage(input_image);
    vgDestroyImage(image_correct);
    vgDestroyImage(first_image);
    vgDestroyImage(second_image);
    vgFlush();
    return CVPI_FALSE;
  }
  void* added_data = malloc(image_size);
  if(added_data == NULL) {
    PRINT_MALLOC_ERROR;
    vgDestroyImage(dummy);
    vgDestroyImage(input_image);
    vgDestroyImage(image_correct);
    vgDestroyImage(first_image);
    vgDestroyImage(second_image);
    vgFlush();
    return CVPI_FALSE;
  }
  void* correct_data = malloc(image_size);
  if(correct_data == NULL) {
    PRINT_MALLOC_ERROR;
    vgDestroyImage(dummy);
    vgDestroyImage(input_image);
    vgDestroyImage(image_correct);
    vgDestroyImage(first_image);
    vgDestroyImage(second_image);
    vgFlush();
    free(added_data);
    return CVPI_FALSE;
  }
  vgGetImageSubData(second_image, added_data, CVPI_PIXEL_BYTES*width, CVPI_COLOR_SPACE, 0, 0, width, height);
  vgGetImageSubData(image_correct, correct_data, CVPI_PIXEL_BYTES*width, CVPI_COLOR_SPACE, 0, 0, width, height); 

  vgFinish();

  vgDestroyImage(dummy);
  vgDestroyImage(input_image);
  vgDestroyImage(image_correct);
  vgDestroyImage(first_image);
  vgDestroyImage(second_image);

  vgFlush();

  int cmp = memcmp(added_data, correct_data, width*height*CVPI_PIXEL_BYTES);
  CVPI_BOOL return_value = CVPI_FALSE;
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
    char* file_path = malloc(sizeof(*file_path) * path_length);
    if(file_path == NULL) {
      PRINT_MALLOC_ERROR;
      goto test_cvpi_add_channels_color_free;
    }
    int file_path_print = sprintf(file_path, "%s%s", IMAGES_TMP_DIR, file_name);
    if(file_path_print < 0) {
      fprintf(cvpi_log_file, "%s:%d:error writing file path: %d\n", __func__, __LINE__, file_path_print);
      free(file_path);
      goto test_cvpi_add_channels_color_free;
    }

    FILE* output = fopen(file_path, "w");
    if(output == NULL) {
      fprintf(cvpi_log_file, "%s: Error opening %s: errno = %d\n", __func__, file_path, errno);
      free(file_path);
      goto test_cvpi_add_channels_color_free;
    } 
    unsigned long fwrite_size = width*height*CVPI_PIXEL_BYTES;
    size_t written  = fwrite(added_data, fwrite_size, 1, output);
    int flushed = fflush(output);
    if(written != fwrite_size || flushed != 0) {
      if(written != fwrite_size) {
	fprintf(cvpi_log_file, "%s: Error writing file: fwrite size discrepancy\nexpected:%d\nreturned:%d.\n",
		__func__, fwrite_size, written);
      } else {
	fprintf(cvpi_log_file, "%s: Error writing file: fflush\nerrno = %d\nreturned = %d\n",
		__func__, errno, flushed);
      }
    }

    fclose(output);
    free(file_path);
  } else {
    fprintf(cvpi_log_file, "%s:%d: file path length = 0\n", __func__, __LINE__);
  }
 test_cvpi_add_channels_color_free:
  free(added_data);
  free(correct_data);
  return return_value;
}

CVPI_BOOL test_cvpi_add_channels_all(void) {
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

  VGImage dummy = vgCreateImage(CVPI_COLOR_SPACE, width, height,VG_IMAGE_QUALITY_NONANTIALIASED);
  vgFinish();
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    fprintf(cvpi_log_file, "%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    return CVPI_FALSE;
  }
  VGImage input_image = vgCreateImage(CVPI_COLOR_SPACE, width, height,VG_IMAGE_QUALITY_NONANTIALIASED);
  vgFinish();
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    fprintf(cvpi_log_file, "%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    vgDestroyImage(dummy);
    vgFlush();
    return CVPI_FALSE;
  }
  VGImage image_correct = vgCreateImage(CVPI_COLOR_SPACE, width, height,VG_IMAGE_QUALITY_NONANTIALIASED);
  vgFinish();
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    fprintf(cvpi_log_file, "%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    vgDestroyImage(dummy);
    vgDestroyImage(input_image);
    vgFlush();
    return CVPI_FALSE;
  }

  vgColorMatrixNormal(input_image, dummy, image_color);
  vgColorMatrixNormal(image_correct, dummy, image_correct_color);

  /* all channels should contain 1's */
  VGImage first_image = cvpi_add_channels_all(input_image, 3, 2, 1, 1, 1, VG_RED|VG_BLUE|VG_GREEN|VG_ALPHA);
  vgFinish();
  if(first_image == VG_INVALID_HANDLE) {
    fprintf(cvpi_log_file, "%s:%d:Unable to add channels\n", __func__, __LINE__);
    vgDestroyImage(dummy);
    vgDestroyImage(input_image);
    vgDestroyImage(image_correct);
    vgFlush();
    return CVPI_FALSE;
  }

  /* green and blue channels should contain 8's and red and blue contain 1's */
  VGImage second_image = cvpi_add_channels_all(first_image, 2, 3, 2, 1, 0, VG_GREEN | VG_ALPHA);
  vgFinish();
  if(second_image == VG_INVALID_HANDLE) {
    fprintf(cvpi_log_file, "%s:%d:Unable to add channels\n", __func__, __LINE__);
    vgDestroyImage(dummy);
    vgDestroyImage(input_image);
    vgDestroyImage(image_correct);
    vgDestroyImage(first_image);
    vgFlush();
    return CVPI_FALSE;
  }

  size_t image_size = width*height*CVPI_PIXEL_BYTES;
  if(image_size <= 0) {
    fprnitf(cvpi_log_file, "%s:%d:malloc size = 0\n", __func__, __LINE__);
    vgDestroyImage(dummy);
    vgDestroyImage(input_image);
    vgDestroyImage(image_correct);
    vgDestroyImage(first_image);
    vgDestroyImage(second_image);
    vgFlush();
    return CVPI_FALSE;
  }
  void* added_data = malloc(image_size);
  if(added_data == NULL) {
    PRINT_MALLOC_ERROR;
    vgDestroyImage(dummy);
    vgDestroyImage(input_image);
    vgDestroyImage(image_correct);
    vgDestroyImage(first_image);
    vgDestroyImage(second_image);
    vgFlush();
    return CVPI_FALSE;
  }
  void* correct_data = malloc(image_size);
  if(correct_data == NULL) {
    PRINT_MALLOC_ERROR;
    vgDestroyImage(dummy);
    vgDestroyImage(input_image);
    vgDestroyImage(image_correct);
    vgDestroyImage(first_image);
    vgDestroyImage(second_image);
    vgFlush();
    free(added_data);
    return CVPI_FALSE;
  }

  vgGetImageSubData(second_image, added_data, CVPI_PIXEL_BYTES*width, CVPI_COLOR_SPACE, 0, 0, width, height);
  vgGetImageSubData(image_correct, correct_data, CVPI_PIXEL_BYTES*width, CVPI_COLOR_SPACE, 0, 0, width, height); 

  vgFinish();

  vgDestroyImage(dummy);
  vgDestroyImage(input_image);
  vgDestroyImage(image_correct);
  vgDestroyImage(first_image);
  vgDestroyImage(second_image);

  vgFlush();

  int cmp = memcmp(added_data, correct_data, width*height*CVPI_PIXEL_BYTES);
  CVPI_BOOL return_value = CVPI_FALSE;
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
    char* file_path = malloc(sizeof(*file_path) * path_length);
    if(file_path == NULL) {
      PRINT_MALLOC_ERROR;
      goto test_cvpi_add_channels_all_free;
    }

    int file_path_print = sprintf(file_path, "%s%s", IMAGES_TMP_DIR, file_name);
    if(file_path_print < 0) {
      fprintf(cvpi_log_file, "%s:%d:error writing file path: %d\n", __func__, __LINE__, file_path_print);
      free(file_path);
      goto test_cvpi_add_channels_all_free;
    }

    FILE* output = fopen(file_path, "w");
    if(output == NULL) {
      fprintf(cvpi_log_file, "%s: Error opening %s: errno = %d\n", __func__, file_path, errno);
      free(file_path);
      goto test_cvpi_add_channels_all_free;
    }
    unsigned long fwrite_size = width*height*CVPI_PIXEL_BYTES;
    size_t written = fwrite(added_data, fwrite_size, 1, output);
    int flushed = fflush(output);
    if(written != fwrite_size || flushed != 0) {
      if(written != fwrite_size) {
	fprintf(cvpi_log_file, "%s: Error writing file: fwrite size discrepancy\nexpected:%d\nreturned:%d.\n",
		__func__, fwrite_size, written);
      } else {
	fprintf(cvpi_log_file, "%s: Error writing file: fflush\nerrno = %d\nreturned = %d\n",
		__func__, errno, flushed);
      }
    }

    fclose(output);

    free(file_path);
  } else {
    fprintf(cvpi_log_file, "%s:%d: file path length = 0\n", __func__, __LINE__);
  }
 test_cvpi_add_channels_all_free:
  free(added_data);
  free(correct_data);

  return return_value;
}

CVPI_BOOL test_cvpi_combine_channelwise(void) {
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

  VGImage dummy = vgCreateImage(CVPI_COLOR_SPACE, width, height,VG_IMAGE_QUALITY_NONANTIALIASED);
  vgFinish();
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    fprintf(cvpi_log_file, "%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    return CVPI_FALSE;
  }
  VGImage image1 = vgCreateImage(CVPI_COLOR_SPACE, width, height,VG_IMAGE_QUALITY_NONANTIALIASED);
  vgFinish();
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    fprintf(cvpi_log_file, "%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    vgDestroyImage(dummy);
    vgFlush();
    return CVPI_FALSE;
  }
  VGImage image2 = vgCreateImage(CVPI_COLOR_SPACE, width, height,VG_IMAGE_QUALITY_NONANTIALIASED);
  vgFinish();
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    fprintf(cvpi_log_file, "%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    vgDestroyImage(dummy);
    vgDestroyImage(image1);
    vgFlush();
    return CVPI_FALSE;
  }
  VGImage image_correct = vgCreateImage(CVPI_COLOR_SPACE, width, height,VG_IMAGE_QUALITY_NONANTIALIASED);
  vgFinish();
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    fprintf(cvpi_log_file, "%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    vgDestroyImage(dummy);
    vgDestroyImage(image1);
    vgDestroyImage(image2);
    vgFlush();
    return CVPI_FALSE;
  }

  vgColorMatrixNormal(image1, dummy, image1_color);
  vgColorMatrixNormal(image2, dummy, image2_color);
  vgColorMatrixNormal(image_correct, dummy, image_correct_color);

  VGImage output_image = cvpi_combine_channelwise(image1, image2, VG_RED|VG_GREEN);
  vgFinish();
  if(output_image == VG_INVALID_HANDLE) {
    fprintf(cvpi_log_file, "%s:%d:Unable to combine channels\n", __func__, __LINE__);
    vgDestroyImage(dummy);
    vgDestroyImage(image1);
    vgDestroyImage(image2);
    vgDestroyImage(image_correct);
    vgFlush();
    return CVPI_FALSE;
  }

  size_t image_size = width*height*CVPI_PIXEL_BYTES;
  if(image_size <= 0) {
    fprnitf(cvpi_log_file, "%s:%d:malloc size = 0\n", __func__, __LINE__);
    vgDestroyImage(dummy);
    vgDestroyImage(image1);
    vgDestroyImage(image2);
    vgDestroyImage(image_correct);
    vgDestroyImage(output_image);
    vgFlush();
    return CVPI_FALSE;
  }
  void* added_data = malloc(image_size);
  if(added_data == NULL) {
    PRINT_MALLOC_ERROR;
    vgDestroyImage(dummy);
    vgDestroyImage(image1);
    vgDestroyImage(image2);
    vgDestroyImage(image_correct);
    vgDestroyImage(output_image);
    vgFlush();
    return CVPI_FALSE;
  }
  void* correct_data = malloc(image_size);
  if(correct_data == NULL) {
    PRINT_MALLOC_ERROR;
    vgDestroyImage(dummy);
    vgDestroyImage(image1);
    vgDestroyImage(image2);
    vgDestroyImage(image_correct);
    vgDestroyImage(output_image);
    vgFlush();
    free(added_data);
    return CVPI_FALSE;
  }

  vgGetImageSubData(output_image, added_data, CVPI_PIXEL_BYTES*width, CVPI_COLOR_SPACE, 0, 0, width, height);
  vgGetImageSubData(image_correct, correct_data, CVPI_PIXEL_BYTES*width, CVPI_COLOR_SPACE, 0, 0, width, height); 

  vgFinish();

  vgDestroyImage(dummy);
  vgDestroyImage(image_correct);
  vgDestroyImage(image1);
  vgDestroyImage(image2);
  vgDestroyImage(output_image);

  vgFlush();

  int cmp = memcmp(added_data, correct_data, image_size);
  CVPI_BOOL return_value = CVPI_FALSE;
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
    char* file_path = malloc(sizeof(*file_path) * path_length);
    if(file_path == NULL) {
      PRINT_MALLOC_ERROR;
      goto test_cvpi_combine_channelwise_free;
    }
    int file_path_print = sprintf(file_path, "%s%s", IMAGES_TMP_DIR, file_name);
    if(file_path_print < 0) {
      fprintf(cvpi_log_file, "%s:%d:error writing file path: %d\n", __func__, __LINE__, file_path_print);
      free(file_path);
      goto test_cvpi_combine_channelwise_free;
    }
    FILE* output = fopen(file_path, "w");
    if(output == NULL) {
      fprintf(cvpi_log_file, "%s: Error opening %s: errno = %d\n", __func__, file_path, errno);
      free(file_path);
      goto test_cvpi_combine_channelwise_free;
    }
    unsigned long fwrite_size = width*height*CVPI_PIXEL_BYTES;
    size_t written = fwrite(added_data, fwrite_size, 1, output);
    int flushed = fflush(output);
    if(written != fwrite_size || flushed != 0) {
      if(written != fwrite_size) {
	fprintf(cvpi_log_file, "%s: Error writing file: fwrite size discrepancy\nexpected:%d\nreturned:%d.\n",
		__func__, fwrite_size, written);
      } else {
	fprintf(cvpi_log_file, "%s: Error writing file: fflush\nerrno = %d\nreturned = %d\n",
		__func__, errno, flushed);
      }
    }
    fclose(output);

    free(file_path);
  } else {
    fprintf(cvpi_log_file, "%s:%d: file path length = 0\n", __func__, __LINE__);
  }
 test_cvpi_combine_channelwise_free:
  free(added_data);
  free(correct_data);

  return return_value;
}

static CVPI_BOOL test_cvpi_channel_threshold_common(char* input_file_name, char* output_file_name,
						    int width, int height, 
						    VGImageChannel channel, VGubyte bound1, VGubyte bound2, 
						    VGubyte fill, CVPI_BOOL invert) {
  size_t path_size = sizeof(*input_file_name) * (strlen(IMAGES_CORRECT_DIR) + strlen(input_file_name));
  if(path_size <= 0) {
    fprnitf(cvpi_log_file, "%s:%d:malloc size = 0\n", __func__, __LINE__);
    return CVPI_FALSE;
  }
  char* input_path = malloc(path_size);
  if(input_path == NULL) {
    PRINT_MALLOC_ERROR;
    return CVPI_FALSE;
  }
  int file_path_print = sprintf(input_path, "%s%s", IMAGES_CORRECT_DIR, input_file_name);
  if(file_path_print < 0) {
    fprintf(cvpi_log_file, "%s:%d:error writing file path: %d\n", __func__, __LINE__, file_path_print);
    free(input_path);
    return CVPI_FALSE;
  }

  FILE* input_file = fopen(input_path, "r");
  if(input_file == NULL) {
    fprintf(cvpi_log_file, "%s:%d: Error opening %s: errno = %d\n", __func__, __LINE__, input_path, errno);
    free(input_path);
    return CVPI_FALSE;
  }

  fseek(input_file, 0L, SEEK_END);
  unsigned long input_file_size = ftell(input_file);
  fseek(input_file, 0L, SEEK_SET);

  if(input_file_size <= 0) {
    fprnitf(cvpi_log_file, "%s:%d:malloc size = 0\n", __func__, __LINE__);
    fclose(input_file);
    free(input_path);
    return CVPI_FALSE;
  }

  void* input_data = malloc(input_file_size);
  if(input_data == NULL) {
    PRINT_MALLOC_ERROR;
    fclose(input_file);
    free(input_path);
    return CVPI_FALSE;
  }

  if(input_file_size != fread(input_data, 1, input_file_size, input_file)) {
    fprintf(stderr, "%s:%d: Reading the input image from memory failed.\n",__func__, __LINE__);
    free(input_data);
    free(input_path);
    fclose(input_file);
    return CVPI_FALSE;
  }

  size_t output_path_size = sizeof(*output_file_name) * (strlen(IMAGES_TMP_DIR) + strlen(output_file_name));
  if(output_path_size <= 0) {
    fprnitf(cvpi_log_file, "%s:%d:malloc size = 0\n", __func__, __LINE__);
    free(input_data);
    free(input_path);
    fclose(input_file);
    return CVPI_FALSE;
  }
  char* output_path = malloc(output_path_size);
  if(output_path == NULL) {
    PRINT_MALLOC_ERROR;
    free(input_data);
    free(input_path);
    fclose(input_file);
    return CVPI_FALSE;
  }

  file_path_print = sprintf(output_path, "%s%s", IMAGES_TMP_DIR, output_file_name);
  if(file_path_print < 0) {
    fprintf(cvpi_log_file, "%s:%d:error writing file path: %d\n", __func__, __LINE__, file_path_print);
    free(output_path);
    free(input_data);
    free(input_path);
    fclose(input_file);
    return CVPI_FALSE;
  }

  VGImage image = vgCreateImage(CVPI_COLOR_SPACE, width, height,VG_IMAGE_QUALITY_NONANTIALIASED);
  vgFinish();
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    fprintf(cvpi_log_file, "%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    free(output_path);
    free(input_data);
    free(input_path);
    fclose(input_file);
    return CVPI_FALSE;
  }

  vgImageSubData(image, input_data, CVPI_PIXEL_BYTES*width, CVPI_COLOR_SPACE, 0, 0, width, height);
  vgFinish();
  VGImage thresholded = cvpi_channel_threshold(image, channel, bound1, bound2, fill, invert);
  vgFinish();
  if(thresholded == VG_INVALID_HANDLE) {
    fprintf(cvpi_log_file, "%s:%d:Unable to threshold channels\n", __func__, __LINE__);
    free(output_path);
    free(input_data);
    free(input_path);
    fclose(input_file);
    vgDestroyImage(image);
    vgFlush();
    return CVPI_FALSE;
  }

  vgDestroyImage(image);
  size_t output_size = width*height*CVPI_PIXEL_BYTES;
  if(output_size <= 0) {
    fprnitf(cvpi_log_file, "%s:%d:malloc size = 0\n", __func__, __LINE__);
    free(output_path);
    free(input_data);
    free(input_path);
    fclose(input_file);
    return CVPI_FALSE;
  }
  void* thresholded_data = malloc(output_size);
  if(thresholded_data == NULL) {
    PRINT_MALLOC_ERROR;
    free(output_path);
    free(input_data);
    free(input_path);
    fclose(input_file);
    return CVPI_FALSE;
  }

  vgGetImageSubData(thresholded, thresholded_data, width*CVPI_PIXEL_BYTES, CVPI_COLOR_SPACE, 0, 0, width, height);
  vgFinish();
  vgDestroyImage(thresholded);
  vgFlush();

  FILE* output_file = fopen(output_path, "w");
  if(output_file == NULL) {
    fprintf(cvpi_log_file, "%s:%d: Error opening %s: errno = %d\n", __func__, __LINE__, output_path, errno);
    free(thresholded_data);
    free(input_data);
    free(input_path);
    free(output_path);
    fclose(input_file);  
    return CVPI_FALSE;
  }  

  //cvpi_pbm_header_write(output_file, width, height);
  size_t written = fwrite(thresholded_data, output_size, 1, output_file);
  int flushed = fflush(output_file);
  if(written != output_size || flushed != 0) {
    if(written != output_size) {
      fprintf(cvpi_log_file, "%s: Error writing file: fwrite size discrepancy\nexpected:%d\nreturned:%d.\n",
	      __func__, output_size, written);
    } else {
      fprintf(cvpi_log_file, "%s: Error writing file: fflush\nerrno = %d\nreturned = %d\n",
	      __func__, errno, flushed);
    }
  }

  free(thresholded_data);
  free(input_data);
  free(input_path);
  free(output_path);
  fclose(input_file);  
  fclose(output_file);  

  return CVPI_TRUE;
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

static CVPI_BOOL test_cvpi_image_threshold_common(const char* input_file_name, const char* output_file_name,
						  const int width, const int height, 
						  const VGubyte* bound1, const VGubyte* bound2, 
						  const VGubyte* fill, const CVPI_BOOL* invert, 
						  const CVPI_BOOL dependent) {
  size_t input_path_size = sizeof(*input_file_name) * (strlen(IMAGES_CORRECT_DIR) + strlen(input_file_name));
  if(input_path_size <= 0) {
    fprnitf(cvpi_log_file, "%s:%d:malloc size = 0\n", __func__, __LINE__);
    return CVPI_FALSE;
  }
  char* input_path = malloc(input_path_size);
  if(input_path == NULL) {
    PRINT_MALLOC_ERROR;
    return CVPI_FALSE;
  }
  int input_path_print = sprintf(input_path, "%s%s", IMAGES_CORRECT_DIR, input_file_name);
  if(input_path_print < 0) {
    fprintf(cvpi_log_file, "%s:%d:error writing file path: %d\n", __func__, __LINE__, input_path);
    free(input_path);
    return CVPI_FALSE;
  }
  FILE* input_file = fopen(input_path, "r");
  if(input_file == NULL) {
    fprintf(cvpi_log_file, "%s:%d: Error opening %s: errno = %d\n", __func__, __LINE__, input_path, errno);
    free(input_path);
    return CVPI_FALSE;
  }

  fseek(input_file, 0L, SEEK_END);
  unsigned long input_file_size = ftell(input_file);
  fseek(input_file, 0L, SEEK_SET);

  if(input_file_size <= 0) {
    fprnitf(cvpi_log_file, "%s:%d:malloc size = 0\n", __func__, __LINE__);
    free(input_path);
    fclose(input_file);
    return CVPI_FALSE;
  }
  void* input_data = malloc(input_file_size);
  if(input_data == NULL) {
    PRINT_MALLOC_ERROR;
    free(input_path);
    fclose(input_file);
    return CVPI_FALSE;
  }

  if(input_file_size != fread(input_data, 1, input_file_size, input_file)) {
    fprintf(cvpi_log_file, "%s: Reading the input image from memory failed.\n",__func__);
    free(input_path);
    free(input_data);
    fclose(input_file);
    return CVPI_FALSE;
  }
  size_t output_path_size = sizeof(*output_file_name) * (strlen(IMAGES_TMP_DIR) + strlen(output_file_name));
  if(output_path_size <= 0) {
    fprnitf(cvpi_log_file, "%s:%d:malloc size = 0\n", __func__, __LINE__);
    free(input_path);
    free(input_data);
    fclose(input_file);
    return CVPI_FALSE;
  }
  char* output_path = malloc(output_path_size);
  if(output_path == NULL) {
    PRINT_MALLOC_ERROR;
    free(input_path);
    free(input_data);
    fclose(input_file);
    return CVPI_FALSE;
  }
  int file_path_print = sprintf(output_path, "%s%s", IMAGES_TMP_DIR, output_file_name);
  if(file_path_print < 0) {
    fprintf(cvpi_log_file, "%s:%d:error writing file path: %d\n", __func__, __LINE__, file_path_print);
    free(input_path);
    free(input_data);
    free(output_path);
    fclose(input_file);
    return CVPI_FALSE;
  }
  VGImage image = vgCreateImage(CVPI_COLOR_SPACE, width, height,VG_IMAGE_QUALITY_NONANTIALIASED);
  vgFinish();
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    fprintf(cvpi_log_file, "%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    free(input_path);
    free(input_data);
    free(output_path);
    fclose(input_file);
    return CVPI_FALSE;
  }
  vgImageSubData(image, input_data, CVPI_PIXEL_BYTES*width, CVPI_COLOR_SPACE, 0, 0, width, height);
  vgFinish();
  VGImage thresholded = cvpi_image_threshold(image, bound1, bound2, fill, invert, dependent);
  vgFinish();
  if(thresholded == VG_INVALID_HANDLE) {
    fprintf(cvpi_log_file, "%s:%d:Unable to threshold image\n", __func__, __LINE__);
    vgDestroyImage(image);
    vgFlush();
    free(input_path);
    free(input_data);
    free(output_path);
    fclose(input_file);
    return CVPI_FALSE;
  }
  vgDestroyImage(image);

  void* thresholded_data = malloc(height*width*CVPI_PIXEL_BYTES);
  vgGetImageSubData(thresholded, thresholded_data, width*CVPI_PIXEL_BYTES, CVPI_COLOR_SPACE, 0, 0, width, height);
  vgFinish();
  vgDestroyImage(thresholded);
  vgFlush();

  FILE* output_file = fopen(output_path, "w");
  //cvpi_pbm_header_write(output_file, width, height);
  fwrite(thresholded_data, width*height*CVPI_PIXEL_BYTES, 1, output_file);
  fflush(output_file);

  free(input_path);
  free(output_path);
  free(thresholded_data);
  free(input_data);
  fclose(input_file);  
  fclose(output_file);  

  return CVPI_TRUE;
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
static CVPI_BOOL test_cvpi_image_threshold_sector_common(const char* input_file_name, const char* output_file_name,
							 const int width, const int height, 
							 const unsigned int sector_width, 
							 const unsigned int sector_height,
							 const VGubyte* fill, const CVPI_BOOL* invert,
							 const CVPI_BOOL dependent,
							 const VGImage (*statistic)(const VGImage image)) {
  if(width <= 0 || height <= 0 || sector_height <= 0 || sector_width <= 0
     || fill == NULL || invert == NULL || statistic == NULL || 
     input_file_name == NULL || output_file_name == NULL) {
    fprintf(stderr, "%s: invalid parameter\n", __func__);
    return CVPI_FALSE;
  }
  if(strlen(IMAGES_CORRECT_DIR) <= 0 || strlen(input_file_name) <= 0) {
    fprintf(stderr, "%s: invalid file name: %s\n", __func__, input_file_name);
    return CVPI_FALSE;
  }
  if(strlen(IMAGES_TMP_DIR) <= 0 || strlen(input_file_name) <= 0) {
    fprintf(stderr, "%s: invalid file name: %s\n", __func__, output_file_name);
    return CVPI_FALSE;
  }
  char* input_path = malloc(sizeof(*input_path) * (strlen(IMAGES_CORRECT_DIR) + strlen(input_file_name)));
  if(input_path == NULL) {
    fprintf(stderr, "%s: Error allocating memory for input_path: errno = %d\n", __func__, errno);
    return CVPI_FALSE;
  }
  sprintf(input_path, "%s%s", IMAGES_CORRECT_DIR, input_file_name);
  FILE* input_file = fopen(input_path, "r");
  if(input_file == NULL) {
    free(input_path);
    fprintf(stderr, "%s: error reading file %s: errno = %d\n", __func__, input_file_name, errno);
    return CVPI_FALSE;
  }
  fseek(input_file, 0L, SEEK_END);
  unsigned long input_file_size = ftell(input_file);
  fseek(input_file, 0L, SEEK_SET);
  
  if(input_file_size <= 0) {
    fprintf(stderr, "%s: Input file (%s) size is 0\n", __func__, input_file_name);
    free(input_path);
    fclose(input_file);
    return CVPI_FALSE;
  }
  void* input_data = malloc(input_file_size);
  if(input_data == NULL || input_file_size != fread(input_data, 1, input_file_size, input_file)) {
    fprintf(stderr, "%s: Reading the input image from memory failed.\n",__func__);
    free(input_data);
    free(input_path);
    fclose(input_file);
    return CVPI_FALSE;
  }

  char* output_path = malloc(sizeof(*output_path) * (strlen(IMAGES_TMP_DIR) + strlen(output_file_name)));
  if(output_path == NULL) {
    fprintf(stderr, "%s: Error allocating memory for output_path: errno = %d\n", __func__, errno);
    free(input_data);
    free(input_path);
    fclose(input_file);
    return CVPI_FALSE;
  }
  sprintf(output_path, "%s%s", IMAGES_TMP_DIR, output_file_name);

  VGImage image = vgCreateImage(CVPI_COLOR_SPACE, width, height,VG_IMAGE_QUALITY_NONANTIALIASED);
  vgFinish();
  vgImageSubData(image, input_data, CVPI_PIXEL_BYTES*width, CVPI_COLOR_SPACE, 0, 0, width, height);
  vgFinish();
  VGImage thresholded = cvpi_image_threshold_sector(image, sector_width, sector_height, 
						    fill, invert, dependent, statistic);
  vgFinish();
  vgDestroyImage(image);

  unsigned long fwrite_size = width*height*CVPI_PIXEL_BYTES;
  if(fwrite_size <= 0) {
    fprnitf(cvpi_log_file, "%s:%d:malloc size = 0\n", __func__, __LINE__);
    goto test_cvpi_image_threshold_sector_common_thresholded_data_error;
  }
  void* thresholded_data = malloc(fwrite_size);
  if(thresholded_data == NULL) {
    fprintf(cvpi_log_file, "%s: Error allocating memory for thresholded_data: errno = %d\n", __func__, errno);
    goto test_cvpi_image_threshold_sector_common_thresholded_data_error;
  }
  vgGetImageSubData(thresholded, thresholded_data, width*CVPI_PIXEL_BYTES, CVPI_COLOR_SPACE, 0, 0, width, height);
  vgFinish();

  FILE* output_file = fopen(output_path, "w");
  if(output_file == NULL) {
    fprintf(cvpi_log_file, "%s:%d: Error opening %s: errno = %d\n", __func__, __LINE__, output_file_name, errno);
    goto test_cvpi_image_threshold_sector_common_thresholded_data_error;
  }
  //cvpi_pbm_header_write(output_file, width, height);
  fwrite(thresholded_data, fwrite_size, 1, output_file);
  fflush(output_file);

  fclose(output_file);  
  free(thresholded_data);
 test_cvpi_image_threshold_sector_common_thresholded_data_error:
  vgDestroyImage(thresholded);
  vgFlush();
  free(input_data);
  free(input_path);
  free(output_path);
  fclose(input_file);  

  return CVPI_TRUE;  
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

/* cvpi_image_channel_mask */
static CVPI_BOOL test_cvpi_image_channel_mask_common(char* output_file_name,
						     VGImageChannel maskChannel, CVPI_BOOL add) {
  cvpi_pixel image_data[2];
  image_data[0].channel[cvpi_pixel_red] = 3;
  image_data[0].channel[cvpi_pixel_green] = 2;
  image_data[0].channel[cvpi_pixel_blue] = 1;
  image_data[0].channel[cvpi_pixel_alpha] = 0;
  image_data[1].channel[cvpi_pixel_red] = 3;
  image_data[1].channel[cvpi_pixel_green] = 2;
  image_data[1].channel[cvpi_pixel_blue] = 1;
  image_data[1].channel[cvpi_pixel_alpha] = ~0;

  VGImage input_image = vgCreateImage(CVPI_COLOR_SPACE, 2, 1, VG_IMAGE_QUALITY_NONANTIALIASED);
  vgFinish();
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    fprintf(cvpi_log_file, "%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    return CVPI_FALSE;
  }
  vgImageSubData(input_image, image_data, 2*CVPI_PIXEL_BYTES, CVPI_COLOR_SPACE, 0,0, 2, 1);
  vgFinish();

  VGImage output_image = cvpi_image_channel_mask(input_image, maskChannel, add);
  vgFinish();
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    fprintf(cvpi_log_file, "%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    vgDestroyImage(input_image);
    vgFlush();
    return CVPI_FALSE;
  }
  vgDestroyImage(input_image);
  vgGetImageSubData(output_image, image_data, 2*CVPI_PIXEL_BYTES, CVPI_COLOR_SPACE, 0, 0, 2, 1);
  vgFinish(); 

  size_t output_path_length = sizeof(*output_file_name) * (strlen(IMAGES_TMP_DIR) + strlen(output_file_name));
  char* output_path = malloc(output_path_length);
  if(output_path == NULL) {
    PRINT_MALLOC_ERROR;
    vgDestroyImage(output_image);
    vgFlush();
    return CVPI_FALSE;
  }
  int file_path_print = sprintf(output_path, "%s%s", IMAGES_TMP_DIR, output_file_name);
  if(file_path_print < 0) {
    fprintf(cvpi_log_file, "%s:%d:error writing file path: %d\n", __func__, __LINE__, file_path_print);
    vgDestroyImage(output_image);
    vgFlush();
    free(output_path);
    return CVPI_FALSE;
  }
  FILE* output_file = fopen(output_path, "w");
  if(output_file == NULL) {
    fprintf(cvpi_log_file, "%s: Error opening %s: errno = %d\n", __func__, output_path, errno);
    vgDestroyImage(output_image);
    vgFlush();
    free(output_path);
    return CVPI_FALSE;
  }
  int fwrite_size = 2*CVPI_PIXEL_BYTES;
  size_t written = fwrite(image_data, fwrite_size, 1, output_file);
  int flushed = fflush(output_file);
  if(written != fwrite_size || flushed != 0) {
    if(written != fwrite_size) {
      fprintf(cvpi_log_file, "%s: Error writing file: fwrite size discrepancy\nexpected:%d\nreturned:%d.\n",
	      __func__, fwrite_size, written);
    } else {
      fprintf(cvpi_log_file, "%s: Error writing file: fflush\nerrno = %d\nreturned = %d\n",
	      __func__, errno, flushed);
    }
  }
  fclose(output_file);
  free(output_path);
  vgDestroyImage(output_image);
  return CVPI_TRUE;  
}
CVPI_BOOL test_cvpi_image_channel_mask_add(void) {
  return test_cvpi_image_channel_mask_common("test_cvpi_image_channel_mask_add", VG_ALPHA, CVPI_TRUE);
}
CVPI_BOOL test_cvpi_image_channel_mask_subtract(void) {
  return test_cvpi_image_channel_mask_common("test_cvpi_image_channel_mask_subtract", VG_ALPHA, CVPI_FALSE);
}

/* static CVPI_BOOL test_cvpi_image_common(int width, int height, VGfloat* image_color) { */
/*   VGImage dummy = vgCreateImage(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED); */
/*   VGImage input = vgCreateImage(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED); */
/*   VGImage image_correct = vgCreateImage(CVPI_COLOR_SPACE, 1, 1, VG_IMAGE_QUALITY_NONANTIALIASED); */

/*   vgColorMatrixNormal(input, dummy, image_color); */
/*   vgCopyImage(image_correct, 0, 0, input, 0,0, 1, 1, VG_FALSE); */
/*   vgFinish(); */
/*   VGImage output = cvpi_image_mean(input); */
/*   vgFinish(); */
  
/*   void* averaged_data = malloc(width*height*CVPI_PIXEL_BYTES); */
/*   void* correct_data = malloc(width*height*CVPI_PIXEL_BYTES); */
/*   vgGetImageSubData(output, averaged_data, CVPI_PIXEL_BYTES*width, CVPI_COLOR_SPACE, 0, 0, width, height); */
/*   vgGetImageSubData(image_correct, correct_data, CVPI_PIXEL_BYTES*width, CVPI_COLOR_SPACE, 0, 0, width, height); */

/*   VGint image_width = vgGetParameteri(output, VG_IMAGE_WIDTH); */
/*   VGint image_height = vgGetParameteri(output, VG_IMAGE_HEIGHT); */

/*   vgDestroyImage(dummy); */
/*   vgDestroyImage(input); */
/*   vgDestroyImage(output); */
/*   vgDestroyImage(image_correct); */
/*   CVPI_BOOL return_value = CVPI_FALSE; */
/*   if(image_width > 1 || image_height > 1) { */
/*     fprintf(stderr, "image_mean width: %d height: %d\n", image_width, image_height); */
/*     goto test_cvpi_image_common_free; */
/*   } */

/*   int cmp = memcmp(averaged_data, correct_data, CVPI_PIXEL_BYTES); */
/*   if(cmp == 0) { */
/*     return_value = CVPI_TRUE; */
/*   } else if(cmp > 0) { */
/*     fprintf(stderr, "%s: averaged_data is greater than the correct data.\n", __func__); */
/*     fprintf(stderr, "average: %08X\ncorrect: %08X\n", *((unsigned long*)averaged_data), *((unsigned long*)correct_data)); */
/*   } else { */
/*     fprintf(stderr, "%s: averaged_data is less than the correct data.\n", __func__); */
/*     fprintf(stderr, "average: %08X\ncorrect: %08X\n", *((unsigned long*)averaged_data), *((unsigned long*)correct_data)); */
/*   } */
/*  test_cvpi_image_common_free: */
/*   free(correct_data); */
/*   free(averaged_data); */

/*   return return_value; */
/* } */
/* CVPI_BOOL test_cvpi_image_mean_small(void) { */
/*   VGfloat image_color[20] = { */
/*     0,0,0,0, */
/*     0,0,0,0, */
/*     0,0,0,0, */
/*     0,0,0,0, */

/*     20,20,20,20 */
/*   }; */

/*   int width = 1; */
/*   int height = 1; */

/*   return test_cvpi_image_common(width, height, image_color); */
/* } */

/* CVPI_BOOL test_cvpi_image_mean_max(void) { */
/*   VGfloat image_color[20] = { */
/*     0,0,0,0, */
/*     0,0,0,0, */
/*     0,0,0,0, */
/*     0,0,0,0, */

/*     255,255,255,255 */
/*   }; */

/*   int width = 10; */
/*   int height = 10; */

/*   return test_cvpi_image_common(width, height, image_color); */
/* } */

/* CVPI_BOOL test_cvpi_image_mean_huge(void) { */
/*   VGfloat image_color[20] = { */
/*     0,0,0,0, */
/*     0,0,0,0, */
/*     0,0,0,0, */
/*     0,0,0,0, */

/*     255,255,255,255 */
/*   }; */

/*   int width = EGL_CONFIG_MAX_WIDTH; */
/*   int height = EGL_CONFIG_MAX_HEIGHT; */

/*   return test_cvpi_image_common(width, height, image_color); */
/* } */

/* CVPI_BOOL test_cvpi_image_mean_compare(void) { */
/*   /\* create a list of files to be read *\/ */
/*   DIR* directory; */
/*   char* dir_name = "/home/pi/cvpi/cvpi_image_mean_test_files/"; */

/*   /\* Open the directory. *\/ */
/*   directory = opendir(dir_name); */
/*   if (!directory) { */
/*     fprintf (stderr, "Cannot open directory '%s': %s\n", */
/* 	     dir_name, strerror(errno)); */
/*     return CVPI_FALSE; */
/*   } */

/*   /\* test each file *\/ */
/*   struct dirent * entry; */
/*   int width; */
/*   int height; */
/*   char* split_name[3]; */
/*   FILE* data_file; */
/*   FILE* output_file; */
/*   VGImage image; */
/*   VGImage mean_image; */
/*   VGubyte red_mean_calculated; */
/*   VGubyte green_mean_calculated; */
/*   VGubyte blue_mean_calculated; */
/*   VGubyte alpha_mean_calculated; */

/*   VGImage red_image; */
/*   VGImage green_image; */
/*   VGImage blue_image; */
/*   VGImage alpha_image; */

/*   char* file_path_full; */

/*   output_file = fopen(IMAGES_TMP_DIR "test_cvpi_image_mean_compare", "w"); */
/*   fprintf(output_file, "GPU|CPU|0|1|2|3|4|5|6|7|8|9|10|11|12|13|14|15|16|17|18|19|20|21|22|23|24|25|26|27|28|29|30|31|32|33|34|35|36|37|38|39|40|41|42|43|44|45|46|47|48|49|50|51|52|53|54|55|56|57|58|59|60|61|62|63|64|65|66|67|68|69|70|71|72|73|74|75|76|77|78|79|80|81|82|83|84|85|86|87|88|89|90|91|92|93|94|95|96|97|98|99|100|101|102|103|104|105|106|107|108|109|110|111|112|113|114|115|116|117|118|119|120|121|122|123|124|125|126|127|128|129|130|131|132|133|134|135|136|137|138|139|140|141|142|143|144|145|146|147|148|149|150|151|152|153|154|155|156|157|158|159|160|161|162|163|164|165|166|167|168|169|170|171|172|173|174|175|176|177|178|179|180|181|182|183|184|185|186|187|188|189|190|191|192|193|194|195|196|197|198|199|200|201|202|203|204|205|206|207|208|209|210|211|212|213|214|215|216|217|218|219|220|221|222|223|224|225|226|227|228|229|230|231|232|233|234|235|236|237|238|239|240|241|242|243|244|245|246|247|248|249|250|251|252|253|254|255\n"); */

/*   void* raw_data; */

/*   struct timespec start_time; */
/*   struct timespec end_time; */

/*   int i = 0; */

/*   while (1) { */
/*     entry = readdir(directory); */
/*     if (!entry) { */
/*       break; */
/*     } */

/*     file_path_full = malloc(sizeof(char) * (strlen(dir_name) + strlen(entry->d_name) + 1)); */
/*     strcpy(file_path_full, dir_name); */
/*     strcpy((file_path_full + strlen(dir_name)), entry->d_name); */
/*     char* f_name = entry->d_name; */
/*     split_name[0] = strtok(f_name, "_"); */
/*     split_name[1] = strtok(NULL, "_"); */
/*     split_name[2] = strtok(NULL, "_"); */
/*     width = strtol(split_name[1], NULL, 10); */

/*     if(errno == ERANGE) { */
/*       fprintf(stderr, "Error parsing width: %s\n", file_path_full); */
/*       continue; */
/*     } */

/*     height = strtol(split_name[2], NULL, 10); */

/*     if(errno == ERANGE) { */
/*       fprintf(stderr, "Error parsing height: %s\n", file_path_full); */
/*       continue; */
/*     } */

/*     data_file = fopen(file_path_full, "r"); */

/*     fseek(data_file, 0L, SEEK_END); */
/*     size_t data_file_size = ftell(data_file); */
/*     fseek(data_file, 0L, SEEK_SET); */

/*     raw_data = malloc(sizeof(*raw_data) * data_file_size); */

/*     size_t fread_ret_val; */
/*     if(/\* entry->d_reclen == *\/ fread_ret_val = fread(raw_data, 1, data_file_size, data_file)) { */
/*       image = vgCreateImage(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED); */
/*       mean_image = vgCreateImage(CVPI_COLOR_SPACE, 1, 1, VG_IMAGE_QUALITY_NONANTIALIASED); */
/*       red_image = vgCreateImage(CVPI_COLOR_SPACE, 1, 1, VG_IMAGE_QUALITY_NONANTIALIASED); */
/*       green_image = vgCreateImage(CVPI_COLOR_SPACE, 1, 1, VG_IMAGE_QUALITY_NONANTIALIASED); */
/*       blue_image = vgCreateImage(CVPI_COLOR_SPACE, 1, 1, VG_IMAGE_QUALITY_NONANTIALIASED); */
/*       alpha_image = vgCreateImage(CVPI_COLOR_SPACE, 1, 1, VG_IMAGE_QUALITY_NONANTIALIASED); */
/*       vgImageSubData(image, raw_data, CVPI_PIXEL_BYTES*width, CVPI_COLOR_SPACE, 0, 0, width, height); */

/*       unsigned int * red_hist = cvpi_channel_histogram(image, VG_RED); */
/*       unsigned int * green_hist = cvpi_channel_histogram(image, VG_GREEN); */
/*       unsigned int * blue_hist = cvpi_channel_histogram(image, VG_BLUE); */
/*       unsigned int * alpha_hist = cvpi_channel_histogram(image, VG_ALPHA); */

/*       clock_gettime(CLOCK_REALTIME, &start_time); */
/*       VGImage mean_image = cvpi_image_mean(image); */
/*       clock_gettime(CLOCK_REALTIME, &end_time); */

/*       vgColorMatrix(red_image, mean_image, cvpi_channel_red); */
/*       vgColorMatrix(green_image, mean_image, cvpi_channel_green); */
/*       vgColorMatrix(blue_image, mean_image, cvpi_channel_blue); */
/*       vgColorMatrix(alpha_image, mean_image, cvpi_channel_alpha); */

/*       vgGetImageSubData(red_image, &red_mean_calculated, CVPI_PIXEL_BYTES, CVPI_COLOR_SPACE, 0, 0, 1, 1); */
/*       vgGetImageSubData(green_image, &green_mean_calculated, CVPI_PIXEL_BYTES, CVPI_COLOR_SPACE, 0, 0, 1, 1); */
/*       vgGetImageSubData(blue_image, &blue_mean_calculated, CVPI_PIXEL_BYTES, CVPI_COLOR_SPACE, 0, 0, 1, 1); */
/*       vgGetImageSubData(alpha_image, &alpha_mean_calculated, CVPI_PIXEL_BYTES, CVPI_COLOR_SPACE, 0, 0, 1, 1); */
/*       vgFinish(); */

/*       /\* GPU *\/ */
/*       fprintf(output_file, "%lld %d %d %d %d|", (long long)(end_time.tv_nsec - start_time.tv_nsec), */
/* 	      red_mean_calculated, green_mean_calculated, blue_mean_calculated, alpha_mean_calculated); */

/*       unsigned long cpu_average_r = 0; */
/*       unsigned long cpu_average_g = 0; */
/*       unsigned long cpu_average_b = 0; */
/*       unsigned long cpu_average_a = 0; */
/*       /\* CPU *\/ */
/*       clock_gettime(CLOCK_REALTIME, &start_time); */
/*       for(i = 0; i < data_file_size; i+=4) { */
/* 	cpu_average_r += *((unsigned long*)(raw_data + i)); */
/* 	cpu_average_g += *((unsigned long*)(raw_data + i + 1)); */
/* 	cpu_average_b += *((unsigned long*)(raw_data + i + 2)); */
/* 	cpu_average_a += *((unsigned long*)(raw_data + i + 3)); */
/*       } */
/*       cpu_average_r /= data_file_size; */
/*       cpu_average_g /= data_file_size; */
/*       cpu_average_b /= data_file_size; */
/*       cpu_average_a /= data_file_size; */
/*       clock_gettime(CLOCK_REALTIME, &end_time); */

/*       fprintf(output_file, "%lld|", (long long)(end_time.tv_nsec - start_time.tv_nsec)); */

/*       for(i = 0; i < 255; ++i) { */
/* 	fprintf(output_file, "%d %d %d %d|", red_hist[i], green_hist[i], blue_hist[i], alpha_hist[i]); */
/*       } */
/*       fprintf(output_file, "%d %d %d %d\n", red_hist[i], green_hist[i], blue_hist[i], alpha_hist[i]); */
/*       vgDestroyImage(image); */
/*       vgDestroyImage(mean_image); */
/*       vgDestroyImage(red_image); */
/*       vgDestroyImage(green_image); */
/*       vgDestroyImage(blue_image); */
/*       vgDestroyImage(alpha_image); */
/*     } else { */
/*       fprintf(stderr, "Input data size did not match: %s\n", file_path_full); */
/*     } */

/*     free(raw_data); */
/*     fclose(data_file); */
/*     free(file_path_full); */
/*   } */
/*   fclose(output_file); */

/*   /\* Close the directory. *\/ */
/*   if(closedir(directory)) { */
/*     fprintf (stderr, "Could not close '%s': %s\n", */
/* 	     dir_name, strerror(errno)); */
/*   } */
/* } */

CVPI_BOOL test_cvpi_image_rgba_to_binary(void) {
  const int width = 1280;
  const int height = 960;

  char* input_path = IMAGES_CORRECT_DIR "mark1_black_yuva.yuv";
  FILE* input_file = fopen(input_path, "r");
  if(input_file == NULL) {
    fprintf(cvpi_log_file, "%s:%d: Error opening %s: errno = %d\n", __func__, __LINE__, input_path, errno);
    return CVPI_FALSE;
  }
  fseek(input_file, 0L, SEEK_END);
  unsigned long input_file_size = ftell(input_file);
  fseek(input_file, 0L, SEEK_SET);
  void* input_data = malloc(input_file_size);
  if(input_file_size != fread(input_data, 1, input_file_size, input_file)) {
    fprintf(stderr, "%s: Reading the input image from memory failed.\n",__func__);
    free(input_path);
    free(input_data);
    fclose(input_file);
    return CVPI_FALSE;
  }

  char* output_path = IMAGES_TMP_DIR "mark1_black_bw.pbm";
  VGImage image = vgCreateImage(CVPI_COLOR_SPACE, width, height,VG_IMAGE_QUALITY_NONANTIALIASED);
  vgFinish();
  vgImageSubData(image, input_data, CVPI_PIXEL_BYTES*width, CVPI_COLOR_SPACE, 0, 0, width, height);
  vgFinish();
  VGImage bw = cvpi_image_rgba_to_bw(image, VG_BLUE, CVPI_TRUE);
  vgFinish();
  vgDestroyImage(image);

  void* bw_data = malloc(height*width/8);
  vgGetImageSubData(bw, bw_data, width/8, VG_BW_1, 0, 0, width, height);
  vgFinish();
  vgDestroyImage(bw);
  vgFlush();

  FILE* output_file = fopen(output_path, "w");
  cvpi_pbm_header_write(output_file, width, height);
  fwrite(bw_data, width*height/8, 1, output_file);
  fflush(output_file);

  free(bw_data);
  free(input_path);
  free(output_path);
  free(input_data);
  fclose(input_file);  
  fclose(output_file);  

  return CVPI_TRUE;
}

static CVPI_BOOL test_cvpi_image_logical_common(
VGImage(*logical_function)(VGImage image1, VGImage image2, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true), 
cvpi_pixel* correct, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true) {
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

  VGImage image1 = vgCreateImage(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  VGImage image2 = vgCreateImage(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  vgImageSubData(image1, input1, CVPI_PIXEL_BYTES*width, CVPI_COLOR_SPACE, 0, 0, width, height);
  vgImageSubData(image2, input2, CVPI_PIXEL_BYTES*width, CVPI_COLOR_SPACE, 0, 0, width, height);
  vgFinish();

  VGImage output = logical_function(image1, image2, t_c, f_c, nonzero_true);
  vgFinish();
  vgGetImageSubData(output, input1, width*CVPI_PIXEL_BYTES, CVPI_COLOR_SPACE, 0, 0, width, height);
  vgFinish();
  vgDestroyImage(image1);
  vgDestroyImage(image2);
  vgDestroyImage(output);
  vgFlush();

  int cmp = memcmp(input1, correct, width*height*CVPI_PIXEL_BYTES);
  CVPI_BOOL return_value = CVPI_FALSE;
  if(cmp == 0) {
    return_value = CVPI_TRUE;
  } else if(cmp > 0) {
    fprintf(stderr, "%s: added_data is greater than the correct data.\n", __func__);
  } else {
    fprintf(stderr, "%s: added_data is less than the correct data.\n", __func__);
  }
  fprintf(stderr, "%08x %08x %08x %08x\n", input1[0].all, input1[1].all, input1[2].all, input1[3].all);

  return return_value;
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
  int width = 8;
  int height = 4;

  VGImage output_image = vgCreateImage(VG_BW_1, width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  VGImage input_image = vgCreateImage(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  
  vgImageSubData(input_image, &data, 1, VG_BW_1, 0, 0, width, height);

  vgFinish();

  VGImage intermediate_image = morph(input_image, t_c, f_c, nonzero_true);
  vgFinish();

  vgCopyImage(output_image, 0, 0, intermediate_image, 0, 0, width, height, VG_FALSE);

  uint32_t result;

  vgGetImageSubData(output_image, &result, 1, VG_BW_1, 0, 0, width, height);
  vgFinish();

  test_cvpi_image_morphology_print(data);
  test_cvpi_image_morphology_print(result);
  vgDestroyImage(output_image);
  vgDestroyImage(input_image);
  vgDestroyImage(intermediate_image);
  return CVPI_TRUE;
}

/* cvpi_image_dialate */
CVPI_BOOL test_cvpi_image_dialate(void) {
  VGImage (*morph)(VGImage image, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);
  morph = cvpi_image_dialate;
  return test_cvpi_image_morphology_common(test_cvpi_image_morphology_image_1, morph, 255, 0, CVPI_TRUE);
}
CVPI_BOOL test_cvpi_image_dialate_NOT(void) {
  VGImage (*morph)(VGImage image, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);
  morph = cvpi_image_dialate;
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

  VGImage input_image = vgCreateImage(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED);

  vgImageSubData(input_image, data, width*CVPI_PIXEL_BYTES, CVPI_COLOR_SPACE, 0, 0, width, height);

  vgFinish();
  VGubyte max = 99;
  VGubyte min = 99;
  VGubyte* max_min = NULL;
  if(func == max_function) {
    max = cvpi_channel_max(input_image, channel);
  } else if(func == min_function) {
    min = cvpi_channel_min(input_image, channel);
  } else {
    max_min = cvpi_channel_max_min(input_image, channel);
    max = max_min[0];
    min = max_min[1];
  }
  vgFinish();

  CVPI_BOOL return_value_max, return_value_min, return_value;

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
    return_value = CVPI_FALSE;
    goto test_cvpi_channel_max_min_common_end;
  }

  if(func == max_function) {
    return_value = return_value_max;
  } else if(func == min_function) {
    return_value = return_value_min;
  } else {
    return_value = CVPI_TRUE_TEST(return_value_max) && CVPI_TRUE_TEST(return_value_min);
  }
 test_cvpi_channel_max_min_common_end:
  vgDestroyImage(input_image);
  if(max_min != NULL) {
    free(max_min);
  }
  return return_value;
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

  VGImage input_image = vgCreateImage(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  vgImageSubData(input_image, &data, CVPI_PIXEL_BYTES * width, CVPI_COLOR_SPACE, 0, 0, width, height);

  unsigned int* histogram = cvpi_channel_histogram(input_image, channel);
  char* error_file_name;
  int name_length = strlen(IMAGES_TMP_DIR) + strlen("test_cvpi_channel_histogram_n");
  CVPI_BOOL return_value = CVPI_TRUE;
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
    return_value = CVPI_FALSE;
    goto test_cvpi_channel_histogram_common_end;
  }

  for(i = 0; i < 256; ++i) {
    if(histogram[i] != 1) {
      return_value = CVPI_FALSE;
    }
  }

  FILE* output = fopen(error_file_name, "w");
  for(i = 0; i < 256; ++i) {
    fprintf(output, "%d ", histogram[i]);
  }
  fclose(output);
 test_cvpi_channel_histogram_common_end:
  free(histogram);
  vgDestroyImage(input_image);
  return return_value;
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

  VGImage input_image = vgCreateImage(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  vgImageSubData(input_image, &data, CVPI_PIXEL_BYTES * width, CVPI_COLOR_SPACE, 0, 0, width, height);

  unsigned int* histogram = cvpi_color_channels_histogram(input_image);

  CVPI_BOOL return_value = CVPI_TRUE;

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

  FILE* output = fopen(error_file_name, "w");
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
  fclose(output);

  free(histogram);
  vgDestroyImage(input_image);
  return return_value;
}

CVPI_BOOL test_cvpi_image_histogram(void) {
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

  VGImage input_image = vgCreateImage(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  vgImageSubData(input_image, &data, CVPI_PIXEL_BYTES * width, CVPI_COLOR_SPACE, 0, 0, width, height);

  unsigned int* histogram = cvpi_image_histogram(input_image);

  CVPI_BOOL return_value = CVPI_TRUE;

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

  FILE* output = fopen(error_file_name, "w");
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
  fclose(output);

  free(histogram);
  vgDestroyImage(input_image);
  return return_value;
}

static CVPI_BOOL test_cvpi_channel_cumulative_distribution_common(VGImageChannel channel) {
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

  VGImage input_image = vgCreateImage(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  vgImageSubData(input_image, &data, CVPI_PIXEL_BYTES * width, CVPI_COLOR_SPACE, 0, 0, width, height);

  unsigned int* histogram = cvpi_channel_histogram(input_image, channel);
  double* cd = cvpi_channel_cumulative_distribution(histogram, width, height);

  char* error_file_name;
  int name_length = strlen(IMAGES_TMP_DIR) + strlen("test_cvpi_channel_histogram_n");
  CVPI_BOOL return_value = CVPI_TRUE;
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
    return_value = CVPI_FALSE;
    goto test_cvpi_channel_cumulative_distribution_common_end;
  }

  /* for(i = 0; i < 256; ++i) { */
  /*   if(histogram[i] != 1) { */
  /*     return_value = CVPI_FALSE; */
  /*   } */
  /* } */

  FILE* output = fopen(error_file_name, "w");
  for(i = 0; i < 256; ++i) {
    fprintf(output, "%f ", cd[i]);
  }
  fclose(output);
 test_cvpi_channel_cumulative_distribution_common_end:
  vgDestroyImage(input_image);
  free(histogram);
  free(cd);
  return return_value;
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

  VGImage input_image = vgCreateImage(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  vgImageSubData(input_image, &data, CVPI_PIXEL_BYTES * width, CVPI_COLOR_SPACE, 0, 0, width, height);

  unsigned int* histogram = cvpi_color_channels_histogram(input_image);
  double* cd = cvpi_color_channels_cumulative_distribution(histogram, width, height);


  CVPI_BOOL return_value = CVPI_TRUE;

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

  FILE* output = fopen(error_file_name, "w");
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
  fclose(output);
  vgDestroyImage(input_image);
  free(histogram);
  free(cd);
  return return_value;
}

CVPI_BOOL test_cvpi_image_cumulative_distribution(void) {
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

  VGImage input_image = vgCreateImage(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  vgImageSubData(input_image, &data, CVPI_PIXEL_BYTES * width, CVPI_COLOR_SPACE, 0, 0, width, height);

  unsigned int* histogram = cvpi_image_histogram(input_image);
  double* cd = cvpi_image_cumulative_distribution(histogram, width, height);

  CVPI_BOOL return_value = CVPI_TRUE;

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

  FILE* output = fopen(error_file_name, "w");
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
  fclose(output);
  vgDestroyImage(input_image);
  free(histogram);
  free(cd);
  return return_value;
}

/* cvpi_image_coordinate_table */
CVPI_BOOL test_cvpi_image_coordinate_table_zero(void) {
  int width = 4;
  int height = 1;
  cvpi_pixel* data = calloc(4, sizeof(*data));
  VGImage image = vgCreateImage(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  vgImageSubData(image, data, CVPI_PIXEL_BYTES * width, CVPI_COLOR_SPACE, 0, 0, width, height);  

  cvpi_coordinate_table* table = cvpi_image_coordinate_table(image,VG_RED|VG_GREEN|VG_BLUE|VG_ALPHA);

  CVPI_BOOL return_value = CVPI_FALSE;
  if(table->length == 0) {
    return_value = CVPI_TRUE;
  } else {
    fprintf(stderr, "%s: incorrect length: %l\n", __func__, table->length);
  }

  vgDestroyImage(image);

  free(data);
  cvpi_image_coordinate_table_destroy(table);

  return return_value;
}

CVPI_BOOL test_cvpi_image_coordinate_table_some(void) {
  int width = 4;
  int height = 1;
  cvpi_pixel* data = calloc(4, sizeof(*data));
  data[0].channel[cvpi_pixel_blue]= 1;
  data[2].channel[cvpi_pixel_red] = 1;
  data[3].channel[cvpi_pixel_alpha] = 1; /* should be ignored */

  VGImage image = vgCreateImage(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  vgImageSubData(image, data, CVPI_PIXEL_BYTES * width, CVPI_COLOR_SPACE, 0, 0, width, height);  

  cvpi_coordinate_table* table = cvpi_image_coordinate_table(image,VG_RED|VG_GREEN|VG_BLUE);

  CVPI_BOOL return_value = CVPI_FALSE;

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

  vgDestroyImage(image);

  free(data);
  cvpi_image_coordinate_table_destroy(table);

  return return_value;
}

CVPI_BOOL test_cvpi_image_coordinate_table_all(void) {
  int width = 2;
  int height = 2;
  cvpi_pixel* data = calloc(4, sizeof(*data));
  data[0].channel[cvpi_pixel_red] = 1;
  data[1].channel[cvpi_pixel_green] = 1;
  data[2].channel[cvpi_pixel_blue] = 1;
  data[3].channel[cvpi_pixel_alpha] = 1;

  VGImage image = vgCreateImage(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  vgImageSubData(image, data, CVPI_PIXEL_BYTES * width, CVPI_COLOR_SPACE, 0, 0, width, height);  

  cvpi_coordinate_table* table = cvpi_image_coordinate_table(image,VG_RED|VG_GREEN|VG_BLUE|VG_ALPHA);

  CVPI_BOOL return_value = CVPI_FALSE;

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

  vgDestroyImage(image);

  free(data);
  cvpi_image_coordinate_table_destroy(table);

  return return_value;
}

/* filters */
CVPI_BOOL test_filter_common(const VGshort* filter_x, const VGshort* filter_y, int f_width, int f_height,
			     const char* output_image_path) {
  const int width = 1280;
  const int height = 960;

  char* input_image_path = IMAGES_CORRECT_DIR "mark1_black_yuva.yuv";

  FILE* input_file = fopen(input_image_path, "r");
  FILE* output_file = fopen(output_image_path, "w");

  fseek(input_file, 0L, SEEK_END);
  int input_size = ftell(input_file);		/* input file size */
  fseek(input_file, 0L, SEEK_SET);

  void* data = malloc(input_size);

  fread(data, 1, input_size, input_file);

  VGImage input_image = vgCreateImage(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  VGImage morphed_image_x = vgCreateImage(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  VGImage morphed_image_y = vgCreateImage(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  
  vgImageSubData(input_image, data, width*CVPI_PIXEL_BYTES, CVPI_COLOR_SPACE, 0, 0, width, height);

  vgConvolve(morphed_image_x, input_image, f_width, f_height, 0, 0, filter_x, 1, 0, VG_TILE_FILL);
  vgConvolve(morphed_image_y, input_image, f_width, f_height, 0, 0, filter_y, 1, 0, VG_TILE_FILL);
  vgFinish();
  VGImage magnitude = cvpi_magnitude(morphed_image_x, morphed_image_y, cvpi_round);
  vgFinish();
  vgGetImageSubData(magnitude, data, width*CVPI_PIXEL_BYTES, CVPI_COLOR_SPACE, 0, 0, width, height);
  vgFinish();
  cvpi_bmp_header_write(output_file, width, height, 100, cvpi_bmp_sARGB_8888);
  fwrite(data, 1, input_size, output_file);
  fflush(output_file);
  fclose(output_file);
  fclose(input_file);
  free(data);

  vgDestroyImage(magnitude);
  vgDestroyImage(morphed_image_y);
  vgDestroyImage(morphed_image_x);
  vgDestroyImage(input_image);

  return CVPI_TRUE;
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
  const int width = 1280;
  const int height = 960;

  char* input_image_path = IMAGES_CORRECT_DIR "mark1_black_yuva.yuv";
  char* output_image_path = IMAGES_TMP_DIR "mark1_black_yuva_inv.bmp";
  char* output_image_path_lt = IMAGES_TMP_DIR "mark1_black_yuva_inv_lt.bmp";

  FILE* input_file = fopen(input_image_path, "r");
  FILE* output_file = fopen(output_image_path, "w");
  FILE* output_file_lt = fopen(output_image_path_lt, "w");

  fseek(input_file, 0L, SEEK_END);
  int input_size = ftell(input_file);		/* input file size */
  fseek(input_file, 0L, SEEK_SET);

  void* data = malloc(input_size);
  void* data2 = malloc(input_size);

  fread(data, 1, input_size, input_file);

  VGImage input_image = vgCreateImage(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  VGImage inv_image = vgCreateImage(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  VGImage inv_image_lt = vgCreateImage(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  
  vgImageSubData(input_image, data, width*CVPI_PIXEL_BYTES, CVPI_COLOR_SPACE, 0, 0, width, height);

  vgColorMatrix(inv_image, input_image, cvpi_invert_colors);
  vgFinish();

  vgLookup(inv_image_lt, input_image, cvpi_inversion_array, cvpi_inversion_array, cvpi_inversion_array, cvpi_identity_array, VG_FALSE, VG_FALSE);
  vgFinish();

  vgGetImageSubData(inv_image, data, width*CVPI_PIXEL_BYTES, CVPI_COLOR_SPACE, 0, 0, width, height);
  vgGetImageSubData(inv_image_lt, data2, width*CVPI_PIXEL_BYTES, CVPI_COLOR_SPACE, 0, 0, width, height);
  vgFinish();

  int cmp = memcmp(data, data2, width*height*CVPI_PIXEL_BYTES);
  CVPI_BOOL return_value = CVPI_FALSE;
  if(cmp == 0) {
    return_value = CVPI_TRUE;
  } else if(cmp > 0) {
    fprintf(stderr, "%s: added_data is greater than the correct data.\n", __func__);
  } else {
    fprintf(stderr, "%s: added_data is less than the correct data.\n", __func__);
  }

  cvpi_bmp_header_write(output_file, width, height, 100, cvpi_bmp_sARGB_8888);
  fwrite(data, 1, input_size, output_file);
  fflush(output_file);

  cvpi_bmp_header_write(output_file_lt, width, height, 100, cvpi_bmp_sARGB_8888);
  fwrite(data2, 1, input_size, output_file_lt);
  fflush(output_file_lt);

  fclose(output_file);
  fclose(output_file_lt);
  fclose(input_file);
  free(data);
  free(data2);

  vgDestroyImage(inv_image);
  vgDestroyImage(inv_image_lt);
  vgDestroyImage(input_image);

  return return_value;  
}
CVPI_BOOL test_cvpi_pixel_average(void) {
  int width = 10;
  int height = 10;

  VGImage correct = vgCreateImagePainted(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED, 
					 7, 7, 7, 7);
  VGImage input = vgCreateImagePainted(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED,
				       1, 3, 8, 16);
  VGImage output = vgCreateImage(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED);

  vgColorMatrix(output, input,
		cvpi_pixel_average);
  vgFinish();
  void* correct_output = malloc(CVPI_PIXEL_BYTES * width * height);
  void* test_output = malloc(CVPI_PIXEL_BYTES * width * height);

  vgGetImageSubData(output, test_output, width*CVPI_PIXEL_BYTES, CVPI_COLOR_SPACE, 0, 0, width, height);
  vgGetImageSubData(correct, correct_output, width*CVPI_PIXEL_BYTES, CVPI_COLOR_SPACE, 0, 0, width, height);

  int cmp = memcmp(test_output, correct_output, width*height*CVPI_PIXEL_BYTES);
  CVPI_BOOL return_value = CVPI_FALSE;
  if(cmp == 0) {
    return_value = CVPI_TRUE;
  } else if(cmp > 0) {
    fprintf(stderr, "%s: added_data is greater than the correct data.\n", __func__);
  } else {
    fprintf(stderr, "%s: added_data is less than the correct data.\n", __func__);
  }

  free(correct_output);
  free(test_output);

  vgDestroyImage(correct);
  vgDestroyImage(input);
  vgDestroyImage(output);

  return return_value;
}
CVPI_BOOL test_cvpi_pixel_color_average(void) {
  int width = 10;
  int height = 10;

  VGImage correct = vgCreateImagePainted(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED, 
					 3, 3, 3, 255);
  VGImage input = vgCreateImagePainted(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED,
				       1, 3, 5, 255);
  VGImage output = vgCreateImage(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED);

  vgColorMatrix(output, input,
		cvpi_pixel_color_average);
  vgFinish();
  void* correct_output = malloc(CVPI_PIXEL_BYTES * width * height);
  void* test_output = malloc(CVPI_PIXEL_BYTES * width * height);

  vgGetImageSubData(output, test_output, width*CVPI_PIXEL_BYTES, CVPI_COLOR_SPACE, 0, 0, width, height);
  vgGetImageSubData(correct, correct_output, width*CVPI_PIXEL_BYTES, CVPI_COLOR_SPACE, 0, 0, width, height);

  int cmp = memcmp(test_output, correct_output, width*height*CVPI_PIXEL_BYTES);
  CVPI_BOOL return_value = CVPI_FALSE;
  if(cmp == 0) {
    return_value = CVPI_TRUE;
  } else if(cmp > 0) {
    fprintf(stderr, "%s: added_data is greater than the correct data.\n", __func__);
  } else {
    fprintf(stderr, "%s: added_data is less than the correct data.\n", __func__);
  }

  free(correct_output);
  free(test_output);

  vgDestroyImage(correct);
  vgDestroyImage(input);
  vgDestroyImage(output);

  return return_value;
}

CVPI_BOOL test_cvpi_avuy2ayuv(void) {
  int width = 10;
  int height = 10;

  /* 0x004696 */
  VGImage correct = vgCreateImagePainted(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED, 
					 0, 70, 150, 255);
  VGImage input = vgCreateImagePainted(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED,
				       150, 70, 0, 255);
  VGImage output = vgCreateImage(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED);

  vgColorMatrix(output, input,
		cvpi_avuy2ayuv);
  vgFinish();
  void* correct_output = malloc(CVPI_PIXEL_BYTES * width * height);
  void* test_output = malloc(CVPI_PIXEL_BYTES * width * height);

  vgGetImageSubData(output, test_output, width*CVPI_PIXEL_BYTES, CVPI_COLOR_SPACE, 0, 0, width, height);
  vgGetImageSubData(correct, correct_output, width*CVPI_PIXEL_BYTES, CVPI_COLOR_SPACE, 0, 0, width, height);

  int cmp = memcmp(test_output, correct_output, width*height*CVPI_PIXEL_BYTES);
  CVPI_BOOL return_value = CVPI_FALSE;
  if(cmp == 0) {
    return_value = CVPI_TRUE;
  } else if(cmp > 0) {
    fprintf(stderr, "%s: added_data is greater than the correct data.\n", __func__);
  } else {
    fprintf(stderr, "%s: added_data is less than the correct data.\n", __func__);
  }

  /* output color should be 0xRRGGBB = 0x004696 */
  char* output_image_path = IMAGES_TMP_DIR "test_cvpi_avuy2ayuv.bmp";
  FILE* output_file = fopen(output_image_path, "w");
  cvpi_bmp_header_write(output_file, width, height, 100, cvpi_bmp_sARGB_8888);
  fwrite(test_output, 1, CVPI_PIXEL_BYTES * width * height, output_file);
  fflush(output_file);

  fclose(output_file);
  free(correct_output);
  free(test_output);

  vgDestroyImage(correct);
  vgDestroyImage(input);
  vgDestroyImage(output);

  return return_value;
}

/* color conversion */
CVPI_BOOL test_cvpi_avuy2argb(void) {
  const int width = 1280;
  const int height = 960;

  char* input_image_path = IMAGES_CORRECT_DIR "mark1_black_yuva.yuv";
  char* output_image_path = IMAGES_TMP_DIR "mark1_black_avuy2argb.bmp";

  FILE* input_file = fopen(input_image_path, "r");
  FILE* output_file = fopen(output_image_path, "w");

  fseek(input_file, 0L, SEEK_END);
  int input_size = ftell(input_file);		/* input file size */
  fseek(input_file, 0L, SEEK_SET);

  cvpi_pixel* input_data = malloc(input_size);
  cvpi_pixel* output_data = malloc(input_size);

  fread(input_data, 1, input_size, input_file);

  cvpi_avuy2argb(input_data, output_data, width, height);

  cvpi_bmp_header_write(output_file, width, height, 100, cvpi_bmp_sARGB_8888);
  fwrite(output_data, 1, input_size, output_file);
  fflush(output_file);
  fclose(output_file);
  fclose(input_file);
  free(input_data);
  free(output_data);

  return CVPI_TRUE;
}

CVPI_BOOL test_cvpi_image_rgba(void) {
  char* input_image_path = IMAGES_CORRECT_DIR "mark1_black_yuva.yuv";  
  char* output_image_path = IMAGES_TMP_DIR "mark1_black_rgba.bmp";
  const int width = 1280;
  const int height = 960;

  FILE* input = fopen(input_image_path, "r");
  FILE* output = fopen(output_image_path, "w");

  fseek(input, 0L, SEEK_END);
  int input_size = ftell(input);		/* input file size */
  fseek(input, 0L, SEEK_SET);

  void* image_in = malloc(input_size);

  if(input_size != fread(image_in, 1, input_size, input)) {
    fprintf(stderr, "%s: Reading the input image from memory failed.\n",__func__);
    goto test_cvpi_image_rgba_takedown_free;
  }

  VGImage input_image = vgCreateImage(CVPI_COLOR_SPACE,width, height,VG_IMAGE_QUALITY_NONANTIALIASED);
  vgImageSubData(input_image, image_in, width*CVPI_PIXEL_BYTES, CVPI_COLOR_SPACE, 0, 0, width, height);
  vgFinish();

  cvpi_pixel* image_out = cvpi_image_rgba(input_image);

  cvpi_bmp_header_write(output, width, height, 100, cvpi_bmp_sARGB_8888);
  fwrite(image_out, 1, input_size, output);
  fflush(output);

  free(image_out);
  vgDestroyImage(input_image);
  vgFinish();
 test_cvpi_image_rgba_takedown_free:
  free(image_in);
  fclose(input);
  fclose(output);

  return CVPI_TRUE;
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

  return test_cvpi_add_images_common(1, 1, image1_color, image2_color, 1, 1, 1, 0, image_correct, "overflow.yuv");
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

  return test_cvpi_add_images_common(1, 1, image1_color, image2_color, 1, -1, 1, 0, image_correct, "negative.yuv");
}
