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

#ifndef CVPI_EGL_SURFACE_FUNCTIONS
#define CVPI_EGL_SURFACE_FUNCTIONS 1

#ifndef CVPI_EGL_CONFIG
#include "cvpi_egl_config.h"
#endif

/* These are possible implemenations of the cvpi_egl_settings struct
   parameters
   surface_pixmap_create_function/surface_pixmap_destroy_function and
   surface_window_create_function/surface_window_destroy_function.  */

/* Function for surface_pixmap_create_function function pointer. */
EGLNativePixmapType cvpi_egl_surface_pixmap_native_creator(cvpi_egl_instance);

/* Function for surface_pixmap_destroy_function function pointer. */
EGLBoolean cvpi_egl_surface_pixmap_native_destroyer(cvpi_egl_instance);

/* The pixmap surface settings for eglCreateGlobalImageBRCM is stored
   in an association list. Make sure that the association list is
   destroyed when it is no longer needed. */
void cvpi_egl_surface_pixmap_list_destroy();

#else
#pragma message __FILE__ ": CVPI_EGL_SURFACE_FUNCTIONS already defined."
#endif
