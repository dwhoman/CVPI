/** \file cvpi_egl_config.h

    \brief A template interface to Khronos EGL.  The file contains
    functions for testing settings to be used with EGL, and creating
    EGL instances. The user can check their settings by creating a
    cvpi_egl_settings struct instance and calling
    cvpi_egl_settings_check(). The user can create a new EGL instance
    by calling cvpi_egl_instance(). 
*/

#ifndef CVPI_EGL_CONFIG
#define CVPI_EGL_CONFIG 1

#ifndef __egl_h_
#include <EGL/egl.h>
/* EGL_OPENGL_BIT, etc. */
#endif

#if HAVE_BCM_HOST == 1
#ifndef EGLEXT_BRCM_H
#include <EGL/eglext_brcm.h>
/* EGL_PIXEL_FORMAT_ARGB_8888_PRE_BRCM, etc. */
#endif
#else
#pragma message __FILE__ ": Not including eglext_brcm.h. "
#endif

/* This file does not exist on the system. */
/* #ifndef EGL_CLIENT_CONFIG_H */
/* #include <egl_client_config.h> */
/* /\* EGL_CONFIG_MAX_WIDTH,... *\/ */
/* #endif */

#ifndef CVPI_BASE
#include "cvpi_base.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* max width and height are defined in egl_client_config.h in the
   userland repository. Possible error noted in egl_client.c:
   "EGL_BAD_NATIVE_WINDOW window is larger than EGL_CONFIG_MAX_WIDTH x
   EGL_CONFIG_MAX_HEIGHT" */
#ifndef EGL_CONFIG_MAX_WIDTH
#define EGL_CONFIG_MAX_WIDTH 2048
#endif

#ifndef EGL_CONFIG_MAX_HEIGHT
#define EGL_CONFIG_MAX_HEIGHT 2048
#endif

/** \def CVPI_EGL_DEFAULT_PIXEL_FORMAT
    \brief CVPI uses ARGB 8888 format for reading and writing image data.
 */
#if HAVE_BCM_HOST == 1
#define CVPI_EGL_DEFAULT_PIXEL_FORMAT EGL_PIXEL_FORMAT_ARGB_8888_BRCM
#endif

/* Many of the EGL settings have a fixed number of possibilities. The
   Broadcom implementation uses #define statements for most of these
   options and their functions simply take integer types. Enums are
   used here to facilitate type checking.
*/


/** 
    All possible pixel formats for reading and writing image data. The
    default, cvpi_egl_ARGB_8888, is recommended because some CVPI functions
    work only with that pixel format. 
 */
#if HAVE_BCM_HOST == 1
enum cvpi_egl_pixel_format {
  cvpi_egl_ARGB_8888_PRE = EGL_PIXEL_FORMAT_ARGB_8888_PRE_BRCM,
  cvpi_egl_ARGB_8888     = EGL_PIXEL_FORMAT_ARGB_8888_BRCM,
  cvpi_egl_XRGB_8888     = EGL_PIXEL_FORMAT_XRGB_8888_BRCM,
  cvpi_egl_RGB_565       = EGL_PIXEL_FORMAT_RGB_565_BRCM,
  cvpi_egl_A_8           = EGL_PIXEL_FORMAT_A_8_BRCM
};
#endif

/** 
    Additional, undocumented settings that appear necessary to pass to
    eglCreateGlobalImageBRCM as part of its third argument.
*/
#if HAVE_BCM_HOST == 1
enum cvpi_egl_pixel_format_brcm {
  cvpi_egl_pixel_format_gles2_texture_brcm = EGL_PIXEL_FORMAT_GLES2_TEXTURE_BRCM,
  cvpi_egl_pixel_format_gles_texture_brcm = EGL_PIXEL_FORMAT_GLES_TEXTURE_BRCM,
  cvpi_egl_pixel_format_render_gles2_brcm = EGL_PIXEL_FORMAT_RENDER_GLES2_BRCM,
  cvpi_egl_pixel_format_render_gles_brcm  = EGL_PIXEL_FORMAT_RENDER_GLES_BRCM,
  cvpi_egl_pixel_format_render_gl_brcm    = EGL_PIXEL_FORMAT_RENDER_GL_BRCM,
  cvpi_egl_pixel_format_render_mask_brcm  = EGL_PIXEL_FORMAT_RENDER_MASK_BRCM,
  cvpi_egl_pixel_format_render_vg_brcm    = EGL_PIXEL_FORMAT_RENDER_VG_BRCM,
  cvpi_egl_pixel_format_texture_mask_brcm = EGL_PIXEL_FORMAT_TEXTURE_MASK_BRCM,
  cvpi_egl_pixel_format_usage_mask_brcm   = EGL_PIXEL_FORMAT_USAGE_MASK_BRCM,
  cvpi_egl_pixel_format_vg_image_brcm     = EGL_PIXEL_FORMAT_VG_IMAGE_BRCM,
  cvpi_egl_pixel_format_brcm_none = 0
};
#endif

