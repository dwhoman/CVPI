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

#ifndef CAMERA_SETUP_H
#include "camera_setup.h"
/* variables camera0, camera1, camera2 */
/* struct buffer */
/* struct camera */
#endif

#ifndef transConfig_h
#include "transConfig.h"
#endif

#ifndef _STDIO_H
#include <stdio.h>
#endif

#ifndef _STDLIB_H
#include <stdlib.h>
#endif

//extern read_frame(struct camera*, struct buffer*);

int main() {
  /* which camera to use */
  struct camera* camera0_ptr = &camera1;
  //  struct camera* camera1_ptr = &camera1;

  /* where in memory to store the camera capture */
  struct buffer* buffer0 = malloc(sizeof(*buffer0));

  camera0_ptr->n_buffers = 1;
  
  /* setup v4l */
  camera_setup(camera0_ptr);
  //  camera_setup(camera1_ptr);

  printf("camera path: %s\n", camera0_ptr->path);
  printf("camera fd: %d\n", camera0_ptr->fd);
  //printf("camera start: %x\n", camera0_ptr->path->start);
  printf("camera # of buffers: %d\n", camera0_ptr->n_buffers);

  /* setup EGL */
  struct egl_settings* settings = &EGL_CONFIG;
  change_renderable_api(egl_renderable_api_openvg);
  egl_deletable* takedown;
  takedown = setupEGL(TRANS_WIDTH,TRANS_HEIGHT,0);
  /* setup OpenVG */
  //   pixel_format_name(TRANS_V4L2);
  /* loop */
  int rf_ret;
  BITMAPHEADER* bmphdr = createBMPHeader(TRANS_WIDTH, TRANS_HEIGHT, 90);
  FILE *write_ptr;  
  char* file_name;
  int j;
  int itter = 0;
  VGImage rgb_image = malloc(sizeof(VGImage));
  VGImage rgb_m[] = {1,1,1,1,0,-0.39465,2.03211,1, 1.13893, -0.5806, 0,1, 0,0,0,0};
  for(; itter < 1; itter++)  { 
    for(j = 0; j < 1000; ++j) {
      rf_ret = read_frame(camera0_ptr, buffer0);
      if(rf_ret == 0) {
	break;
      }
    }
    if(0 != rf_ret) {
      fprintf(stderr, "read_frame failed\n");
      break;
    }
    vgColorMatrix(rgb_image, (VGImage)buffer0, rgb_m);
    sprintf(file_name, "/tmp/test%02u.rgb", itter);
    write_ptr = fopen(file_name,"wb");
    //  writeBMPHeader(bmphdr, write_ptr);
    //fwrite(bmphdr, sizeof(*bmphdr), 1, write_ptr);
    //fflush(write_ptr);
    //v4lconvert_convert(v4lconvert_data,&src_fmt,&fmt,buffer0->start, buffer0->length,
    //		       dst_buf, fmt.fmt.pix.sizeimage);
    fwrite(buffer0->start, buffer0->length, 1, write_ptr);
    fflush(write_ptr);
    fclose(write_ptr);
  }
  pixel_format* pf = pixel_format_name(TRANS_BMP);
  printf("red format: %x\n", pf->bV4RedMask);
  printf("%d, %d, %d, %d\n", TRANS_RGB_565,TRANS_RGB_888,TRANS_ARGB_8888,TRANS_COLOR_FMT );
  printf("buffer0 start: %x, length: %u\n", buffer0->start, buffer0->length);
  free(bmphdr);

  vgReadPixels(buffer0->start, pixel_format_name(TRANS_BYTES), pixel_format_name(TRANS_VG), 0,0, TRANS_WIDTH, TRANS_HEIGHT);
  int error = vgGetError();
  if(VG_UNSUPPORTED_IMAGE_FORMAT_ERROR == error) {
    fprintf(stderr, "Error in vgReadPixels: %s\n", vgErrorString(error));
    goto eglTakedown;
  }
  if(VG_ILLEGAL_ARGUMENT_ERROR == error) {
    fprintf(stderr, "Error in vgReadPixels: %s\n", vgErrorString(error));
    goto eglTakedown;
  }
  
  /* takedown OpenVG */
  /* takedown EGL */
 eglTakedown:
  printf("takedown ptr: %p\n", takedown);
  takedownEGL(takedown);
  /* takedown v4l */
  camera_takedown(camera0_ptr);
  //  camera_takedown(camera1_ptr);
  free(buffer0);
  return 0;
}
