/*
  This file is part of CVPI.

  Copyright 2017 Devin Homan
 
  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at
 
      http://www.apache.org/licenses/LICENSE-2.0
 
  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/

/* Augmented version of OpenVG functions. */

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

  /* Map the VGErrorCode to a descriptive string. */
  char* cvpi_vg_error_string(VGErrorCode error);

  /* The vgConvolve and vgSeparableConvolve bias parameter is a floating point
     value between -1 and 1. The GPU operates on 8-bit unsigned integer
     values. vgConvolveNormal expects the bias parameter to be a value between
     -255 and 255, inclusive.  When vgConvolve adds the bias, pixels are scaled
     from 0 to 1 rather than 0 to 255. vgConvolveNormal saves the user from
     having to do the mapping. */
  void vgConvolveNormal(VGImage dst, VGImage src,
			VGint kernelWidth, VGint kernelHeight,
			VGint shiftX, VGint shiftY,
			const VGshort * kernel,
			VGfloat scale,
			VGfloat bias,
			VGTilingMode tilingMode);

  void vgSeparableConvolveNormal(VGImage dst, VGImage src,
				 VGint kernelWidth, VGint kernelHeight,
				 VGint shiftX, VGint shiftY,
				 const VGshort * kernelX,
				 const VGshort * kernelY,
				 VGfloat scale,
				 VGfloat bias,
				 VGTilingMode tilingMode);
  
  /* The vgConvolve will shift the output relative to the input
     towards the origin unless shiftX = ceil(width/2) and shiftY =
     ceil(height/2).

     vgConvolveNoShift, vgSeparableConvolveNoShift,
     vgConvolveNormalNoShift and vgSeparableConvolveNormalNoShift do
     that shift automatically. */
  void vgConvolveNoShift(VGImage dst, VGImage src,
			 VGint kernelWidth, VGint kernelHeight,
			 const VGshort * kernel,
			 VGfloat scale,
			 VGfloat bias,
			 VGTilingMode tilingMode);

  void vgSeparableConvolveNoShift(VGImage dst, VGImage src,
				  VGint kernelWidth, VGint kernelHeight,
				  const VGshort * kernelX,
				  const VGshort * kernelY,
				  VGfloat scale,
				  VGfloat bias,
				  VGTilingMode tilingMode);
  
  void vgConvolveNormalNoShift(VGImage dst, VGImage src,
			       VGint kernelWidth, VGint kernelHeight,
			       const VGshort * kernel,
			       VGfloat scale,
			       VGfloat bias,
			       VGTilingMode tilingMode);

  void vgSeparableConvolveNormalNoShift(VGImage dst, VGImage src,
					VGint kernelWidth, VGint kernelHeight,
					const VGshort * kernelX,
					const VGshort * kernelY,
					VGfloat scale,
					VGfloat bias,
					VGTilingMode tilingMode);

  /* The vgColorMatrix convolution matrix elements m[0][4], m[1][4],
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

  /* VGPixel vgPixelFormatColorBitOffsets(VGImageFormat format); */

#ifdef __cplusplus
} /* extern "C" */
#endif

#else
#pragma message __FILE__ ": CVPI_VG_EXT already defined."
#endif
