/** \file cvpi_camera_setup.c
    \brief based on LINUX MEDIA INFRASTRUCTURE API Appendix D. Video Capture Example 
*/
#ifndef CVPI_CAMERA_SETUP
#include "cvpi_camera_setup.h"
#endif

#ifndef _ASSERT_H
#include <assert.h>
#endif

#ifndef __LINUX_VIDEODEV2_H
#include <linux/videodev2.h>
/* V4L2_BUF_TYPE_VIDEO_CAPTURE */
/* V4L2_MEMORY_MMAP */
#endif

#ifndef _ERRNO_H
#include <errno.h>
/* from errno-base.h */
/* EINTR */
#endif

#ifndef _STDLIB_H
#include <stdlib.h>
/* malloc, calloc, free */
#endif

#ifndef _STDIO_H
#include <stdio.h>
/* stderr */
/* sprintf */
#endif

#ifndef	_SYS_STAT_H
#include <sys/stat.h>
/* struct stat */
/* S_ISCHR() */
#endif

#ifndef _FCNTL_H
#include <fcntl.h>
/* O_RDWR */
#endif

#ifndef _SYS_MMAN_H
#include <sys/mman.h>
/* mmap */
#endif

#ifndef _STRING_H
#include <string.h>
/* memset() */
#endif

#ifndef	_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif

#ifndef	_UNISTD_H
#include <unistd.h>
/* close() */
#endif

#define CLEAR(x) memset(&(x), 0, sizeof(x))

/* v4l2 example capture.c says that the buffer min size is 4 */
/* see what 1 does, 2 should be enough */
#define CAM_BUFFERS_MIN 1

unsigned long CVPI_CAMERA_MAX_IOCTL_REQUESTS = 100;

typedef struct {
        void   *start;
        size_t  length;
} buffer;

struct cvpi_camera {
  unsigned long width;
  unsigned long height;
  char* path;		      /* dev file path */
  int fd; /* open camera identifier IMPORTANT */
  buffer *buffers; /* where the camera stores its output, array of buffer structs length cam_buffers */
  unsigned int cam_buffers;/* number of camera memory buffers */
  unsigned long camera_format;
};

static int xioctl(int fh, int request, void *arg);

static void errno_exit(const char *function, const char *s);

/* \returns CVPI_CAMERA_TAKEDOWN_ERROR if unsuccessful */
/* \returns CVPI_TRUE if sucessful */
static int stop_capturing(cvpi_camera cam);

/* \returns CVPI_CAMERA_START_ERROR if unsuccessful */
/* \returns CVPI_TRUE if sucessful */
static int start_capturing(cvpi_camera cam);

/* \returns CVPI_CAMERA_START_ERROR if unsuccessful */
/* \returns CVPI_TRUE if sucessful */
static int open_device(cvpi_camera cam);

/* \returns CVPI_CAMERA_START_ERROR if unsuccessful */
/* \returns CVPI_TRUE if sucessful */
static int init_mmap(cvpi_camera cam);

/* \returns CVPI_CAMERA_START_ERROR if unsuccessful */
/* \returns CVPI_TRUE if sucessful */
static int init_device(cvpi_camera cam);

/* \returns CVPI_CAMERA_TAKEDOWN_ERROR if unsuccessful */
/* \returns CVPI_TRUE if sucessful */
static int uninit_device(cvpi_camera cam);

/* \returns CVPI_CAMERA_TAKEDOWN_ERROR if unsuccessful */
/* \returns CVPI_TRUE if sucessful */
static int close_device(cvpi_camera cam);

cvpi_camera cvpi_camera_create(char* dev_path,
			       unsigned long width,
			       unsigned long height,
			       unsigned long cam_buffers,
			       unsigned long camera_format) {
  if(width == 0 || height == 0 || cam_buffers < CAM_BUFFERS_MIN) {
    fprintf(stderr, "Camera_create errror\nwidth and height must be > 0\ncam_buffers must be > %d\n", CAM_BUFFERS_MIN);
    return NULL;
  }

  cvpi_camera cam = malloc(sizeof(*cam));
  if(cam == NULL) {
    fprintf(stderr, "%s:%d:%s: malloc returned NULL errno = %d\n", __FILE__, __LINE__, __func__, errno);
    return NULL;
  }
  cam->width = width;
  cam->height = height;
  cam->path = dev_path;
  cam->buffers = NULL;		/* gets set by init_mmap */
  cam->fd = CVPI_CAMERA_START_ERROR;
  cam->cam_buffers = cam_buffers;
  cam->camera_format = camera_format;

  return cam;
}

