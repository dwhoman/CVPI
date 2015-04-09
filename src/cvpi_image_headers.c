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

#ifndef _STDLIB_H
#include <stdlib.h>
#endif

#ifndef	_ERRNO_H
#include <errno.h>
#endif

#ifndef _MATH_H
#include <math.h>
/* abs() */
#endif

#ifndef CVPI_IMAGE_HEADERS
#include "cvpi_image_headers.h"
#endif

#ifndef CVPI_BASE
#include "cvpi_base.h"
#endif

#ifndef CVPI_PRIVATE_HEADER
#include "cvpi_private_header.h"
#endif

#define WORD cvpi_word
#define DWORD cvpi_dword
#define LONG cvpi_long

/* from http://msdn.microsoft.com/en-us/library/windows/desktop/dd183392%28v=vs.85%29.aspx */
/* currently using the bitmap verison 4 header */
struct cvpi_bitmap_header {
  WORD  bfType;
  /* The file type; must be BM = 0x424d. */

  DWORD bfSize;
  /* The size, in bytes, of the bitmap file. */

  WORD  bfReserved1;
  /* Reserved; must be zero. */

  WORD  bfReserved2;
  /* Reserved; must be zero. */

  DWORD bfOffBits;
  /* The offset, in bytes, from the beginning of the BITMAPFILEHEADER structure to the bitmap bits */

  DWORD        bV4Size;
/*     The number of bytes required by the structure. Applications
       should use this member to determine which bitmap information
       header structure is being used. */

  LONG         bV4Width;
/*     The width of the bitmap, in pixels. */
/*     If bV4Compression is BI_JPEG or BI_PNG, the bV4Width member
       specifies the width of the decompressed JPEG or PNG image in
       pixels. */
  LONG         bV4Height;
/*     The height of the bitmap, in pixels. If the value of bV4Height
       is positive, the bitmap is a bottom-up DIB and its origin is
       the lower-left corner. If bV4Height value is negative, the
       bitmap is a top-down DIB and its origin is the upper-left
       corner. */

/*     If bV4Height is negative, indicating a top-down DIB,
       bV4Compression must be either BI_RGB or BI_BITFIELDS. Top-down
       DIBs cannot be compressed. */

/*     If bV4Compression is BI_JPEG or BI_PNG, the bV4Height member
       specifies the height of the decompressed JPEG or PNG image in
       pixels. */
  WORD         bV4Planes;
/*     The number of planes for the target device. This value must be
       set to 1. */
  WORD         bV4BitCount;
/*     The number of bits that define each pixel and the maximum
       number of colors in the bitmap. */
  DWORD        bV4Compression;
  DWORD        bV4SizeImage;
  LONG         bV4XPelsPerMeter;
  LONG         bV4YPelsPerMeter;
  DWORD        bV4ClrUsed;
  DWORD        bV4ClrImportant;
  DWORD        bV4RedMask;
  DWORD        bV4GreenMask;
  DWORD        bV4BlueMask;
  DWORD        bV4AlphaMask;
  DWORD        bV4CSType;
  int8_t       bV4Endpoints_0;
  int8_t       bV4Endpoints_1;
  int8_t       bV4Endpoints_2;
  int8_t       bV4Endpoints_3;
  int8_t       bV4Endpoints_4;
  int8_t       bV4Endpoints_5;
  int8_t       bV4Endpoints_6;
  int8_t       bV4Endpoints_7;
  int8_t       bV4Endpoints_8;
  int8_t       bV4Endpoints_9;
  int8_t       bV4Endpoints_10;
  int8_t       bV4Endpoints_11;
  int8_t       bV4Endpoints_12;
  int8_t       bV4Endpoints_13;
  int8_t       bV4Endpoints_14;
  int8_t       bV4Endpoints_15;
  int8_t       bV4Endpoints_16;
  int8_t       bV4Endpoints_17;
  int8_t       bV4Endpoints_18;
  int8_t       bV4Endpoints_19;
  int8_t       bV4Endpoints_20;
  int8_t       bV4Endpoints_21;
  int8_t       bV4Endpoints_22;
  int8_t       bV4Endpoints_23;
  int8_t       bV4Endpoints_24;
  int8_t       bV4Endpoints_25;
  int8_t       bV4Endpoints_26;
  int8_t       bV4Endpoints_27;
  int8_t       bV4Endpoints_28;
  int8_t       bV4Endpoints_29;
  int8_t       bV4Endpoints_30;
  int8_t       bV4Endpoints_31;
  int8_t       bV4Endpoints_32;
  int8_t       bV4Endpoints_33;
  int8_t       bV4Endpoints_34;
  int8_t       bV4Endpoints_35;
  DWORD        bV4GammaRed;
  DWORD        bV4GammaGreen;
  DWORD        bV4GammaBlue;     
};

