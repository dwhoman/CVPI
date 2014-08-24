#ifndef CAMERA_SETUP_H
#define CAMERA_SETUP_H 1

#define IO_METHOD_MMAP 1
#define IO_METHOD_USERPTR 2

#define IO_METHOD_CHOICE IO_METHOD_MMAP

#define CAMERA_SETUP_ERROR -1

#ifndef __STDDEF_H
#include <stddef.h>
/* size_t */
#endif

struct buffer {
        void   *start;
        size_t  length;
};

struct camera {
  char* path;		      /* dev file path */
  int fd; /* open camera identiifier IMPORTANT */
  struct buffer *buffers; /* where the camera stores its output */
  unsigned int n_buffers; /* number of output buffers */
};

static struct camera camera0 = {"/dev/video0", CAMERA_SETUP_ERROR, NULL, 0};
static struct camera camera1 = {"/dev/video1", CAMERA_SETUP_ERROR, NULL, 0};
static struct camera camera2 = {"/dev/video2", CAMERA_SETUP_ERROR, NULL, 0};

typedef void (*process_image_function)(const void *p, int size);

static int xioctl(int fh, int request, void *arg);

static void errno_exit(const char *s);

int read_frame(struct camera* cam, struct buffer* buffer);

static int stop_capturing(struct camera* cam);

static int start_capturing(struct camera* cam);

static int open_device(struct camera* cam);

static int init_mmap(struct camera* cam);

static int init_userp(struct camera* cam, unsigned int buffer_size);

static int init_device(struct camera* cam);

int camera_setup(struct camera* cam);

static int uninit_device(struct camera* cam);

static int close_device(struct camera* cam);

void camera_takedown(struct camera* cam);
#endif
