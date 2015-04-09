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

  /* The vgConvolve and vgSeparableConvolve bias parameter is a floating
     point value between -1 and 1. The GPU operates on 8-bit unsigned
     integer values. vgConvolveNormal uses a short between -255 and 255
     for the bias so that the user does not have to do the conversion
     from integer to float. */
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