/* from http://msdn.microsoft.com/en-us/library/cc250396.aspx */

/* note: fwrite reversed the bit order for non-decimal numbers */
enum LogicalColorSpace {
  LCS_CALIBRATED_RGB = 0x00000000,
  LCS_sRGB = 0x42475273,
  LCS_WINDOWS_COLOR_SPACE = 0x206E6957
};

typedef struct {
  WORD         bytes;		/* bytes per pixel */
  DWORD        bV4RedMask;
  DWORD        bV4GreenMask;
  DWORD        bV4BlueMask;
  DWORD        bV4AlphaMask;
  DWORD        bV4GammaRed;
  DWORD        bV4GammaGreen;
  DWORD        bV4GammaBlue;
  WORD         padding;		/* bytes */
  DWORD        bV4SizeImage; /* bytes per pixel + padding  */
  DWORD        bV4CSType;
} pixel_format_s;


typedef struct cvpi_bitmap_header* cvpi_bitmap_header;

CVPI_BMP cvpi_bmp_header_alloc(cvpi_long width, 
			       cvpi_long height,
			       cvpi_dword ppm, 
			       enum cvpi_bmp_pixel_format pixel_f) {
  pixel_format_s pf;

  cvpi_bitmap_header bmph = NULL;

  switch(pixel_f) {
  case cvpi_bmp_sRGBA_8888:
    pf.bytes = 4;
    pf.bV4RedMask = 0xFF000000;
    pf.bV4GreenMask = 0x00FF0000; /* 2#00000000111111110000000000000000 */
    pf.bV4BlueMask = 0x0000FF00; /* 2#00000000000000001111111100000000 */
    pf.bV4AlphaMask = 0x000000FF; /* 2#00000000000000000000000011111111 */
    pf.bV4GammaRed = 0;
    pf.bV4GammaGreen = 0;
    pf.bV4GammaBlue = 0;
    pf.padding = 0;
    pf.bV4SizeImage = 4;
    pf.bV4CSType = LCS_sRGB;
    break;
  case cvpi_bmp_sARGB_8888:
    pf.bytes = 4;
    pf.bV4RedMask = 0x00FF0000; /* 2#00000000111111110000000000000000 */
    pf.bV4GreenMask = 0x0000FF00; /* 2#00000000000000001111111100000000 */
    pf.bV4BlueMask = 0x000000FF; /* 2#00000000000000000000000011111111 */
    pf.bV4AlphaMask = 0xFF000000;
    pf.bV4GammaRed = 0;
    pf.bV4GammaGreen = 0;
    pf.bV4GammaBlue = 0;
    pf.padding = 0;
    pf.bV4SizeImage = 4;
    pf.bV4CSType = LCS_sRGB;
    break;
  case cvpi_bmp_sRGB_565:
    pf.bytes =         2;
    pf.bV4RedMask =    0x0000F800; /* 2#00000000000000001111100000000000 */
    pf.bV4GreenMask =  0x000007e0; /* 2#00000000000000000000011111100000 */
    pf.bV4BlueMask =   0x0000001F; /* 2#00000000000000000000000000011111 */
    pf.bV4AlphaMask =  0;
    pf.bV4GammaRed =   0;
    pf.bV4GammaGreen = 0;
    pf.bV4GammaBlue =  0;
    pf.padding =       0;
    pf.bV4SizeImage =  2;
    pf.bV4CSType =     LCS_sRGB;
    break;
  case cvpi_bmp_sBGR_565:
    pf.bytes = 2;
    pf.bV4RedMask = 0x001F0000; /* 2#00000000000111110000000000000000 */
    pf.bV4GreenMask = 0x07E00000; /* 2#00000111111000000000000000000000 */
    pf.bV4BlueMask = 0xF8000000; /* 2#11111000000000000000000000000000 */
    pf.bV4AlphaMask = 0;
    pf.bV4GammaRed = 0;
    pf.bV4GammaGreen = 0;
    pf.bV4GammaBlue = 0;
    pf.padding = 0;
    pf.bV4SizeImage = 2;
    pf.bV4CSType = LCS_sRGB;
    break;
  case cvpi_bmp_sXRGB_8888:
    pf.bytes = 3;
    pf.bV4RedMask = 0x00FF0000; /* 2#00000000111111110000000000000000 */
    pf.bV4GreenMask = 0x0000FF00; /* 2#00000000000000001111111100000000 */
    pf.bV4BlueMask = 0x000000FF; /* 2#00000000000000000000000011111111 */
    pf.bV4AlphaMask = 0; 
    pf.bV4GammaRed = 0; 
    pf.bV4GammaGreen = 0; 
    pf.bV4GammaBlue = 0; 
    pf.padding = 1; 
    pf.bV4SizeImage = 4;
    pf.bV4CSType = LCS_sRGB;
    break;
  case cvpi_bmp_sXBGR_8888:
    pf.bytes = 3;
    pf.bV4RedMask = 0x0000FF00; /* 2#00000000000000001111111100000000 */
    pf.bV4GreenMask = 0x00FF0000; /* 2#00000000111111110000000000000000 */
    pf.bV4BlueMask = 0xFF000000; /* 2#11111111000000000000000000000000 */
    pf.bV4AlphaMask = 0;
    pf.bV4GammaRed = 0;
    pf.bV4GammaGreen = 0;
    pf.bV4GammaBlue = 0;
    pf.padding = 1;
    pf.bV4SizeImage = 4;
    pf.bV4CSType = LCS_sRGB;
    break;
  case cvpi_bmp_sBGRA_8888:
    pf.bytes = 4;
    pf.bV4RedMask = 0x000000FF;
    pf.bV4GreenMask = 0x0000FF00; /* 2#00000000000000001111111100000000 */
    pf.bV4BlueMask = 0x00FF0000; /* 2#00000000111111110000000000000000 */
    pf.bV4AlphaMask = 0xFF000000; /* 2#11111111000000000000000000000000 */
    pf.bV4GammaRed = 0;
    pf.bV4GammaGreen = 0;
    pf.bV4GammaBlue = 0;
    pf.padding = 0;
    pf.bV4SizeImage = 4;
    pf.bV4CSType = LCS_sRGB;
    break;
  }

  bmph = malloc(sizeof(*bmph));
  if(bmph == NULL) {
    cvpi_log_3("malloc returned NULL: errno = ", __FILE__, __LINE__, errno);
    return NULL;
  }

  DWORD offset = 122;		/* BMPv4 is 122 bytes long */
  bmph->bfOffBits = offset;
  bmph->bV4BitCount = pf.bytes*8; /* bits per pixel */
  /* equation from http://en.wikipedia.org/wiki/BMP_file_format */
  bmph->bV4SizeImage = floor((bmph->bV4BitCount * abs(width) + 31)/32)*4 * abs(height);
  /* but this should be faster and return the same value */
  //bmph->bV4SizeImage = (width*pf->bytes + pf->padding) * height;
  bmph->bfSize = offset + bmph->bV4SizeImage;
  bmph->bfType = 0x4d42;	/* bitmap magic number */
  bmph->bfReserved1 = 0;
  bmph->bfReserved2 = 0;
  /* size of the total bitmap header - size of the BITMAPFILEHEADER; size of the BITMAPV4HEADER */
  /* will have to be modified if a color table or padding is used */
  bmph->bV4Size = offset - 3*sizeof(WORD) - 2*sizeof(DWORD);
  bmph->bV4Width = width;
  bmph->bV4Height = height;
  bmph->bV4Planes = 1;
  bmph->bV4Compression = 0; /* no compression */
  bmph->bV4XPelsPerMeter = ppm; /* print resolution; assume little endian */
  bmph->bV4YPelsPerMeter = ppm;
  bmph->bV4ClrUsed = 0; /* no colors in pallet */
  bmph->bV4ClrImportant = 0; /* all colors are used */
  bmph->bV4RedMask = pf.bV4RedMask;
  bmph->bV4GreenMask = pf.bV4GreenMask;
  bmph->bV4BlueMask = pf.bV4BlueMask;
  bmph->bV4AlphaMask = pf.bV4AlphaMask;
  bmph->bV4CSType = pf.bV4CSType;
  bmph->bV4Endpoints_0 = 0; 	
  bmph->bV4Endpoints_1 = 0; 
  bmph->bV4Endpoints_2 = 0; 
  bmph->bV4Endpoints_3 = 0; 
  bmph->bV4Endpoints_4 = 0; 
  bmph->bV4Endpoints_5 = 0; 
  bmph->bV4Endpoints_6 = 0; 
  bmph->bV4Endpoints_7 = 0; 
  bmph->bV4Endpoints_8 = 0; 
  bmph->bV4Endpoints_9 = 0; 
  bmph->bV4Endpoints_10 = 0;
  bmph->bV4Endpoints_11 = 0;
  bmph->bV4Endpoints_12 = 0;
  bmph->bV4Endpoints_13 = 0;
  bmph->bV4Endpoints_14 = 0;
  bmph->bV4Endpoints_15 = 0;
  bmph->bV4Endpoints_16 = 0;
  bmph->bV4Endpoints_17 = 0;
  bmph->bV4Endpoints_18 = 0;
  bmph->bV4Endpoints_19 = 0;
  bmph->bV4Endpoints_20 = 0;
  bmph->bV4Endpoints_21 = 0;
  bmph->bV4Endpoints_22 = 0;
  bmph->bV4Endpoints_23 = 0;
  bmph->bV4Endpoints_24 = 0;
  bmph->bV4Endpoints_25 = 0;
  bmph->bV4Endpoints_26 = 0;
  bmph->bV4Endpoints_27 = 0;
  bmph->bV4Endpoints_28 = 0;
  bmph->bV4Endpoints_29 = 0;
  bmph->bV4Endpoints_30 = 0;
  bmph->bV4Endpoints_31 = 0;
  bmph->bV4Endpoints_32 = 0;
  bmph->bV4Endpoints_33 = 0;
  bmph->bV4Endpoints_34 = 0;
  bmph->bV4Endpoints_35 = 0;
  bmph->bV4GammaRed = pf.bV4GammaRed;
  bmph->bV4GammaGreen = pf.bV4GammaGreen;
  bmph->bV4GammaBlue = pf.bV4GammaBlue;

  return (CVPI_BMP)bmph;
}

