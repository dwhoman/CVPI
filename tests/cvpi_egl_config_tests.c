/*
  This file is part of CVPI.

  Copyright (C) 2015

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

#ifndef CVPI_EGL_CONFIG_TESTS
#include "cvpi_egl_config_tests.h"
#endif

#ifndef CVPI
#include "cvpi.h"
#endif

#ifndef CVPI_EGL_SURFACE_FUNCTIONS
#include "cvpi_egl_surface_functions.h"
#endif

#ifndef _STDLIB_H
#include <stdlib.h>
#endif

CVPI_BOOL test_cvpi_egl_settings_create(void) {
  cvpi_egl_settings settings = cvpi_egl_settings_create();
  int check = cvpi_egl_settings_check(settings);
  if(settings != NULL) {
    free(settings);
  }
  return check;
}
CVPI_BOOL test_cvpi_egl_instance_setup_takedown(void) {
  cvpi_egl_settings settings = cvpi_egl_settings_create();
  /* settings->current_surface_type = cvpi_egl_surface_type_pixmap; */
  /* settings->surface_pixmap_create_function = cvpi_egl_surface_pixmap_native_creator; */
  /* settings->surface_pixmap_destroy_function = cvpi_egl_surface_pixmap_native_destroyer; */
  cvpi_egl_instance instance = cvpi_egl_instance_setup(settings);
  int passed = NULL != instance;
  cvpi_egl_instance_takedown(instance);

  if(!passed) {
    fprintf(stderr,"%s: Instance was not set up.\n", __func__);
  }

  if(settings != NULL) {
    free(settings);
  }
  return passed;
}

CVPI_BOOL test_cvpi_egl_instance_openvg_pixmap(void) {
  cvpi_egl_settings settings = cvpi_egl_settings_create();

  if(settings == NULL) {
    return CVPI_FALSE;
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
    if(settings != NULL) {
      free(settings);
    }
    return good;
  } else {
    cvpi_egl_instance instance = cvpi_egl_instance_setup(settings);
    int passed = NULL != instance;
    cvpi_egl_instance_takedown(instance);

    if(!passed) {
      fprintf(stderr,"%s: Instance was not set up.\n", __func__);
    }
    if(settings != NULL) {
      free(settings);
    }
    return passed;
  }
}
