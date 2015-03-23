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
#define CVPI_PIXEL_BYTES 4

/* Pixel mapping used by CVPI when outputting to cpu memory. The RGBA
   indexes are specific to the VG_sARGB_8888 color space. The YUV
   mappings are made by cvpi_yuyv2yuva. cvpi_pixel_alpha is the same
   index for both mappings.  */
enum {cvpi_pixel_red = 2, cvpi_pixel_green = 1, cvpi_pixel_blue = 0, cvpi_pixel_alpha = 3};
enum {cvpi_pixel_v = 2, cvpi_pixel_u = 1, cvpi_pixel_y = 0};

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

/* Sobel, Scharr, Prewitt, and Robert's Cross are filter matricies
   that can be used with vgConvolve as the kernel argument. The
   gradient magnitude can be computed using cvpi_magnitude. */
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
   VG_INVALID_HANDLE if a VG error occurs while gettings the width or height. */
VGImage cvpi_yuyv2yuva(const VGImage yuyv_image);

/*!
   Add two image matrices each scaled by `a' and `b' respectively; for
   each pixel $C_{i,j} = s\cdot(a\cdot A_{i,j} + b\cdot B_{i,j})+t$. If `a' or `b'
   is negative, then the images are subtracted. Returns
   VG_INVALID_HANDLE if an error occurs.

   Preconditions: img1 and img2 must have the same dimensions
 */
VGImage cvpi_add_images(const VGImage img1, const VGImage img2, VGshort a, VGshort b, VGfloat scale, unsigned char bias);

/* Add two channels in the same image.  The result is copied to all four channels. */
VGImage cvpi_add_channels(const VGImage image, VGImageChannel channel1, VGImageChannel channel2,
			  VGshort a, VGshort b, VGfloat scale, VGfloat bias, VGbitfield output_channels);

/* Adds the red, green, and blue channels. The result is copied to the
   channels specified in output_channels. The function uses
   vgColorMatrixNormal to add the channels so bias can vary between 0
   and 255. Each channel is scaled independently before adding. */
VGImage cvpi_add_channels_color(const VGImage image, VGfloat scale_r, VGfloat scale_g,
				VGfloat scale_b, VGfloat bias, VGbitfield output_channels);

/* Adds the red, green, blue, and alpha channels. The result is copied
   to the channels specified in output_channels. The function uses
   vgColorMatrixNormal to add the channels so bias can vary between 0
   and 255. Each channel is scaled independently before adding. */
VGImage cvpi_add_channels_all(const VGImage image, VGfloat scale_r, VGfloat scale_g,
			      VGfloat scale_b, VGfloat scale_a, VGfloat bias, VGbitfield output_channels);

/* Combine two images by selecting the channels to include from the
   first image (VG_RED | VG_GREEN | VG_BLUE | VG_ALPHA). The
   unselected channels are included from the second image. */
VGImage cvpi_combine_channelwise(const VGImage img1, const VGImage img2, VGbitfield img1_channels);

/* Return an image that only contains colors between two indices,
   bound1 and bound2 (inclusive), if 'invert' is CVPI_FALSE; or
   outside two indices (exclusive), if invert is CVPI_TRUE. Areas not
   included are set to the fill value. */
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

/* TODO */
VGImage cvpi_channel_threshold_sector();
/* TODO */
VGImage cvpi_image_threshold_adaptive(const VGImage image, unsigned int sector_width, unsigned int sector_height, 
				      const VGubyte* fill, const CVPI_BOOL* invert, CVPI_BOOL dependent, 
				      const VGuint (*statistic)(const VGImage image));
/* TODO */
VGImage cvpi_channel_threshold_adaptive();

/* Add or subtract the specified channel from the other image channels
   Adding will create a white mask and subtracting will create a black
   mask. If add is true, then the channels will be added, otherwise the
   mask will be subtracted from the image. */
VGImage cvpi_image_channel_mask(const VGImage image, VGImageChannel maskChannel, CVPI_BOOL add);

/* Return the mean value of all the pixels in an image. */
VGImage cvpi_image_mean_gpu(const VGImage image);
VGImage cvpi_image_mean_arithmetic(const VGImage image);

/* Returns a VG_BW_1 image representative of sourceChannel. If
   nonzero_true is CVPI_TRUE then cvpi_binary_array is used for
   conversion to VG_BW_1, else cvpi_binary_array_inverted is used. */
VGImage cvpi_image_rgba_to_bw(const VGImage color_image, VGImageChannel sourceChannel, CVPI_BOOL nonzero_true);

/* For the following logical operations, if nonzero_true is CVPI_TRUE
   then 0 will be mapped to false and non-zero will be mapped to
   true. The arguments t_c and f_c are the values that true and false
   pixels will take in each chanel. */

/* image1 AND image2 */
inline VGImage cvpi_image_logical_and(const VGImage image1, const VGImage image2, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);

/* image1 NAND image2 */
inline VGImage cvpi_image_logical_nand(const VGImage image1, const VGImage image2, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);

/* image1 OR image2 */
inline VGImage cvpi_image_logical_or(const VGImage image1, const VGImage image2, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);

/* image1 NOR image2 */
inline VGImage cvpi_image_logical_nor(const VGImage image1, const VGImage image2, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);

/* image1 XOR image2 */
inline VGImage cvpi_image_logical_xor(const VGImage image1, const VGImage image2, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);

/* image1 XNOR image2 */
inline VGImage cvpi_image_logical_xnor(const VGImage image1, const VGImage image2, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);

/* Relative complement; the elements of image1 that are not in image2; image1 \ image2. */
inline VGImage cvpi_image_logical_complement(const VGImage image1, const VGImage image2, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);

