#ifndef CVPI_EGL_CONFIG_TESTS
#include "cvpi_egl_config_tests.h"
#endif

#ifndef CVPI
#include "cvpi.h"
#endif

#ifndef CVPI_EGL_SURFACE_FUNCTIONS
#include "cvpi_egl_surface_functions.h"
#endif

CVPI_BOOL test_cvpi_egl_settings_create(void) {
  cvpi_egl_settings settings = cvpi_egl_settings_create();
  int check = cvpi_egl_settings_check(settings);
  free(settings);
  return check;
}
CVPI_BOOL test_cvpi_egl_instance_setup_takedown(void) {
  cvpi_egl_settings settings = cvpi_egl_settings_create();
  cvpi_egl_instance instance = cvpi_egl_instance_setup(settings);
  int passed = NULL != instance;
  cvpi_egl_instance_takedown(instance);

  if(!passed) {
    fprintf(stderr,"%s: Instance was not set up.\n", __func__);
  }

  free(settings);
  return passed;
}

CVPI_BOOL test_cvpi_egl_instance_openvg_pixmap(void) {
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

  /* should return a warning after this, since no surface will be selected. */
  good &=
    CVPI_TRUE_TEST(cvpi_egl_settings_surface_type(settings, cvpi_egl_surface_bits_pbuffer, cvpi_egl_settings_remove));
  good &=
    CVPI_TRUE_TEST(cvpi_egl_settings_surface_type(settings, cvpi_egl_surface_bits_pixmap, cvpi_egl_settings_add));
  good &=
    CVPI_TRUE_TEST(cvpi_egl_settings_surface_type(settings, cvpi_egl_surface_bits_window, cvpi_egl_settings_remove));

  if(!good) {
    free(settings);
    return good;
  } else {
    cvpi_egl_instance instance = cvpi_egl_instance_setup(settings);
    int passed = NULL != instance;
    cvpi_egl_instance_takedown(instance);

    if(!passed) {
      fprintf(stderr,"%s: Instance was not set up.\n", __func__);
    }

    free(settings);
    return passed;
  }
}
