#ifndef _STDINT_H
#include <stdint.h>
/* uint32_t, uint16_t, int32_t */
#endif

#ifndef __STDDEF_H
#include <stddef.h>
/* size_t */
#endif

#ifndef _STDIO_H
#include <stdio.h>
/* FILE */
#endif

#ifndef transConfig_h
#include "transConfig.h"
#endif

#ifndef BITMAP_H
#define BITMAP_H 1
/* Begin bitmap output header */
typedef uint32_t DWORD;
typedef uint16_t WORD;
typedef int32_t  LONG;

/* from http://msdn.microsoft.com/en-us/library/windows/desktop/dd183392%28v=vs.85%29.aspx */
/* currently using the bitmap verison 4 header */
typedef struct __attribute__ ((__packed__)) {
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
} BITMAPHEADER;

/* from http://msdn.microsoft.com/en-us/library/cc250396.aspx */

/* note: fwrite reversed the bit order for non-decimal numbers */
typedef  enum 
{
  LCS_CALIBRATED_RGB = 0x00000000,
  LCS_sRGB = 0x42475273,
  LCS_WINDOWS_COLOR_SPACE = 0x206E6957
} LogicalColorSpace;

typedef struct {
  unsigned char bytes;		/* bytes per pixel */
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
} pixel_format;

  /* VG_sRGB_565       */
/* #if TRANS_COLOR_FMT == TRANS_RGB_565 */
/* static const pixel_format pf_VG_sRGB_565 = {2, */
/* 					    0x0000F800, /\* 2#00000000000000001111100000000000 *\/ */
/* 					    0x000007e0, /\* 2#00000000000000000000011111100000 *\/ */
/* 					    0x0000001F, /\* 2#00000000000000000000000000011111 *\/ */
/* 					    0,  */
/* 					    0,  */
/* 					    0,  */
/* 					    0,  */
/* 					    0,  */
/* 					    2, */
/* 					    LCS_sRGB}; */
  /* VG_sBGR_565       */
/* commented out with emacs comment-region, currently not using, could not find the corresponding EGL macro */
#if TRANS_COLOR_FMT == TRANS_RGB_565
static const pixel_format pf_VG_sRGB_565 = {2,
					    0x001F0000, /* 2#00000000000111110000000000000000 */
					    0x07E00000, /* 2#00000111111000000000000000000000 */
					    0xF8000000, /* 2#11111000000000000000000000000000 */
					    0,
					    0,
					    0,
					    0,
					    0,
					    2,
					    LCS_sRGB};
  /* VG_sXRGB_8888     */
#elif TRANS_COLOR_FMT == TRANS_RGB_888
static const pixel_format pf_VG_sXRGB_8888 = {3,
					      0x00FF0000, /* 2#00000000111111110000000000000000 */
					      0x0000FF00, /* 2#00000000000000001111111100000000 */
					      0x000000FF, /* 2#00000000000000000000000011111111 */
					      0, 
					      0, 
					      0, 
					      0, 
					      1, 
					      4,
					      LCS_sRGB};
  /* VG_sXBGR_8888     */
/* #elif TRANS_COLOR_FMT == TRANS_BGR_888 */
/* static const pixel_format pf_VG_sXBGR_8888 = {3, */
/* 					      0x0000FF00, /\* 2#00000000000000001111111100000000 *\/ */
/* 					      0x00FF0000, /\* 2#00000000111111110000000000000000 *\/ */
/* 					      0xFF000000, /\* 2#11111111000000000000000000000000 *\/ */
/* 					      0,  */
/* 					      0,  */
/* 					      0,  */
/* 					      0,  */
/* 					      1,  */
/* 					      4, */
/* 					      LCS_sRGB}; */
  /* VG_sARGB_8888     */
#elif TRANS_COLOR_FMT == TRANS_ARGB_8888
static const pixel_format pf_VG_sARGB_8888 = {4,
					      0xFF000000,
					      0x00FF0000, /* 2#00000000111111110000000000000000 */
					      0x0000FF00, /* 2#00000000000000001111111100000000 */
					      0x000000FF, /* 2#00000000000000000000000011111111 */
					      0,
					      0,
					      0,
					      0,
					      4,
					      LCS_sRGB};
  /* VG_sBGRA_8888     */
/* #elif TRANS_COLOR_FMT == TRANS_ARGB_8888 */
/* static const pixel_format pf_VG_sARGB_8888 = {4, */
/* 					      0x000000FF, */
/* 					      0x0000FF00, /\* 2#00000000000000001111111100000000 *\/ */
/* 					      0x00FF0000, /\* 2#00000000111111110000000000000000 *\/ */
/* 					      0xFF000000, /\* 2#11111111000000000000000000000000 *\/ */
/* 					      0, */
/* 					      0, */
/* 					      0, */
/* 					      0, */
/* 					      4, */
/* 					      LCS_sRGB}; */

  /* VG_sBGRX_8888     */
  /* VG_sRGBX_8888     */
  /* VG_sRGBA_8888     */
  /* VG_sRGBA_8888_PRE */
  /* VG_sRGBA_5551     */
  /* VG_sRGBA_4444     */
  /* VG_sL_8           */
  /* VG_lRGBX_8888     */
  /* VG_lRGBA_8888     */
  /* VG_lRGBA_8888_PRE */
  /* VG_lL_8           */
  /* VG_A_8            */
  /* VG_BW_1           */
  /* VG_A_1            */
  /* VG_A_4            */
  /* VG_sARGB_8888_PRE */
  /* VG_sARGB_1555     */
  /* VG_sARGB_4444     */
  /* VG_lXRGB_8888     */
  /* VG_lARGB_8888     */
  /* VG_lARGB_8888_PRE */
  /* VG_sBGRA_8888_PRE */
  /* VG_sBGRA_5551     */
  /* VG_sBGRA_4444     */
  /* VG_lBGRX_8888     */
  /* VG_lBGRA_8888     */
  /* VG_lBGRA_8888_PRE */
  /* VG_sABGR_8888     */
  /* VG_sABGR_8888_PRE */
  /* VG_sABGR_1555     */
  /* VG_sABGR_4444     */
  /* VG_lXBGR_8888     */
  /* VG_lABGR_8888     */
  /* VG_lABGR_8888_PRE */
#endif

BITMAPHEADER* createBMPHeader(unsigned long width, unsigned long height, DWORD ppm);
inline size_t writeBMPHeader(BITMAPHEADER* bmphdr, FILE *write_ptr);
#endif
