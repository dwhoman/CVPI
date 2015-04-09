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

/* The main CVPI header. Contains computer vision functions. */

#ifndef CVPI_IMAGE_FUNCTIONS
#define CVPI_IMAGE_FUNCTIONS 1

#ifndef _STDINT_H
#include <stdint.h>
#endif

#ifndef _OPENVG_H
#include <VG/openvg.h>
#endif

#ifndef CVPI_BASE
#include "cvpi_base.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

  /* the color space assumed for all functions */
#define CVPI_COLOR_SPACE VG_sARGB_8888
  /* number of bytes per pixel */
#define CVPI_PIXEL_BYTES 4

  /* Pixel mapping used by CVPI when outputting to cpu memory. The RGBA
     indexes are specific to the VG_sARGB_8888 color space. The YUV
     mappings are made by cvpi_yuyv2yuva. cvpi_pixel_alpha is the same
     index for both mappings.  */
  enum {cvpi_pixel_red = 2, cvpi_pixel_green = 1, cvpi_pixel_blue = 0, cvpi_pixel_alpha = 3};
  enum {cvpi_pixel_v = 2, cvpi_pixel_u = 1, cvpi_pixel_y = 0};

  /* represents an image pixel in main memory */
  typedef union {
    VGuint all;
    VGubyte channel[4];
  } cvpi_pixel;

  /*!
    Preconditions: For all functions, EGL must be enabled with OpenVG
    and EGL_PIXEL_FORMAT_ARGB_8888_BRCM.

    Images to be passed to these functions should be read in using
    VG_sARGB_8888 color space and be non-anti-aliased.
  */

  /*
    vgColorMatrix matrix for inverting an image's color channels
  */
  extern const VGfloat cvpi_invert_colors[20];
  /*
    vgColorMatrix matrix for reordering YUV color channels to the
    conventional order.  */
  extern const VGfloat cvpi_avuy2ayuv[20];

  /* vgColorMatrix matrix to compute the average channel value for each
     pixel. Puts the average in each channel. */
  extern const VGfloat cvpi_pixel_average[20];

  /* vgColorMatrix matrix to compute the average color channel value for
     each pixel. Puts the average in each color channel. The alpha
     channel remains unaffected. */
  extern const VGfloat cvpi_pixel_color_average[20];

  /* Replace all the data in each pixel channel with the data in the
     red, green, blue, or alpha channel respectively */
  extern const VGfloat cvpi_channel_red[20];
  extern const VGfloat cvpi_channel_green[20];
  extern const VGfloat cvpi_channel_blue[20];
  extern const VGfloat cvpi_channel_alpha[20];

  /* Mappings for $\lceil\sqrt{x}\rceil$, $\lfloor\sqrt{x}\rfloor$, and
     $round(\sqrt{x})$. For cvpi_sqrt_array_round, the input was rounded up when the fraction part
     was exactly one half. */
  extern const VGubyte cvpi_sqrt_array_floor[256];
  extern const VGubyte cvpi_sqrt_array_ceil[256];
  extern const VGubyte cvpi_sqrt_array_round[256];

  /* Arrays for conversion to binary format. */
  /* Maps 0 to 0 and non-zeros to 255 */
  extern const VGuint cvpi_binary_array[256];
  /* Maps 0 to 255 and non-zeros to 0 */
  extern const VGuint cvpi_binary_array_inverted[256];

  /* Utility arrays for use with vgLookup */
  extern const VGubyte cvpi_zeros_array[256]; /* all zeros */
  extern const VGubyte cvpi_255_array[256];   /* all 255s */
  extern const VGubyte cvpi_identity_array[256]; /* value = index */
  extern const VGubyte cvpi_inversion_array[256]; /* identity array in reverse */

  /* Sobel, Scharr, Prewitt, and Robert's Cross are filter matrices
     that can be used with vgConvolve as the kernel argument. The
     gradient magnitude can be computed using cvpi_image_magnitude. */
  extern const VGshort cvpi_filter_sobel_x[9];
  extern const VGshort cvpi_filter_sobel_y[9];

  extern const VGshort cvpi_filter_scharr_x[9];
  extern const VGshort cvpi_filter_scharr_y[9];

  extern const VGshort cvpi_filter_prewitt_x[9];
  extern const VGshort cvpi_filter_prewitt_y[9];

  extern const VGshort cvpi_filter_roberts_cross_x[4];
  extern const VGshort cvpi_filter_roberts_cross_y[4];

  /*!
    Convert YUYV (YUV 4:2:2) to YUVA with eight bits per channel, Y
    being the least significant byte so that writing to a file will
    produce YUVA in least significant byte first order. YUVA is mapped
    as VUYA to OpenVG's RGBA data structure. The input image is
    normally an image returned by a web camera and read in by
    vgCreateImage with CVPI_COLOR_SPACE for the format parameter. The
    input yuyv_image's width is half that of the output image's. The
    width and height are taken from YUYV_IMAGE. The function returns
    VG_INVALID_HANDLE if an error occurs. */
  VGImage cvpi_yuyv2yuva(const VGImage yuyv_image);

  /*!
    Add two image matrices each scaled by `a' and `b' respectively; for
    each pixel $C_{i,j} = s\cdot(a\cdot A_{i,j} + b\cdot
    B_{i,j})+t$. If `a' or `b' is negative, then the images are
    subtracted. BIAS is divided by 255 before begin passed to
    vgConvolve. Returns VG_INVALID_HANDLE if an error occurs.

    Preconditions: img1 and img2 must have the same dimensions
  */
  VGImage cvpi_image_add(const VGImage img1, const VGImage img2, VGshort a, VGshort b, VGfloat scale, VGfloat bias);

  /* Add two channels, CHANNEL1 and CHANNEL2, in the same image. BIAS is
     divided by 255 before being passed to vgConvolve. The result is
     copied to OUTPUT_CHANNELS. The function follows the same formula as
     cvpi_image_add. */
  VGImage cvpi_channel_add(const VGImage image, VGImageChannel channel1, VGImageChannel channel2,
			   VGshort a, VGshort b, VGfloat scale, 
			   VGfloat bias_r, VGfloat bias_g, VGfloat bias_b, VGfloat bias_a,
			   VGbitfield output_channels);

  /* Adds the red, green, and blue channels. The result is copied to the
     channels specified in output_channels. BIAS is divided by 255
     before being passed to vgConvolve. Each channel is scaled
     independently before adding. */
  VGImage cvpi_color_channels_add(const VGImage image, VGfloat scale_r, VGfloat scale_g,
				  VGfloat scale_b,
				  VGfloat bias_r, VGfloat bias_g, VGfloat bias_b,
				  VGbitfield output_channels);

  /* Adds the red, green, blue, and alpha channels. The result is
     copied to the channels specified in output_channels. The function
     uses vgColorMatrixNormal to add the channels so bias gets divided
     by 255. Each channel is scaled independently before adding. Bias
     only gets applied to output_channels. The values of unspecified
     channels are unchanged.*/
  VGImage cvpi_all_channels_add(const VGImage image, VGfloat scale_r, VGfloat scale_g,
				VGfloat scale_b, VGfloat scale_a,
				VGfloat bias_r, VGfloat bias_g, VGfloat bias_b, VGfloat bias_a,
				VGbitfield output_channels);

  /* Combine two images by selecting the channels to include from the
     first image (VG_RED | VG_GREEN | VG_BLUE | VG_ALPHA). The
     unselected channels are included from the second image. */
  VGImage cvpi_image_combine_channelwise(const VGImage img1, const VGImage img2, VGbitfield img1_channels);

  /* Return an image that only contains colors between two indices,
     bound1 and bound2 (inclusive), if 'invert' is CVPI_FALSE; or
     outside two indices (exclusive), if invert is CVPI_TRUE. Areas
     not included are set to the fill value. Channels not specified by
     CHANNEL are unchanged. */
  VGImage cvpi_channel_threshold(const VGImage image, VGImageChannel channel, 
				 VGubyte bound1, VGubyte bound2, 
				 VGubyte fill, CVPI_BOOL invert);

  /* Threshold an image. Channels are treated independently if DEPENDENT
     if CVPI_FALSE; else if true, then all of the channels in a pixel
     will be thresholded if all of them fall outside the bounds. If any
     of the INVERT values are true, then  all of the channels in a pixel
     will be thresholded if all of them fall inside the bounds. */
  VGImage cvpi_image_threshold(const VGImage image, 
			       const VGubyte* lower_bound, const VGubyte* upper_bound, 
			       const VGubyte* fill, const CVPI_BOOL* invert, CVPI_BOOL dependent);

  /* Block threshold an image by breaking it into sectors of size
     SECTOR_WIDTH and SECTOR_HEIGHT, and then finding a STATISTIC of
     those sectors and thresholding those sectors with the calculated
     statistic. FILL gets passed to
     cvpi_image_threshold. INVERT is passed to the INVERT
     parameter of cvpi_image_threshold. If INVERT is CVPI_TRUE,
     then the pixels with values ranging from 0 to the computed
     threshold will be filled, otherwise the pixels ranging from the
     computed threshold to 255 will be filled. DEPENDENT is also passed
     to cvpi_channel_threshold. */
  VGImage cvpi_image_threshold_sector(const VGImage image, unsigned int sector_width,
				      unsigned int sector_height,
				      const VGubyte* fill, const CVPI_BOOL* invert, CVPI_BOOL dependent,
				      const VGuint (*statistic)(const VGImage image));

  /* Same as cvpi_image_threshold_sector, but only applied to one
     channel. The other channels are unaffected. */
  VGImage cvpi_channel_threshold_sector(const VGImage image, VGImageChannel channel,
					unsigned int sector_width, unsigned int sector_height,
					const VGubyte fill, const CVPI_BOOL invert,
					const VGubyte (*statistic)(const VGImage image));

  /* Adaptively threshold an image by using convolution to produce a
     mean value image. If WHITE is CVPI_TRUE, then the fill value is
     255 for each channel, else the fill value is 0. If ABOVE is
     CVPI_TRUE, then channel values at or above the threshold are kept
     and ones below are filled, else channel values at or below the
     threshold are kept and ones above are filled. If DEPENDENT is
     true, then only those pixels where all channel values fall within
     the threshold will be kept. The KERNEL_SIZE is the length of one
     side of a square kernel. If the square of KERNEL_SIZE is greater
     than VG_MAX_KERNEL_SIZE or if KERNEL_SIZE is less than 1, then
     VG_INVALID_HANDLE is returned. The function uses
     vgConvolveNormalNoShift, so BIAS is divided by 255 before being
     passed to vgConvolve. TILINGMODE is also passed to vgConvolve. */
  VGImage cvpi_image_threshold_adaptive_mean(const VGImage image, VGshort kernel_size,
					     CVPI_BOOL above, CVPI_BOOL white, CVPI_BOOL dependent,
					     VGfloat bias, VGTilingMode tilingMode);
  
  /* Add or subtract the specified channel from the other image
     channels. The user is responsible for setting the masking channel
     to consist of values of 255 and 0.  Adding will create a white
     mask and subtracting will create a black mask. If add is true,
     then the channels will be added, otherwise the mask will be
     subtracted from the image. */
  VGImage cvpi_image_mask_channel(const VGImage image, VGImageChannel maskChannel, CVPI_BOOL add);

  /* Computes a mean-like statistic by averaging vertically adjacent
     pixels until there is only an image of height 1. Then averaging
     is done horizontally. ITERATIONS is number of iterations of
     vertical averaging that should be done before returning the
     resulting image. If ITERATIONS is negative, then horizontal
     averaging will be done. If ITERATIONS is zero, the the resulting
     image will have a height of 1 and a width equal to the input
     IMAGE width. */
  VGImage cvpi_image_mean_gpu(const VGImage image, int iterations);

  /* Return the mean value of all the pixels in an image. */
  VGImage cvpi_image_mean_arithmetic(const VGImage image);

  /* Returns a VG_BW_1 image representative of sourceChannel. If
     nonzero_true is CVPI_TRUE then cvpi_binary_array is used for
     conversion to VG_BW_1, else cvpi_binary_array_inverted is used. */
  VGImage cvpi_image_rgba2bw(const VGImage color_image, VGImageChannel sourceChannel, CVPI_BOOL nonzero_true);

  /* For the following logical operations, if nonzero_true is CVPI_TRUE
     then 0 will be mapped to false and non-zero will be mapped to
     true. The arguments t_c and f_c are the values that true and false
     pixels will take in each channel. */

  /* image1 AND image2 */
  VGImage cvpi_image_logical_and(const VGImage image1, const VGImage image2, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);

  /* image1 NAND image2 */
  VGImage cvpi_image_logical_nand(const VGImage image1, const VGImage image2, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);

  /* image1 OR image2 */
  VGImage cvpi_image_logical_or(const VGImage image1, const VGImage image2, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);

  /* image1 NOR image2 */
  VGImage cvpi_image_logical_nor(const VGImage image1, const VGImage image2, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);

  /* image1 XOR image2 */
  VGImage cvpi_image_logical_xor(const VGImage image1, const VGImage image2, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);

  /* image1 XNOR image2 */
  VGImage cvpi_image_logical_xnor(const VGImage image1, const VGImage image2, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);

  /* Relative complement; the elements of image1 that are not in image2; image1 \ image2. */
  VGImage cvpi_image_logical_complement(const VGImage image1, const VGImage image2, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);

  /* The inverse of image1 \ image2. */
  VGImage cvpi_image_logical_complement_inv(const VGImage image1, const VGImage image2,
					    VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);

  /* Expand non-zero regions of IMAGE, decreasing zero filled regions,
     if nonzero_true is CVPI_TRUE, else expand zero regions of IMAGE,
     decreasing non-zero regions. See
     http://homepages.inf.ed.ac.uk/rbf/HIPR2/dilate.htm. */
  VGImage cvpi_image_dilate(const VGImage image, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);

  /* Retract non-zero regions of IMAGE, increasing zero filled
     regions, if nonzero_true is CVPI_TRUE, else retract zero regions
     of IMAGE, increasing non-zero regions. See
     http://homepages.inf.ed.ac.uk/rbf/HIPR2/erode.htm.*/
  VGImage cvpi_image_erode(const VGImage image, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);

  /* Perform a hit-and-miss transform, see http://homepages.inf.ed.ac.uk/rbf/HIPR2/hitmiss.htm */
  VGImage cvpi_image_hit_miss(const VGImage image, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);

  /* Similar to cvpi_image_dilate but uses cvpi_image_hit_miss. See
     http://homepages.inf.ed.ac.uk/rbf/HIPR2/thick.htm. */
  VGImage cvpi_image_thin(const VGImage image, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);
  /* Similar to cvpi_image_erode but uses cvpi_image_hit_miss. See
     http://homepages.inf.ed.ac.uk/rbf/HIPR2/thin.htm. */
  VGImage cvpi_image_thicken(const VGImage image, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);

  /* Returns a pointer to data on the heap that is of the specified
     CHANNEL in the given IMAGE. The data size in bytes is the IMAGE
     width * the IMAGE height. If an error occurs, then NULL is
     returned. */
  VGubyte* cvpi_channel2data(const VGImage image, VGImageChannel channel);
  /* Find the maximum value in the specified channel. If an error occurs
     then 0 is returned. */
  VGubyte cvpi_channel_max(const VGImage image, VGImageChannel channel);

  /* Find the minimum value in the specified channel. If an error occurs
     then 255 is returned. */
  VGubyte cvpi_channel_min(const VGImage image, VGImageChannel channel);

  /* Return a union containing the max (at index 0) and min (at index 1)
     values for the specified channel. If an error occurs then
     (uint32_t)~0 is returned. */
  typedef union {
    uint32_t all;
    uint16_t max_min[2];
  } cvpi_max_min;
  
  cvpi_max_min cvpi_channel_max_min(const VGImage image, VGImageChannel channel);

  /* Return an array of length 256, each index representing the
     number of occurrences of that color value. */
  unsigned int* cvpi_channel_histogram(const VGImage image, VGImageChannel channel);

  /* Return an array of length 768, each index representing the number
     of occurrences of that color value. The first 256 values are
     VG_RED values counts, the next 256 are VG_GREEN, and the last 256
     are VG_BLUE value counts. */
  unsigned int* cvpi_color_channels_histogram(const VGImage image);

  /* Return an array of length 1024, each index representing the
     number of occurrences of that color value. The first 256 values
     are VG_RED values counts, the next 256 are VG_GREEN, the next 256
     are VG_BLUE, and the last 256 are VG_ALPHA value counts. */
  unsigned int* cvpi_image_histogram(const VGImage image);

  /* Calculate the cumulative distribution for the specified
     HISTOGRAM. The histogram must be an array of length at-least
     256. WIDTH and HEIGHT are the image dimensions, which is used to
     calculate the number of elements, N, in the image being
     represented. The cumulative distribution is the cumulative sum of
     the histogram starting from index 0, divided by N. The result is
     rounded down to the nearest integer. */
  VGubyte* cvpi_channel_cumulative_distribution(const unsigned int* histogram, VGint width, VGint height);

  /* Calculate the cumulative distribution for the specified
     HISTOGRAM. The histogram must be an array of length at-least
     768. WIDTH and HEIGHT are the image dimensions, which is used to
     calculate the number of elements, N, in the image being
     represented. The cumulative distribution is the cumulative sum of
     the histogram starting from index 0, divided by N. The result is
     rounded down to the nearest integer. */
  VGubyte* cvpi_color_channels_cumulative_distribution(const unsigned int* histogram, VGint width, VGint height);
  /* Calculate the cumulative distribution for the specified
     HISTOGRAM. The histogram must be an array of length at-least
     1024. WIDTH and HEIGHT are the image dimensions, which is used to
     calculate the number of elements, N, in the image being
     represented. The cumulative distribution is the cumulative sum of
     the histogram starting from index 0, divided by N. The result is
     rounded down to the nearest integer. */
  VGubyte* cvpi_image_cumulative_distribution(const unsigned int* histogram, VGint width, VGint height);

  /* Return a histogram equalized image. If SCALE is true, then non-zero values
     are scaled to range between 0 and 255. */
  VGImage cvpi_channel_histogram_equalization(const VGImage image, VGImageChannel channel, CVPI_BOOL scale);
  VGImage cvpi_color_channels_histogram_equalization(const VGImage image, CVPI_BOOL scale);
  VGImage cvpi_image_histogram_equalization(const VGImage image, CVPI_BOOL scale);

  /* TODO use previous three functions to implement sector or block histogram equalization */
  VGImage cvpi_channel_histogram_equalization_sector(const VGImage image, VGImageChannel channel, CVPI_BOOL scale);
  VGImage cvpi_color_channels_histogram_equalization_sector(const VGImage image, CVPI_BOOL scale);
  VGImage cvpi_image_histogram_equalization(const VGImage image, CVPI_BOOL scale);
  
  enum cvpi_integer_truncation {cvpi_floor, cvpi_ceil, cvpi_round};

  /* Calculate the pixel magnitude between two images $\sqrt{img1^2 +
     img2^2}$. cvpi_integer_truncation selects the truncation type used
     in square and square root functions. */
  VGImage cvpi_image_magnitude(const VGImage image1, const VGImage image2,
			       enum cvpi_integer_truncation trunc);

  /* Using a union instead of a struct for coordinate values because
     the ARM cpu (including the Raspberry Pi) has instructions, such
     as ssub16, which can be used for parallel computation on
     half-words in regular r# registers. See section 3.7 Parallel
     Instructions, in "ARM Compiler tool-chain Version 4.1 Assembler
     Reference". xy[0] is the x value and xy[1] is the y value. */
  typedef union {
    uint32_t all;
    uint16_t xy[2];
  } cvpi_coordinate;

  typedef struct {
    cvpi_coordinate* coordinates;
    unsigned long length;
  } cvpi_coordinate_table;

  /* Create a table on the heap of x,y coordinate pairs where there are
     non-zero pixels in any of the channels.  If ORIGIN is FALSE, then
     function assumes that the image origin is in the upper left-hand
     corner with the x-axis proceeding in the positive to the right, and
     the y-axis proceeding positive downward. If ORIGIN is TRUE, then
     the y-coordinates will be subtracted from the image height so that
     the origin is in the lower left. */
  cvpi_coordinate_table* cvpi_image_coordinate_table(const VGImage image, VGbitfield channels,
						     CVPI_BOOL origin);
  void cvpi_image_coordinate_table_destroy(cvpi_coordinate_table *coordinate_table);

  /* Convert a cvpi image from YUVA to RGBA. The conversion is done on
     the CPU. YUVA is the input image and RGBA is the output image. YUVA
     and RGBA must be the same size; at least WIDTH * HEIGHT * 4
     bytes. If YUVA == RGBA, then the conversion is done
     destructively. Returns CVPI_FALSE if an error occurs.*/
  CVPI_BOOL cvpi_avuy2argb(cvpi_pixel* yuva, cvpi_pixel* rgba, unsigned long width, unsigned long height);

  /* Convert a VGImage to RGBA using cvpi_avuy2argb. Returns NULL if
     an error occurs. */
  cvpi_pixel* cvpi_image2argb(const VGImage image);

#ifdef __cplusplus
} /* extern "C" */
#endif

#else
#pragma message __FILE__ ": CVPI_IMAGE_FUNCTIONS already defined."
#endif
