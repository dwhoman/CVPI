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
#include <random>


bool failedOnce = false;
const int MAX_WIDTH = 500;
const int MAX_HEIGHT = 500;

// EGL_CONFIG_MAX_WIDTH * EGL_CONFIG_MAX_HEIGHT;
const int MAX_SIZE = MAX_WIDTH * MAX_HEIGHT;

struct Dimensions {
  uint32_t width;
  uint32_t height;
};

// small dimensions
struct SDimensions : public Dimensions {};

// big dimensions
struct BDimensions : public Dimensions {};

namespace rc {
  template<>
  struct Arbitrary<SDimensions> {
    static Gen<SDimensions> arbitrary() {
      return gen::build<SDimensions>(gen::set(&SDimensions::width, gen::inRange(1, MAX_WIDTH + 1)),
				     gen::set(&SDimensions::height, gen::inRange(1, MAX_HEIGHT + 1)));
    };
  };
  template<>
  struct Arbitrary<BDimensions> {
    static Gen<BDimensions> arbitrary() {
      return gen::build<BDimensions>(gen::set(&BDimensions::width, gen::inRange(MAX_WIDTH + 1, EGL_CONFIG_MAX_WIDTH + 1)),
				     gen::set(&BDimensions::height, gen::inRange(MAX_HEIGHT + 1, EGL_CONFIG_MAX_HEIGHT + 1)));
    };
  };
  // template<>
  // struct Arbitrary<cvpi_pixel> {
  //   static Gen<cvpi_pixel> arbitrary() {
  //     return gen::build<cvpi_pixel>(gen::set(&cvpi_pixel::all, gen::inRange(0, 4294967296)));
  //   };
  // };
};

class EGL_session {
public:
  cvpi_egl_settings settings = NULL;
  cvpi_egl_instance instance = NULL;
  EGL_session() {
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
  };

  ~EGL_session() {
    cvpi_egl_instance_takedown(instance);
    if(settings != NULL) {
      free(settings);
      settings = NULL;
    }
  };
};

bool compare_mats(cv::Mat &cvpi_mat, cv::Mat &ocv_mat) {
  bool success = false;
  if(cvpi_mat.rows == ocv_mat.rows) {
    if(cvpi_mat.cols == ocv_mat.cols) {
      success = std::equal(ocv_mat.begin<uint32_t>(), ocv_mat.end<uint32_t>(), cvpi_mat.begin<uint32_t>());
      if(!success  && !failedOnce) {
	std::cerr << "Values differ" << std::endl;
	std::cerr << "height: " << cvpi_mat.rows << " width: " << cvpi_mat.cols << std::endl;
	//std::cerr << "CVPI" << std::endl << cvpi_mat << std::endl << std::endl;
	//std::cerr << "OpenCV" << std::endl << ocv_mat << std::endl << std::endl;
      }
    } else {
      success = false;
      // std::cerr << "heights differ: " << cvpi_mat.cols << " " << ocv_mat.cols << std::endl;
    }
  } else {
    success = false;
    // std::cerr << "widths differ: " << cvpi_mat.rows << " " << ocv_mat.rows << std::endl;
  }
  return success;
}

bool add_image_test_common(uint32_t *image_data_1, uint32_t *image_data_2, Dimensions &dimensions) {
  uint32_t image_pix_cnt = dimensions.width * dimensions.height;
  EGL_session *session = NULL;
  try {
    session = new EGL_session();
  } catch(std::exception& e) {
    RC_FAIL(e.what());
  }

  uint32_t *cvpi_out = new uint32_t[image_pix_cnt];
  cv::Mat cv_out;

  VGImage image1 = vgCreateImage(CVPI_COLOR_SPACE, dimensions.width, dimensions.height,VG_IMAGE_QUALITY_NONANTIALIASED);
  VGImage image2 = vgCreateImage(CVPI_COLOR_SPACE, dimensions.width, dimensions.height,VG_IMAGE_QUALITY_NONANTIALIASED);
  vgFinish();
  vgImageSubData(image1, image_data_1, dimensions.width*CVPI_PIXEL_BYTES, CVPI_COLOR_SPACE, 0, 0, dimensions.width, dimensions.height);
  vgImageSubData(image2, image_data_2, dimensions.width*CVPI_PIXEL_BYTES, CVPI_COLOR_SPACE, 0, 0, dimensions.width, dimensions.height);
  vgFinish();
  VGImage cvpi_sum = cvpi_image_add(image1, image2, 1, 1, 1, 0);
  vgFinish();
  vgGetImageSubData(cvpi_sum, cvpi_out, CVPI_PIXEL_BYTES*dimensions.width, CVPI_COLOR_SPACE, 0, 0, dimensions.width, dimensions.height);
  vgFinish();
  vgDestroyImage(image1);
  vgDestroyImage(image2);
  vgDestroyImage(cvpi_sum);

  // OpenCV calculation
  cv::Mat cv_image_1 = cv::Mat(dimensions.height, dimensions.width, CV_8UC4, image_data_1);
  cv::Mat cv_image_2 = cv::Mat(dimensions.height, dimensions.width, CV_8UC4, image_data_2);

  cv::add(cv_image_1, cv_image_2, cv_out);

  cv::Mat cvpi_sum_mat = cv::Mat(dimensions.height, dimensions.width, CV_8UC4, cvpi_out);

  bool success = compare_mats(cvpi_sum_mat, cv_out);
  // if(!success && !failedOnce) {
  //   std::cerr << std::endl << cvpi_sum_mat << std::endl << std::endl;
  //   std::cerr << std::endl << cv_image_1 << std::endl << std::endl;
  //   std::cerr << std::endl << cv_image_2 << std::endl << std::endl;
  //   failedOnce = true;
  // }

  if(session != NULL) {
    delete session;
  }

  delete cvpi_out;

  return success;
}

