#ifndef CAMERA_SETUP_H
#include "camera_setup.h"
/* variables camera0, camera1, camera2 */
/* struct buffer */
/* struct camera */
#endif

#ifndef _STDIO_H
#include <stdio.h>
#endif

#ifndef _STDLIB_H
#include <stdlib.h>
#endif
#include <stdint.h>

#ifndef BITMAP_H
#include "bitmap.h"
#endif

int width = 640;
int height = 480;

FILE *write_ptr;

static void processing_function(void* start, uint32_t length) {
  BMPHEADER bmphdr = createBMPHeader(width, height, 90, pf_VG_sRGBA_8888);
  char* file_name;

  file_name = malloc(20);
  sprintf(file_name, "/tmp/test.yuv");
  write_ptr = fopen(file_name,"wb");
  writeBMPHeader(bmphdr, write_ptr);
  fwrite(start, length, 1, write_ptr);
  fflush(write_ptr);
  fclose(write_ptr);
  free(file_name);
  free(bmphdr);
}

process_image process_image_fn = processing_function;

int main() {

  /* which camera to use */
  CAMERA camera0_ptr = camera_create("/dev/video0", width, height, 4);
  if(camera0_ptr == NULL) {
    return 1;
  }
  
  /* setup v4l */
  camera_start(camera0_ptr);

  int j;
  int max = 10;
  for(j = 0; j < max; ++j) {
    if(!camera_read_frame(camera0_ptr, process_image_fn)) {
      break;
    }
  }
  if(j == max) {
    printf("Failed to capture");
  }
    
  /* takedown v4l */
  camera_takedown(camera0_ptr);
  return 0;
}
