/*
  This file is part of CVPI.

  Copyright (C) 2015

  This program is free software: you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public License
  as published by the Free Software Foundation, either version 3 of
  the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/* Functions for setting up, taking down, and capturing video4linux
   supported cameras. */

#ifndef CVPI_CAMERA_SETUP
#define CVPI_CAMERA_SETUP 1

#ifndef CVPI_BASE
#include "cvpi_base.h"
/* CVPI_BOOL */
#endif


#ifndef _STDINT_H
#include <stdint.h>
/* uint32_t */
#endif

#ifndef __LINUX_VIDEODEV2_H
#include <linux/videodev2.h>
#endif

#define CVPI_SYS_ERROR -1
#define CVPI_CAMERA_START_ERROR -2
#define CVPI_CAMERA_READ_FRAME_ERROR -3
#define CVPI_CAMERA_TAKEDOWN_ERROR -4

typedef struct cvpi_camera* cvpi_camera;

/* Number tries to get a file handler for the camera before giving up. */
/* Default value = 100 */
extern unsigned long CVPI_CAMERA_MAX_IOCTL_REQUESTS;

/* format for cvpi_yuyv2yuva input */
#define CVPI_CAMERA_FORMAT V4L2_PIX_FMT_YUYV

/* The camera functions are listed here in the order in which they
   must be called. */

/* dev_path is the camera device path, usually /dev/video0. */
/* width and height are the frame pixel dimensions. They must be
   greater than 0.*/
/* camera_buffers_n is the number of memory buffers that the camera
   will have available for storing frames. The v4l2 documentation
   recommends that this value be at least 4. */
/* camera_format is the format used for reading frames. See v4l2-ctl
   for supported formats. CVPI_CAMERA_FORMAT is set to a format common
   in usb web cameras. */

/* If there is an error, NULL will be returned. */
cvpi_camera cvpi_camera_create(char* dev_path, 
			       unsigned long width,
			       unsigned long height,
			       unsigned long camera_buffers_n,
			       unsigned long camera_format);
/* Turns on the camera. */
/* Returns true if successful, else returns false. */
CVPI_BOOL cvpi_camera_start(cvpi_camera);

/* A user specified function telling cvpi_camera_read_frame what to do
   after capturing a frame. The function arguments are the frame's
   location in memory and byte length. The frame is stored in volatile
   memory. */
typedef void (*cvpi_process_image)(void* start, uint32_t length);

/* Runs the specified cvpi_process_image on a frame captured by
   cvpi_camera. Returns true if successful, else returns false. */
CVPI_BOOL cvpi_camera_read_frame(cvpi_camera, cvpi_process_image process);

/* Turns off the camera, and destroys the cvpi_camera and sets it to NULL. */
void cvpi_camera_takedown(cvpi_camera);
#else
#pragma message __FILE__ ": CVPI_CAMERA_SETUP already defined."
#endif
