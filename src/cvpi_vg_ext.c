/*
  This file is part of CVPI.

  Copyright (C) 2015 Devin Homan

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

#ifndef CVPI_VG_EXT
#include "cvpi_vg_ext.h"
#endif

#ifndef _OPENVG_H
#include <VG/openvg.h>
#endif

#ifndef _STDIO_H
#include <stdio.h>
#endif

/* 1/255 = 0.00392156862745 */
#define INV255 0.00392156862745

char* cvpi_vg_error_string(VGErrorCode error) {
  switch(error) {
  case VG_NO_ERROR:
    return " VG_NO_ERROR ";
  case VG_BAD_HANDLE_ERROR:
    return " VG_BAD_HANDLE_ERROR ";
  case VG_ILLEGAL_ARGUMENT_ERROR:
    return " VG_ILLEGAL_ARGUMENT_ERROR ";
  case VG_OUT_OF_MEMORY_ERROR:
    return " VG_OUT_OF_MEMORY_ERROR ";
  case VG_PATH_CAPABILITY_ERROR:
    return " VG_PATH_CAPABILITY_ERROR ";
  case VG_UNSUPPORTED_IMAGE_FORMAT_ERROR:
    return " VG_UNSUPPORTED_IMAGE_FORMAT_ERROR ";
  case VG_UNSUPPORTED_PATH_FORMAT_ERROR:
    return " VG_UNSUPPORTED_PATH_FORMAT_ERROR ";
  case VG_IMAGE_IN_USE_ERROR:
    return " VG_IMAGE_IN_USE_ERROR ";
  case VG_NO_CONTEXT_ERROR:
    return " VG_NO_CONTEXT_ERROR ";
  default:
    return " undefined error ";
  }
}

void vgConvolveNormal(VGImage dst, VGImage src,
		      VGint kernelWidth, VGint kernelHeight,
		      VGint shiftX, VGint shiftY,
		      const VGshort * kernel,
		      VGfloat scale,
		      VGfloat bias,
		      VGTilingMode tilingMode) {
  vgConvolve(dst, src,
	     kernelWidth, kernelHeight, shiftX, shiftY,
	     kernel, scale, bias * INV255, tilingMode);
}

void vgSeparableConvolveNormal(VGImage dst, VGImage src,
			       VGint kernelWidth, VGint kernelHeight,
			       VGint shiftX, VGint shiftY,
			       const VGshort * kernelX,
			       const VGshort * kernelY,
			       VGfloat scale,
			       VGfloat bias,
			       VGTilingMode tilingMode) {
  vgSeparableConvolve(dst, src,
		      kernelWidth, kernelHeight,
		      shiftX, shiftY,
		      kernelX,
		      kernelY,
		      scale,
		      bias * INV255,
		      tilingMode);
}

void vgConvolveNoShift(VGImage dst, VGImage src,
		       VGint kernelWidth, VGint kernelHeight,
		       const VGshort * kernel,
		       VGfloat scale,
		       VGfloat bias,
		       VGTilingMode tilingMode) {
  vgConvolve(dst, src,
	     kernelWidth, kernelHeight,
	     kernelWidth/2, kernelHeight/2,
	     kernel, scale, bias, tilingMode);
}

void vgSeparableConvolveNoShift(VGImage dst, VGImage src,
				VGint kernelWidth, VGint kernelHeight,
				const VGshort * kernelX,
				const VGshort * kernelY,
				VGfloat scale,
				VGfloat bias,
				VGTilingMode tilingMode) {
  vgSeparableConvolve(dst, src,
		      kernelWidth, kernelHeight,
		      kernelWidth/2, kernelHeight/2,
		      kernelX,
		      kernelY,
		      scale,
		      bias,
		      tilingMode);
}

void vgConvolveNormalNoShift(VGImage dst, VGImage src,
			     VGint kernelWidth, VGint kernelHeight,
			     const VGshort * kernel,
			     VGfloat scale,
			     VGfloat bias,
			     VGTilingMode tilingMode) {
  vgConvolveNormal(dst, src,
		   kernelWidth, kernelHeight,
		   kernelWidth/2, kernelHeight/2,
		   kernel, scale, bias, tilingMode);
}

void vgSeparableConvolveNormalNoShift(VGImage dst, VGImage src,
				      VGint kernelWidth, VGint kernelHeight,
				      const VGshort * kernelX,
				      const VGshort * kernelY,
				      VGfloat scale,
				      VGfloat bias,
				      VGTilingMode tilingMode) {
  vgSeparableConvolveNormal(dst, src,
			    kernelWidth, kernelHeight,
			    kernelWidth/2, kernelHeight/2,
			    kernelX,
			    kernelY,
			    scale,
			    bias,
			    tilingMode);
}

