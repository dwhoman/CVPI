#ifndef _STDIO_H
#include <stdio.h>
#endif

#ifndef	_STDLIB_H
#include <stdlib.h>
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

static cvpi_egl_instance cvpi_egl_wrapper_create() {
  cvpi_egl_settings settings = cvpi_egl_settings_create();
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

void test_runner(CVPI_BOOL(*test_function)(void), char* name) {   
  if(CVPI_TRUE_TEST(test_function())) {
    printf("PASSED: %s\n", name);
  } else {
    printf("FAILED: %s\n", name);
  }
  printf("--------------------\n");
}

#define TEST(f) do {CVPI_BOOL(*p18025ldgfaln1293)(void) = (f); test_runner(p18025ldgfaln1293,(#f));} while(0)

int main() {
  /* cvpi_log_file = stderr; */

  /* egl setup tests */
  TEST(test_cvpi_egl_settings_create);
  TEST(test_cvpi_egl_instance_setup_takedown);
  TEST(test_cvpi_egl_instance_openvg_pixmap);

  /* tests not requiring egl */
  TEST(test_cvpi_avuy2argb);

  /* tests requiring egl */
  cvpi_egl_instance instance = cvpi_egl_wrapper_create();
  if(instance == NULL) {
    fprintf(stderr, "cvpi_tests.c: cvpi_egl_instance creation failed\n", __func__);
    return 1;
  }
  TEST(test_cvpi_pixel);

  TEST(test_cvpi_yuyv2yuva);
  TEST(test_cvpi_image_add_odd);
  TEST(test_cvpi_image_add_single);
  TEST(test_cvpi_image_add_even);
  TEST(test_cvpi_subtract_images_odd);
  TEST(test_cvpi_image_add_odd_scale_add);
  TEST(test_cvpi_subtract_images_odd_scale_add);
  /* these tests can run out of memory */
  /* TEST(test_cvpi_image_add_huge_even); */
  /* TEST(test_cvpi_image_add_huge_odd); */

  TEST(test_cvpi_channel_add_RB);
  TEST(test_cvpi_channel_add_RR);
  TEST(test_cvpi_color_channels_add);
  TEST(test_cvpi_all_channels_add);
  TEST(test_cvpi_image_combine_channelwise);

  TEST(test_cvpi_channel_threshold);
  TEST(test_cvpi_channel_threshold_inv);
  TEST(test_cvpi_channel_threshold_same);
  TEST(test_cvpi_channel_threshold_inv_same);

  TEST(test_cvpi_image_threshold);
  TEST(test_cvpi_image_threshold_inv);
  TEST(test_cvpi_image_threshold_dependent);
  TEST(test_cvpi_image_threshold_inv_dependent);

  TEST(test_cvpi_image_threshold_sector);
  TEST(test_cvpi_image_threshold_sector_inv);
  TEST(test_cvpi_image_threshold_sector_dependent);
  TEST(test_cvpi_image_threshold_sector_inv_dependent);

  TEST(test_cvpi_image_mask_channel_add);
  TEST(test_cvpi_image_mask_channel_subtract);

  TEST(test_cvpi_image_logical_and);
  TEST(test_cvpi_image_logical_and_NOT);
  TEST(test_cvpi_image_logical_nand);
  TEST(test_cvpi_image_logical_nand_NOT);
  TEST(test_cvpi_image_logical_or);
  TEST(test_cvpi_image_logical_or_NOT);
  TEST(test_cvpi_image_logical_nor);
  TEST(test_cvpi_image_logical_nor_NOT);
  TEST(test_cvpi_image_logical_xor);
  TEST(test_cvpi_image_logical_xor_NOT);
  TEST(test_cvpi_image_logical_xnor);
  TEST(test_cvpi_image_logical_xnor_NOT);
  TEST(test_cvpi_image_logical_complement);
  TEST(test_cvpi_image_logical_complement_NOT);
  TEST(test_cvpi_image_logical_complement_NOT);
  TEST(test_cvpi_image_logical_complement_inv_NOT);

  TEST(test_cvpi_image_dialate);
  TEST(test_cvpi_image_dialate_NOT);
  TEST(test_cvpi_image_erode);
  TEST(test_cvpi_image_erode_NOT);
  TEST(test_cvpi_image_hit_miss);
  TEST(test_cvpi_image_hit_miss_NOT);
  TEST(test_cvpi_image_thin);
  TEST(test_cvpi_image_thin_NOT);
  TEST(test_cvpi_image_thicken);
  TEST(test_cvpi_image_thicken_NOT);

  TEST(test_cvpi_channel_max_alpha);
  TEST(test_cvpi_channel_max_red);
  TEST(test_cvpi_channel_max_green);
  TEST(test_cvpi_channel_max_blue);
  TEST(test_cvpi_channel_min_alpha);
  TEST(test_cvpi_channel_min_red);
  TEST(test_cvpi_channel_min_green);
  TEST(test_cvpi_channel_min_blue);
  TEST(test_cvpi_channel_max_min_alpha);
  TEST(test_cvpi_channel_max_min_red);
  TEST(test_cvpi_channel_max_min_green);
  TEST(test_cvpi_channel_max_min_blue);

  TEST(test_cvpi_channel_histogram_alpha);
  TEST(test_cvpi_channel_histogram_red);
  TEST(test_cvpi_channel_histogram_green);
  TEST(test_cvpi_channel_histogram_blue);

  TEST(test_cvpi_color_channels_histogram);
  TEST(test_cvpi_image_histogram);

  TEST(test_cvpi_channel_cumulative_distribution_alpha);
  TEST(test_cvpi_channel_cumulative_distribution_red);
  TEST(test_cvpi_channel_cumulative_distribution_green);
  TEST(test_cvpi_channel_cumulative_distribution_blue);

  TEST(test_cvpi_color_channels_cumulative_distribution);
  TEST(test_cvpi_image_cumulative_distribution);

  TEST(test_cvpi_image_coordinate_table_zero);
  TEST(test_cvpi_image_coordinate_table_some);
  TEST(test_cvpi_image_coordinate_table_all);
  TEST(test_cvpi_sobel);
  TEST(test_cvpi_scharr);
  TEST(test_cvpi_prewitt);
  TEST(test_cvpi_roberts_cross);

  TEST(test_cvpi_invert_colors);
  TEST(test_cvpi_pixel_average);
  TEST(test_cvpi_pixel_color_average);
  TEST(test_cvpi_avuy2ayuv);

  /* TEST(test_cvpi_image_mean_small); */
  /* TEST(test_cvpi_image_mean_max); */
  /* TEST(test_cvpi_image_mean_huge); */
  //TEST(test_cvpi_image_mean_compare);

  TEST(test_cvpi_image2rgba);

  TEST(test_cvpi_image_rgba_to_binary);
  TEST(test_overflow_behavior);
  TEST(test_negative_behavior);

  cvpi_egl_wrapper_destroy(instance);

  return 0;
}
