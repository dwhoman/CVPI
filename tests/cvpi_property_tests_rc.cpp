#ifndef CVPI
extern "C" {
#include "cvpi.h"
#ifndef CVPI_EGL_SURFACE_FUNCTIONS
#include "cvpi_egl_surface_functions.h"
#endif
};
#endif
#include <rapidcheck.h>
#include <array>
#include <exception>
#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <ostream>
#include <iostream>
#include <utility>
#include <cmath>

const uint32_t MAX_SIZE = EGL_CONFIG_MAX_WIDTH * EGL_CONFIG_MAX_HEIGHT;
const uint32_t max_vec_length = 50;

std::vector<std::pair<uint32_t, uint32_t>> factors(uint32_t size) {
  std::vector<std::pair<uint32_t, uint32_t>> ret_val = {};
  for(uint32_t width = 1; width <= size && width <= EGL_CONFIG_MAX_WIDTH; ++width) {
    if(size % width == 0) {
      uint32_t height = size/width;
      if(height <= EGL_CONFIG_MAX_HEIGHT) {
	ret_val.push_back(std::pair<uint32_t, uint32_t>(width, height));
      }
    }
  }

  return ret_val;
}

// struct UINT32_T {
//   uint32_t val;
// };

// namespace rc {
//   template<>
//   struct Arbitrary<UINT32_T> {
//     static Gen<UINT32_T> arbitrary() {
//       return gen::build<UINT32_T>(
// 				  gen::set(&UINT32_T::val, gen::inRange(0, max_vec_length)));
//     });
// }
// };

// } // namespce rc

int main(int argc, char **argv) {
  std::cout << "In main" << std::endl;
  rc::check("add images", [](const std::vector<uint32_t> &image_array_combined){
      RC_PRE(image_array_combined.size() > 1);

      if(image_array_combined.size() % 2 != 0) {
	image_array_combined.push_back(0);
      }

      uint32_t data_length = image_array_combined.size()/2;

      uint32_t *image_data_1 = &image_array_combined[0];
      uint32_t *image_data_2 = &image_array_combined[data_length];

      std::vector<std::pair<uint32_t, uint32_t>> dims = factors(data_length);
      // make sure that there are image dimensions to choose from
      RC_PRE(dims.size() > 0);

      // TODO: make the index variable
      uint32_t image_width = dims.back().first;
      uint32_t image_height = dims.back().second;

      // std::cout << "Setting up EGL" << std::endl;

      // begin CVPI setup
      cvpi_egl_settings settings = cvpi_egl_settings_create();
      if(settings == NULL) {
	RC_FAIL("cvpi_egl_settings_create failed.");
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
	RC_FAIL("Invalid EGL settings.");
      }
      cvpi_egl_instance instance = cvpi_egl_instance_setup(settings);
      if(instance == NULL) {
	free(settings);
	settings = NULL;
	RC_FAIL("cvpi_egl_instance creation failed.");
      }
      // end CVPI setup

      uint32_t *cvpi_out = malloc(CVPI_PIXEL_BYTES * image_width * image_height);
      cv::Mat cv_out;

      VGImage image1 = vgCreateImage(CVPI_COLOR_SPACE, image_width, image_height,VG_IMAGE_QUALITY_NONANTIALIASED);
      VGImage image2 = vgCreateImage(CVPI_COLOR_SPACE, image_width, image_height,VG_IMAGE_QUALITY_NONANTIALIASED);
      vgFinish();
      vgImageSubData(image1, &image_data_1, image_width*CVPI_PIXEL_BYTES, CVPI_COLOR_SPACE, 0, 0, image_width, image_height);
      vgImageSubData(image2, &image_data_2, image_width*CVPI_PIXEL_BYTES, CVPI_COLOR_SPACE, 0, 0, image_width, image_height);
      vgFinish();
      VGImage cvpi_sum = cvpi_image_add(image1, image2, 1, 1, 1, 0);
      vgFinish();
      vgGetImageSubData(cvpi_sum, cvpi_out, CVPI_PIXEL_BYTES*image_width, CVPI_COLOR_SPACE, 0, 0, image_width, image_height);
      vgFinish();
      vgDestroyImage(image1);
      vgDestroyImage(image2);
      vgDestroyImage(cvpi_sum);

      // OpenCV calculation
      cv::Mat cv_image_1 = cv::Mat(image_height, image_width, CV_8UC4, &image_data_1);
      cv::Mat cv_image_2 = cv::Mat(image_height, image_width, CV_8UC4, &image_data_2);

      cv::add(cv_image_1, cv_image_2, cv_out);

      cv::Mat cvpi_sum_mat = cv::Mat(image_height, image_width, CV_8UC4, cvpi_out);

      bool success = false;
      if(cvpi_sum_mat.rows == cv_out.rows) {
	if(cvpi_sum_mat.cols == cv_out.cols) {
	  success = std::equal(cv_out.begin<uint32_t>(), cv_out.end<uint32_t>(), cvpi_sum_mat.begin<uint32_t>());
	  if(!success) {
	    std::cout << "sums differ" << std::endl;
	    std::cout << "height: " << cvpi_sum_mat.rows << " width: " << cvpi_sum_mat.cols << std::endl;
	    std::cout << "CVPI" << std::endl << cvpi_sum_mat << std::endl << std::endl;
	    
	    std::cout << "OpenCV" << std::endl << cv_out << std::endl << std::endl;
	    
	    std::cout << std::endl << cv_image_1 << std::endl << std::endl;
	    
	    std::cout << std::endl << cv_image_2 << std::endl << std::endl;
	  }
	} else {
	  success = false;
	  std::cout << "heights differ: " << cvpi_sum_mat.cols << " " << cv_out.cols << std::endl;
	}
      } else {
	success = false;
	std::cout << "widths differ: " << cvpi_sum_mat.rows << " " << cv_out.rows << std::endl;
      }

      free(cvpi_out);
      cvpi_out = NULL;

      cvpi_egl_instance_takedown(instance);
      if(settings != NULL) {
	free(settings);
	settings = NULL;
      }
      RC_ASSERT(success);
    });
  return 0;
}
