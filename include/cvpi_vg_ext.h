#ifndef CVPI_VG_EXT
#define CVPI_VG_EXT 1

#ifndef _OPENVG_H
#include <VG/openvg.h>
#endif

#ifndef CVPI_BASE
#include "cvpi_base.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

char* cvpi_vg_error_string(VGErrorCode error);

/* The vgConvolve bias parameter is a floating point value between -1
   and 1. The GPU operates on 8-bit unsigned integter
   values. vgConvolveNormal uses a short between -255 and 255 for the
   bias so that the user does not have to do the conversion from
   integer to float. */
inline void vgConvolveNormal(VGImage dst, VGImage src,
			     VGint kernelWidth, VGint kernelHeight,
			     VGint shiftX, VGint shiftY,
			     const VGshort * kernel,
			     VGfloat scale,
			     short bias,
			     VGTilingMode tilingMode);

inline void vgSeparableConvolveNormal(VGImage dst, VGImage src,
				      VGint kernelWidth, VGint kernelHeight,
				      VGint shiftX, VGint shiftY,
				      const VGshort * kernelX,
				      const VGshort * kernelY,
				      VGfloat scale,
				      short bias,
				      VGTilingMode tilingMode);
  
  /* The vgConvolve will shift the output relative to the input unless
     shiftX = width -1 and shiftY = height -
     1. vgConvolveNormalNoShift and
     vgSeparableConvolveNormalNoShift do that shift automatically. */
inline void vgConvolveNormalNoShift(VGImage dst, VGImage src,
				    VGint kernelWidth, VGint kernelHeight,
				    const VGshort * kernel,
				    VGfloat scale,
				    short bias,
				    VGTilingMode tilingMode);

  inline void vgSeparableConvolveNormalNoShift(VGImage dst, VGImage src,
					       VGint kernelWidth, VGint kernelHeight,
					       const VGshort * kernelX,
					       const VGshort * kernelY,
					       VGfloat scale,
					       short bias,
					       VGTilingMode tilingMode);

/* The vgColorMatrix convolution matrix elemeents m[0][4], m[1][4],
   m[2][4], m[3][4] each require a floating point value between 0 and
   1. vgColorMatrixNormal allows for these values to be between 0 and
   255, which is consistent with the image output. */
void vgColorMatrixNormal(VGImage dst, VGImage src, const VGfloat * matrix);

/* Create a solid color (RGBA) image */
VGImage vgCreateImagePainted(VGImageFormat fmt,
			     VGint width, VGint height, VGbitfield quality,
			     unsigned char red, unsigned char green, 
			     unsigned char blue, unsigned char alpha);

  /* Returns the number of bits in FORMAT */
VGint vgPixelBits(VGImageFormat format);

#define vgDestroyImageSafe(image)do{if(image != VG_INVALID_HANDLE){vgDestroyImage(image); image=VG_INVALID_HANDLE;}}while(0)

/* VGPixel vgPixelFormatColorBitOffsets(VGImageFormat format); */

#ifdef __cplusplus
} /* extern "C" */
#endif

#else
#pragma message __FILE__ ": CVPI_VG_EXT already defined."
#endif
