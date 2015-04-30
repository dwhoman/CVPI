/*
  This file is part of CVPI.

  Copyright (C) 2015 Devin Homan

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

#ifndef CVPI
#define CVPI 1

//#
//include "config.h"

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

#else
#pragma message __FILE__ ": CVPI already defined."
#endif
