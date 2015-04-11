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

#ifndef __egl_h_
#include <EGL/egl.h>
#endif

#ifndef __eglext_h_
#include <EGL/eglext.h>
#endif

#ifndef __eglplatform_h_
#include <EGL/eglplatform.h>
#endif

#if HAVE_BCM_HOST == 1
#ifndef BCM_HOST_H
#include <bcm_host.h>
#endif
#endif

#ifndef EGL_EGLEXT_PROTOTYPES
#pragma message __FILE__ ": EGL_EGLEXT_PROTOTYPES must be defined for eglCreateGlobalImageBRCM prototype to be defined."
#endif

#if HAVE_BCM_HOST == 1
#ifndef EGLEXT_BRCM_H
#include <EGL/eglext_brcm.h>
/* eglCreateGlobalImageBRCM */
#endif
#endif

#ifndef CVPI
#include "cvpi.h"
#endif

#ifndef CVPI_EGL_CONFIG
#include "cvpi_egl_config.h"
#endif

#ifndef CVPI_EGL_SURFACE_FUNCTIONS
#include "cvpi_egl_surface_functions.h"
#endif

#ifndef _STDIO_H
#include <stdio.h>
#endif

/* EGL Pixmap */

/* the surface_pixmap_create_function function pointer type */
typedef EGLNativePixmapType (*pixmap_function_pointer)(cvpi_egl_instance);

/* function for surface_pixmap_create_function function pointer */
EGLNativePixmapType cvpi_egl_surface_pixmap_native_creator(cvpi_egl_instance egl_instance) {
  cvpi_egl_settings egl_settings_p = egl_instance->egl_settings;

  EGLint* pixmap_id = malloc(sizeof(*pixmap_id) * 5);
  if(pixmap_id == NULL) {
    return NULL;
  }

  pixmap_id[0] = 0;
  pixmap_id[1] = 0;
  pixmap_id[2] = egl_settings_p->width;
  pixmap_id[3] = egl_settings_p->height;
  pixmap_id[4] = egl_settings_p->pixel_format | egl_settings_p->pixel_format_brcm;

  /* eglCreateGlobalImageBRCM is undocumented, necessary to set the id */
  /* going off of http://www.raspberrypi.org/forums/viewtopic.php?f=63&t=6488 */
  EGLint stride = cvpi_egl_bytes_per_pixel(egl_settings_p->pixel_format) * egl_settings_p->width;
  eglCreateGlobalImageBRCM(egl_settings_p->width, egl_settings_p->height, 
			   pixmap_id[4], NULL, stride, pixmap_id);
  if(!(pixmap_id[0]) && !(pixmap_id[1])) {
    fprintf(stderr, "%s: eglCreateGlobalImageBRCM returned %s\n", __func__, cvpi_egl_error_string(eglGetError()));
    if(pixmap_id != NULL) {
      free(pixmap_id);
    }
    return NULL;
  } 
  egl_instance->native_data = pixmap_id;
  return pixmap_id;
}

/* function for surface_pixmap_destroy_function function pointer */
EGLBoolean cvpi_egl_surface_pixmap_native_destroyer(cvpi_egl_instance egl_instance) {
  if(egl_instance->native_data != NULL) {
    EGLBoolean retval = EGL_TRUE;
    if(!eglDestroyGlobalImageBRCM(egl_instance->native_data)) {
      fprintf(stderr, "eglDestroyGlobalImageBRCM returned EGL_FALSE.\n");
      retval = EGL_FALSE;
    }
    free(egl_instance->native_data);
    egl_instance->native_data = NULL;
    return retval;
  } else {
    return EGL_FALSE;
  }
}
/* end EGL Pixmap */



/* EGL Window */
/* EGLNativeWindowType cvpi_egl_surface_window_native_creator(cvpi_egl_instance egl_instance) { */
/*   cvpi_egl_settings egl_settings_p = egl_instance->egl_settings; */
/*   vc_vchi_dispmanx_init(); */
/*   DISPMANX_ELEMENT_HANDLE_T element; */

/*   element = vc_dispmanx_element_add ( update, display, */
/* 				      1/\*layer*\/, &dst_rect, 0/\*src*\/, */
/* 				      &src_rect, DISPMANX_PROTECTION_NONE, 0 /\*alpha*\/, 0/\*clamp*\/, 0/\*transform*\/); */

/*  EGL_DISPMANX_WINDOW_T window = {element, egl_settings_p->width, egl_settings_p->height}; */

/* NativeWindowType window */

/* /\* */
/* client_egl_get_window(CLIENT_GET_THREAD_STATE(),  */
/* 		      CLIENT_PROCESS_STATE_T *process,  */
/* 		      EGLNativeWindowType window) *\/ */
/* } */

/* EGLBoolean cvpi_egl_surface_window_native_destroyer(cvpi_egl_instance egl_instance) { */
/*   cvpi_egl_settings egl_settings_p = egl_instance->egl_settings; */
/* } */
