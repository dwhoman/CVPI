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
