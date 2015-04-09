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

#ifndef CVPI_IMAGE_TESTS
#define CVPI_IMAGE_TESTS 1

#ifndef CVPI
#include "cvpi.h"
#endif

/* cvpi_pixel */
CVPI_BOOL test_cvpi_pixel(void);

/* cvpi_yuyv2yuva */
CVPI_BOOL test_cvpi_yuyv2yuva(void);

/* cvpi_image_add */
CVPI_BOOL test_cvpi_image_add_odd(void);
CVPI_BOOL test_cvpi_image_add_single(void);
CVPI_BOOL test_cvpi_image_add_even(void);
CVPI_BOOL test_cvpi_subtract_images_odd(void);
CVPI_BOOL test_cvpi_image_add_odd_scale_add(void);
CVPI_BOOL test_cvpi_subtract_images_odd_scale_add(void);
CVPI_BOOL test_cvpi_image_add_huge_even(void);
CVPI_BOOL test_cvpi_image_add_huge_odd(void);

/* cvpi_channel_add */
CVPI_BOOL test_cvpi_channel_add_RB(void);
CVPI_BOOL test_cvpi_channel_add_RR(void);

/* cvpi_color_channels_add */
CVPI_BOOL test_cvpi_color_channels_add(void);

/* cvpi_all_channels_add */
CVPI_BOOL test_cvpi_all_channels_add(void);

/* cvpi_image_combine_channelwise */
CVPI_BOOL test_cvpi_image_combine_channelwise(void);

/* cvpi_channel_threshold */
CVPI_BOOL test_cvpi_channel_threshold(void);
CVPI_BOOL test_cvpi_channel_threshold_inv(void);
CVPI_BOOL test_cvpi_channel_threshold_same(void);
CVPI_BOOL test_cvpi_channel_threshold_inv_same(void);

/* cvpi_image_threshold */
CVPI_BOOL test_cvpi_image_threshold(void);
CVPI_BOOL test_cvpi_image_threshold_inv(void);
CVPI_BOOL test_cvpi_image_threshold_dependent(void);
CVPI_BOOL test_cvpi_image_threshold_inv_dependent(void);

/* cvpi_image_threshold_sector */
CVPI_BOOL test_cvpi_image_threshold_sector(void);
CVPI_BOOL test_cvpi_image_threshold_sector_inv(void);
CVPI_BOOL test_cvpi_image_threshold_sector_dependent(void);
CVPI_BOOL test_cvpi_image_threshold_sector_inv_dependent(void);

/* cvpi_channel_threshold_sector */
/* cvpi_image_threshold_adaptive */
/* cvpi_channel_threshold_adaptive */

/* cvpi_image_mask_channel */
CVPI_BOOL test_cvpi_image_mask_channel_add(void);
CVPI_BOOL test_cvpi_image_mask_channel_subtract(void);

/* cvpi_image_mean */
CVPI_BOOL test_cvpi_image_mean_small(void);
CVPI_BOOL test_cvpi_image_mean_max(void);
CVPI_BOOL test_cvpi_image_mean_huge(void);

/* Compare the mean function against the arithmetic mean functoion. */
CVPI_BOOL test_cvpi_image_mean_compare(void);

/* cvpi_image_rgba2bw */
CVPI_BOOL test_cvpi_image_rgba_to_binary(void);

/* Logical operation tests ending in NOT test zero = true and non-zero
   = false. Logical operation tests not ending in NOT test zero =
   false and non-zero = true. */
/* cvpi_image_logical_and */
CVPI_BOOL test_cvpi_image_logical_and(void);
CVPI_BOOL test_cvpi_image_logical_and_NOT(void);
/* cvpi_image_logical_nand */
CVPI_BOOL test_cvpi_image_logical_nand(void);
CVPI_BOOL test_cvpi_image_logical_nand_NOT(void);
/* cvpi_image_logical_or */
CVPI_BOOL test_cvpi_image_logical_or(void);
CVPI_BOOL test_cvpi_image_logical_or_NOT(void);
/* cvpi_image_logical_nor */
CVPI_BOOL test_cvpi_image_logical_nor(void);
CVPI_BOOL test_cvpi_image_logical_nor_NOT(void);
/* cvpi_image_logical_xor */
CVPI_BOOL test_cvpi_image_logical_xor(void);
CVPI_BOOL test_cvpi_image_logical_xor_NOT(void);
/* cvpi_image_logical_xnor */
CVPI_BOOL test_cvpi_image_logical_xnor(void);
CVPI_BOOL test_cvpi_image_logical_xnor_NOT(void);
/* cvpi_image_logical_complement */
CVPI_BOOL test_cvpi_image_logical_complement(void);
CVPI_BOOL test_cvpi_image_logical_complement_NOT(void);
/* cvpi_image_logical_complement_inv */
CVPI_BOOL test_cvpi_image_logical_complement_inv(void);
CVPI_BOOL test_cvpi_image_logical_complement_inv_NOT(void);
/* cvpi_image_dilate */
CVPI_BOOL test_cvpi_image_dilate(void);
CVPI_BOOL test_cvpi_image_dilate_NOT(void);
/* cvpi_image_erode */
CVPI_BOOL test_cvpi_image_erode(void);
CVPI_BOOL test_cvpi_image_erode_NOT(void);
/* cvpi_image_hit_miss */
CVPI_BOOL test_cvpi_image_hit_miss(void);
CVPI_BOOL test_cvpi_image_hit_miss_NOT(void);
/* cvpi_image_thin */
CVPI_BOOL test_cvpi_image_thin(void);
CVPI_BOOL test_cvpi_image_thin_NOT(void);
/* cvpi_image_thicken */
CVPI_BOOL test_cvpi_image_thicken(void);
CVPI_BOOL test_cvpi_image_thicken_NOT(void);