void vgColorMatrixNormal(VGImage dst, VGImage src,
			 const VGfloat * matrix) {
  VGfloat m[20];
  m[0] = matrix[0];
  m[1] = matrix[1];
  m[2] = matrix[2];
  m[3] = matrix[3];
  m[4] = matrix[4];
  m[5] = matrix[5];
  m[6] = matrix[6];
  m[7] = matrix[7];
  m[8] = matrix[8];
  m[9] = matrix[9];
  m[10] = matrix[10];
  m[11] = matrix[11];
  m[12] = matrix[12];
  m[13] = matrix[13];
  m[14] = matrix[14];
  m[15] = matrix[15];
  m[16] = matrix[16] * INV255;
  m[17] = matrix[17] * INV255;
  m[18] = matrix[18] * INV255;
  m[19] = matrix[19] * INV255;
  vgColorMatrix(dst, src, m);
}

VGImage vgCreateImagePainted(VGImageFormat fmt,
			     VGint width, VGint height, VGbitfield quality,
			     unsigned char red, unsigned char green,
			     unsigned char blue, unsigned char alpha) {
  const VGfloat image1_color[20] = {
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,

    red,green,blue,alpha
  };
  VGErrorCode error;
  VGImage image0 = vgCreateImage(fmt, width, height, quality);
#ifdef CVPI_ERROR_CHECK
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    return VG_INVALID_HANDLE;
  }
#endif
  VGImage image1 = vgCreateImage(fmt, width, height, quality);
#ifdef CVPI_ERROR_CHECK
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    cvpi_log_2("%s:%d:%s\n", __func__, __LINE__, cvpi_vg_error_string(error));
    vgDestroyImage(image0);
    vgFlush();
    return VG_INVALID_HANDLE;
  }
#endif
  vgColorMatrixNormal(image1, image0, image1_color);
  vgFinish();
  vgDestroyImage(image0);
  vgFlush();
  return image1;
}

VGint vgPixelBits(VGImageFormat format) {
  switch(format) {
  case VG_sRGBX_8888:
  case VG_sRGBA_8888:
  case VG_sRGBA_8888_PRE:
  case VG_lRGBA_8888_PRE:
  case VG_lRGBX_8888:
  case VG_lRGBA_8888:
  case VG_sXRGB_8888:
  case VG_sARGB_8888:
  case VG_sARGB_8888_PRE:
  case VG_lXRGB_8888:
  case VG_lARGB_8888:
  case VG_lARGB_8888_PRE:
  case VG_sBGRX_8888:
  case VG_sBGRA_8888:
  case VG_sBGRA_8888_PRE:
  case VG_lBGRX_8888:
  case VG_lBGRA_8888:
  case VG_lBGRA_8888_PRE:
  case VG_sXBGR_8888:
  case VG_sABGR_8888:
  case VG_sABGR_8888_PRE:
  case VG_lXBGR_8888:
  case VG_lABGR_8888:
  case VG_lABGR_8888_PRE:
    return 32;
  case VG_sBGR_565:
  case VG_sRGB_565:
  case VG_sABGR_1555:
  case VG_sBGRA_5551:
  case VG_sARGB_1555:
  case VG_sRGBA_5551:
  case VG_sARGB_4444:
  case VG_sRGBA_4444:
  case VG_sBGRA_4444:
  case VG_sABGR_4444:
    return 16;
  case VG_sL_8:
  case VG_lL_8:
  case VG_A_8:
    return 8;
  case VG_A_4:
    return 4;
  case VG_A_1:
  case VG_BW_1:
    return 1;
  default:
    /* VG_UNSUPPORTED_IMAGE_FORMAT_ERROR */
    cvpi_log_1("%s: Unsupported image format.\n", __func__);
    return 0;
  }
}

/* VGPixel vgPixelFormatColorBitOffsets(VGImageFormat format) { */
/*   VGPixel offsets; */
/*   switch(format) { */
/*   case VG_sRGBX_8888: */
/*   case VG_sRGBA_8888: */
/*   case VG_sRGBA_8888_PRE: */
/*   case VG_lRGBA_8888_PRE: */
/*   case VG_lRGBX_8888: */
/*   case VG_lRGBA_8888: */
/*     offsets.rgba[0] = 0; */
/*     offsets.rgba[1] = 8; */
/*     offsets.rgba[2] = 16; */
/*     offsets.rgba[3] = 24; */
/*     return offsets; */

