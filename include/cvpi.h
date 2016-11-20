/*
  This file is part of CVPI.

  Copyright 2017 Devin Homan
 
  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at
 
      http://www.apache.org/licenses/LICENSE-2.0
 
  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
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
