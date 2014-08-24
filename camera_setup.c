/* based on LINUX MEDIA INFRASTRUCTURE API Appendix D. Video Capture Example */
#ifndef CAMERA_SETUP_H
#include "camera_setup.h"
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
#endif

#ifndef	_SYS_STAT_H
#include <sys/stat.h>
/* struct stat */
#endif

#ifndef _FCNTL_H
#include <fcntl.h>
/* O_RDWR */
#endif

#ifndef _SYS_MMAN_H
#include <sys/mman.h>
/* mmap */
#endif

#ifndef transConfig_h
#include "transConfig.h"
#endif

#ifndef _STRING_H
#include "string.h"
/* memset() */
#endif

/* number of camera memory buffers */
#define CAMERA_INIT_COUNT 4

#define CAMERA_FORMAT V4L2_PIX_FMT_YUYV

#define CLEAR(x) memset(&(x), 0, sizeof(x))

#define MAX_IOCTL_REQUESTS 10000

static int xioctl(int fh, int request, void *arg) {
  int r;
  unsigned long itter = 0;
  
  do {
    r = ioctl(fh, request, arg);
    if(itter > MAX_IOCTL_REQUESTS) {
      fprintf(stderr, "xioctl timed out\n");
      return CAMERA_SETUP_ERROR;
    }
    ++itter;
  } while (CAMERA_SETUP_ERROR == r && EINTR == errno);
  
  return r;
}

static void errno_exit(const char *s) {
  fprintf(stderr, "%s error %d, %s\n", s, errno, strerror(errno));
}

int read_frame(struct camera* cam, struct buffer* buffer) {
  struct v4l2_buffer buf;
  CLEAR(buf);
  printf("read_frame\n");

  fd_set fds;			/* set man 2 select */
  struct timeval tv;
  int r;

  FD_ZERO(&fds);
  FD_SET(cam->fd, &fds);
  
  /* Timeout. */
  tv.tv_sec = 2;
  tv.tv_usec = 0;
  
  r = select(cam->fd + 1, &fds, NULL, NULL, &tv);
  
  if (-1 == r) {
    errno_exit("select");
    return CAMERA_SETUP_ERROR;
  }
  
  if (0 == r) {
    fprintf(stderr, "select timeout\n");
    return CAMERA_SETUP_ERROR;
  }
  
#if IO_METHOD_CHOICE == IO_METHOD_MMAP
  
  buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  buf.memory = V4L2_MEMORY_MMAP;
  
  if (CAMERA_SETUP_ERROR == xioctl(cam->fd, VIDIOC_DQBUF, &buf)) {
    switch (errno) {
    case EAGAIN:
      printf("read_frame EAGAIN\n");
      //return 0;
      return CAMERA_SETUP_ERROR;
      
    case EIO:
      /* Could ignore EIO, see spec. */
      
      /* fall through */
      
    default:
      errno_exit("VIDIOC_DQBUF");
      return CAMERA_SETUP_ERROR;
    }
  }
  
  assert(buf.index < cam->n_buffers);
  
  buffer->start = cam->buffers[buf.index].start;
  buffer->length = buf.bytesused;
  printf("read_frame start: %x, length: %u\n", buffer->start, buffer->length);
  
  if (CAMERA_SETUP_ERROR == xioctl(cam->fd, VIDIOC_QBUF, &buf)) {
    errno_exit("VIDIOC_QBUF");
    return CAMERA_SETUP_ERROR;
  }
#else  /* IO_METHOD_USERPTR */
  buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  buf.memory = V4L2_MEMORY_USERPTR;
  buf.parm.capture.timeperframe.numerator = 1;  
  buf.parm.capture.timeperframe.denominator = 30; /* 30 fps */
  
  if (CAMERA_SETUP_ERROR == xioctl(cam->fd, VIDIOC_DQBUF, &buf)) {
    /* grab a camera capture */
    /* initialized by start_capturing() */
    switch (errno) {
    case EAGAIN:
      return 0;
      
    case EIO:
      /* Could ignore EIO, see spec. */
      
      /* fall through */
      
    default:
      errno_exit("VIDIOC_DQBUF");
      return CAMERA_SETUP_ERROR;
    }
  }
  
  for (i = 0; i < cam->n_buffers; ++i) {
    /* increment i */
    if (buf.m.userptr == (unsigned long)(cam->buffers)[i].start
	&& buf.length == cam->buffers[i].length) {
      break;
    }
  }
  
  assert(i < cam->n_buffers);
  
  buffer->start = (void *)buf.m.userptr;
  buffer->length = buf.bytesused;
  printf("read_frame start: %x, length: %u\n", buffer->start, buffer->length);

  if (CAMERA_SETUP_ERROR == xioctl(cam->fd, VIDIOC_QBUF, &buf)) {
    /* queue for another capture */
    errno_exit("VIDIOC_QBUF");
    return CAMERA_SETUP_ERROR;
  }
#endif
  return 0;
}

