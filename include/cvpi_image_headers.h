#ifndef CVPI_IMAGE_HEADERS
#define CVPI_IMAGE_HEADERS 1

#ifndef _STDIO_H
#include <stdio.h>
/* FILE */
#endif

#ifndef _STDINT_H
#include <stdint.h>
/* uint32_t, uint16_t, int32_t */
#endif

#ifndef CVPI_BASE
#include "cvpi_base.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define CVPI_BMP_DEFAULT cvpi_bmp_sARGB_8888

  typedef void* CVPI_BMP;

  enum cvpi_bmp_pixel_format {
    cvpi_bmp_sRGB_565,
    cvpi_bmp_sBGR_565,
    cvpi_bmp_sXRGB_8888,
    cvpi_bmp_sXBGR_8888,
    cvpi_bmp_sARGB_8888,
    cvpi_bmp_sBGRA_8888,
    cvpi_bmp_sRGBA_8888
  };
  
  /* types used in the BMP standard */
  typedef uint32_t cvpi_dword;
  typedef uint16_t cvpi_word;
  typedef int32_t  cvpi_long;
  
  /* Return a bitmap header in heap memory for an image with the given
     pixel WIDTH, HEIGHT, print resolution PPM, and
     cvpi_bmp_pixel_format. */
  CVPI_BMP cvpi_bmp_header_alloc(cvpi_long width, 
				 cvpi_long height,
				 cvpi_dword ppm, 
				 enum cvpi_bmp_pixel_format);
  /* Output the given BMP header to the given file. */
  CVPI_BOOL cvpi_bmp_header_write(FILE*, CVPI_BMP);

  /* Create a bitmap header for an image with the given pixel WIDTH,
     HEIGHT, print resolution PPM, and cvpi_bmp_pixel_format, and output
     to FILE. */
  CVPI_BOOL cvpi_bmp_header_alloc_write(FILE*, 
					cvpi_long width, 
					cvpi_long height,
					cvpi_dword ppm, 
					enum cvpi_bmp_pixel_format);


  /* portable bitmap format (PBM) */
  /* for used with VG_BW_1 and VG_A_1 */
  /* Create a PBM header for an image with the given pixel WIDTH and
     HEIGHT, and output to FILE. */
  void cvpi_pbm_header_write(FILE*, unsigned long width, unsigned long height);

  /* portable graymap format (PGM) */
  /* for used with VG_A_4, VG_A_8, vg_sL_8, and VG_lL_8 */
  /* Create a PGM header for an image with the given pixel WIDTH and
     HEIGHT, and output to FILE. */
  void cvpi_pgm_header_write(FILE*, unsigned long width, unsigned long height, unsigned char bits);

#ifdef __cplusplus
} /* extern "C" */
#endif

#else
#pragma message __FILE__ ": CVPI_IMAGE_HEADERS already defined."
#endif
