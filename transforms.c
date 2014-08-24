#ifndef transConfig_h
#include "transConfig.h"
#endif

#ifndef EGL_CONFIG
#include "eglConfig.h"
#endif

#ifndef VG_CONFIG
#include "vgConfig.h"
#endif

#ifndef _OPENVG_H
#include <VG/openvg.h>
#endif

#ifndef BITMAP_H
#include "bitmap.h"
#endif

#ifndef _STDIO_H
#include <stdio.h>
#endif

#ifndef _STDLIB_H
#include <stdlib.h>
#endif

#include <stdint.h>

#define COLOR_SPACE VG_sRGBA_8888
const int a=0,r=3,g=2,b=1;

//#define COLOR_SPACE VG_sARGB_8888
//const int a=3,r=2,g=1,b=0;


#define MATRICES 24

const VGfloat conversion[MATRICES + 1][20] = {{
    1,0,0,0,			/* 1 */
    0,1,0,0,
    0,0,1,0,
    0,0,0,1,    
    0,0,0,0    
  },{
    1,0,0,0,			/* 2 */
    0,1,0,0,
    0,0,0,1,    
    0,0,1,0,
    0,0,0,0
  },{
    1,0,0,0,			/* 3 */
    0,0,1,0,
    0,1,0,0,
    0,0,0,1,    
    0,0,0,0    
  },{
    1,0,0,0,			/* 4 */
    0,0,1,0,
    0,0,0,1,    
    0,1,0,0,
    0,0,0,0    
  },{
    1,0,0,0,			/* 5 */
    0,0,0,1,    
    0,1,0,0,
    0,0,1,0,
    0,0,0,0    
  },{
    1,0,0,0,			/* 6 */
    0,0,0,1,    
    0,0,1,0,
    0,1,0,0,
    0,0,0,0    
  },{
    0,1,0,0,			/* 1 */
    1,0,0,0,
    0,0,1,0,
    0,0,0,1,    
    0,0,0,0    
  },{
    0,1,0,0,			/* 2 */
    1,0,0,0,
    0,0,0,1,    
    0,0,1,0,
    0,0,0,0
  },{
    0,1,0,0,			/* 3 */
    0,0,1,0,
    1,0,0,0,
    0,0,0,1,    
    0,0,0,0    
  },{
    0,1,0,0,			/* 4 */
    0,0,1,0,
    0,0,0,1,    
    1,0,0,0,
    0,0,0,0    
  },{
    0,1,0,0,			/* 5 */
    0,0,0,1,    
    1,0,0,0,
    0,0,1,0,
    0,0,0,0    
  },{
    0,1,0,0,			/* 6 */
    0,0,0,1,    
    0,0,1,0,
    1,0,0,0,
    0,0,0,0    
  },{
    0,0,1,0,			/* 1 */
    1,0,0,0,
    0,1,0,0,
    0,0,0,1,    
    0,0,0,0    
  },{
    0,0,1,0,			/* 2 */
    1,0,0,0,
    0,0,0,1,
    0,1,0,0,    
    0,0,0,0
  },{
    0,0,1,0,			/* 3 */
    0,1,0,0,
    1,0,0,0,
    0,0,0,1,    
    0,0,0,0    
  },{
    0,0,1,0,			/* 4 */
    0,1,0,0,
    0,0,0,1,    
    1,0,0,0,
    0,0,0,0    
  },{
    0,0,1,0,			/* 5 */
    0,0,0,1,    
    1,0,0,0,
    0,1,0,0,
    0,0,0,0    
  },{
    0,0,1,0,			/* 6 */
    0,0,0,1,    
    0,1,0,0,
    1,0,0,0,
    0,0,0,0
  },{
    0,0,0,1,			/* 1 */
    1,0,0,0,
    0,1,0,0,
    0,0,1,0,    
    0,0,0,0    
  },{
    0,0,0,1,			/* 2 */
    1,0,0,0,
    0,0,1,0,
    0,1,0,0,    
    0,0,0,0
  },{
    0,0,0,1,			/* 3 */
    0,1,0,0,
    1,0,0,0,
    0,0,1,0,    
    0,0,0,0    
  },{
    0,0,0,1,			/* 4 */
    0,1,0,0,
    0,0,1,0,    
    1,0,0,0,
    0,0,0,0    
  },{
    0,0,0,1,			/* 5 */
    0,0,1,0,    
    1,0,0,0,
    0,1,0,0,
    0,0,0,0    
  },{
    0,0,0,1,			/* 6 */
    0,0,1,0,    
    0,1,0,0,
    1,0,0,0,
    0,0,0,0
  },{
    1,0,0,0,
    0,1,0,0,    
    0,0,1,0,
    0,0,0,1,
    0.1,0.2,0.3,0.4
  }
};

