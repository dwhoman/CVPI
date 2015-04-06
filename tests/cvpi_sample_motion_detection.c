#ifndef CAMERA_SETUP_H
#include "cvpi_camera_setup.h"
/* variables camera0, camera1, camera2 */
/* struct buffer */
/* struct camera */
#endif

#ifndef CVPI
#include "cvpi.h"
#endif

#include "time.h"

#include "cvpi_egl_surface_functions.h"

/* nano-seconds per second */
#define NSPS 1000000000

/* images to communicate between v4l and OpenVG double buffering */
/* Points to NULL if shared_data has been copied into GPU
   memory. Points to a Transfer_Data otherwise */

unsigned long timespan(struct timespec start, struct timespec end) {
  return (unsigned long)(NSPS*(end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec));
}

typedef struct {
  int index;
  void* data;
  size_t size;
} file_output_thread_data;

static void file_output_threaded(void* data) {
  int status = pthread_detach(pthread_self());
  if(data == NULL) {
    fprintf(stderr, "thread data is NULL\n");
    abort();
  }

  FILE* output_file = NULL;

  char str[] = "cvpi_sample_motion_detect_output/img_00000.yuv";

  file_output_thread_data* file = (file_output_thread_data*)data;

  sprintf(str, "cvpi_sample_motion_detect_output/img_%05d.yuv", file->index);
  output_file = fopen(str, "w");
  if(output_file == NULL) {
    fprintf(stderr, "fopen returned NULL\n");
    goto THREAD_TAKEDOWN;
  }

  if(file->data == NULL) {
    fprintf(stderr, "file data is NULL\n");
    goto THREAD_TAKEDOWN;
  }
    
  if(file->size != fwrite(file->data, 1, file->size, output_file)) {
    fprintf(stderr, "fwrite returned bad size\n");
    goto THREAD_TAKEDOWN;
  } 
 THREAD_TAKEDOWN:
  if(output_file != NULL) {
    fflush(output_file);
    fclose(output_file);
  }

  if(file != NULL) {
    if(file->data != NULL) {
      free(file->data);
    }
    free(file);
  }

  if(status != 0) {
    fprintf(stderr, "print_threaded failed to detach with status = %d\n", status);
    abort();
  }
}


typedef struct {
  VGImage image;
  int stride;
  int width;
  int height;
  CVPI_BOOL success;
} yuyv_settings;

yuyv_settings* yuyv = NULL;

void v4l2shared_data(void* start, uint32_t length) {
  if(yuyv != NULL && yuyv->image != VG_INVALID_HANDLE) {
    vgImageSubData(yuyv->image, start, yuyv->stride, CVPI_COLOR_SPACE, 0, 0, yuyv->width, yuyv->height);
    vgFinish();
    VGErrorCode error = vgGetError();
    if(error != VG_NO_ERROR) {
      fprintf(stderr, "Error getting image data: %s\n", cvpi_vg_error_string(error));
      yuyv->success = CVPI_FALSE;
    } else {
      yuyv->success = CVPI_TRUE;
    }
  } else {
    yuyv->success = CVPI_FALSE;
  }
}

/* use two POSIX threads, one for v4l, the other for image
   processing. Use a semaphore to coordinate data passing between
   threads. */
