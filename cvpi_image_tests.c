#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
/* bcmp */

#ifndef CVPI_IMAGE_TESTS
#include "cvpi_image_tests.h"
#endif

#ifndef CVPI
#include "cvpi.h"
#endif

#ifndef CVPI_EGL_SURFACE_FUNCTIONS
#include "cvpi_egl_surface_functions.h"
#endif

#define IMAGES_CORRECT_DIR "./test_images_correct/"
#define IMAGES_TMP_DIR "./test_images_tmp/"

static cvpi_egl_instance cvpi_egl_wrapper_create() {
  cvpi_egl_settings settings = cvpi_egl_settings_create();
  settings->surface_pixmap_create_function = cvpi_egl_surface_pixmap_native_creator;
  settings->surface_pixmap_destroy_function = cvpi_egl_surface_pixmap_native_destroyer;
  settings->renderable_api = cvpi_egl_renderable_api_openvg;

  CVPI_BOOL good = CVPI_TRUE;
  good &=
    CVPI_TRUE_TEST(cvpi_egl_settings_conformant(settings, cvpi_egl_conformant_bit_openvg, cvpi_egl_settings_add));
  good &=
    CVPI_TRUE_TEST(cvpi_egl_settings_renderable_type(settings, cvpi_egl_renderable_bits_openvg, cvpi_egl_settings_add));

  good &=
    CVPI_TRUE_TEST(cvpi_egl_settings_pixel_format_brcm(settings, cvpi_egl_pixel_format_vg_image_brcm, cvpi_egl_settings_add));

  good &=
    CVPI_TRUE_TEST(cvpi_egl_settings_pixel_format_brcm(settings,cvpi_egl_pixel_format_render_vg_brcm, cvpi_egl_settings_add));

  good &=
    CVPI_TRUE_TEST(cvpi_egl_settings_pixel_format_brcm(settings,cvpi_egl_pixel_format_usage_mask_brcm, cvpi_egl_settings_add));

  good &=
    CVPI_TRUE_TEST(cvpi_egl_settings_buffer_size(settings, 32));

  good &=
    CVPI_TRUE_TEST(cvpi_egl_settings_surface_type(settings, cvpi_egl_surface_bits_pixmap, cvpi_egl_settings_add));
  good &=
    CVPI_TRUE_TEST(cvpi_egl_settings_surface_type(settings, cvpi_egl_surface_bits_pbuffer, cvpi_egl_settings_remove));
  good &=
    CVPI_TRUE_TEST(cvpi_egl_settings_surface_type(settings, cvpi_egl_surface_bits_window, cvpi_egl_settings_remove));

  if(!good) {
    free(settings);
    return NULL;
  }
  
  return cvpi_egl_instance_setup(settings);
}
static void cvpi_egl_wrapper_destroy(cvpi_egl_instance instance) {
  free(instance->egl_settings);
  cvpi_egl_instance_takedown(instance);
}

CVPI_BOOL test_cvpi_yuyv2yuva() {
  const int INPUT_WIDTH = 640;
  const int INPUT_HEIGHT = 960;
  const int OUTPUT_WIDTH = 1280;
  const int OUTPUT_HEIGHT = 960;
  CVPI_BOOL return_value = CVPI_FALSE;

  cvpi_egl_instance instance = cvpi_egl_wrapper_create();
  if(instance == NULL) {
    return CVPI_FALSE;
  }

  char* yuyv2yuva_input = IMAGES_CORRECT_DIR "mark1_black.yuv";
  char* yuyv2yuva_correct_result = IMAGES_CORRECT_DIR "mark1_black_yuva.yuv";
  char* yuyv2yuva_tmp_result = IMAGES_TMP_DIR "mark1_black_yuva.yuv";

  FILE* input = fopen(yuyv2yuva_input, "r");
  FILE* correct = fopen(yuyv2yuva_correct_result, "r");
  FILE* output = fopen(yuyv2yuva_tmp_result, "w");
  
  fseek(input, 0L, SEEK_END);
  int input_size = ftell(input);		/* input file size */
  fseek(input, 0L, SEEK_SET);

  fseek(correct, 0L, SEEK_END);
  int correct_size = ftell(correct);		/* correct file size */
  fseek(correct, 0L, SEEK_SET);

  if(input_size*2 != correct_size) {
    fprintf(stderr, "%s: Input file and correct file size mismatch. %d and %d\n", __func__, input_size*2, correct_size);
    goto test_cvpi_yuyv2yuva_takedown;
  }

  void* image_in = malloc(input_size);
  void* image_out = malloc(2*input_size);
  void* image_correct = malloc(correct_size);

  fread(image_in, 1, input_size, input);
  fread(image_correct, 1, correct_size, correct);

  VGImage input_image = vgCreateImage(CVPI_COLOR_SPACE,INPUT_WIDTH, INPUT_HEIGHT,VG_IMAGE_QUALITY_NONANTIALIASED);
  VGImage output_image = vgCreateImage(CVPI_COLOR_SPACE,OUTPUT_WIDTH, OUTPUT_HEIGHT,VG_IMAGE_QUALITY_NONANTIALIASED);
  
  EGLint pixel_size = cvpi_egl_bytes_per_pixel(CVPI_COLOR_SPACE);

  output_image = cvpi_yuyv2yuva(input_image);
  vgGetImageSubData(output_image, image_out, pixel_size*OUTPUT_WIDTH, CVPI_COLOR_SPACE, 0, 0, OUTPUT_WIDTH, OUTPUT_HEIGHT);
  vgFinish();

  vgDestroyImage(input_image);
  vgDestroyImage(output_image);

  if(!bcmp(image_out, image_correct, correct_size)) {
    return_value = CVPI_TRUE;
  } else {
    fwrite(image_out, pixel_size*OUTPUT_WIDTH, OUTPUT_HEIGHT, output);
    fflush(output);
  }

  free(image_in);
  free(image_out);
  free(image_correct);

 test_cvpi_yuyv2yuva_takedown:
  fclose(input);
  fclose(output);
  fclose(correct);
  cvpi_egl_wrapper_destroy(instance);

  return return_value;
}