union Ix4 {
  uint32_t full;
  uint8_t part[4];
};

int main() {
  change_renderable_api(egl_renderable_api_openvg);
  change_buffer_size(32);
  change_red_size(8);
  change_green_size(8);
  change_blue_size(8);
  change_alpha_size(8);
  egl_deletable* takedown;

  union Ix4 input;
  input.full = 0x44332211;
  printf("0x%X\n", input.full);

  union Ix4 output;

  takedown = setupEGL(1, 1, 4, EGL_PIXEL_FORMAT_ARGB_8888_BRCM, 0);
  if(takedown == NULL) {
    printf("setupEGL returned NULL pointer \n");
    return 1;
  }

  VGImage image_1 = vgCreateImage(COLOR_SPACE, 1, 1, VG_IMAGE_QUALITY_NONANTIALIASED);
  VGImage image_2 = vgCreateImage(COLOR_SPACE, 1, 1, VG_IMAGE_QUALITY_NONANTIALIASED);

  vgImageSubData(image_1, &input, 4, COLOR_SPACE, 0, 0, 1, 1);

  int i;
  for(i = 0; i < MATRICES; ++i) {
    vgColorMatrix(image_2, image_1, conversion[i]);

    vgGetImageSubData(image_2, &(output.full), 4, COLOR_SPACE, 0, 0, 1, 1);
    vgFinish();
    printf("\\[[[%02X],[%02X],[%02X],[%02X]] := [[%1.0f, %1.0f, %1.0f, %1.0f],", output.part[r], output.part[g], output.part[b], output.part[a] , conversion[i][0], conversion[i][4], conversion[i][8], conversion[i][12]);
    printf("[%1.0f, %1.0f, %1.0f, %1.0f],", conversion[i][1], conversion[i][5], conversion[i][9], conversion[i][13]);
    printf("[%1.0f, %1.0f, %1.0f, %1.0f],", conversion[i][2], conversion[i][6], conversion[i][10], conversion[i][14]);
    printf("[%1.0f, %1.0f, %1.0f, %1.0f]]", conversion[i][3], conversion[i][7], conversion[i][11], conversion[i][15]);
    printf(" * [[%02X],[%02X],[%02X],[%02X]] =>\\]\n\n", input.part[r], input.part[g], input.part[b], input.part[a]);
  }

    vgColorMatrix(image_2, image_1, conversion[MATRICES]);

    vgGetImageSubData(image_2, &output, 4, COLOR_SPACE, 0, 0, 1, 1);
    vgFinish();
    printf("\\[[[%02d],[%02d],[%02d],[%02d]] := [[%02d],[%02d],[%02d],[%02d]] + [[%f], [%f], [%f], [%f]] =>\\]\n\n", 
	   output.part[r], output.part[g], output.part[b], output.part[a], input.part[r], input.part[g], input.part[b], input.part[a], conversion[i][16] * 256, conversion[i][17] * 256, conversion[i][18] * 256, conversion[i][19] * 256);

  vgDestroyImage(image_1);
  vgDestroyImage(image_2);
  vgFinish();
  takedownEGL(takedown);
  return 0;
}
