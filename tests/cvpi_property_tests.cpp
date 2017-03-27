#ifndef CVPI
extern "C" {
#include "cvpi.h"
#ifndef CVPI_EGL_SURFACE_FUNCTIONS
#include "cvpi_egl_surface_functions.h"
#endif
};
#endif
#include <gtest/gtest.h>
#include <rapidcheck.h>
#include <rapidcheck/gtest.h>
#include <array>
#include <exception>
#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <ostream>
#include <iostream>

  const int image_width = 2048;
  const int image_height = 2048;
  const int image_size = 2048 * 2048;

// namespace rc {

// template<>
// struct Arbitrary<cvpi_pixel> {
//   static Gen<cvpi_pixel> arbitrary() {
//     return gen::build<cvpi_pixel>(gen::set(&cvpi_pixel.channel[0], gen::inRange(0, 256)),
// 				  gen::set(&cvpi_pixel.channel[1], gen::inRange(0, 256)),
// 				  gen::set(&cvpi_pixel.channel[2], gen::inRange(0, 256)),
// 				  gen::set(&cvpi_pixel.channel[3], gen::inRange(0, 256)));
//     });
//   }
// };

// } // namespce rc

  

  class CVPITest : public :: testing :: Test {
protected:
  cvpi_egl_settings settings = NULL;
  cvpi_egl_instance instance = NULL;
  CVPITest() {
  settings = cvpi_egl_settings_create();
  if(settings == NULL) {
  throw std::runtime_error("cvpi_egl_settings_create failed.");
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
  throw std::runtime_error("Invalid EGL settings.");
}
  instance = cvpi_egl_instance_setup(settings);
  if(instance == NULL) {
  free(settings);
  settings = NULL;
  throw std::runtime_error("cvpi_egl_instance creation failed.");
}
}

  virtual ~CVPITest() {
  cvpi_egl_instance_takedown(instance);
  if(settings != NULL) {
  free(settings);
  settings = NULL;
}
}
};

RC_GTEST_FIXTURE_PROP(CVPITest,
		      add_images,
		      (const std::array<uint32_t, image_size> &image_array_1,
		       const std::array<uint32_t, image_size> &image_array_2)) {
  std::cout << "Running test" << std::endl;

  // [1, 2049)
  uint32_t *cvpi_out = NULL;
  cv::Mat cv_out;
  
  // const auto width = *rc::gen::inRange(1, 2049);
  // const auto height = *rc::gen::inRange(1, 2049);
  // const auto size = width * height;

  VGImage image1 = vgCreateImage(CVPI_COLOR_SPACE, image_width, image_height,VG_IMAGE_QUALITY_NONANTIALIASED);
  VGImage image2 = vgCreateImage(CVPI_COLOR_SPACE, image_width, image_height,VG_IMAGE_QUALITY_NONANTIALIASED);

  vgImageSubData(image1, &image_array_1, image_width*CVPI_PIXEL_BYTES, CVPI_COLOR_SPACE, 0, 0, image_width, image_height);
  vgImageSubData(image2, &image_array_2, image_width*CVPI_PIXEL_BYTES, CVPI_COLOR_SPACE, 0, 0, image_width, image_height);

  VGImage cvpi_sum = cvpi_image_add(image1, image2, 1, 1, 1, 0);
  vgGetImageSubData(cvpi_sum, cvpi_out, CVPI_PIXEL_BYTES*image_width, CVPI_COLOR_SPACE, 0, 0, image_width, image_height);

  vgDestroyImage(image1);
  vgDestroyImage(image2);
  vgDestroyImage(cvpi_sum);

  // OpenCV calculation
  const void* image_1_data = &image_array_1;
  const void* image_2_data = &image_array_2;
  cv::Mat cv_image_1 = cv::Mat(image_height, image_width, CV_8UC4, image_1_data);
  cv::Mat cv_image_2 = cv::Mat(image_height, image_width, CV_8UC4, image_2_data);

  cv::add(cv_image_1, cv_image_2, cv_out);

  bool success = true;
  for(int i = image_size; i != 0;) {
    --i;
    if(cvpi_out[i] != cv_out.at<uint32_t>(i)) {
      success = false;
      break;
    }
  }
  RC_ASSERT(success == true);

  free(cvpi_out);
  cvpi_out = NULL;
  }

int main(int argc, char **argv) {
  std::cout << "Starting" << std::endl;
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
