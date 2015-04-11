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

/* Check whether EGL_KHR_lock_surface macro is enabled. Developement
   was done with it disabled. */
#ifndef __egl_h_
#include <EGL/egl.h>
#endif

#ifndef EGLEXT_BRCM_H
#include "eglext_brcm.h"
#endif

#ifndef __eglplatform_h_
#include <EGL/eglplatform.h>
#endif

#ifndef _STDIO_H
#include <stdio.h>
#endif

#ifndef BCM_HOST_H
#include "bcm_host.h"
#endif

#ifndef __eglext_h_
#include "eglext.h"
#endif

int main() {
  bcm_host_init();
#if EGL_KHR_lock_surface 
  printf("EGL_KHR_lock_surface defined\n");
#else
  printf("EGL_KHR_lock_surface not defined\n");
#endif
  bcm_host_deinit();
}
