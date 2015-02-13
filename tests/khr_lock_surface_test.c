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