/**  EGL_SURFACE_TYPE bitmask bits
 */
enum cvpi_egl_surface_bits {
  cvpi_egl_surface_bits_pbuffer = EGL_PBUFFER_BIT
  ,cvpi_egl_surface_bits_pixmap = EGL_PIXMAP_BIT
  ,cvpi_egl_surface_bits_window = EGL_WINDOW_BIT
  ,cvpi_egl_surface_bits_colorspace = EGL_VG_COLORSPACE_LINEAR_BIT
  ,cvpi_egl_surface_bits_alpha = EGL_VG_ALPHA_FORMAT_PRE_BIT
  ,cvpi_egl_surface_bits_multisample = EGL_MULTISAMPLE_RESOLVE_BOX_BIT
  ,cvpi_egl_surface_bits_swap = EGL_SWAP_BEHAVIOR_PRESERVED_BIT
  ,cvpi_egl_surface_bits_dc = EGL_DONT_CARE
};
/** EGL_RENDERABLE_TYPE bitmask bits
*/
enum cvpi_egl_renderable_bits {
  cvpi_egl_renderable_bits_opengl_es = EGL_OPENGL_ES_BIT
  ,cvpi_egl_renderable_bits_openvg = EGL_OPENVG_BIT
  ,cvpi_egl_renderable_bits_opengl_es2 = EGL_OPENGL_ES2_BIT
  /* OpenGL does not appear to be supported */
  ,cvpi_egl_renderable_bits_opengl = EGL_OPENGL_BIT
};
/* EGL_TRANSPARENT_TYPE enum */
/* From egl_client.c, this setting is EGL_NONE regardless of what the
   user selects; does not do anything in the Broadcom
   implementation. */
enum cvpi_egl_transparent_type {
  cvpi_egl_transparent_type_dc = EGL_DONT_CARE,
  cvpi_egl_transparent_type_none = EGL_NONE,
  cvpi_egl_transparent_type_rgb = EGL_TRANSPARENT_RGB
};

/* From egl_client.c, this setting is EGL_RGB_BUFFER regardless of
   what the user selects; does not do anything in the Broadcom
   implementation. */
enum cvpi_egl_color_buffer_type {
  cvpi_egl_color_buffer_type_dc = EGL_DONT_CARE,
  cvpi_egl_color_buffer_type_rgb = EGL_RGB_BUFFER,
  cvpi_egl_color_buffer_type_luminance = EGL_LUMINANCE_BUFFER
};

/* From egl_client.c, this setting is EGL_NONE regardless of what the
   user selects; does not do anything in the Broadcom
   implementation. */
enum cvpi_egl_config_caveat {
  cvpi_egl_config_caveat_dc = EGL_DONT_CARE,
  cvpi_egl_config_caveat_none = EGL_NONE,
  cvpi_egl_config_caveat_slow = EGL_SLOW_CONFIG,
  cvpi_egl_config_caveat_nonconformant = EGL_NON_CONFORMANT_CONFIG
};

/* API's allowed by Broadcom's EGL implementation. */
/* See userland/interface/khronos/egl/egl_client.c eglBindAPI(). */
/* OpenVG is the default for cvpi_egl_settings_create(). */
enum cvpi_egl_renderable_api {
  cvpi_egl_renderable_api_openvg = EGL_OPENVG_API,
  cvpi_egl_renderable_api_opengl = EGL_OPENGL_ES_API,
  /* EGL_NONE is a valid argument to eglBindAPI according to the 1.4
     spec p. 43, but it is unsupported */
  //  cvpi_egl_renderable_api_none = EGL_NONE,
  /* If the user does not want to change the current redering API */
  cvpi_egl_renderable_api_current = 0
};

