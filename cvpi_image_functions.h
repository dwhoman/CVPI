#ifndef _OPENVG_H
#include <VG/openvg.h>
#endif

#ifndef CVPI_IMAGE_FUNCTIONS
#define CVPI_IMAGE_FUNCTIONS 1

/* the color space assumed for all functions */
#define CVPI_COLOR_SPACE VG_sARGB_8888

/*!
   Preconditions: For all functions, EGL must be enabled with OpenVG
   and EGL_PIXEL_FORMAT_ARGB_8888_BRCM.

   Images to be passed to these functions should be read in using
   VG_sARGB_8888 color space and be non-anti-aliased.
 */

/*!
   Convert YUYV (YUV 4:2:2) to YUVA with eight bits per channel, Y
   being the least significant byte so that writing to a file will
   produce YUVA in least significant byte first order. YUVA is mapped
   as VUYA to OpenVG's RGBA data structure. The input image is
   normally an image returned by a web camera and read in by
   vgCreateImage with CVPI_COLOR_SPACE for the format parameter. The
   input yuyv_image's width is half that of the output image's. */
VGImage cvpi_yuyv2yuva(VGImage yuyv_image);

/*!
   Add two image matrices each scaled by `a' and `b' respectively; for
   each pixel \f$C_{i,j} = s\cdot(a\cdot A_{i,j} + b\cdot B_{i,j})+b\f$. If `a' or `b'
   is negative, then the images are subtracted. Returns
   VG_INVALID_HANDLE if an error occurs.

   Preconditions: img1 and img2 must have the same dimensions
 */
VGImage cvpi_add_images(VGImage img1, VGImage img2, VGshort a, VGshort b, VGfloat scale, VGfloat bias);

/*!
   vgColorMatrix matrix for inverting an image's color
*/
extern const VGfloat cvpi_invert_colors[20];

/*!
   vgColorMatrix matrix to convert YUVA to RGBA. YUVA is mapped as
   VUYA to OpenVG's RGBA data structure. The output is RGBA mapped to
   OpenVG's RGBA data structure.
 */
extern const VGfloat cvpi_yuva2rgba[20];

/* arrays for use with vgLookup */
extern const VGubyte cvpi_zeros_array[256];
extern const VGubyte cvpi_255_array[256];
extern const VGubyte cvpi_identity_array[256];
extern const VGubyte cvpi_inversion_array[256]; /* identity array in reverse */

//void split_channels(VGImage yuva_image, VGImage y_image, VGImage u_image, VGImage v_image);

/* VGfloat cvpi_filter_canny(VGImage image, enum FilterDirection separable); */

/* VGfloat cvpi_filter_sobel(VGImage image, enum FilterDirection separable); */

/* VGfloat cvpi_filter_box(VGImage image, unsigned long dimension, enum FilterDirection separable); */

/* VGfloat cvpi_filter_bilinear(VGImage image, enum FilterDirection separable); */

/* VGfloat cvpi_filter_gaussian(VGImage image, enum FilterDirection separable); */

/* VGfloat cvpi_filter_corner(VGImage image, enum FilterDirection separable); */
#else
#pragma message __FILE__ ": CVPI_IMAGE_FUNCTIONS already defined."
#endif
