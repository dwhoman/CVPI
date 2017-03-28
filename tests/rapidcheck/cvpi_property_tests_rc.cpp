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

const int MAX_WIDTH = 500;
const int MAX_HEIGHT = 500;

// EGL_CONFIG_MAX_WIDTH * EGL_CONFIG_MAX_HEIGHT;
const int MAX_SIZE = MAX_WIDTH * MAX_HEIGHT;

struct Dimensions {
  uint32_t width;
  uint32_t height;
};

namespace rc {
  template<>
  struct Arbitrary<Dimensions> {
    static Gen<Dimensions> arbitrary() {
      return gen::build<Dimensions>(gen::set(&Dimensions::width, gen::inRange(1, MAX_WIDTH + 1)),
				    gen::set(&Dimensions::height, gen::inRange(1, MAX_HEIGHT + 1)));
    };
  };
};

int main(int argc, char **argv) {
  rc::check("add images", [](const std::array<uint32_t, MAX_SIZE> &image_data_1,
			     const std::array<uint32_t, MAX_SIZE> &image_data_2,
			     const Dimensions &dimensions){
	      //std::cout << "Setting up EGL" << std::endl;

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
      //std::cout << "EGL setup" << std::endl;

      uint32_t *cvpi_out = malloc(CVPI_PIXEL_BYTES * dimensions.width * dimensions.height);
      cv::Mat cv_out;

      VGImage image1 = vgCreateImage(CVPI_COLOR_SPACE, dimensions.width, dimensions.height,VG_IMAGE_QUALITY_NONANTIALIASED);
      VGImage image2 = vgCreateImage(CVPI_COLOR_SPACE, dimensions.width, dimensions.height,VG_IMAGE_QUALITY_NONANTIALIASED);
      vgFinish();
      vgImageSubData(image1, &image_data_1, dimensions.width*CVPI_PIXEL_BYTES, CVPI_COLOR_SPACE, 0, 0, dimensions.width, dimensions.height);
      vgImageSubData(image2, &image_data_2, dimensions.width*CVPI_PIXEL_BYTES, CVPI_COLOR_SPACE, 0, 0, dimensions.width, dimensions.height);
      vgFinish();
      VGImage cvpi_sum = cvpi_image_add(image1, image2, 1, 1, 1, 0);
      vgFinish();
      vgGetImageSubData(cvpi_sum, cvpi_out, CVPI_PIXEL_BYTES*dimensions.width, CVPI_COLOR_SPACE, 0, 0, dimensions.width, dimensions.height);
      vgFinish();
      vgDestroyImage(image1);
      vgDestroyImage(image2);
      vgDestroyImage(cvpi_sum);

      // OpenCV calculation
      cv::Mat cv_image_1 = cv::Mat(dimensions.height, dimensions.width, CV_8UC4, &image_data_1);
      cv::Mat cv_image_2 = cv::Mat(dimensions.height, dimensions.width, CV_8UC4, &image_data_2);

      cv::add(cv_image_1, cv_image_2, cv_out);

      cv::Mat cvpi_sum_mat = cv::Mat(dimensions.height, dimensions.width, CV_8UC4, cvpi_out);

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