/* 
   ioctl wrapper
   if ioctl fails, retries CVPI_CAMERA_MAX_IOCTL_REQUESTS times
 */
static int xioctl(int fh, int request, void *arg) {
  int r;
  unsigned long itter = 0;
  
  do {
    r = ioctl(fh, request, arg);
    if(itter > CVPI_CAMERA_MAX_IOCTL_REQUESTS) {
      fprintf(stderr, "xioctl timed out\n");
      return CVPI_SYS_ERROR;
    }
    ++itter;
  } while (CVPI_SYS_ERROR == r && EINTR == errno);
  
  return r;
}

/* wrapper */
static void errno_exit(const char *function, const char *s) {
  fprintf(stderr, "%s: %s error %d, %s\n", function, s, errno, strerror(errno));
}

CVPI_BOOL cvpi_camera_read_frame(cvpi_camera cam, cvpi_process_image process) {
  struct v4l2_buffer buf;
  CLEAR(buf);

  fd_set fds;			/* set of file descriptors, see man 2 select */
  struct timeval tv;		/* time value struct, see man gettimeofday */
  int r;

  FD_ZERO(&fds);
  FD_SET(cam->fd, &fds);	/* add fd to the set of fds */
  
  /* Timeout. */
  tv.tv_sec = 2;
  tv.tv_usec = 0;
  
  /* wait for one of the file descriptors to become ready */
  r = select(cam->fd + 1, &fds, NULL, NULL, &tv);
  
  if (CVPI_SYS_ERROR == r) {
    errno_exit(__func__, "select");
    return CVPI_CAMERA_READ_FRAME_ERROR;
  }
  
  if (0 == r) {
    fprintf(stderr, "%s: select timeout\n", __func__);
    return CVPI_CAMERA_READ_FRAME_ERROR;
  }
  
  buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  buf.memory = V4L2_MEMORY_MMAP; 
  /* stop the camera from writing to a buffer */
  if (CVPI_SYS_ERROR == xioctl(cam->fd, VIDIOC_DQBUF, &buf)) {
    switch (errno) {
    case EAGAIN:
      printf("read_frame EAGAIN\n");
      return CVPI_CAMERA_READ_FRAME_ERROR;
      
    case EIO:
      /* Could ignore EIO, see spec. */
      
      /* fall through */
      
    default:
      errno_exit(__func__, "VIDIOC_DQBUF");
      return CVPI_CAMERA_READ_FRAME_ERROR;
    }
  }

  int return_value = CVPI_TRUE;
  if(process != NULL) {
    if(!(buf.flags & V4L2_BUF_FLAG_ERROR)) {
      process(cam->buffers[buf.index].start, buf.bytesused); 
    } else {
      fprintf(stderr, "%s: V4L2_BUF_FLAG_ERROR\n", __func__);
      return_value = CVPI_CAMERA_READ_FRAME_ERROR;
    }
  }
  
  if(buf.index >= cam->cam_buffers) {
    fprintf(stderr,"%s: buffer index = %d, exceeds number of camera buffers %d\n", __func__, 
	    buf.index, cam->cam_buffers);
    return_value = CVPI_CAMERA_READ_FRAME_ERROR;
  }
  
  /* allow the cammera to write another frame to memory */
  if (CVPI_SYS_ERROR == xioctl(cam->fd, VIDIOC_QBUF, &buf)) {
    errno_exit(__func__, "VIDIOC_QBUF");
    return_value = CVPI_CAMERA_READ_FRAME_ERROR;
  }

  return return_value;
}

static int stop_capturing(cvpi_camera cam) {
  enum v4l2_buf_type type;
  
  type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if (CVPI_SYS_ERROR == xioctl(cam->fd, VIDIOC_STREAMOFF, &type)) {
    errno_exit(__func__, "VIDIOC_STREAMOFF");
    return CVPI_CAMERA_TAKEDOWN_ERROR;
  }
  return CVPI_TRUE;
}