static int stop_capturing(struct camera* cam) {
  enum v4l2_buf_type type;
  
  type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if (CAMERA_SETUP_ERROR == xioctl(cam->fd, VIDIOC_STREAMOFF, &type)) {
    errno_exit("VIDIOC_STREAMOFF");
    return CAMERA_SETUP_ERROR;
  }
  return 0;
}

static int start_capturing(struct camera* cam) {
  unsigned int i;
  enum v4l2_buf_type type;
  
#if IO_METHOD_CHOICE == IO_METHOD_MMAP
  for (i = 0; i < cam->n_buffers; ++i) {
    struct v4l2_buffer buf;
    
    CLEAR(buf);
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = i;
			
    if (CAMERA_SETUP_ERROR == xioctl(cam->fd, VIDIOC_QBUF, &buf)) {
      errno_exit("VIDIOC_QBUF");
      return CAMERA_SETUP_ERROR;
    }
  }
  type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if (CAMERA_SETUP_ERROR == xioctl(cam->fd, VIDIOC_STREAMON, &type)) {
    errno_exit("VIDIOC_STREAMON");
    return CAMERA_SETUP_ERROR;
  }
#else
  for (i = 0; i < n_buffers; ++i) {
    struct v4l2_buffer buf;
    
    CLEAR(buf);
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_USERPTR;
    buf.index = i;
    buf.m.userptr = (unsigned long)(cam->buffers)[i].start;
    buf.length = (cam->buffers)[i].length;
    
    if (CAMERA_SETUP_ERROR == xioctl(cam->fd, VIDIOC_QBUF, &buf)) {
      errno_exit("VIDIOC_QBUF");
      return CAMERA_SETUP_ERROR;
    }
  }
  type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if (CAMERA_SETUP_ERROR == xioctl(cam->fd, VIDIOC_STREAMON, &type)) {
    errno_exit("VIDIOC_STREAMON");
    return CAMERA_SETUP_ERROR;
  }
#endif
  return 0;
}

static int open_device(struct camera* cam) {
  struct stat st;
  
  if (CAMERA_SETUP_ERROR == stat(cam->path, &st)) {
    fprintf(stderr, "Cannot identify '%s': %d, %s\n",
	    cam->path, errno, strerror(errno));
    return CAMERA_SETUP_ERROR;
  }
  
  if (!S_ISCHR(st.st_mode)) {
    fprintf(stderr, "%s is no device\n", cam->path);
    return CAMERA_SETUP_ERROR;
  }
  
  cam->fd = open(cam->path, O_RDWR /* required */ | O_NONBLOCK, 0);
  
  if (CAMERA_SETUP_ERROR == cam->fd) {
    fprintf(stderr, "Cannot open '%s': %d, %s\n",
	    cam->path, errno, strerror(errno));
    return CAMERA_SETUP_ERROR;
  }
  return 0;
}

static int init_mmap(struct camera* cam) {
  struct v4l2_requestbuffers req;
  
  CLEAR(req);
  
  req.count = CAMERA_INIT_COUNT;
  req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  req.memory = V4L2_MEMORY_MMAP;

  if (CAMERA_SETUP_ERROR == xioctl(cam->fd, VIDIOC_REQBUFS, &req)) {
    if (EINVAL == errno) {
      fprintf(stderr, "%s does not support memory mapping\n", cam->path);
      return CAMERA_SETUP_ERROR;
    } else {
      errno_exit("VIDIOC_REQBUFS");
      return CAMERA_SETUP_ERROR;
    }
  }

  if (req.count < 2) {
    fprintf(stderr, "Insufficient buffer memory on %s\n", cam->path);
      return CAMERA_SETUP_ERROR;
  }

  cam->buffers = calloc(req.count, sizeof(*(cam->buffers)));

  if (!cam->buffers) {
    fprintf(stderr, "Out of memory\n");
    return CAMERA_SETUP_ERROR;
  }

  int n = 0;
  for (n = 0; n < req.count; ++n) {
    struct v4l2_buffer buf;
    
    CLEAR(buf);

    buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory      = V4L2_MEMORY_MMAP;
    buf.index       = n;

    if (CAMERA_SETUP_ERROR == xioctl(cam->fd, VIDIOC_QUERYBUF, &buf)) {
      errno_exit("VIDIOC_QUERYBUF");
      return CAMERA_SETUP_ERROR;
    }
    
    cam->buffers[n].length = buf.length;
    cam->buffers[n].start =
      mmap(NULL /* start anywhere */,
	   buf.length,
	   PROT_READ | PROT_WRITE /* required */,
	   MAP_SHARED /* recommended */,
	   cam->fd, buf.m.offset);
    
    if (MAP_FAILED == cam->buffers[n].start) {
      errno_exit("mmap");
      return CAMERA_SETUP_ERROR;
    }
  }
}