/* Allowed bit values for EGL_CONFORMANT, except for EGL_DONT_CARE
   which is not a bit value but an entire EGLint value. */
/* See userland/interface/khronos/egl/egl_client_config_cr.c case EGL_CONFORMANT: */
enum cvpi_egl_conformant_bits {
  cvpi_egl_conformant_bit_dc = EGL_DONT_CARE,
  cvpi_egl_conformant_bit_opengl = EGL_OPENGL_BIT,
  cvpi_egl_conformant_bit_opengl_es = EGL_OPENGL_ES_BIT,
  cvpi_egl_conformant_bit_opengl_es2 = EGL_OPENGL_ES2_BIT,
  cvpi_egl_conformant_bit_openvg = EGL_OPENVG_BIT
};

/* EGLBoolean is defined as unsigned int in egl.h, this interface
   further restricts it  */
enum CVPI_EGLBoolean {
  CVPI_EGL_TRUE = EGL_TRUE,
  CVPI_EGL_FALSE = EGL_FALSE,
  CVPI_EGL_DONT_CARE = EGL_DONT_CARE
};

/* The current surface can only have one of three types. A new surface
   will not be created if cvpi_egl_surface_type_none is selected. */
enum cvpi_egl_current_surface_type {
  cvpi_egl_surface_type_none,
  cvpi_egl_surface_type_pixmap,
  cvpi_egl_surface_type_pbuffer,
  cvpi_egl_surface_type_window
};
/* Samples per pixel */
/* From egl_client.c, these are the only two possible values. */
enum cvpi_egl_samples {
  cvpi_egl_samples_zero = 0,
  cvpi_egl_samples_one = 1,
};

/* eglSurfaceAttrib */
/* EGL_MIPMAP_LEVEL, EGL_MULTISAMPLE_RESOLVE, */
/* EGL_SWAP_BEHAVIOR */

/* egl window surface attributes */
/* EGL_RENDER_BUFFER (values: EGL_SINGLE_BUFFER, EGL_BACK_BUFFER), 
   EGL_VG_COLORSPACE (values: EGL_VG_COLORSPACE_sRGB (default), EGL_VG_COLORSPACE_LINEAR), 
   EGL_VG_ALPHA_FORMAT (values: EGL_VG_COLORSPACE_LINEAR (default), EGL_VG_ALPHA_FORMAT_PRE) */

  /* egl pbuffer surface attributes */
/* EGL_WIDTH, EGL_HEIGHT, EGL_LARGEST_PBUFFER, EGL_TEXTURE_FORMAT, */
/*    EGL_TEXTURE_TARGET, EGL_MIPMAP_TEXTURE, EGL_VG_COLORSPACE, and EGL_VG_ALPHA_FORMAT */

/* Surface attributes Table 3.5 in EGL spec 1.4 */
/* EGL_VG_ALPHA_FORMAT */
/* EGL_VG_COLORSPACE */
/* EGL_CONFIG_ID Type */
/* EGL_HEIGHT */
/* EGL_HORIZONTAL_RESOLUTION */
/* EGL_LARGEST_PBUFFER */
/* EGL_MIPMAP_TEXTURE */
/* EGL_MIPMAP_LEVEL */
/* EGL_MULTISAMPLE_RESOLVE */
/* EGL_PIXEL_ASPECT_RATIO */
/* EGL_RENDER_BUFFER */
/* EGL_SWAP_BEHAVIOR */
/* EGL_TEXTURE_FORMAT integer */
/* EGL_TEXTURE_TARGET */
/* EGL_VERTICAL_RESOLUTION */
/* EGL_WIDTH  */

typedef struct cvpi_egl_instance_s* cvpi_egl_instance;