/* The inverse of image1 \ image2. */
inline VGImage cvpi_image_logical_complement_inv(const VGImage image1, const VGImage image2, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);

VGImage cvpi_image_dialate(const VGImage image, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);

VGImage cvpi_image_erode(const VGImage image, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);

VGImage cvpi_image_hit_miss(const VGImage image, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);

VGImage cvpi_image_thin(const VGImage image, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);

VGImage cvpi_image_thicken(const VGImage image, VGubyte t_c, VGubyte f_c, CVPI_BOOL nonzero_true);

/* Find the maximum value in the specified channel. */
VGubyte cvpi_channel_max(const VGImage image, VGImageChannel channel);

/* Find the minimum value in the specified channel. */
VGubyte cvpi_channel_min(const VGImage image, VGImageChannel channel);

/* Return an array on the heap containing the max (at index 0) and min
   (at index 1) values for the specified channel. */
VGubyte* cvpi_channel_max_min(const VGImage image, VGImageChannel channel);

/* Return an array of length 256, each index representing the
   number of occurances of that color value. */
unsigned int* cvpi_channel_histogram(const VGImage image, VGImageChannel channel);

/* Return an array of length 768, each index representing the
   number of occurances of that color value. */
unsigned int* cvpi_color_channels_histogram(const VGImage image);

unsigned int* cvpi_image_histogram(const VGImage image);

double* cvpi_channel_cumulative_distribution(const unsigned int* histogram, VGint width, VGint height);

double* cvpi_color_channels_cumulative_distribution(const unsigned int* histogram, VGint width, VGint height);

double* cvpi_image_cumulative_distribution(const unsigned int* histogram, VGint width, VGint height);

enum cvpi_integer_truncation {cvpi_floor, cvpi_ceil, cvpi_round};

/* Calculate the pixel magnitude between two images $\sqrt{img1^2 +
   img2^2}$. cvpi_integer_truncation selects the truncation type used
   in square and square root functions. */
VGImage cvpi_magnitude(const VGImage image1, const VGImage image2, enum cvpi_integer_truncation trunc);

/* Using a union instead of a struct for coordinate values because the
   ARM cpu (including the Raspberry Pi) has instructions, such as
   ssub16, which can be used for parallel computation on half-words in
   regular r# registers. See section 3.7 Parallel Instructions, in
   "ARM Compiler toolchain Version 4.1 Assembler Reference". */
typedef union {
  uint32_t all;
  uint16_t xy[2];
} cvpi_coordinate;

typedef struct {
  cvpi_coordinate* coordinates;
  unsigned long length;
} cvpi_coordinate_table;

/* Create a table on the heap of x,y coordinate pairs where there are
   non-zero pixels in any of the channels. The resulting table has a
   maximum of width*height elements. If the number of non-zero pixels
   is less than the number of possible coordinate pairs, the data
   table will be terminated by a ~0. */
cvpi_coordinate_table* cvpi_image_coordinate_table(const VGImage image, VGbitfield channels);
void cvpi_image_coordinate_table_destroy(cvpi_coordinate_table *coordinate_table);

typedef struct {
  double x;
  double y;
} cvpi_precise_coordinate;

typedef struct {
  cvpi_precise_coordinate* coordinates;
  unsigned long length;
} cvpi_precise_coordinate_table;

/* Uses the lens correction parameters (k1, k2, p1, p2, k3) determined
 by OpenCV's camera calibration functionality to correct for camera
 distortion in the location of data points. This is not intended to
 correct an entire image as the correction is CPU intensive and the
 GPU cannot be relied upon. The cvpi_coordinate_table is generated by cvpi_image_coordinate_table. See
 http://docs.opencv.org/trunk/doc/tutorials/calib3d/camera_calibration/camera_calibration.html */
cvpi_precise_coordinate_table* cvpi_coordinate_table_lens_correct(const cvpi_coordinate_table *coordinate_table,
								  double k1, double k2, double p1, double p2, double k3);
void cvpi_precise_coordinate_table_destroy(cvpi_precise_coordinate_table *coordinate_table);

typedef struct {
  double radius;		
  double theta;
} cvpi_polar_coordinate;

typedef struct {
  cvpi_polar_coordinate* coordinates;
  unsigned long length;
} cvpi_polar_coordinate_table;

/* Given a cvpi_coordinate_table generated by
   cvpi_image_coordinate_table, map the table values to polar
   coordinates. */
cvpi_polar_coordinate_table* cvpi_image_polar_coordinate_table(const cvpi_coordinate_table *ct);

/* Given a cvpi_precise_coordinate_table generated by
   cvpi_image_coordinate_table_lens_correct, map the table values to polar
   coordinates. */
cvpi_polar_coordinate_table* cvpi_image_polar_coordinate_table_lens_corrected(const cvpi_precise_coordinate_table *ct);
void cvpi_image_polar_coordinate_table_destroy(cvpi_polar_coordinate_table *ct);

/* Convert a cvpi image from YUVA to RGBA. The conversion is done on
   the CPU. YUVA is the input image and RGBA is the output image. YUVA
   and RGBA must be the same size; at least WIDTH * HEIGHT * 4
   bytes. If YUVA == RGBA, then the conversion is done
   destructively. Use cvpi_bmp_sARGB_8888 if the output is being
   output into a bitmap image.*/
void cvpi_avuy2argb(cvpi_pixel* yuva, cvpi_pixel* rgba, unsigned long width, unsigned long height);
cvpi_pixel* cvpi_image_rgba(const VGImage image);

#ifdef __cplusplus
} /* extern "C" */
#endif

#else
#pragma message __FILE__ ": CVPI_IMAGE_FUNCTIONS already defined."
#endif
