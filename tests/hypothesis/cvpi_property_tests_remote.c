#include "cvpi.h"
#ifndef CVPI_EGL_SURFACE_FUNCTIONS
#include "cvpi_egl_surface_functions.h"
#endif

#include "stdio.h"

int test_cvpi_image_add(void* image_1, void* image_2, int height, int width, int a, int b, float scale, float bias) {
  // fprintf(stderr, "adding images.\n");
  int size = CVPI_PIXEL_BYTES * width * height;
  char *cvpi_out = malloc(size);
  //fprintf(stderr, "Getting image data1.\n");
  VGImage image1 = vgCreateImage(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  VGImage image2 = vgCreateImage(CVPI_COLOR_SPACE, width, height, VG_IMAGE_QUALITY_NONANTIALIASED);
  vgFinish();
  // fprintf(stderr, "Getting image data2.\n");
  vgImageSubData(image1, image_1, width*CVPI_PIXEL_BYTES, CVPI_COLOR_SPACE, 0, 0, width, height);
  //fprintf(stderr, "Getting image data2.1.\n");
  vgImageSubData(image2, image_2, width*CVPI_PIXEL_BYTES, CVPI_COLOR_SPACE, 0, 0, width, height);
  //fprintf(stderr, "Getting image data2.2.\n");
  vgFinish();
  //fprintf(stderr, "Too add.\n");
  VGImage cvpi_sum = cvpi_image_add(image1, image2, a, b, scale, bias);
  //fprintf(stderr, "Added.\n");
  vgFinish();
  //fprintf(stderr, "Getting image data3.\n");
  vgGetImageSubData(cvpi_sum, cvpi_out, CVPI_PIXEL_BYTES*width, CVPI_COLOR_SPACE, 0, 0, width, height);
  vgFinish();
  vgDestroyImage(image1);
  vgDestroyImage(image2);
  vgDestroyImage(cvpi_sum);

  //fprintf(stderr, "printing images.\n");

  for(int i = 0; i < size; ++i) {
    printf("%02x", cvpi_out[i]);
  }
  printf("\n");

  free(cvpi_out);
}

int main(int argc, char **argv) {

  /* 
     cvpi_property_test_remote <cvpi_image_function> [<image function args> ]

     Images are in hex, integers are non-float decimals, and floats are floating
     point decimals.
     
     Prints the resulting image or value in hex.
 */
  
  if(argc < 2) {
    fprintf(stderr, "Not enough arguments.\n");
    return 1;
  }

  // begin CVPI setup
  cvpi_egl_settings settings = cvpi_egl_settings_create();
  if(settings == NULL) {
    fprintf(stderr, "cvpi_egl_settings_create failed.\n");
    return 2;
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

  if(!good) {
    if(settings != NULL) {
      free(settings);
      settings = NULL;
    }
    fprintf(stderr, "Invalid EGL settings.\n");
    return 3;
  }
  cvpi_egl_instance instance = cvpi_egl_instance_setup(settings);
  if(instance == NULL) {
    free(settings);
    settings = NULL;
    fprintf(stderr, "cvpi_egl_instance creation failed.\n");
    return 4;
  }
  // end CVPI setup
  if(argc >= 10 && strcmp("cvpi_image_add", argv[1]) == 0) {
    //fprintf(stderr, "going to run test_cvpi_image_add\n");
    int height, width, a, b;
    float scale, bias;
    sscanf(argv[4], "%d", &width);
    sscanf(argv[5], "%d", &height);
    sscanf(argv[6], "%d", &a);
    sscanf(argv[7], "%d", &b);
    sscanf(argv[8], "%f", &scale);
    sscanf(argv[9], "%f", &bias);

    //fprintf(stderr, "%d %d %d %d %f %f\n", width, height, a, b, scale, bias);
    //fprintf(stderr, "%s\n\n%s\n\n", argv[2], argv[3]);

    char* image_1 = malloc(width * height * CVPI_PIXEL_BYTES);
    char* image_2 = malloc(width * height * CVPI_PIXEL_BYTES);

    for(int i = 0; i < width * height * CVPI_PIXEL_BYTES; ++i) {
      sscanf(&argv[2][2*i], "%2hhx", &image_1[i]);
      sscanf(&argv[3][2*i], "%2hhx", &image_2[i]);
    }

    //fprintf(stderr, "Image 1 data\n");
    /* for(int i = 0; i < width * height * CVPI_PIXEL_BYTES; i++) { */
    /*   fprintf(stderr, "%02x", image_1[i]); */
    /* } */
    /* fprintf(stderr, "\n\n"); */

    /* fprintf(stderr, "Image 2 data\n"); */
    /* for(int i = 0; i < width * height * CVPI_PIXEL_BYTES; i++) { */
    /*   fprintf(stderr, "%02x", image_2[i]); */
    /* } */
    /* fprintf(stderr, "\n"); */

    
    test_cvpi_image_add(image_1, image_2, height, width, a, b, scale, bias);

    free(image_1);
    free(image_2);
  } else {
    fprintf(stderr, "Invalid args argc: %d %s\n", argc, argv[1]);
  }

  cvpi_egl_instance_takedown(instance);
  if(settings != NULL) {
    free(settings);
    settings = NULL;
  }
  return 0;
}
 