/** \struct cvpi_egl_settings

    \brief User set EGL parameters.

   Allowing client direct access to the struct. EGL required
   parameters are well defined and unlikely to change.  The user is
   responsible for destroying. Setting and sanity check functions are
   given below.

*/
struct cvpi_egl_settings_s {
  /* width and height may be required by the
     surface_pixmap_create_function or the
     surface_window_create_function, otherwise unused */
  unsigned long width;
  unsigned long height;	
  enum cvpi_egl_renderable_api renderable_api;
  /* which surface will be rendered to */
  enum cvpi_egl_current_surface_type current_surface_type;
#if HAVE_BCM_HOST == 1
  enum cvpi_egl_pixel_format pixel_format;
#endif
#if HAVE_BCM_HOST == 1
  EGLint pixel_format_brcm;	/* additional settings for
				   eglCreateGlobalImageBRCM */
#endif
  /* default: EGL_NO_CONTEXT, passed to eglCreateContext */
  EGLContext share_context;

  /* display_id defaults to EGL_DEFAULT_DISPLAY. There no point in
     changing it because Broadcom's implementation only supports one
     display, according to egl_client.c */
  NativeDisplayType display_id;	

  /* pixmapSurface/pbufferSurface/windowSurface/eglCreateContext attribute list */
  /* The list must be structured similar to an association list, the
     attribute name followed by its value. The last element of the
     list must be EGL_NONE. The default value is NULL. */
  const EGLint *pixmap_surface_attrib_list;
  const EGLint *pbuffer_surface_attrib_list;
  const EGLint *window_surface_attrib_list;
  const EGLint *context_attrib_list;

  /* eglCreatePixmapSurface's and eglCreateWindowSurface's third
     arguments, pixmap and win respectively, are implementation
     dependent. The user must define how to get the pixmap and win. */
  EGLNativePixmapType (*surface_pixmap_create_function)(cvpi_egl_instance);
  EGLNativeWindowType (*surface_window_create_function)(cvpi_egl_instance);

  /* Functions to call when taking down an EGL instance to undo what
     was done by the surface_pixmap_create_function and
     surface_window_create_function. */
  EGLBoolean (*surface_pixmap_destroy_function)(cvpi_egl_instance);
  EGLBoolean (*surface_window_destroy_function)(cvpi_egl_instance);


  /** \defgroup Parameters from tables 3.1 and 3.4 in the EGL 1.4 spec. */
  EGLint buffer_size;
  EGLint red_size;
  EGLint green_size;
  EGLint blue_size;
  EGLint luminance_size;
  EGLint alpha_size;
  EGLint alpha_mask_size;
  enum CVPI_EGLBoolean bind_to_texture_rgb;
  enum CVPI_EGLBoolean bind_to_texture_rgba;
  /* EGL_COLOR_BUFFER_TYPE always EGL_RGB_BUFFER, from egl_client.c */
  enum cvpi_egl_color_buffer_type color_buffer_type;
  /* EGL_CONFIG_CAVEAT always EGL_NONE, from egl_client.c. */
  enum cvpi_egl_config_caveat config_caveat;
  EGLint config_id;
  EGLint conformant;
  EGLint depth_size;

  /* this setting appears ignored according to egl_client_config_cr.c */
  EGLint level;
  /* Pbuffer width and height are ignored by Broadcom's EGL implementation */
  EGLint max_pbuffer_width;
  EGLint max_pbuffer_height;

  /* this setting appears ignored according to egl_client_config_cr.c */
  EGLint max_pbuffer_pixels;

  /* Note: EGL_NONE is not supported by Broadcom's EGL
     implementation, but you can use EGL_DONT_CARE, see
     egl_client_config_cr.c case EGL_MATCH_NATIVE_PIXMAP: */
  EGLint match_native_pixmap;
  /* Min and Max swap interval are ignored by Broadcom's EGL implementation */
  EGLint max_swap_interval;
  EGLint min_swap_interval;

  /* From egl_client.c, this setting is always true, regardless. */
  enum CVPI_EGLBoolean native_renderable;