static int init_userp(struct camera* cam, unsigned int buffer_size) {
  struct v4l2_requestbuffers req;
  
  CLEAR(req);
  
  req.count  = CAMERA_INIT_COUNT;
  req.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  req.memory = V4L2_MEMORY_USERPTR;
  
  if (CAMERA_SETUP_ERROR == xioctl(cam->fd, VIDIOC_REQBUFS, &req)) {
    if (EINVAL == errno) {
      fprintf(stderr, "%s does not support user pointer i/o\n", cam->path);
      return CAMERA_SETUP_ERROR;
    } else {
      errno_exit("VIDIOC_REQBUFS");
      return CAMERA_SETUP_ERROR;
    }
  }
  
  cam->buffers = calloc(CAMERA_INIT_COUNT, sizeof(*(cam->buffers)));
  
  if (!cam->buffers) {
    fprintf(stderr, "Out of memory\n");
    return CAMERA_SETUP_ERROR;
  }
  
  int n = 0;
  for (; n < CAMERA_INIT_COUNT; ++n) {
    cam->buffers[n].length = buffer_size;
    cam->buffers[n].start = malloc(buffer_size);
    
    if (!cam->buffers[n].start) {
      fprintf(stderr, "Out of memory\n");
      return CAMERA_SETUP_ERROR;
    }
  }
}

static int init_device(struct camera* cam) {
  struct v4l2_capability cap;
  struct v4l2_cropcap cropcap;
  struct v4l2_crop crop;
  struct v4l2_format fmt;
  unsigned int min;

  if (CAMERA_SETUP_ERROR == xioctl(cam->fd, VIDIOC_QUERYCAP, &cap)) {
    if (EINVAL == errno) {
      fprintf(stderr, "%s is no V4L2 device\n", cam->path);
    } else {
      errno_exit("VIDIOC_QUERYCAP");
    }
    return CAMERA_SETUP_ERROR;
  }
  
  if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
    fprintf(stderr, "%s is no video capture device\n", cam->path);
    return CAMERA_SETUP_ERROR;
  }

  if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
    fprintf(stderr, "%s does not support streaming i/o\n",
	    cam->path);
    return CAMERA_SETUP_ERROR;
  }
  

  /* Select video input, video standard and tune here. */

  CLEAR(cropcap);

  cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

  if (0 == xioctl(cam->fd, VIDIOC_CROPCAP, &cropcap)) {
    crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    crop.c = cropcap.defrect; /* reset to default */
    
    if (CAMERA_SETUP_ERROR == xioctl(cam->fd, VIDIOC_S_CROP, &crop)) {
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

  fmt.fmt.pix.width       = TRANS_WIDTH;
  fmt.fmt.pix.height      = TRANS_HEIGHT;
  fmt.fmt.pix.pixelformat = CAMERA_FORMAT;
  printf("pixel format %d\n", fmt.fmt.pix.pixelformat);
  fmt.fmt.pix.field       = V4L2_FIELD_ANY; //V4L2_FIELD_INTERLACED;
  
  if (CAMERA_SETUP_ERROR == xioctl(cam->fd, VIDIOC_S_FMT, &fmt)) {
    errno_exit("VIDIOC_S_FMT");
    return CAMERA_SETUP_ERROR;
  }
  printf("pixel format %d\n", fmt.fmt.pix.pixelformat);

  /* Buggy driver paranoia. */
  min = fmt.fmt.pix.width * pixel_format_name(TRANS_BYTES);
  if (fmt.fmt.pix.bytesperline < min)
    fmt.fmt.pix.bytesperline = min;
  min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
  if (fmt.fmt.pix.sizeimage < min)
    fmt.fmt.pix.sizeimage = min;
  
#if IO_METHOD_CHOICE == IO_METHOD_MMAP
  return init_mmap(cam);
#else
  return init_userp(cam, fmt.fmt.pix.sizeimage);
#endif
}

int camera_setup(struct camera* cam) {
  int ret = open_device(cam);
  if(ret == CAMERA_SETUP_ERROR) {
    return CAMERA_SETUP_ERROR;
  }
  ret = init_device(cam);
  if(ret == CAMERA_SETUP_ERROR) {
    close_device(cam);
    return CAMERA_SETUP_ERROR;
  }
  ret = start_capturing(cam);
  if(ret == CAMERA_SETUP_ERROR) {
    ret = uninit_device(cam);
    if(ret != CAMERA_SETUP_ERROR) {
      close_device(cam);
    }
    return CAMERA_SETUP_ERROR;
  }
  return 0;
}

static int uninit_device(struct camera* cam) {
  unsigned int i;
  
#if IO_METHOD_CHOICE == IO_METHOD_MMAP
  for (i = 0; i < cam->n_buffers; ++i) {
    if (-1 == munmap(cam->buffers[i].start, cam->buffers[i].length)) {
      errno_exit("munmap");
      return CAMERA_SETUP_ERROR;
    }
  }
  
#else
  for (i = 0; i < cam->n_buffers; ++i) {
    free(cam->buffers[i].start);
  }
#endif
  free(cam->buffers);
  return 0;
}

static int close_device(struct camera* cam) {
  if (-1 == close(cam->fd)) {
    errno_exit("close");
    return CAMERA_SETUP_ERROR;
  }
  cam->fd = -1;
  return 0;
}

void camera_takedown(struct camera* cam) {
  stop_capturing(cam);
  uninit_device(cam);
  close_device(cam);
}
