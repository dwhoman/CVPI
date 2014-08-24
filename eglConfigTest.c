//#ifndef EGL_GENERATED_CONFIG_HEADER_H
//#include "eglGeneratedConfigHeader.h"
//#endif

#ifndef EGL_CONFIG
#include "eglConfig.h"
#endif

#ifndef VG_CONFIG
#include "vgConfig.h"
#endif

#ifndef _OPENVG_H
#include <VG/openvg.h>
#endif

//#include "interface/khronos/Vg/vg_client.h"

#ifndef _STDIO_H
#include <stdio.h>
#endif

#ifndef _STDLIB_H
#include <stdlib.h>
#endif

int main(void) {
  struct egl_settings* settings = &EGL_CONFIG;

   //  change_surface_type(settings, egl_surface_bits_pixmap);
  //  change_renderable_type(settings, egl_renderable_bits_openvg);
  //  change_transparent_type(settings, egl_transparent_type_none);
  //  change_color_buffer_type(settings, egl_color_buffer_type_rgb);
  //  change_config_caveat(settings, egl_config_caveat_none);
  change_renderable_api(settings, egl_renderable_api_openvg);

  egl_deletable* takedown;
  takedown = (egl_deletable*) setupEGLFull(800, 480, 0,0,0);

  VGImage image = vgCreateImage(VG_sRGB_565, vgGeti(VG_MAX_IMAGE_WIDTH), vgGeti(VG_MAX_IMAGE_HEIGHT), VG_IMAGE_QUALITY_FASTER);
  if(VG_INVALID_HANDLE == image) {
    fprintf(stderr, "Error in vgCreateImage: %s\n", vgErrorString(vgGetError()));
    goto eglTakedown;
  }

  VGfloat color[4] = { 1.0f, 1.0f, 0.0f, 1.0f }; /* Opaque yellow */
  //VGfloat color[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; /* Opaque yellow */
  vgSetfv(VG_CLEAR_COLOR, 4, color);
  vgClearImage(image, 0, 0, vgGetParameteri(image, VG_IMAGE_WIDTH), vgGetParameteri(image, VG_IMAGE_HEIGHT));
  printf("Clear image: %s\n", vgErrorString(vgGetError()));

  /* VGfloat color2[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; */
  /* vgSetfv(VG_CLEAR_COLOR, 4, color2); */
  /* vgClear(0,0,200,200); */
  /* printf("Clear: %s\n", vgErrorString(vgGetError())); */

  vgDrawImage(image);
  printf("Draw image: %s\n", vgErrorString(vgGetError()));

  VGint dataStride = 1600;
  VGImageFormat dataFormat = VG_sRGB_565;
  VGint sx = 0;
  VGint sy = 0;
  VGint width = 800;
  VGint height = 480;
  void * data = vcos_malloc(dataStride * height, "image data store");
  vgReadPixels(data, dataStride, dataFormat, sx, sy, width, height);
  printf("Read Pixels: %s\n", vgErrorString(vgGetError()));
  vgFinish();
  FILE *write_ptr;
  
  write_ptr = fopen("/tmp/test.bmp","wb");

  BITMAPHEADER* bmphdr = createBMPHeader(&pf_VG_sRGB_565, width, height, 90);
  writeBMPHeader(bmphdr, write_ptr);

  fwrite(data, dataStride, height, write_ptr);
 
  free(bmphdr);
  vcos_free(data);
  vgDestroyImage(image);
  printf("Destroy image: %s\n", vgErrorString(vgGetError()));

 eglTakedown:
  printf("takedown ptr: %p\n", takedown);
  eglDelete(takedown);
  return 0;
}
