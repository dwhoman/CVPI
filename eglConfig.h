#ifndef __eglplatform_h_
#include <EGL/eglplatform.h>
/* functions */

/* variables */
#endif
#ifndef __egl_h_
#include <EGL/egl.h>
/* functions */
/* eglGetDisplay */

/* variables */
#endif

#ifndef _STDIO_H
#include <stdio.h>
#endif
/* functions */
/* printf */
/* fprintf */
#ifndef BCM_HOST_H
#include "bcm_host.h"
#endif
/* functions */
/* bcm_host_init */

/* variables */
/*  */

#ifndef EGLEXT_BRCM_H
#include "eglext_brcm.h"
#endif

#ifndef EGL_GENERATED_CONFIG_HEADER_H
#include "eglGeneratedConfigHeader.h"
#endif

#ifndef EGL_CONFIG
#define EGL_CONFIG egl_settings_custom
#define EGL_CONFIG_SP struct egl_settings*

/* include the specified egl_settings struct from eglGeneratedConfigHeader.h, exclude the others */
EGL_CONFIG()

typedef struct {
  EGLDisplay eglDisplay;	/* set by eglGetDisplay */
  EGLSurface eglSurface;	/* set by create*Surface */
  EGLContext eglContext;	/* set by eglGetCurrentContext */
  NativeDisplayType display_id;	/* user defined */
  EGLint major;			/* user defined */
  EGLint minor;			/* user defined */
  EGLint attrib_list[63];	/* 31 attibutes + EGL_NONE, set by generated header */
  EGLint numberConfigs;				/* set by eglChooseConfigs */
  EGLConfig* matchingConfigs;			/* set by eglChooseConfigs */
  EGLint* id;					/* set by eglCreateGlobalImageBRCM */
} egl_deletable;

void* setupEGL(EGLint width, EGLint height, NativeDisplayType display_id);
void takedownEGL(void* deletable); /* caste to egl_deletable */

enum egl_surface_bits {
  egl_surface_bits_pbuffer = EGL_PBUFFER_BIT
  ,egl_surface_bits_pixmap = EGL_PIXMAP_BIT
  ,egl_surface_bits_window = EGL_WINDOW_BIT
  /* ,colorspace = EGL_VG_COLORSPACE_LINEAR_BIT */
  /* ,alpha = EGL_VG_ALPHA_FORMAT_PRE_BIT */
  /* ,multisample = EGL_MULTISAMPLE_RESOLVE_BOX_BIT */
  /* ,swap = EGL_SWAP_BEHAVIOR_PRESERVED_BIT */
};

void change_surface_type(enum egl_surface_bits bit);

enum egl_renderable_bits {
  egl_renderable_bits_opengl_es = EGL_OPENGL_ES_BIT
  ,egl_renderable_bits_openvg = EGL_OPENVG_BIT
  ,egl_renderable_bits_opengl_es2 = EGL_OPENGL_ES2_BIT
  ,egl_renderable_bits_opengl = EGL_OPENGL_BIT
};
void change_renderable_type(enum egl_renderable_bits bit);

enum egl_transparent_type {
  egl_transparent_type_none = EGL_NONE,
  egl_transparent_type_transparent = EGL_TRANSPARENT_RGB
};
void change_transparent_type(enum egl_transparent_type trans);

enum egl_color_buffer_type {
  egl_color_buffer_type_rgb = EGL_RGB_BUFFER,
  egl_color_buffer_type_luminance = EGL_LUMINANCE_BUFFER
};
void change_color_buffer_type(enum egl_color_buffer_type color);

enum egl_config_caveat {
  egl_config_caveat_none = EGL_NONE,
  egl_config_caveat_slow = EGL_SLOW_CONFIG,
  egl_config_caveat_nonconformant = EGL_NON_CONFORMANT_CONFIG
};

void change_config_caveat(enum egl_config_caveat caveat);

enum egl_renderable_api {
  egl_renderable_api_openvg = EGL_OPENVG_API,
  egl_renderable_api_opengl = EGL_OPENGL_ES_API
};

void change_renderable_api(enum egl_renderable_api api);

char* eglErrorString(EGLint);

#endif