/*   case VG_sXRGB_8888: */
/*   case VG_sARGB_8888: */
/*   case VG_sARGB_8888_PRE: */
/*   case VG_lXRGB_8888: */
/*   case VG_lARGB_8888: */
/*   case VG_lARGB_8888_PRE: */
/*     offsets.rgba[0] = 8; */
/*     offsets.rgba[1] = 16; */
/*     offsets.rgba[2] = 24; */
/*     offsets.rgba[3] = 0; */
/*     return offsets; */

/*   case VG_sBGRX_8888: */
/*   case VG_sBGRA_8888: */
/*   case VG_sBGRA_8888_PRE: */
/*   case VG_lBGRX_8888: */
/*   case VG_lBGRA_8888: */
/*   case VG_lBGRA_8888_PRE: */
/*     offsets.rgba[0] = 16; */
/*     offsets.rgba[1] = 8; */
/*     offsets.rgba[2] = 0; */
/*     offsets.rgba[3] = 24; */
/*     return offsets; */

/*   case VG_sXBGR_8888: */
/*   case VG_sABGR_8888: */
/*   case VG_sABGR_8888_PRE: */
/*   case VG_lXBGR_8888: */
/*   case VG_lABGR_8888: */
/*   case VG_lABGR_8888_PRE: */
/*     offsets.rgba[0] = 24; */
/*     offsets.rgba[1] = 16; */
/*     offsets.rgba[2] = 8; */
/*     offsets.rgba[3] = 0; */
/*     return offsets; */

/*   case VG_sABGR_1555: */
/*     offsets.rgba[0] = 11; */
/*     offsets.rgba[1] = 6; */
/*     offsets.rgba[2] = 1; */
/*     offsets.rgba[3] = 0; */
/*     return offsets; */

/*   case VG_sBGRA_5551: */
/*     offsets.rgba[0] = 10; */
/*     offsets.rgba[1] = 5; */
/*     offsets.rgba[2] = 0; */
/*     offsets.rgba[3] = 15; */
/*     return offsets; */

/*   case VG_sARGB_1555: */
/*     offsets.rgba[0] = 1; */
/*     offsets.rgba[1] = 6; */
/*     offsets.rgba[2] = 11; */
/*     offsets.rgba[3] = 0; */
/*     return offsets; */

/*   case VG_sRGBA_5551: */
/*     offsets.rgba[0] = 0; */
/*     offsets.rgba[1] = 5; */
/*     offsets.rgba[2] = 10; */
/*     offsets.rgba[3] = 15; */
/*     return offsets; */

/*   case VG_sABGR_4444: */
/*     offsets.rgba[0] = 12; */
/*     offsets.rgba[1] = 8; */
/*     offsets.rgba[2] = 4; */
/*     offsets.rgba[3] = 0; */
/*     return offsets; */

/*   case VG_sARGB_4444: */
/*     offsets.rgba[0] = 4; */
/*     offsets.rgba[1] = 8; */
/*     offsets.rgba[2] = 12; */
/*     offsets.rgba[3] = 0; */
/*     return offsets; */

/*   case VG_sBGRA_4444: */
/*     offsets.rgba[0] = 8; */
/*     offsets.rgba[1] = 4; */
/*     offsets.rgba[2] = 0; */
/*     offsets.rgba[3] = 12; */
/*     return offsets; */
/*   case VG_sRGBA_4444: */
/*     offsets.rgba[0] = 0; */
/*     offsets.rgba[1] = 4; */
/*     offsets.rgba[2] = 8; */
/*     offsets.rgba[3] = 12; */
/*     return offsets; */

/*   case VG_sBGR_565: */
/*     offsets.rgba[0] = 11; */
/*     offsets.rgba[1] = 5; */
/*     offsets.rgba[2] = 0; */
/*     offsets.rgba[3] = 0; */
/*     return offsets; */

/*   case VG_sRGB_565: */
/*     offsets.rgba[0] = 0; */
/*     offsets.rgba[1] = 5; */
/*     offsets.rgba[2] = 11; */
/*     offsets.rgba[3] = 0; */
/*     return offsets; */

/*   case VG_sL_8: */
/*   case VG_lL_8: */
/*   case VG_A_8: */
/*   case VG_A_4: */
/*   case VG_A_1: */
/*   case VG_BW_1: */
/*     offsets.all = 0; */
/*     return offsets; */
/*   default: */
/*     /\* VG_UNSUPPORTED_IMAGE_FORMAT_ERROR *\/ */
/*     fprintf(cvpi_log_file, "%s: Unsupported image format.\n", __func__); */
/*     offsets.all = ~0; */
/*     return offsets; */
/*   } */
/* } */