  /* native_visual_id and native_visual_type are ignored according
     to egl_client.c and egl_client_config_cr.c */
  EGLint native_visual_id;
  EGLint native_visual_type;
  EGLint renderable_type;
  EGLint sample_buffers;
  enum cvpi_egl_samples samples;
  EGLint stencil_size;
  /* supported surfaces */
  EGLint surface_type;
  /* EGL_TRANSPARENT_TYPE is always none */
  enum cvpi_egl_transparent_type transparent_type;
  EGLint transparent_red_value;
  EGLint transparent_green_value;
  EGLint transparent_blue_value;
  /* end group */
};

typedef struct cvpi_egl_settings_s* cvpi_egl_settings;

struct cvpi_egl_instance_s {
  EGLDisplay egl_display;	/* set by eglGetDisplay */
  EGLSurface egl_surface;	/* set by createPixmapSurface */
  EGLContext egl_context;	/* set by eglGetCurrentContext */
  EGLint major;			/* system's EGL version = 1.4 */
  EGLint minor;
  EGLint attrib_list[67];	/* 33 attibutes + EGL_NONE, set by
				   generated header */

  /* eglChooseConfigs gets a list of matching configs, but not all of
     them match. The first real matching config is at matching_config_index. */
  EGLint matching_config_index;
  EGLConfig* matching_configs;

  /* points to the cvpi_egl_settings instance passed into
     cvpi_egl_instance_setup by the user */
  cvpi_egl_settings egl_settings;

  /* link that can be used by EGLNativePixmapType generating function */
  void* native_data;
};

/** \fn cvpi_egl_settings cvpi_egl_settings_create(void) 

    \brief Convenience function for creating a struct
    cvpi_egl_settings instance with values set. Defaults to creating a
    specification to create an OpenVG Pixmap that is not to be set as
    the current surface.  */
cvpi_egl_settings cvpi_egl_settings_create(void);

/* Create a new EGL instance. If current_surface_type is
   cvpi_egl_surface_type_none, then the surface created will not be
   made current; necessary if there is already a current surface.

   On success the function returns a pointer to a cvpi_egl_instance
   struct, which should be passed to cvpi_egl_instance_takedown to
   take down the EGL session.

   If unsuccessful, what has been setup by the function will be taken
   down and a NULL pointer will be returned.
 */
cvpi_egl_instance cvpi_egl_instance_setup(cvpi_egl_settings);

/* Side-effects :: Sets the cvpi_egl_instance parameter to NULL */
void cvpi_egl_instance_takedown(cvpi_egl_instance);

/* For settings whose types cannot be enforced statically, the
   following functions allow the user to check and modify those
   settings. All of the functions return an integer greater than zero
   if successful, otherwise returning an integer <= 0 if unsucessful.
   */

/* For bitmap settings, modifying functions have a parameter to
   specify whether to add or remove a particular bit. */
enum cvpi_egl_settings_change {cvpi_egl_settings_add, cvpi_egl_settings_remove};

/* Runs all of the check functions. */
CVPI_BOOL cvpi_egl_settings_check(cvpi_egl_settings);

CVPI_BOOL cvpi_egl_settings_width(cvpi_egl_settings, unsigned long width);
CVPI_BOOL cvpi_egl_settings_width_check(unsigned long width);

CVPI_BOOL cvpi_egl_settings_height(cvpi_egl_settings, unsigned long height);
CVPI_BOOL cvpi_egl_settings_height_check(unsigned long width);

#if HAVE_BCM_HOST == 1
CVPI_BOOL cvpi_egl_settings_pixel_format_brcm_check(cvpi_egl_settings);
#endif
#if HAVE_BCM_HOST == 1
CVPI_BOOL cvpi_egl_settings_pixel_format_brcm(cvpi_egl_settings, 
					      enum cvpi_egl_pixel_format_brcm pixel_format_brcm,
					      enum cvpi_egl_settings_change);