static int start_capturing(cvpi_camera cam) {
  unsigned int i;
  enum v4l2_buf_type type;
  
  for (i = 0; i < cam->cam_buffers; ++i) {
    struct v4l2_buffer buf;
    
    CLEAR(buf);
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = i;

    /* ready the camera buffer to receive images */
    if (CVPI_SYS_ERROR == xioctl(cam->fd, VIDIOC_QBUF, &buf)) {
      errno_exit(__func__, "VIDIOC_QBUF");
      return CVPI_SYS_ERROR;
    }
  }
  type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  /* turn on the camera */
  if (CVPI_SYS_ERROR == xioctl(cam->fd, VIDIOC_STREAMON, &type)) {
    errno_exit(__func__, "VIDIOC_STREAMON");
    return CVPI_CAMERA_START_ERROR;
  }

  return CVPI_TRUE;
}

static int open_device(cvpi_camera cam) {
  struct stat st;
  /* get information about the /dev/video device, see man 2 stat */
  if (CVPI_SYS_ERROR == stat(cam->path, &st)) {
    fprintf(stderr, "%s: Cannot identify '%s': %d, %s\n", __func__,
	    cam->path, errno, strerror(errno));
    return CVPI_CAMERA_START_ERROR;
  }
  
  /* http://pubs.opengroup.org/onlinepubs/000095399/basedefs/sys/stat.h.html */
  /* check that it is a character special file */
  if (!S_ISCHR(st.st_mode)) {	
    fprintf(stderr, "%s: %s is no device\n", __func__, cam->path);
    return CVPI_CAMERA_START_ERROR;
  }
  
  cam->fd = open(cam->path, O_RDWR /* required */ | O_NONBLOCK, 0);
  
  if (CVPI_SYS_ERROR == cam->fd) {
    fprintf(stderr, "%s: Cannot open '%s': %d, %s\n", __func__,
	    cam->path, errno, strerror(errno));
    return CVPI_CAMERA_START_ERROR;
  }
  return CVPI_TRUE;
}

static int init_mmap(cvpi_camera cam) {
  /* create buffers to store frames */
  struct v4l2_requestbuffers req; /* http://www.linuxtv.org/downloads/v4l-dvb-apis/vidioc-reqbufs.html */
  
  CLEAR(req);
  
  req.count = cam->cam_buffers;
  req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  req.memory = V4L2_MEMORY_MMAP;
  /* initiate MMAP I/O */
  int xio = xioctl(cam->fd, VIDIOC_REQBUFS, &req);
  if (CVPI_SYS_ERROR == xio) {
    if (EINVAL == errno) {
      fprintf(stderr, "%s: %s does not support memory mapping\n", __func__, cam->path);
      return CVPI_CAMERA_START_ERROR;
    } else {
      errno_exit(__func__, "VIDIOC_REQBUFS");
      return CVPI_CAMERA_START_ERROR;
    }
  }

  if (req.count < CAM_BUFFERS_MIN) {
    fprintf(stderr, "%s: Insufficient buffer memory on %s\n", __func__, cam->path);
      return CVPI_CAMERA_START_ERROR;
  }
  /* create camera buffers array */
  cam->buffers = calloc(req.count, sizeof(*(cam->buffers)));

  if (!cam->buffers) {
    fprintf(stderr, "%s: Out of memory\n", __func__);
    return CVPI_CAMERA_START_ERROR;
  }

  int n = 0;
  for (n = 0; n < req.count; ++n) {
    struct v4l2_buffer buf;
    
    CLEAR(buf);

    buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory      = V4L2_MEMORY_MMAP;
    buf.index       = n;
    /* query the status of buf */
    if (CVPI_SYS_ERROR == xioctl(cam->fd, VIDIOC_QUERYBUF, &buf)) {
      errno_exit(__func__, "VIDIOC_QUERYBUF");
      return CVPI_CAMERA_START_ERROR;
    }
    
    cam->buffers[n].length = buf.length;
    cam->buffers[n].start =
      mmap(NULL /* start anywhere */,
	   buf.length,
	   PROT_READ | PROT_WRITE /* required */,
	   MAP_SHARED /* recommended */,
	   cam->fd, buf.m.offset);
    
    if (MAP_FAILED == cam->buffers[n].start) {
      errno_exit(__func__, "mmap");
      return CVPI_CAMERA_START_ERROR;
    }
  }
  return CVPI_TRUE;
}