int main(int argc, char **argv) {
  int test_num = 0;
  if(argc > 1) {
    std::stringstream ss(argv[1]);
    if((ss >> test_num).fail()) {
      std::cerr << "Failed to select a check to run." << std::endl;
      return 1;
    }
  }
  if(test_num == 0 || test_num == 1) {
    rc::check("add images large const", [](const BDimensions &dimensions, const unsigned int &val_1, const unsigned int &val_2) {
	uint32_t image_pix_cnt = dimensions.width * dimensions.height;
	uint32_t* image_data_1 = new uint32_t[image_pix_cnt];
	uint32_t* image_data_2 = new uint32_t[image_pix_cnt];
	std::fill_n(image_data_1, image_pix_cnt, val_1);
	std::fill_n(image_data_2, image_pix_cnt, val_2);

	bool success = add_image_test_common(image_data_1, image_data_2, (Dimensions&)dimensions);

	delete image_data_1;
	delete image_data_2;

	RC_ASSERT(success);
      });
  }
  if(test_num == 0 || test_num == 2) {
    rc::check("add images large const small", [](const BDimensions &dimensions, const unsigned char &val_1, const unsigned char &val_2) {
	uint32_t image_pix_cnt = dimensions.width * dimensions.height;
	uint32_t* image_data_1 = new uint32_t[image_pix_cnt];
	uint32_t* image_data_2 = new uint32_t[image_pix_cnt];
	cvpi_pixel pix_img_1;
	cvpi_pixel pix_img_2;
	pix_img_1.channel[0] = val_1;
	pix_img_1.channel[1] = val_1;
	pix_img_1.channel[2] = val_1;
	pix_img_1.channel[3] = val_1;
	pix_img_2.channel[0] = val_2;
	pix_img_2.channel[1] = val_2;
	pix_img_2.channel[2] = val_2;
	pix_img_2.channel[3] = val_2;
	std::fill_n(image_data_1, image_pix_cnt, pix_img_1.all);
	std::fill_n(image_data_2, image_pix_cnt, pix_img_2.all);

	bool success = add_image_test_common(image_data_1, image_data_2, (Dimensions&)dimensions);

	delete image_data_1;
	delete image_data_2;

	RC_ASSERT(success);
      });
  }
  if(test_num == 0 || test_num == 3) {
    rc::check("add images large random", [](const BDimensions &dimensions, const unsigned int &seed_1,  const unsigned int &seed_2) {
	uint32_t image_pix_cnt = dimensions.width * dimensions.height;
	uint32_t* image_data_1 = new uint32_t[image_pix_cnt];
	uint32_t* image_data_2 = new uint32_t[image_pix_cnt];

	std::default_random_engine generator_1;
	std::default_random_engine generator_2;
	std::uniform_int_distribution<uint32_t> distribution(0,4294967295);
	for(uint32_t i = 0; i < image_pix_cnt; ++i) {
	  image_data_1[i] = distribution(generator_1);
	  image_data_2[i] = distribution(generator_2);
	}

	bool success = add_image_test_common(image_data_1, image_data_2, (Dimensions&)dimensions);

	delete image_data_1;
	delete image_data_2;

	RC_ASSERT(success);
      });
  }
  if(test_num == 0 || test_num == 4) {
    rc::check("add images small", [](const std::array<uint32_t, MAX_SIZE> &image_data_1,
				     const std::array<uint32_t, MAX_SIZE> &image_data_2,
				     const SDimensions &dimensions) {
		uint32_t image_pix_cnt = dimensions.width * dimensions.height;
		uint32_t* image_1 = new uint32_t[image_pix_cnt];
		uint32_t* image_2 = new uint32_t[image_pix_cnt];
		std::copy_n(image_data_1.begin(), image_pix_cnt, image_1);
		std::copy_n(image_data_2.begin(), image_pix_cnt, image_2);
		RC_ASSERT(add_image_test_common(image_1, image_2, (Dimensions&)dimensions));
	      });
  }
  return 0;
}