#endif
CVPI_BOOL cvpi_egl_surface_functions_check(cvpi_egl_settings);
CVPI_BOOL cvpi_egl_settings_buffer_size_check(EGLint buffer_size);
CVPI_BOOL cvpi_egl_settings_buffer_size(cvpi_egl_settings, EGLint buffer_size);
CVPI_BOOL cvpi_egl_settings_red_size_check(EGLint red_size);
CVPI_BOOL cvpi_egl_settings_red_size(cvpi_egl_settings, EGLint red_size);
CVPI_BOOL cvpi_egl_settings_green_size_check(EGLint green_size);
CVPI_BOOL cvpi_egl_settings_green_size(cvpi_egl_settings, EGLint green_size);
CVPI_BOOL cvpi_egl_settings_blue_size_check(EGLint blue_size);
CVPI_BOOL cvpi_egl_settings_blue_size(cvpi_egl_settings, EGLint blue_size);
CVPI_BOOL cvpi_egl_settings_alpha_size_check(EGLint alpha_size);
CVPI_BOOL cvpi_egl_settings_alpha_size(cvpi_egl_settings, EGLint alpha_size);
CVPI_BOOL cvpi_egl_settings_alpha_mask_size_check(EGLint alpha_size);
CVPI_BOOL cvpi_egl_settings_alpha_mask_size(cvpi_egl_settings, EGLint alpha_size);

CVPI_BOOL cvpi_egl_settings_config_id_check(EGLint id);
CVPI_BOOL cvpi_egl_settings_config_id(cvpi_egl_settings, EGLint id);

CVPI_BOOL cvpi_egl_settings_conformant_check(cvpi_egl_settings);
CVPI_BOOL cvpi_egl_settings_conformant(cvpi_egl_settings,
				       enum cvpi_egl_conformant_bits bit,
				       enum cvpi_egl_settings_change change);

CVPI_BOOL cvpi_egl_settings_depth_size_check(EGLint depth_size);
CVPI_BOOL cvpi_egl_settings_depth_size(cvpi_egl_settings, EGLint depth_size);

CVPI_BOOL cvpi_egl_settings_max_pbuffer_width_check(EGLint width);
CVPI_BOOL cvpi_egl_settings_max_pbuffer_height_check(EGLint height);
CVPI_BOOL cvpi_egl_settings_max_pbuffer_pixels_check(EGLint pixels);

CVPI_BOOL cvpi_egl_settings_match_native_pixmap_check(EGLint match_native_pixmap);

CVPI_BOOL cvpi_egl_settings_max_swap_interval_check(EGLint interval);
CVPI_BOOL cvpi_egl_settings_max_swap_interval(cvpi_egl_settings, EGLint interval);
CVPI_BOOL cvpi_egl_settings_min_swap_interval_check(EGLint interval);
CVPI_BOOL cvpi_egl_settings_min_swap_interval(cvpi_egl_settings, EGLint interval);

CVPI_BOOL cvpi_egl_settings_renderable_type_check(cvpi_egl_settings);
CVPI_BOOL cvpi_egl_settings_renderable_type(cvpi_egl_settings,
					    enum cvpi_egl_renderable_bits bit,
					    enum cvpi_egl_settings_change change);

CVPI_BOOL cvpi_egl_settings_stencil_size_check(EGLint size);
CVPI_BOOL cvpi_egl_settings_stencil_size(cvpi_egl_settings, EGLint size);

CVPI_BOOL cvpi_egl_settings_surface_type_check(cvpi_egl_settings);
CVPI_BOOL cvpi_egl_settings_surface_type(cvpi_egl_settings,
					 enum cvpi_egl_surface_bits bit,
					 enum cvpi_egl_settings_change change);

CVPI_BOOL cvpi_egl_settings_transparent_red_value_check(EGLint red_value);
CVPI_BOOL cvpi_egl_settings_transparent_red_value(cvpi_egl_settings, EGLint red_value);
CVPI_BOOL cvpi_egl_settings_transparent_green_value_check(EGLint green_value);
CVPI_BOOL cvpi_egl_settings_transparent_green_value(cvpi_egl_settings, EGLint green_value);
CVPI_BOOL cvpi_egl_settings_transparent_blue_value_check(EGLint blue_value);
CVPI_BOOL cvpi_egl_settings_transparent_blue_value(cvpi_egl_settings, EGLint blue_value);

/* Given an EGL error number, such as one produced by eglGetError(),
   return a string describing it. */
char* cvpi_egl_error_string(EGLint);

#if HAVE_BCM_HOST == 1
EGLint cvpi_egl_bytes_per_pixel(enum cvpi_egl_pixel_format);
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#else
#pragma message __FILE__ ": CVPI_EGL_CONFIG already defined."
#endif
