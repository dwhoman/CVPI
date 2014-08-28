#ifndef _MATH_H
#include "math.h"
/* abs() */
#endif

#ifndef BITMAP_H
#include "bitmap.h"
#endif

#ifndef transConfig_h
#include "transConfig.h"
#endif

/* Create a bitmap header binary data structure for an image with the
   given pixel WIDTH and HEIGHT and print resolution PPM. The pixel
   format is set in transConfig.h The header is written to a file
   using writeBMPHeader. Non-decimal numbers are written in big
   endian, since for some reason fwrite reverses them, and so they
   come out as little endian. */
BITMAPHEADER* createBMPHeader(unsigned long width, unsigned long height, DWORD ppm) {
  pixel_format* pf = pixel_format_name(TRANS_BMP);
  BITMAPHEADER* bmph = malloc(sizeof(*bmph));

  DWORD offset = sizeof(BITMAPHEADER);
  bmph->bfOffBits = offset;
  bmph->bV4BitCount = pf->bytes*8; /* bits per pixel */
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
  bmph->bV4RedMask = pf->bV4RedMask;
  bmph->bV4GreenMask = pf->bV4GreenMask;
  bmph->bV4BlueMask = pf->bV4BlueMask;
  bmph->bV4AlphaMask = pf->bV4AlphaMask;
  bmph->bV4CSType = pf->bV4CSType;
  bmph->bV4Endpoints_0 = 0; 	/* easier to manage with malloc and free than using a pointer to an array */
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
  bmph->bV4GammaRed = pf->bV4GammaRed;
  bmph->bV4GammaGreen = pf->bV4GammaGreen;
  bmph->bV4GammaBlue = pf->bV4GammaBlue;

  return bmph;
};

/* write the BITMAPHEADER out to the FILE. Returns fwrite's return
   value. */
inline size_t writeBMPHeader(BITMAPHEADER* bmphdr, FILE *write_ptr) {
  return fwrite(bmphdr, sizeof(* bmphdr), 1, write_ptr);
  fflush(write_ptr);
}
