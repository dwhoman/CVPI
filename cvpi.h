#ifndef CVPI
#define CVPI 1

#ifndef IMAGE_FUNCTIONS
#include "image_functions.h"
#endif

#ifndef EGL_CONFIG
#include "egl_config.h"
#endif

#ifndef VG_EXT
#include "vg_ext.h"
#endif

#ifndef IMAGE_HEADERS
#include "image_headers.h"
#endif

#ifndef CAMERA_SETUP
#include "camera_setup.h"
#endif

#else
#pragma message __FILE__ ": CVPI already defined."
#endif