#define write_member(member)\
  do {\
    size_t member_size = sizeof(member);			\
    size_t written = fwrite(&(member), 1, member_size, write_ptr);	\
    int flushed = fflush(write_ptr);				\
    if(written != member_size || flushed != 0) {		\
      if(written != member_size) {					\
	cvpi_log_4("Error writing bmp header: fwrite size discrepancy:", \
		   __func__, __LINE__, member_size, written);		\
      } else {								\
	cvpi_log_4("Error writing bmp header: fflush errno = ",		\
		   __func__, __LINE__, errno, flushed);			\
      }									\
      return CVPI_FALSE;						\
    }									\
  } while(0)

CVPI_BOOL cvpi_bmp_header_write(FILE *write_ptr, CVPI_BMP h) {
  cvpi_bitmap_header header = (cvpi_bitmap_header)h;

  if(header == NULL || write_ptr == NULL) {
    return CVPI_FALSE;
  }

  write_member(header->bfType);
  write_member(header->bfSize);
  write_member(header->bfReserved1);
  write_member(header->bfReserved2);
  write_member(header->bfOffBits);
  write_member(header->bV4Size);
  write_member(header->bV4Width);
  write_member(header->bV4Height);
  write_member(header->bV4Planes);
  write_member(header->bV4BitCount);
  write_member(header->bV4Compression);	 
  write_member(header->bV4SizeImage);	 
  write_member(header->bV4XPelsPerMeter); 
  write_member(header->bV4YPelsPerMeter); 
  write_member(header->bV4ClrUsed);	 
  write_member(header->bV4ClrImportant);	 
  write_member(header->bV4RedMask);	 
  write_member(header->bV4GreenMask);	 
  write_member(header->bV4BlueMask);	 
  write_member(header->bV4AlphaMask);	 
  write_member(header->bV4CSType);	 
  write_member(header->bV4Endpoints_0);	 
  write_member(header->bV4Endpoints_1);	 
  write_member(header->bV4Endpoints_2);	 
  write_member(header->bV4Endpoints_3);	 
  write_member(header->bV4Endpoints_4);	 
  write_member(header->bV4Endpoints_5);	 
  write_member(header->bV4Endpoints_6);	 
  write_member(header->bV4Endpoints_7);	 
  write_member(header->bV4Endpoints_8);	 
  write_member(header->bV4Endpoints_9);	 
  write_member(header->bV4Endpoints_10);	 
  write_member(header->bV4Endpoints_11);	 
  write_member(header->bV4Endpoints_12);	 
  write_member(header->bV4Endpoints_13);	 
  write_member(header->bV4Endpoints_14);	 
  write_member(header->bV4Endpoints_15);	 
  write_member(header->bV4Endpoints_16);	 
  write_member(header->bV4Endpoints_17);	 
  write_member(header->bV4Endpoints_18);	 
  write_member(header->bV4Endpoints_19);	 
  write_member(header->bV4Endpoints_20);	 
  write_member(header->bV4Endpoints_21);	 
  write_member(header->bV4Endpoints_22);	 
  write_member(header->bV4Endpoints_23);	 
  write_member(header->bV4Endpoints_24);	 
  write_member(header->bV4Endpoints_25);	 
  write_member(header->bV4Endpoints_26);	 
  write_member(header->bV4Endpoints_27);	 
  write_member(header->bV4Endpoints_28);	 
  write_member(header->bV4Endpoints_29);	 
  write_member(header->bV4Endpoints_30);	 
  write_member(header->bV4Endpoints_31);	 
  write_member(header->bV4Endpoints_32);	 
  write_member(header->bV4Endpoints_33);	 
  write_member(header->bV4Endpoints_34);	 
  write_member(header->bV4Endpoints_35);	 
  write_member(header->bV4GammaRed);	 
  write_member(header->bV4GammaGreen);	 
  write_member(header->bV4GammaBlue);     

  return CVPI_TRUE;
}
#undef write_member