static int init_device(cvpi_camera cam) {
  /* set camera settings: width, height, encoding */
  struct v4l2_capability cap;
  struct v4l2_cropcap cropcap;
  struct v4l2_crop crop;
  struct v4l2_format fmt;
  unsigned long min;
  /* query the device's capabilities */
  if (CVPI_SYS_ERROR == xioctl(cam->fd, VIDIOC_QUERYCAP, &cap)) {
    if (EINVAL == errno) {
      fprintf(stderr, "%s: %s is no V4L2 device\n", __func__, cam->path);
    } else {
      errno_exit(__func__, "VIDIOC_QUERYCAP");
    }
    return CVPI_CAMERA_START_ERROR;
  }
  
  if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
    fprintf(stderr, "%s: %s is no video capture device\n", __func__, cam->path);
    return CVPI_CAMERA_START_ERROR;
  }

  if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
    fprintf(stderr, "%s: %s does not support streaming i/o\n", __func__,
	    cam->path);
    return CVPI_CAMERA_START_ERROR;
  }
  

  /* Select video input, video standard and tune here. */

  CLEAR(cropcap);

  cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  /* query cropping and scaling abilities */
  if (0 == xioctl(cam->fd, VIDIOC_CROPCAP, &cropcap)) {
    crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    crop.c = cropcap.defrect; /* reset to default */
    
    if (CVPI_SYS_ERROR == xioctl(cam->fd, VIDIOC_S_CROP, &crop)) {
      switch (errno) {
      case EINVAL:
	/* Cropping not supported. */
	break;
      default:
	/* Errors ignored. */
	break;
      }
    }
  } else {
    /* Errors ignored. */
  }

  CLEAR(fmt);

  fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

  fmt.fmt.pix.width       = cam->width;
  fmt.fmt.pix.height      = cam->height;
  fmt.fmt.pix.pixelformat = cam->camera_format;
  fprintf(stderr, "pixel format %d\n", fmt.fmt.pix.pixelformat);
  fmt.fmt.pix.field       = V4L2_FIELD_ANY; // how lines in video are interlaced when transmitted
  
  /* set/apply the video format */
  if (CVPI_SYS_ERROR == xioctl(cam->fd, VIDIOC_S_FMT, &fmt)) {
    errno_exit(__func__, "VIDIOC_S_FMT");
    return CVPI_CAMERA_START_ERROR;
  }
  fprintf(stderr, "pixel format %d\n", fmt.fmt.pix.pixelformat);

  /* Buggy driver paranoia. */
  min = fmt.fmt.pix.width * cam->camera_format;
  if (fmt.fmt.pix.bytesperline < min)
    fmt.fmt.pix.bytesperline = min;
  min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
  if (fmt.fmt.pix.sizeimage < min)
    fmt.fmt.pix.sizeimage = min;

  /* setup memory */
  return init_mmap(cam);
}

CVPI_BOOL cvpi_camera_start(cvpi_camera cam) {
  int ret = open_device(cam);	/* open("/dev/video",...), set the file descriptor cam->fd */
  if(CVPI_FALSE_TEST(ret)) {
    return CVPI_CAMERA_START_ERROR;
  }
  ret = init_device(cam);	/* apply width, height cam settings and other settings to camera.  */
  if(CVPI_FALSE_TEST(ret)) {
    close_device(cam);
    return CVPI_CAMERA_START_ERROR;
  }
  ret = start_capturing(cam);
  if(CVPI_FALSE_TEST(ret)) {
    ret = uninit_device(cam);
    if(CVPI_TRUE_TEST(ret)) {
      close_device(cam);
    }
    return CVPI_CAMERA_START_ERROR;
  }
  return CVPI_TRUE;
}

static int uninit_device(cvpi_camera cam) {
  unsigned long i;

  for (i = 0; i < cam->cam_buffers; ++i) {
    if (CVPI_SYS_ERROR == munmap(cam->buffers[i].start, cam->buffers[i].length)) {
      errno_exit(__func__, "munmap");
      return CVPI_CAMERA_TAKEDOWN_ERROR;
    }
  }
  
  free(cam->buffers);
  cam->buffers = NULL;
  return CVPI_TRUE;
}

static int close_device(cvpi_camera cam) {
  if (CVPI_SYS_ERROR == close(cam->fd)) {
    errno_exit(__func__, "close");
    return CVPI_CAMERA_TAKEDOWN_ERROR;
  }
  cam->fd = CVPI_SYS_ERROR;
  return CVPI_TRUE;
}

void cvpi_camera_takedown(cvpi_camera cam) {
  stop_capturing(cam);
  uninit_device(cam);
  close_device(cam);

  if(cam->buffers != NULL) {
    free(cam->buffers);
    cam->buffers = NULL;
  }
  free(cam);
  cam = NULL;
}