int main(int argc, char** argv) {
  cvpi_egl_settings egl_settings = NULL;
  cvpi_egl_instance egl_instance = NULL;

  VGImage yuva_image_1 = VG_INVALID_HANDLE;
  VGImage yuva_image_2 = VG_INVALID_HANDLE;
  int switcher = 1;

  cvpi_camera camera = NULL;


  if(argc < 4) {
    fprintf(stderr, "Too few arguments\n");
    return 1;
  }

  int frames = atoi(argv[1]);  
  int yuva_width = atoi(argv[2]);
  int image_height = atoi(argv[3]);

  if(frames < 1 || yuva_width < 1 || image_height < 1) {
    fprintf(stderr, "%s: Invalid arguments\n", __FILE__);
    return 1;
  }

  VGErrorCode error;

  VGint yuva_stride = yuva_width*CVPI_PIXEL_BYTES;
  VGint image_size = image_height*yuva_stride;

  yuyv = malloc(sizeof(* yuyv));
  if(yuyv == NULL) {
    fprintf(stderr, "Failed to allocate for yuyv struct\n");
    goto TAKEDOWN;
  }

  yuyv->height = image_height;
  yuyv->image = VG_INVALID_HANDLE;
  yuyv->success = CVPI_FALSE;
  yuyv->stride = yuva_stride/2;
  yuyv->width = yuva_width/2;

  struct timespec begin, end, begin_inner, end_inner;
  unsigned long inner_total = 0;

  cvpi_process_image process_image_fn = v4l2shared_data;
  camera = cvpi_camera_create("/dev/video0", yuva_width, image_height, 4, CVPI_CAMERA_FORMAT);

  if(camera == NULL) {
    fprintf(stderr, "Failed to setup camera.\n");
    goto TAKEDOWN;
  }

  CVPI_BOOL camera_started = cvpi_camera_start(camera);
  if(CVPI_FALSE_TEST(camera_started)) {
    fprintf(stderr, "camera failed to start\n");
    goto TAKEDOWN;
  }

  egl_settings = cvpi_egl_settings_create();
  if(egl_settings == NULL) {
    fprintf(stderr, "cvpi_egl_settings_create failed\n");
    goto TAKEDOWN;
  }
  egl_settings->surface_pixmap_create_function = cvpi_egl_surface_pixmap_native_creator;
  egl_settings->surface_pixmap_destroy_function = cvpi_egl_surface_pixmap_native_destroyer;
  egl_settings->renderable_api = cvpi_egl_renderable_api_openvg;
  egl_settings->current_surface_type = cvpi_egl_surface_type_pixmap;

  CVPI_BOOL good = CVPI_TRUE;
  good &=
    CVPI_TRUE_TEST(cvpi_egl_settings_conformant(egl_settings, cvpi_egl_conformant_bit_openvg, cvpi_egl_settings_add));
  good &=
    CVPI_TRUE_TEST(cvpi_egl_settings_renderable_type(egl_settings, cvpi_egl_renderable_bits_openvg, cvpi_egl_settings_add));
#if HAVE_BCM_HOST == 1
  good &=
    CVPI_TRUE_TEST(cvpi_egl_settings_pixel_format_brcm(egl_settings, cvpi_egl_pixel_format_vg_image_brcm, cvpi_egl_settings_add));
#endif
#if HAVE_BCM_HOST == 1
  good &=
    CVPI_TRUE_TEST(cvpi_egl_settings_pixel_format_brcm(egl_settings,cvpi_egl_pixel_format_render_vg_brcm, cvpi_egl_settings_add));
#endif
#if HAVE_BCM_HOST == 1
  good &=
    CVPI_TRUE_TEST(cvpi_egl_settings_pixel_format_brcm(egl_settings,cvpi_egl_pixel_format_usage_mask_brcm, cvpi_egl_settings_add));
#endif
#if HAVE_BCM_HOST == 1
  good &=
    CVPI_TRUE_TEST(cvpi_egl_settings_pixel_format_brcm(egl_settings,cvpi_egl_pixel_format_texture_mask_brcm, cvpi_egl_settings_add));
#endif
#if HAVE_BCM_HOST == 1
  good &=
    CVPI_TRUE_TEST(cvpi_egl_settings_pixel_format_brcm(egl_settings,cvpi_egl_pixel_format_render_mask_brcm, cvpi_egl_settings_add));
#endif

  good &=
    CVPI_TRUE_TEST(cvpi_egl_settings_surface_type(egl_settings,cvpi_egl_surface_bits_colorspace, cvpi_egl_settings_add));
  good &=
    CVPI_TRUE_TEST(cvpi_egl_settings_surface_type(egl_settings,cvpi_egl_surface_bits_alpha, cvpi_egl_settings_add));
  good &=
    CVPI_TRUE_TEST(cvpi_egl_settings_surface_type(egl_settings,cvpi_egl_surface_bits_multisample, cvpi_egl_settings_add));
  good &=
    CVPI_TRUE_TEST(cvpi_egl_settings_surface_type(egl_settings,cvpi_egl_surface_bits_swap, cvpi_egl_settings_add));


  good &=
    CVPI_TRUE_TEST(cvpi_egl_settings_buffer_size(egl_settings, 32));

  good &=
    CVPI_TRUE_TEST(cvpi_egl_settings_surface_type(egl_settings, cvpi_egl_surface_bits_pixmap, cvpi_egl_settings_add));
  good &=
    CVPI_TRUE_TEST(cvpi_egl_settings_surface_type(egl_settings, cvpi_egl_surface_bits_pbuffer, cvpi_egl_settings_remove));
  good &=
    CVPI_TRUE_TEST(cvpi_egl_settings_surface_type(egl_settings, cvpi_egl_surface_bits_window, cvpi_egl_settings_remove));

  if(!good) {
    fprintf(stderr, "cvpi_egl_settings modification failed\n");
    goto TAKEDOWN;
  }

  egl_instance = cvpi_egl_instance_setup(egl_settings);
  if(egl_instance == NULL) {
    fprintf(stderr, "cvpi_egl_instance_setup failed\n");
    goto TAKEDOWN;
  }

  yuyv->image = vgCreateImage(CVPI_COLOR_SPACE, yuyv->width, yuyv->height, VG_IMAGE_QUALITY_NONANTIALIASED);
  vgFinish();
  error = vgGetError();
  if(error != VG_NO_ERROR) {
    fprintf(stderr, "Error creating yuyv->image: %s\n", cvpi_vg_error_string(error));
    goto TAKEDOWN;
  }

  int max = frames + 50;
  int thread_not_created;
  //begin = clock();
  clock_gettime(CLOCK_REALTIME, &begin);

  VGImage new_image;
  VGImage old_image;
  VGImage added_1 = VG_INVALID_HANDLE;
  VGImage added_2 = VG_INVALID_HANDLE;
  VGImage mask = VG_INVALID_HANDLE;
  VGImage motion = VG_INVALID_HANDLE;
  CVPI_BOOL read;
  file_output_thread_data* thread_data = NULL;
  for(int i = 0; i <= frames && max != 0; --max) {
    clock_gettime(CLOCK_REALTIME, &begin_inner);
    thread_not_created = 1;
    read = cvpi_camera_read_frame(camera, process_image_fn);
    if(CVPI_FALSE_TEST(read) || CVPI_FALSE_TEST(yuyv->success)) {
      fprintf(stderr, "Camera failed to read\n");
      goto LOOP_TAKEDOWN;
    }

    if(switcher) {
      yuva_image_1 = cvpi_yuyv2yuva(yuyv->image);
      if(yuva_image_1 == VG_INVALID_HANDLE) {
	fprintf(stderr, "cvpi_yuyv2yuva failed\n");
	goto LOOP_TAKEDOWN;
      }
      new_image = yuva_image_1;
      old_image = yuva_image_2;
    } else {
      yuva_image_2 = cvpi_yuyv2yuva(yuyv->image);
      if(yuva_image_2 == VG_INVALID_HANDLE) {
	fprintf(stderr, "cvpi_yuyv2yuva failed\n");
	goto LOOP_TAKEDOWN;
      }
      new_image = yuva_image_2;
      old_image = yuva_image_1;
    }

    switcher = !switcher;

    /* need two itterations before creating output */
    if(i == 0) {
      ++i;
      continue;
    }

    /* thread_data gets freed by the thread, unless thread creation
       fails, then it gets freed here */
    thread_data = malloc(sizeof(*thread_data));
    if(thread_data == NULL) {
      fprintf(stderr, "Failed to allocate file output thread data\n");
      goto LOOP_TAKEDOWN;
    }

    thread_data->index = i;
    thread_data->size = image_size;

    thread_data->data = malloc(image_size);
    if(thread_data->data == NULL) {
      fprintf(stderr, "Error allocating file output data: %s\n");
      goto LOOP_TAKEDOWN;
    }

    added_1 = cvpi_image_add(new_image, old_image, 1, -1, 1, 0);
    if(added_1 == VG_INVALID_HANDLE) {
      fprintf(stderr, "Computing added_1 failed.\n");
      goto LOOP_TAKEDOWN;
    }
    added_2 = cvpi_image_add(old_image, new_image, 1, -1, 1, 0);
    if(added_2 == VG_INVALID_HANDLE) {
      fprintf(stderr, "Computing added_2 failed.\n");
      goto LOOP_TAKEDOWN;
    }

    mask = cvpi_image_logical_or(added_1, added_2, 0, 255, CVPI_TRUE);
    if(mask == VG_INVALID_HANDLE) {
      fprintf(stderr, "Computing mask failed.\n");
      goto LOOP_TAKEDOWN;
    }

    motion = cvpi_image_add(new_image, mask, 1, -1, 1, 0);
    if(motion == VG_INVALID_HANDLE) {
      fprintf(stderr, "Computing motion failed.\n");
      goto LOOP_TAKEDOWN;
    }

    vgGetImageSubData(motion, thread_data->data, yuva_stride, CVPI_COLOR_SPACE, 0, 0, yuva_width, image_height);
    error = vgGetError();
    if(error != VG_NO_ERROR) {
      fprintf(stderr, "Error getting image data: %s\n", cvpi_vg_error_string(error));
      goto LOOP_TAKEDOWN;
    }
    vgFinish();
    error = vgGetError();
    if(error != VG_NO_ERROR) {
      fprintf(stderr, "Error getting image data: %s\n", cvpi_vg_error_string(error));
      goto LOOP_TAKEDOWN;
    }

    clock_gettime(CLOCK_REALTIME, &end_inner);
    inner_total += timespan(begin_inner, end_inner);

    pthread_t id;    
    thread_not_created = pthread_create(&id, 0, file_output_threaded, (void*)thread_data);
    pthread_join(id, 0);

    ++i;
  LOOP_TAKEDOWN:
    if(added_1 != VG_INVALID_HANDLE) {
      vgDestroyImage(added_1);
      added_1 = VG_INVALID_HANDLE;
    }
    if(added_2 != VG_INVALID_HANDLE) {
      vgDestroyImage(added_2);
      added_2 = VG_INVALID_HANDLE;
    }
    if(mask != VG_INVALID_HANDLE) {
      vgDestroyImage(mask);
      mask = VG_INVALID_HANDLE;
    }
    if(motion != VG_INVALID_HANDLE) {
      vgDestroyImage(motion);
      motion = VG_INVALID_HANDLE;
    }

    if(thread_not_created) {
      if(thread_data != NULL) {
	if(thread_data->data != NULL) {
	  free(thread_data->data);
	  thread_data->data = NULL;
	}
	free(thread_data);
      }
    }
    /* got to here before increamenting i */
    if(i == 0) {
      break;
    }
  }
  clock_gettime(CLOCK_REALTIME, &end);

  double fps = (double)frames * NSPS / (double)(timespan(begin, end));
  /* (frames * ns/s)/(total ns)  */
  double fps_inner = (double)frames * NSPS / (double)inner_total;

  printf("%f,%f", fps_inner, fps);

 TAKEDOWN:
  if(yuyv->image != VG_INVALID_HANDLE) {
    vgDestroyImage(yuyv->image);
  }

  if(yuva_image_1 != VG_INVALID_HANDLE) {
    vgDestroyImage(yuva_image_1);
  }

  if(yuva_image_2 != VG_INVALID_HANDLE) {
    vgDestroyImage(yuva_image_2);
  }

  if(camera != NULL) {
    cvpi_camera_takedown(camera);
  }

  if(egl_instance != NULL) {
    cvpi_egl_instance_takedown(egl_instance);
  }
  if(egl_settings != NULL) {
    free(egl_settings);
  }

  if(yuyv != NULL) {
    free(yuyv);
  }
}