CVPI_BOOL cvpi_bmp_header_alloc_write(FILE* write_ptr, 
				      cvpi_long width, 
				      cvpi_long height,
				      cvpi_dword ppm, 
				      enum cvpi_bmp_pixel_format pixel_f) {
  cvpi_bitmap_header header = (cvpi_bitmap_header)cvpi_bmp_header_alloc(width, height, ppm, pixel_f);
  if(header != NULL) {
    CVPI_BOOL ret = cvpi_bmp_header_write(write_ptr, header);
    freeSafe(header);
    return ret;
  } else {
    return CVPI_FALSE;
  }
}

/* portable bitmap format (PBM) */
void cvpi_pbm_header_write(FILE *write_ptr, unsigned long width, unsigned long height) {
  int written = fprintf(write_ptr, "P4\n%ld %ld\n", width, height);
  int flushed = fflush(write_ptr);
  if(written < 0 || flushed != 0) {
    if(written < 0) {
      cvpi_log_3("Error writing pbm header: fflush errno = ", __func__, __LINE__, written);
    } else {
      cvpi_log_4("Error writing pbm header: fflush errno = ",
		 __func__, __LINE__, errno, flushed);
    }
  }
}

/* portable graymap format (PGM) */
void cvpi_pgm_header_write(FILE *write_ptr, unsigned long width, unsigned long height, unsigned char bits) {
  int written = fprintf(write_ptr, "P5\n%ld %ld\n%hhd\n", width, height, bits);
  int flushed = fflush(write_ptr);
  if(written < 0 || flushed != 0) {
    if(written < 0) {
      cvpi_log_3("Error writing pgm header: fprintf returned:",
		 __func__, __LINE__, written);
    } else {
      cvpi_log_4("Error writing pgm header: fflush errno = ",
		 __func__, __LINE__, errno, flushed);
    }
  }
}
