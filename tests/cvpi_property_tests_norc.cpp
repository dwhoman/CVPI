#ifndef CVPI
extern "C" {
#include "cvpi.h"
#ifndef CVPI_EGL_SURFACE_FUNCTIONS
#include "cvpi_egl_surface_functions.h"
#endif
};
#endif
#include <array>
#include <exception>
#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <ostream>
#include <iostream>
#include <algorithm>

  const int image_width = 3;
  const int image_height = 2;
  const int image_size = 6;

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

int main(int argc, char **argv) {
  std::cout << "In main" << std::endl;
  const std::vector<uint32_t> image_vec_1 = {0,1,2,3,4,5};
  const std::vector<uint32_t> image_vec_2 = {10,11,9,7,8,6};

  // begin CVPI setup
  cvpi_egl_settings settings = cvpi_egl_settings_create();
  if(settings == NULL) {
    std::cerr << "cvpi_egl_settings_create failed." << std::endl;
    return 1;
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
    std::cerr << "Invalid EGL settings." << std::endl;
    return 2;
  }

  std::cout << "Created settings struct" << std::endl;
  
  cvpi_egl_instance instance = cvpi_egl_instance_setup(settings);
  if(instance == NULL) {
    free(settings);
    settings = NULL;
    std::cerr << "cvpi_egl_instance creation failed." << std::endl;
    return 3;
  }
  std::cout << "Created EGL instance" << std::endl;
  // end CVPI setup
	      
  uint32_t *cvpi_out = malloc(CVPI_PIXEL_BYTES*image_width * image_height);
  cv::Mat cv_out;
  
  // const auto width = *rc::gen::inRange(1, 2049);
  // const auto height = *rc::gen::inRange(1, 2049);
  // const auto size = width * height;

  VGImage image1 = vgCreateImage(CVPI_COLOR_SPACE, image_width, image_height,VG_IMAGE_QUALITY_NONANTIALIASED);
  VGImage image2 = vgCreateImage(CVPI_COLOR_SPACE, image_width, image_height,VG_IMAGE_QUALITY_NONANTIALIASED);
  vgFinish();
  vgImageSubData(image1, &image_vec_1[0], image_width*CVPI_PIXEL_BYTES, CVPI_COLOR_SPACE, 0, 0, image_width, image_height);
  vgImageSubData(image2, &image_vec_2[0], image_width*CVPI_PIXEL_BYTES, CVPI_COLOR_SPACE, 0, 0, image_width, image_height);
  vgFinish();
  VGImage cvpi_sum = cvpi_image_add(image1, image2, 1, 1, 1, 0);
  vgFinish();
  vgGetImageSubData(cvpi_sum, cvpi_out, CVPI_PIXEL_BYTES*image_width, CVPI_COLOR_SPACE, 0, 0, image_width, image_height);
  vgFinish();
  vgDestroyImage(image1);
  vgDestroyImage(image2);
  vgDestroyImage(cvpi_sum);

  // OpenCV calculation
  cv::Mat cv_image_1 = cv::Mat(image_height, image_width, CV_8UC4, &image_vec_1[0]);
  cv::Mat cv_image_2 = cv::Mat(image_height, image_width, CV_8UC4, &image_vec_2[0]);

  cv::add(cv_image_1, cv_image_2, cv_out);

  std::cout << "Converting to mat: cvpi_out ptr:" << cvpi_out << std::endl;

  cv::Mat cvpi_sum_mat = cv::Mat(image_height, image_width, CV_8UC4, cvpi_out);

  bool success = false;
  if(cvpi_sum_mat.rows == cv_out.rows) {
    if(cvpi_sum_mat.cols == cv_out.cols) {
      std::cout << "Checking equality 1" << std::endl;
      success = std::equal(cv_out.begin<uint32_t>(), cv_out.end<uint32_t>(), cvpi_sum_mat.begin<uint32_t>());
    } else {
      std::cout << "heights differ: " << cvpi_sum_mat.cols << " " << cv_out.cols << std::endl;
    }
  } else {
    std::cout << "widths differ: " << cvpi_sum_mat.rows << " " << cv_out.rows << std::endl;
  }
  if(success) {
    std::cout << "They are equal" << std::endl;
  } else {
    std::cout << "They are different" << std::endl;
  }

  std::cout << std::endl << cvpi_sum_mat << std::endl << std::endl;

  std::cout << std::endl << cv_out << std::endl << std::endl;

  std::cout << std::endl << cv_image_1 << std::endl << std::endl;

  std::cout << std::endl << cv_image_2 << std::endl << std::endl;
  
  free(cvpi_out);
  cvpi_out = NULL;

  cvpi_egl_instance_takedown(instance);
  if(settings != NULL) {
    free(settings);
    settings = NULL;
  }

return 0;
}
