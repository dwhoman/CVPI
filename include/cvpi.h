#ifndef CVPI
#define CVPI 1

#ifndef CVPI_BASE
#include "cvpi_base.h"
#endif

#ifndef CVPI_IMAGE_FUNCTIONS
#include "cvpi_image_functions.h"
#endif

#ifndef CVPI_EGL_CONFIG
#include "cvpi_egl_config.h"
#endif

#ifndef CVPI_VG_EXT
#include "cvpi_vg_ext.h"
#endif

#ifndef CVPI_IMAGE_HEADERS
#include "cvpi_image_headers.h"
#endif

#if HAVE_CONFIG_H
# include "config.h"
#endif

#else
#pragma message __FILE__ ": CVPI already defined."
#endif