/* cvpi_channel_max */
CVPI_BOOL test_cvpi_channel_max_alpha(void);
CVPI_BOOL test_cvpi_channel_max_red(void);
CVPI_BOOL test_cvpi_channel_max_green(void);
CVPI_BOOL test_cvpi_channel_max_blue(void);
/* cvpi_channel_min */
CVPI_BOOL test_cvpi_channel_min_alpha(void);
CVPI_BOOL test_cvpi_channel_min_red(void);
CVPI_BOOL test_cvpi_channel_min_green(void);
CVPI_BOOL test_cvpi_channel_min_blue(void);
/* cvpi_channel_max_min */
CVPI_BOOL test_cvpi_channel_max_min_alpha(void);
CVPI_BOOL test_cvpi_channel_max_min_red(void);
CVPI_BOOL test_cvpi_channel_max_min_green(void);
CVPI_BOOL test_cvpi_channel_max_min_blue(void);
/* cvpi_channel_histogram */
CVPI_BOOL test_cvpi_channel_histogram_alpha(void);
CVPI_BOOL test_cvpi_channel_histogram_red(void);
CVPI_BOOL test_cvpi_channel_histogram_green(void);
CVPI_BOOL test_cvpi_channel_histogram_blue(void);
/* cvpi_color_channels_histogram */
CVPI_BOOL test_cvpi_color_channels_histogram(void);
/* cvpi_image_histogram */
CVPI_BOOL test_cvpi_image_histogram(void);
/* cvpi_channel_cumulative_distribution */
CVPI_BOOL test_cvpi_channel_cumulative_distribution_alpha(void);
CVPI_BOOL test_cvpi_channel_cumulative_distribution_red(void);
CVPI_BOOL test_cvpi_channel_cumulative_distribution_green(void);
CVPI_BOOL test_cvpi_channel_cumulative_distribution_blue(void);
/* cvpi_color_channels_cumulative_distribution */
CVPI_BOOL test_cvpi_color_channels_cumulative_distribution(void);
/* cvpi_image_cumulative_distribution */
CVPI_BOOL test_cvpi_image_cumulative_distribution(void);

/* cvpi_image_coordinate_table */
CVPI_BOOL test_cvpi_image_coordinate_table_zero(void);
CVPI_BOOL test_cvpi_image_coordinate_table_some(void);
CVPI_BOOL test_cvpi_image_coordinate_table_all(void);

/* filters */
CVPI_BOOL test_cvpi_sobel(void);
CVPI_BOOL test_cvpi_scharr(void);
CVPI_BOOL test_cvpi_prewitt(void);
CVPI_BOOL test_cvpi_roberts_cross(void);

/* color conversion */
CVPI_BOOL test_cvpi_avuy2argb(void);
CVPI_BOOL test_cvpi_invert_colors(void);
CVPI_BOOL test_cvpi_pixel_average(void);
CVPI_BOOL test_cvpi_pixel_color_average(void);
/* CVPI_BOOL test_cvpi_pixel2yuv(void); */
CVPI_BOOL test_cvpi_avuy2ayuv(void);

CVPI_BOOL test_cvpi_image2argb(void);

/* OpenVG behavior tests */
/* Overflow behavior is undefined in the standard. The implementation
   appears to not allow values to exceed their max or wrap around, so
   255 + 1 = 255. Similaraly 0 - 1 = 0. Some functions rely on this
   behavior such as masking. */
CVPI_BOOL test_overflow_behavior(void);
CVPI_BOOL test_negative_behavior(void);

CVPI_BOOL test_vgConvolveNoShift(void);

CVPI_BOOL test_cvpi_bmp_header_alloc_write(void);
#else
#pragma message __FILE__ ": CVPI_IMAGE_TESTS already defined."
#endif
