#ifndef __egl_h_
#include <EGL/egl.h>
/* functions */
/* eglGetDisplay */

/* variables */
/* EGL_SUCCESS, ... */
#endif

#ifndef __eglplatform_h_
#include <EGL/eglplatform.h>
#endif

#ifndef EGLEXT_BRCM_H
#include <eglext_brcm.h>
#endif

#ifndef _STDIO_H
#include <stdio.h>
#endif
/* functions */
/* printf */
/* fprintf */
#ifndef BCM_HOST_H
#include <bcm_host.h>
#endif
/* functions */
/* bcm_host_init */

/* variables */
/*  */

#ifndef CVPI_EGL_CONFIG
#include "cvpi_egl_config.h"
#endif

#ifndef CVPI_BASE
#include "cvpi_base.h"
#endif

#define CVPI_EGL_VERSION_MAJOR 1
#define CVPI_EGL_VERSION_MINOR 4

/* used for printing error conditions */
/* from section 3.1 in the EGL spec */
char* cvpi_egl_error_string(EGLint error) {
  char* error_str;
  switch(error) {
  case EGL_SUCCESS:
    return " EGL_SUCCESS ";
  case EGL_NOT_INITIALIZED:
    return " EGL_NOT_INITIALIZED ";
  case EGL_BAD_ACCESS:
    return " EGL_BAD_ACCESS ";
  case EGL_BAD_ALLOC:
    return " EGL_BAD_ALLOC ";
  case EGL_BAD_ATTRIBUTE:
    return " EGL_BAD_ATTRIBUTE ";
  case EGL_BAD_CONTEXT:
    return " EGL_BAD_CONTEXT ";
  case EGL_BAD_CONFIG:
    return " EGL_BAD_CONFIG ";
  case EGL_BAD_CURRENT_SURFACE:
    return " EGL_BAD_CURRENT_SURFACE ";
  case EGL_BAD_DISPLAY:
    return " EGL_BAD_DISPLAY ";
  case EGL_BAD_SURFACE:
    return " EGL_BAD_SURFACE ";
  case EGL_BAD_MATCH:
    return " EGL_BAD_MATCH ";
  case EGL_BAD_PARAMETER:
    return " EGL_BAD_PARAMETER ";
  case EGL_BAD_NATIVE_PIXMAP:
    return " EGL_BAD_NATIVE_PIXMAP ";
  case EGL_BAD_NATIVE_WINDOW:
    return " EGL_BAD_NATIVE_WINDOW ";
  case EGL_CONTEXT_LOST:
    return " EGL_CONTEXT_LOST ";
  default:
    sprintf(error_str, " undefined error %d ", error);
    return error_str;
  }
}

/* based on code from egl_brcm_global_image_client.c */
EGLint cvpi_egl_bytes_per_pixel(enum cvpi_egl_pixel_format pixel_format) {
  switch(pixel_format) {
  case EGL_PIXEL_FORMAT_ARGB_8888_BRCM:     return 4;
  case EGL_PIXEL_FORMAT_A_8_BRCM:           return 1;
  case EGL_PIXEL_FORMAT_ARGB_8888_PRE_BRCM: return 4;
  case EGL_PIXEL_FORMAT_XRGB_8888_BRCM:     return 4;
  case EGL_PIXEL_FORMAT_RGB_565_BRCM:       return 2;
  default:                                  return 0; /* invalid */
  }
}

cvpi_egl_settings cvpi_egl_settings_create(void) {
  cvpi_egl_settings egl_settings_p = malloc(sizeof(*egl_settings_p));
#ifdef CVPI_ERROR_CHECK
  if(egl_settings_p == NULL) {
    fprintf(cvpi_log_file, "%s:%d: malloc returned NULL: errno = %d\n", __func__, __LINE__, errno);
    return NULL;
  }
#endif

  egl_settings_p->width = EGL_CONFIG_MAX_WIDTH;
  egl_settings_p->height = EGL_CONFIG_MAX_HEIGHT;
  egl_settings_p->share_context = EGL_NO_CONTEXT;
  egl_settings_p->current_surface_type = cvpi_egl_surface_type_none;
  egl_settings_p->surface_pixmap_create_function = NULL;
  egl_settings_p->surface_window_create_function = NULL;
  egl_settings_p->surface_pixmap_destroy_function = NULL;
  egl_settings_p->surface_window_destroy_function = NULL;
  egl_settings_p->pixel_format = CVPI_EGL_DEFAULT_PIXEL_FORMAT;
  egl_settings_p->pixel_format_brcm = cvpi_egl_pixel_format_brcm_none;
  egl_settings_p->display_id = EGL_DEFAULT_DISPLAY;
  egl_settings_p->buffer_size = EGL_DONT_CARE;
  egl_settings_p->red_size = EGL_DONT_CARE;
  egl_settings_p->green_size = EGL_DONT_CARE;
  egl_settings_p->blue_size = EGL_DONT_CARE;
  egl_settings_p->luminance_size = EGL_DONT_CARE;
  egl_settings_p->alpha_size = EGL_DONT_CARE;
  egl_settings_p->alpha_mask_size = EGL_DONT_CARE;
  egl_settings_p->config_id = EGL_DONT_CARE;
  egl_settings_p->conformant = EGL_DONT_CARE;
  egl_settings_p->depth_size = EGL_DONT_CARE;
  egl_settings_p->level = 0;
  egl_settings_p->max_pbuffer_width = EGL_DONT_CARE;
  egl_settings_p->max_pbuffer_height = EGL_DONT_CARE;
  egl_settings_p->max_pbuffer_pixels = EGL_DONT_CARE;
  egl_settings_p->max_swap_interval = EGL_DONT_CARE;
  egl_settings_p->min_swap_interval = EGL_DONT_CARE;
  egl_settings_p->native_visual_id = EGL_DONT_CARE;
  egl_settings_p->native_visual_type = EGL_DONT_CARE;
  egl_settings_p->sample_buffers = EGL_DONT_CARE;
  egl_settings_p->samples = cvpi_egl_samples_one;
  egl_settings_p->stencil_size = EGL_DONT_CARE;
  egl_settings_p->transparent_red_value = EGL_DONT_CARE;
  egl_settings_p->transparent_green_value = EGL_DONT_CARE;
  egl_settings_p->transparent_blue_value = EGL_DONT_CARE;
  egl_settings_p->bind_to_texture_rgb = EGL_DONT_CARE;
  egl_settings_p->bind_to_texture_rgba = EGL_DONT_CARE;
  egl_settings_p->native_renderable = CVPI_EGL_TRUE;
  egl_settings_p->surface_type = EGL_PBUFFER_BIT|EGL_VG_ALPHA_FORMAT_PRE_BIT|EGL_SWAP_BEHAVIOR_PRESERVED_BIT|EGL_VG_COLORSPACE_LINEAR_BIT;
  egl_settings_p->renderable_type = EGL_OPENVG_BIT;
  egl_settings_p->transparent_type = cvpi_egl_transparent_type_none;
  egl_settings_p->color_buffer_type = cvpi_egl_color_buffer_type_rgb;
  egl_settings_p->config_caveat = cvpi_egl_config_caveat_none;
  egl_settings_p->renderable_api = cvpi_egl_renderable_api_openvg;
  egl_settings_p->pixmap_surface_attrib_list = NULL;
  egl_settings_p->pbuffer_surface_attrib_list = NULL;
  egl_settings_p->window_surface_attrib_list = NULL;
  egl_settings_p->context_attrib_list = NULL;
  egl_settings_p->match_native_pixmap = EGL_DONT_CARE;
  /* section 3.4.1 in 1.4 Spec says that this isn't a valid value for
     EGL_MATCH_NATIVE_PIXMAP, but see comment on line 165 in
     userland/interface/khronos/egl/egl_client_config_cr.c */

  return egl_settings_p;
}

cvpi_egl_instance cvpi_egl_instance_setup(cvpi_egl_settings egl_settings_p) {
  cvpi_egl_instance egl_instance = malloc(sizeof(*egl_instance));
#ifdef CVPI_ERROR_CHECK
  if(egl_instance == NULL) {
    fprintf(cvpi_log_file, "%s:%d: malloc returned NULL: errno = %d\n", __func__, __LINE__, errno);
    return NULL;
  }
#endif  
  egl_instance->egl_settings = egl_settings_p;

  /* tests needed in multiple places */
  /* if changed, also change in cvpi_egl_instance_takedown */
  char pixmap_surface_bool = 
    egl_settings_p->current_surface_type == cvpi_egl_surface_type_pixmap && 
    egl_settings_p->surface_pixmap_create_function != NULL &&
    egl_settings_p->surface_pixmap_destroy_function != NULL;
  char window_surface_bool = 
    egl_settings_p->current_surface_type == cvpi_egl_surface_type_window &&
    egl_settings_p->surface_pixmap_destroy_function != NULL &&
    egl_settings_p->surface_window_destroy_function != NULL;
  char pbuffer_surface_bool = 
    egl_settings_p->current_surface_type == cvpi_egl_surface_type_pbuffer;

  /* Used to capture egl succeed/fail return values */
  EGLBoolean good = EGL_TRUE;

  /* has to be called before any gpu calls are made, why is not documented */
  bcm_host_init();

  /* get the display matching display_id */
  /* Possible values does not appear to be documented and is probably
     implementation specific. */
  /* Broadcom's implementation only supports one value, the default. */
  egl_instance->egl_display = eglGetDisplay(egl_settings_p->display_id);
  if(EGL_NO_DISPLAY == egl_instance->egl_display) {
    fprintf(cvpi_log_file, "%s: eglGetDisplay returned EGL_NO_DISPLAY\n", __func__);
    goto undoBase;
  }

  /* see egl.h header */
  egl_instance->major = CVPI_EGL_VERSION_MAJOR;
  egl_instance->minor = CVPI_EGL_VERSION_MINOR;
  /* initialize the display */
  good = eglInitialize(egl_instance->egl_display, &(egl_instance->major), &(egl_instance->minor));
  if(EGL_TRUE != good) {
    fprintf(cvpi_log_file, "%s: eglInitialize returned %s\n", __func__, cvpi_egl_error_string(good));
    goto undoBase;
  }

  /* set API to EGL_OPENVG_API or EGL_OPENGL_ES_API */
  if(egl_settings_p->renderable_api != cvpi_egl_renderable_api_current) {
    good = eglBindAPI(egl_settings_p->renderable_api);
    if(EGL_TRUE != good) {
      fprintf(cvpi_log_file, "%s: eglBindAPI opengl returned %s\n", __func__, cvpi_egl_error_string(good));
      goto undoInitialize;
    }
  }

  /* create an EGLConfig */
  (egl_instance->attrib_list)[0] = EGL_BUFFER_SIZE;
  (egl_instance->attrib_list)[1] = egl_settings_p->buffer_size;  
  (egl_instance->attrib_list)[2] = EGL_RED_SIZE;
  (egl_instance->attrib_list)[3] = egl_settings_p->red_size;
  (egl_instance->attrib_list)[4] = EGL_GREEN_SIZE;
  (egl_instance->attrib_list)[5] = egl_settings_p->green_size;
  (egl_instance->attrib_list)[6] = EGL_BLUE_SIZE;
  (egl_instance->attrib_list)[7] = egl_settings_p->blue_size;
  (egl_instance->attrib_list)[8] = EGL_LUMINANCE_SIZE;
  (egl_instance->attrib_list)[9] = egl_settings_p->luminance_size;
  (egl_instance->attrib_list)[10] = EGL_ALPHA_SIZE;
  (egl_instance->attrib_list)[11] = egl_settings_p->alpha_size;
  (egl_instance->attrib_list)[12] = EGL_ALPHA_MASK_SIZE;
  (egl_instance->attrib_list)[13] = egl_settings_p->alpha_mask_size;
  (egl_instance->attrib_list)[14] = EGL_CONFIG_ID;
  (egl_instance->attrib_list)[15] = egl_settings_p->config_id;
  (egl_instance->attrib_list)[16] = EGL_DEPTH_SIZE;
  (egl_instance->attrib_list)[17] = egl_settings_p->depth_size;
  (egl_instance->attrib_list)[18] = EGL_LEVEL;
  (egl_instance->attrib_list)[19] = egl_settings_p->level;
  (egl_instance->attrib_list)[20] = EGL_MAX_PBUFFER_WIDTH;
  (egl_instance->attrib_list)[21] = egl_settings_p->max_pbuffer_width;
  (egl_instance->attrib_list)[22] = EGL_MAX_PBUFFER_HEIGHT;
  (egl_instance->attrib_list)[23] = egl_settings_p->max_pbuffer_height;
  (egl_instance->attrib_list)[24] = EGL_MAX_PBUFFER_PIXELS;
  (egl_instance->attrib_list)[25] = egl_settings_p->max_pbuffer_pixels;
  (egl_instance->attrib_list)[26] = EGL_MAX_SWAP_INTERVAL;
  (egl_instance->attrib_list)[27] = egl_settings_p->max_swap_interval;
  (egl_instance->attrib_list)[28] = EGL_MIN_SWAP_INTERVAL;
  (egl_instance->attrib_list)[29] = egl_settings_p->min_swap_interval;
  (egl_instance->attrib_list)[30] = EGL_NATIVE_VISUAL_ID;
  (egl_instance->attrib_list)[31] = egl_settings_p->native_visual_id;
  (egl_instance->attrib_list)[32] = EGL_NATIVE_VISUAL_TYPE;
  (egl_instance->attrib_list)[33] = egl_settings_p->native_visual_type;
  (egl_instance->attrib_list)[34] = EGL_SAMPLE_BUFFERS;
  (egl_instance->attrib_list)[35] = egl_settings_p->sample_buffers;
  (egl_instance->attrib_list)[36] = EGL_SAMPLES;
  (egl_instance->attrib_list)[37] = egl_settings_p->samples;
  (egl_instance->attrib_list)[38] = EGL_STENCIL_SIZE;
  (egl_instance->attrib_list)[39] = egl_settings_p->stencil_size;
  (egl_instance->attrib_list)[40] = EGL_TRANSPARENT_RED_VALUE;
  (egl_instance->attrib_list)[41] = egl_settings_p->transparent_red_value;
  (egl_instance->attrib_list)[42] = EGL_TRANSPARENT_GREEN_VALUE;
  (egl_instance->attrib_list)[43] = egl_settings_p->transparent_green_value;
  (egl_instance->attrib_list)[44] = EGL_TRANSPARENT_BLUE_VALUE;
  (egl_instance->attrib_list)[45] = egl_settings_p->transparent_blue_value;
  (egl_instance->attrib_list)[46] = EGL_BIND_TO_TEXTURE_RGB;
  (egl_instance->attrib_list)[47] = egl_settings_p->bind_to_texture_rgb;
  (egl_instance->attrib_list)[48] = EGL_BIND_TO_TEXTURE_RGBA;
  (egl_instance->attrib_list)[49] = egl_settings_p->bind_to_texture_rgba;
  (egl_instance->attrib_list)[50] = EGL_NATIVE_RENDERABLE;
  (egl_instance->attrib_list)[51] = egl_settings_p->native_renderable;
  (egl_instance->attrib_list)[52] = EGL_SURFACE_TYPE;
  (egl_instance->attrib_list)[53] = egl_settings_p->surface_type;
  (egl_instance->attrib_list)[54] = EGL_RENDERABLE_TYPE;
  (egl_instance->attrib_list)[55] = egl_settings_p->renderable_type;
  (egl_instance->attrib_list)[56] = EGL_TRANSPARENT_TYPE;
  (egl_instance->attrib_list)[57] = egl_settings_p->transparent_type;
  (egl_instance->attrib_list)[58] = EGL_COLOR_BUFFER_TYPE;
  (egl_instance->attrib_list)[59] = egl_settings_p->color_buffer_type;
  (egl_instance->attrib_list)[60] = EGL_CONFIG_CAVEAT;
  (egl_instance->attrib_list)[61] = egl_settings_p->config_caveat;
  (egl_instance->attrib_list)[62] = EGL_CONFORMANT;
  (egl_instance->attrib_list)[63] = egl_settings_p->conformant;
  (egl_instance->attrib_list)[64] = EGL_MATCH_NATIVE_PIXMAP;
  (egl_instance->attrib_list)[65] = egl_settings_p->match_native_pixmap;
  (egl_instance->attrib_list)[66] = EGL_NONE;

  /* get the total number of configs matching attrib_list */
  EGLint number_configs = 0;
  good = eglChooseConfig(egl_instance->egl_display, egl_instance->attrib_list, NULL, 0, &(number_configs));
  if(1 < number_configs) {
    fprintf(cvpi_log_file, "%s: eglChooseConfig number of configurations is %d\n", __func__, number_configs);
  }

  /* TODO: when this error is raised, it currently segfaults. */
  if(0 == number_configs || EGL_TRUE != good) {
    if(!number_configs) {
      fprintf(cvpi_log_file, "%s: first call to eglChooseConfig: number of configurations is zero\n", __func__);
    }
    fprintf(cvpi_log_file, "%s: first call to eglChooseConfig returned %s\n", __func__, cvpi_egl_error_string(good));
    goto undoInitialize;
  }
  egl_instance->matching_configs = (EGLConfig*)malloc(number_configs * sizeof(egl_instance->matching_configs));
#ifdef CVPI_ERROR_CHECK
  if(egl_instance->matching_configs == NULL) {
    fprintf(cvpi_log_file, "%s:%d: malloc returned NULL: errno = %d\n", __func__, __LINE__, errno);
    goto undoInitialize;
  }
#endif
  good = eglChooseConfig(egl_instance->egl_display, egl_instance->attrib_list, egl_instance->matching_configs, number_configs, &(number_configs));

  if(EGL_TRUE != good) {
    fprintf(cvpi_log_file, "%s: second call to eglChooseConfig returned %s\n", __func__, cvpi_egl_error_string(good));
    goto undoChooseConfig;
  }
  
  /* PIXMAP surface */
  if(pixmap_surface_bool) {
    EGLNativePixmapType pixmap = (egl_settings_p->surface_pixmap_create_function)(egl_instance);

    if(pixmap == NULL) {
      fprintf(cvpi_log_file, "%s: EGLNativePixmapType function returned NULL pointer.\n", __func__);
      goto undoChooseConfig;
    }

    /* create surface */
    /* not all matching_configs truely match so loop untill a matching one is found, bug in eglChooseConfig */
    unsigned int i = 0;
    do {
      egl_instance->egl_surface = eglCreatePixmapSurface(egl_instance->egl_display, (egl_instance->matching_configs)[i], pixmap, egl_settings_p->pixmap_surface_attrib_list);
      ++i;
    } while(egl_instance->egl_surface == EGL_NO_SURFACE && i < number_configs);

    /* see line 998 in egl_client.c */
    if(egl_instance->egl_surface == EGL_NO_SURFACE) {
      good = eglGetError();
      fprintf(cvpi_log_file, "%s: pixmap surface creation failed %s %x.\n", __func__, cvpi_egl_error_string(good), good);
      goto undoPixmap;
    }
    egl_instance->matching_config_index = i-1;
  } 
  /* PBUFFER surface */
  else if(pbuffer_surface_bool) {
    unsigned int i = 0;
    do {
      egl_instance->egl_surface = eglCreatePbufferSurface(egl_instance->egl_display, 
							  (egl_instance->matching_configs)[i], 
							  egl_settings_p->pbuffer_surface_attrib_list);
      ++i;
    } while(egl_instance->egl_surface == EGL_NO_SURFACE && i < number_configs);
    if(egl_instance->egl_surface == EGL_NO_SURFACE) {
      fprintf(cvpi_log_file, "%s: pbuffer surface creation failed %s.\n", __func__, cvpi_egl_error_string(eglGetError()));
      goto undoPixmap;
    }
    egl_instance->matching_config_index = i-1;
  } 
  /* WINDOW surface */
  else if(window_surface_bool) {
    /* client_egl_get_window not documented */
    EGLNativeWindowType win = (egl_settings_p->surface_window_create_function)(egl_instance);
    if(win == NULL) {
      fprintf(cvpi_log_file, "%s: EGLNativeWindowType returned NULL.\n", __func__);
      goto undoPixmap;
    }
    
    unsigned int i = 0;
    do {
      egl_instance->egl_surface = eglCreateWindowSurface(egl_instance->egl_display, 
							 (egl_instance->matching_configs)[i], 
							 win,
							 egl_settings_p->window_surface_attrib_list);
      ++i;
    } while(egl_instance->egl_surface == EGL_NO_SURFACE && i < number_configs);
    if(egl_instance->egl_surface == EGL_NO_SURFACE) {
      fprintf(cvpi_log_file, "%s: pbuffer surface creation failed %s.\n", __func__, cvpi_egl_error_string(eglGetError()));
      goto undoWindow;
    }
    egl_instance->matching_config_index = i-1;
  } else { 
    fprintf(cvpi_log_file, "%s: No surface type selected: %u\n", __func__, egl_settings_p->surface_type);
  }

  egl_instance->egl_context = eglCreateContext(egl_instance->egl_display, egl_instance->matching_configs[egl_instance->matching_config_index], egl_settings_p->share_context, egl_settings_p->context_attrib_list);
  if(EGL_NO_CONTEXT == egl_instance->egl_context) {
    fprintf(cvpi_log_file, "%s: eglCreateContext returned %s\n", __func__, cvpi_egl_error_string(eglGetError()));
    goto undoCreateSurface;
  }

  /* Only one surface type per rendering context is allowed.  Read and
     draw parameters for eglMakeCurrent are the same because OpenVG
     requires it and it simplifies the interface. */
  switch(egl_settings_p->current_surface_type) {
  case cvpi_egl_surface_type_pixmap:
  case cvpi_egl_surface_type_pbuffer:
  case cvpi_egl_surface_type_window:
    good = eglMakeCurrent(egl_instance->egl_display,
			  egl_instance->egl_surface, egl_instance->egl_surface,
			  egl_instance->egl_context);
    break;
  case cvpi_egl_surface_type_none:
  default:
    /* do nothing */
    break;
  }
  if(EGL_FALSE == good) {
    fprintf(cvpi_log_file, "%s: eglMakeCurrent returned false: %s\n", __func__, cvpi_egl_error_string(eglGetError()));
    goto undoCreateContext;
  }
  return egl_instance;

  /* If there is an error in setup, then take down */

 undoCreateContext:
  good = eglDestroyContext(egl_instance->egl_display, egl_instance->egl_context);
  if(EGL_TRUE != good) {
    fprintf(cvpi_log_file, "%s: eglDestroyContext returned %s\n", __func__, cvpi_egl_error_string(good));
  }

 undoCreateSurface:
  if(window_surface_bool) {
    good = eglDestroySurface(egl_instance->egl_display, egl_instance->egl_surface);
    if(EGL_FALSE == good) {
      fprintf(cvpi_log_file, "%s: eglDestroySurface Window returned false\n", __func__);
    }
  }

  if(pixmap_surface_bool) {
    good = eglDestroySurface(egl_instance->egl_display, egl_instance->egl_surface);
    if(EGL_FALSE == good) {
      fprintf(cvpi_log_file, "%s: eglDestroySurface Pixmap returned false\n", __func__);
    }
  }
  if(pbuffer_surface_bool) {
    good = eglDestroySurface(egl_instance->egl_display, egl_instance->egl_surface);
    if(EGL_FALSE == good) {
      fprintf(cvpi_log_file, "%s: eglDestroySurface Pbuffer returned false\n", __func__);
    }
  }
 undoWindow:
  if(window_surface_bool) {
    (egl_settings_p->surface_window_destroy_function)(egl_instance);
  }
 undoPixmap:
  if(pixmap_surface_bool) {
    (egl_settings_p->surface_pixmap_destroy_function)(egl_instance);
  }

 undoChooseConfig:
  free(egl_instance->matching_configs);
 undoInitialize:
  good = eglTerminate(egl_instance->egl_display);
  if(EGL_TRUE != good) {
    fprintf(cvpi_log_file, "%s: eglTerminate returned %s\n", __func__, cvpi_egl_error_string(good));
  }
 undoBase:

  free(egl_instance);
  //bcm_host_deinit();

  return NULL;
}

/* takedown EGL */
void cvpi_egl_instance_takedown(cvpi_egl_instance deletable) {
  if(deletable == NULL) {
    return;
  }
  cvpi_egl_settings egl_settings_p = deletable->egl_settings;
  EGLBoolean good = EGL_TRUE;
  /* undo eglInitialize */
  good = eglDestroyContext(deletable->egl_display, deletable->egl_context);
  if(EGL_TRUE != good) {
    fprintf(cvpi_log_file, "%s: eglDestroyContext returned %s\n", __func__, cvpi_egl_error_string(good));
  }

  /* The following tests for each of the three if statements must be
     equivalent to the three tests created at the beginning of
     cvpi_egl_instance_setup.
  */
  if(egl_settings_p->current_surface_type == cvpi_egl_surface_type_window &&
     egl_settings_p->surface_pixmap_destroy_function != NULL &&
     egl_settings_p->surface_window_destroy_function != NULL) {
    good = eglDestroySurface(deletable->egl_display, deletable->egl_surface);
    if(EGL_FALSE == good) {
      fprintf(cvpi_log_file, "%s: eglDestroySurface returned false\n", __func__);
    }
    (egl_settings_p->surface_window_destroy_function)(deletable);
  }

  if(egl_settings_p->current_surface_type == cvpi_egl_surface_type_pixmap && 
     egl_settings_p->surface_pixmap_create_function != NULL &&
     egl_settings_p->surface_pixmap_destroy_function != NULL) {
    good = eglDestroySurface(deletable->egl_display, deletable->egl_surface);
    if(EGL_FALSE == good) {
      fprintf(cvpi_log_file, "%s: eglDestroySurface returned false\n", __func__);
    }
    (egl_settings_p->surface_pixmap_destroy_function)(deletable);
  }
  if(egl_settings_p->current_surface_type == cvpi_egl_surface_type_pbuffer) {
    good = eglDestroySurface(deletable->egl_display, deletable->egl_surface);
    if(EGL_FALSE == good) {
      fprintf(cvpi_log_file, "%s: eglDestroySurface Pbuffer returned false\n", __func__);
    }
  }

  free(deletable->matching_configs);

  good = eglTerminate(deletable->egl_display);
  if(EGL_TRUE != good) {
    fprintf(cvpi_log_file, "%s: eglTerminate returned %s\n", __func__, cvpi_egl_error_string(good));
  }
  free(deletable);
  //bcm_host_deinit();		/* does not do anything /userland/.../bcm_host.c */
}

/* Turn on or off a BIT in BITFIELD depending on the CHANGE type. */
#define CVPI_CHANGE_TYPE(change, bitfield, bit) do{if((change) == cvpi_egl_settings_add) {(bitfield) |= (bit);} else {(bitfield) &= ~(bit);}}while(0)

/* CVPI_TRUE_TEST defined cvpi_base.h */
CVPI_BOOL cvpi_egl_settings_check(cvpi_egl_settings egl_settings_p) {
  int check = 1;
  check &= CVPI_TRUE_TEST(cvpi_egl_settings_width_check(egl_settings_p->width));
  check &= CVPI_TRUE_TEST(cvpi_egl_settings_height_check(egl_settings_p->width));
  check &= CVPI_TRUE_TEST(cvpi_egl_settings_pixel_format_brcm_check(egl_settings_p));
  check &= CVPI_TRUE_TEST(cvpi_egl_surface_functions_check(egl_settings_p));
  check &= CVPI_TRUE_TEST(cvpi_egl_settings_buffer_size_check(egl_settings_p->buffer_size));
  check &= CVPI_TRUE_TEST(cvpi_egl_settings_red_size_check(egl_settings_p->red_size));
  check &= CVPI_TRUE_TEST(cvpi_egl_settings_green_size_check(egl_settings_p->green_size));
  check &= CVPI_TRUE_TEST(cvpi_egl_settings_blue_size_check(egl_settings_p->blue_size));
  check &= CVPI_TRUE_TEST(cvpi_egl_settings_alpha_size_check(egl_settings_p->alpha_size));
  check &= CVPI_TRUE_TEST(cvpi_egl_settings_alpha_mask_size_check(egl_settings_p->alpha_mask_size));
  check &= CVPI_TRUE_TEST(cvpi_egl_settings_config_id_check(egl_settings_p->config_id));
  check &= CVPI_TRUE_TEST(cvpi_egl_settings_conformant_check(egl_settings_p));
  check &= CVPI_TRUE_TEST(cvpi_egl_settings_depth_size_check(egl_settings_p->depth_size));
  check &= CVPI_TRUE_TEST(cvpi_egl_settings_max_pbuffer_width_check(egl_settings_p->max_pbuffer_width));
  check &= CVPI_TRUE_TEST(cvpi_egl_settings_max_pbuffer_height_check(egl_settings_p->max_pbuffer_height));
  check &= CVPI_TRUE_TEST(cvpi_egl_settings_max_pbuffer_pixels_check(egl_settings_p->max_pbuffer_pixels));
  check &= CVPI_TRUE_TEST(cvpi_egl_settings_match_native_pixmap_check(egl_settings_p->match_native_pixmap));
  check &= CVPI_TRUE_TEST(cvpi_egl_settings_max_swap_interval_check(egl_settings_p->max_swap_interval));
  check &= CVPI_TRUE_TEST(cvpi_egl_settings_min_swap_interval_check(egl_settings_p->min_swap_interval));
  check &= CVPI_TRUE_TEST(cvpi_egl_settings_renderable_type_check(egl_settings_p));
  check &= CVPI_TRUE_TEST(cvpi_egl_settings_stencil_size_check(egl_settings_p->stencil_size));
  check &= CVPI_TRUE_TEST(cvpi_egl_settings_surface_type_check(egl_settings_p));
  check &= CVPI_TRUE_TEST(cvpi_egl_settings_transparent_red_value_check(egl_settings_p->transparent_red_value));
  check &= CVPI_TRUE_TEST(cvpi_egl_settings_transparent_green_value_check(egl_settings_p->transparent_green_value));
  check &= CVPI_TRUE_TEST(cvpi_egl_settings_transparent_blue_value_check(egl_settings_p->transparent_blue_value));
  return check;
}

CVPI_BOOL cvpi_egl_settings_width_check(unsigned long width) {
  if(width > EGL_CONFIG_MAX_WIDTH) {
    fprintf(cvpi_log_file, "%s: width must be <= %d.\n", __func__, EGL_CONFIG_MAX_WIDTH);
    return CVPI_FALSE;
  } else {
    return CVPI_TRUE;
  }
}

CVPI_BOOL cvpi_egl_settings_width(cvpi_egl_settings egl_settings_p, unsigned long width) {
  if(CVPI_TRUE_TEST(cvpi_egl_settings_width_check(width))) {
    egl_settings_p->width = width;
    return CVPI_TRUE;
  } else {
    return CVPI_FALSE;
  }
}

CVPI_BOOL cvpi_egl_settings_height_check(unsigned long height) {
  if(height > EGL_CONFIG_MAX_HEIGHT) {
    fprintf(cvpi_log_file, "%s: height must be <= %d.\n", __func__, EGL_CONFIG_MAX_HEIGHT);
    return CVPI_FALSE;
  } else {
    return CVPI_TRUE;
  }
}
CVPI_BOOL cvpi_egl_settings_height(cvpi_egl_settings egl_settings_p, unsigned long height) {
  if(CVPI_TRUE_TEST(cvpi_egl_settings_height_check(height))) {
    egl_settings_p->height = height;
    return CVPI_TRUE;
  } else {
    return CVPI_FALSE;
  }
}

CVPI_BOOL cvpi_egl_settings_pixel_format_brcm_check(cvpi_egl_settings cvpi_egl_settings_p) {
  EGLint options = cvpi_egl_pixel_format_gles2_texture_brcm|
    cvpi_egl_pixel_format_gles_texture_brcm |
    cvpi_egl_pixel_format_render_gles2_brcm |
    cvpi_egl_pixel_format_render_gles_brcm  |
    cvpi_egl_pixel_format_render_gl_brcm    |
    cvpi_egl_pixel_format_render_mask_brcm  |
    cvpi_egl_pixel_format_render_vg_brcm    |
    cvpi_egl_pixel_format_texture_mask_brcm |
    cvpi_egl_pixel_format_usage_mask_brcm   |
    cvpi_egl_pixel_format_vg_image_brcm     ;
  
  if(cvpi_egl_settings_p->pixel_format_brcm & ~options) {
    fprintf(cvpi_log_file, "%s Illegal option.\n", __func__);
    return CVPI_FALSE;
  } else {
    return CVPI_TRUE;
  }
}
CVPI_BOOL cvpi_egl_settings_pixel_format_brcm(cvpi_egl_settings cvpi_egl_settings_p, 
					      enum cvpi_egl_pixel_format_brcm pixel_format_brcm,
					      enum cvpi_egl_settings_change change) {
  if(pixel_format_brcm == cvpi_egl_pixel_format_brcm_none) {
    cvpi_egl_settings_p->pixel_format_brcm = cvpi_egl_pixel_format_brcm_none;
    return CVPI_TRUE;
  } else {
    EGLint old_value = cvpi_egl_settings_p->pixel_format_brcm;
    CVPI_CHANGE_TYPE(change, cvpi_egl_settings_p->pixel_format_brcm, pixel_format_brcm);
    if(CVPI_TRUE_TEST(cvpi_egl_settings_pixel_format_brcm_check(cvpi_egl_settings_p))) {
      return CVPI_TRUE;
    } else {
      cvpi_egl_settings_p->pixel_format_brcm = old_value;
      return CVPI_FALSE;
    }
  }
}

CVPI_BOOL cvpi_egl_surface_functions_check(cvpi_egl_settings cvpi_egl_settings_p) {
  /* xnor(create, destroy) & xnor(create, destroy) */
  char p_c = cvpi_egl_settings_p->surface_pixmap_create_function == NULL ? 0 : 1;
  char p_d = cvpi_egl_settings_p->surface_pixmap_destroy_function == NULL ? 0 : 1;
  char w_c = cvpi_egl_settings_p->surface_window_create_function == NULL ? 0 : 1;
  char w_d = cvpi_egl_settings_p->surface_window_destroy_function == NULL ? 0 : 1;
  /* xnor = ~xor */
  if(~(p_c ^ p_d) & ~(w_c ^ w_d)) {
    return CVPI_TRUE;
  } else {
    fprintf(cvpi_log_file, "%s: A creation or destroy function exists without its complement.\n", __func__);
    return CVPI_FALSE;
  }
}

CVPI_BOOL cvpi_egl_settings_buffer_size_check(EGLint buffer_size) {
  if(buffer_size >= 0 || buffer_size == EGL_DONT_CARE) {
    return CVPI_TRUE;
  } else {
    fprintf(cvpi_log_file, "%s: buffer_size must be non-negative or EGL_DONT_CARE.\n", __func__);
    return CVPI_FALSE;
  }
}
CVPI_BOOL cvpi_egl_settings_buffer_size(cvpi_egl_settings egl_settings_p, EGLint buffer_size) {
  if(CVPI_TRUE_TEST(cvpi_egl_settings_buffer_size_check(buffer_size))) {
    egl_settings_p->buffer_size = buffer_size;
    return CVPI_TRUE;
  } else {
    return CVPI_FALSE;
  }
}
CVPI_BOOL cvpi_egl_settings_red_size_check(EGLint red_size) {
  if(red_size >= 0 || red_size == EGL_DONT_CARE) {
    return CVPI_TRUE;
  } else {
    fprintf(cvpi_log_file, "%s: red_size must be non-negative or EGL_DONT_CARE.\n", __func__);
    return CVPI_FALSE;
  }
}
CVPI_BOOL cvpi_egl_settings_red_size(cvpi_egl_settings egl_settings_p, EGLint red_size) {
  if(CVPI_TRUE_TEST(cvpi_egl_settings_red_size_check(red_size))) {
    egl_settings_p->red_size = red_size;
    return CVPI_TRUE;
  } else {
    return CVPI_FALSE;
  }
}
CVPI_BOOL cvpi_egl_settings_green_size_check(EGLint green_size) {
  if(green_size >= 0 || green_size == EGL_DONT_CARE) {
    return CVPI_TRUE;
  } else {
    fprintf(cvpi_log_file, "%s: green_size must be non-negative or EGL_DONT_CARE.\n", __func__);
    return CVPI_FALSE;
  }
}
CVPI_BOOL cvpi_egl_settings_green_size(cvpi_egl_settings egl_settings_p, EGLint green_size) {
  if(CVPI_TRUE_TEST(cvpi_egl_settings_green_size_check(green_size))) {
    egl_settings_p->green_size = green_size;
    return CVPI_TRUE;
  } else {
    return CVPI_FALSE;
  }
}
CVPI_BOOL cvpi_egl_settings_blue_size_check(EGLint blue_size) {
  if(blue_size >= 0 || blue_size == EGL_DONT_CARE) {
    return CVPI_TRUE;
  } else {
    fprintf(cvpi_log_file, "%s: blue_size must be non-negative or EGL_DONT_CARE.\n", __func__);
    return CVPI_FALSE;
  }
}
CVPI_BOOL cvpi_egl_settings_blue_size(cvpi_egl_settings egl_settings_p, EGLint blue_size) {
  if(CVPI_TRUE_TEST(cvpi_egl_settings_blue_size_check(blue_size))) {
    egl_settings_p->blue_size = blue_size;
    return CVPI_TRUE;
  } else {
    return CVPI_FALSE;
  }
}
CVPI_BOOL cvpi_egl_settings_alpha_size_check(EGLint alpha_size) {
  if(alpha_size >= 0 || alpha_size == EGL_DONT_CARE) {
    return CVPI_TRUE;
  } else {
    fprintf(cvpi_log_file, "%s: alpha_size must be non-negative or EGL_DONT_CARE.\n", __func__);
    return CVPI_FALSE;
  }
}
CVPI_BOOL cvpi_egl_settings_alpha_size(cvpi_egl_settings egl_settings_p, EGLint alpha_size) {
  if(CVPI_TRUE_TEST(cvpi_egl_settings_alpha_size_check(alpha_size))) {
    egl_settings_p->alpha_size = alpha_size;
    return CVPI_TRUE;
  } else {
    return CVPI_FALSE;
  }
}
CVPI_BOOL cvpi_egl_settings_alpha_mask_size_check(EGLint alpha_size) {
  if(alpha_size >= 0 || alpha_size == EGL_DONT_CARE) {
    return CVPI_TRUE;
  } else {
    fprintf(cvpi_log_file, "%s: alpha_mask_size must be non-negative or EGL_DONT_CARE.\n", __func__);
    return CVPI_FALSE;
  }
}
CVPI_BOOL cvpi_egl_settings_alpha_mask_size(cvpi_egl_settings egl_settings_p, EGLint alpha_size) {
  if(CVPI_TRUE_TEST(cvpi_egl_settings_alpha_mask_size_check(alpha_size))) {
    egl_settings_p->alpha_mask_size = alpha_size;
    return CVPI_TRUE;
  } else {
    return CVPI_FALSE;
  }
}

CVPI_BOOL cvpi_egl_settings_config_id_check(EGLint id) {
  if(id == EGL_DONT_CARE || id >= 1) {
    return CVPI_TRUE;
  } else {
    fprintf(cvpi_log_file, "%s: Invalid EGL_CONFIG_ID\n", __func__);
    return CVPI_FALSE;
  }
}
CVPI_BOOL cvpi_egl_settings_config_id(cvpi_egl_settings egl_settings_p, EGLint id) {
  if(CVPI_TRUE_TEST(cvpi_egl_settings_config_id_check(id))) {
    egl_settings_p->config_id = id;
    return CVPI_TRUE;
  } else {
    return CVPI_FALSE;
  }
}

CVPI_BOOL cvpi_egl_settings_conformant_check(cvpi_egl_settings egl_settings_p) {
  if (egl_settings_p->conformant != EGL_DONT_CARE && 
      (egl_settings_p->conformant & ~(EGL_OPENGL_BIT|EGL_OPENGL_ES_BIT|EGL_OPENGL_ES2_BIT|EGL_OPENVG_BIT))) {
    fprintf(cvpi_log_file, "%s: Change makes EGL_CONFORMANT value invalid.\n", __func__);
    return CVPI_FALSE;
  } else {
    return CVPI_TRUE;
  }
}
CVPI_BOOL cvpi_egl_settings_conformant(cvpi_egl_settings egl_settings_p, 
				       enum cvpi_egl_conformant_bits bit,
				       enum cvpi_egl_settings_change change) {
  EGLint old_value = egl_settings_p->conformant;
  if(bit != cvpi_egl_conformant_bit_dc) {
    CVPI_CHANGE_TYPE(change, egl_settings_p->conformant, bit);
  } else {
    egl_settings_p->conformant = cvpi_egl_conformant_bit_dc;
  }

  if (CVPI_FALSE_TEST(cvpi_egl_settings_conformant_check(egl_settings_p))) {
    egl_settings_p->conformant = old_value;
    return CVPI_FALSE;
  } else {
    return CVPI_TRUE;
  }
}

CVPI_BOOL cvpi_egl_settings_depth_size_check(EGLint depth_size) {
  if (depth_size == EGL_DONT_CARE || depth_size >= 0) {
    return CVPI_TRUE;
  } else {
    fprintf(cvpi_log_file, "%s: Invalid EGL_DEPTH_SIZE.\n", __func__);
    return CVPI_FALSE;
  }
}
CVPI_BOOL cvpi_egl_settings_depth_size(cvpi_egl_settings egl_settings_p, EGLint depth_size) {
  if(CVPI_TRUE_TEST(cvpi_egl_settings_depth_size_check(depth_size))) {
    egl_settings_p->depth_size = depth_size;
    return CVPI_TRUE;
  } else {
    return CVPI_FALSE;
  }
}

/* EGL spec does not restrict values for EGL_MAX_PBUFFER_WIDTH,
   EGL_MAX_PBUFFER_HEIGHT, and EGL_MAX_PBUFFER_PIXELS, and no
   restrictions appear in egl_client_config_cr.c, but negative values
   seem nonsensical */
CVPI_BOOL cvpi_egl_settings_max_pbuffer_width_check(EGLint width) {
  if (width == EGL_DONT_CARE || width >= 0) {
    return CVPI_TRUE;
  } else {
    fprintf(cvpi_log_file, "%s: Possibly invalid EGL_MAX_PBUFFER_WIDTH.\n", __func__);
    return CVPI_FALSE;
  }
}
CVPI_BOOL cvpi_egl_settings_max_pbuffer_height_check(EGLint height) {
  if (height == EGL_DONT_CARE || height >= 0) {
    return CVPI_TRUE;
  } else {
    fprintf(cvpi_log_file, "%s: Possibly invalid EGL_MAX_PBUFFER_HEIGHT.\n", __func__);
    return CVPI_FALSE;
  }
}
CVPI_BOOL cvpi_egl_settings_max_pbuffer_pixels_check(EGLint pixels) {
  if (pixels == EGL_DONT_CARE || pixels >= 0) {
    return CVPI_TRUE;
  } else {
    fprintf(cvpi_log_file, "%s: Possibly invalid EGL_MAX_PBUFFER_PIXELS.\n", __func__);
    return CVPI_FALSE;
  }
}

CVPI_BOOL cvpi_egl_settings_match_native_pixmap_check(EGLint match_native_pixmap) {
  if(match_native_pixmap == EGL_NONE) {
    fprintf(cvpi_log_file, "%s: EGL_NONE not supported. \nSee egl_client_config_cr.c\n", __func__);
    return CVPI_FALSE;
  } else {
    return CVPI_TRUE;
  }
}

CVPI_BOOL cvpi_egl_settings_max_swap_interval_check(EGLint interval) {
  if(interval == EGL_DONT_CARE || interval >= 0) {
    return CVPI_TRUE;
  } else {
    fprintf(cvpi_log_file, "%s: Invalid EGL_MAX_SWAP_INTERVAL\n", __func__);
    return CVPI_FALSE;
  }
}
CVPI_BOOL cvpi_egl_settings_max_swap_interval(cvpi_egl_settings egl_settings_p, EGLint interval) {
  if(CVPI_TRUE_TEST(cvpi_egl_settings_max_swap_interval_check(interval))) {
    egl_settings_p->max_swap_interval = interval;
    return CVPI_TRUE;
  } else {
    return CVPI_FALSE;
  }
}
CVPI_BOOL cvpi_egl_settings_min_swap_interval_check(EGLint interval) {
  if(interval == EGL_DONT_CARE || interval >= 0) {
    return CVPI_TRUE;
  } else {
    fprintf(cvpi_log_file, "%s: Invalid EGL_MIN_SWAP_INTERVAL\n", __func__);
    return CVPI_FALSE;
  }
}
CVPI_BOOL cvpi_egl_settings_min_swap_interval(cvpi_egl_settings egl_settings_p, EGLint interval) {
  if(CVPI_TRUE_TEST(cvpi_egl_settings_min_swap_interval_check(interval))) {
    egl_settings_p->min_swap_interval = interval;
    return CVPI_TRUE;
  } else {
    return CVPI_FALSE;
  }
}
CVPI_BOOL cvpi_egl_settings_renderable_type_check(cvpi_egl_settings egl_settings_p) {
  if (egl_settings_p->renderable_type == EGL_DONT_CARE || 
      (egl_settings_p->renderable_type & (EGL_OPENGL_BIT|EGL_OPENGL_ES_BIT|EGL_OPENGL_ES2_BIT|EGL_OPENVG_BIT))) {

    /* check that the pixel format bits make sense for the renderable type */
    if(egl_settings_p->renderable_type & EGL_OPENGL_BIT && 
       !(egl_settings_p->pixel_format_brcm & EGL_PIXEL_FORMAT_RENDER_GL_BRCM)) {
      fprintf(cvpi_log_file,"%s: Renderable type opengl may require EGL_PIXEL_FORMAT_RENDER_GL_BRCM.\n",__func__);
    }
    if(egl_settings_p->renderable_type & EGL_OPENGL_ES_BIT &&
       !(egl_settings_p->pixel_format_brcm & EGL_PIXEL_FORMAT_RENDER_GLES_BRCM &&
	 egl_settings_p->pixel_format_brcm & EGL_PIXEL_FORMAT_GLES_TEXTURE_BRCM)) {
      fprintf(cvpi_log_file,"%s: Renderable type opengl_es may require EGL_PIXEL_FORMAT_RENDER_GLES_BRCM and EGL_PIXEL_FORMAT_GLES_TEXTURE_BRCM.\n",__func__);
    }
    if(egl_settings_p->renderable_type & EGL_OPENGL_ES2_BIT &&
       !(egl_settings_p->pixel_format_brcm & EGL_PIXEL_FORMAT_RENDER_GLES2_BRCM &&
	 egl_settings_p->pixel_format_brcm & EGL_PIXEL_FORMAT_GLES2_TEXTURE_BRCM)) {
      fprintf(cvpi_log_file,"%s: Renderable type opengl_es2 may require EGL_PIXEL_FORMAT_RENDER_GLES2_BRCM and EGL_PIXEL_FORMAT_GLES2_TEXTURE_BRCM.\n",__func__);
    }
    if(egl_settings_p->renderable_type & EGL_OPENVG_BIT &&
       !(egl_settings_p->pixel_format_brcm & EGL_PIXEL_FORMAT_RENDER_VG_BRCM &&
	 egl_settings_p->pixel_format_brcm & EGL_PIXEL_FORMAT_VG_IMAGE_BRCM)) {
      fprintf(cvpi_log_file,"%s: Renderable type openvg may require EGL_PIXEL_FORMAT_RENDER_VG_BRCM and EGL_PIXEL_FORMAT_VG_IMAGE_BRCM.\n",__func__);
    }

    return CVPI_TRUE;
  } else{
    fprintf(cvpi_log_file, "%s: Invalid EGL_RENDERABLE_TYPE.\n", __func__);
    return CVPI_FALSE;
  }
}
CVPI_BOOL cvpi_egl_settings_renderable_type(cvpi_egl_settings egl_settings_p, 
					    enum cvpi_egl_renderable_bits bit,
					    enum cvpi_egl_settings_change change) {
  EGLint old_type = egl_settings_p->renderable_type;
  CVPI_CHANGE_TYPE(change, egl_settings_p->renderable_type, bit);
  if(CVPI_TRUE_TEST(cvpi_egl_settings_renderable_type_check(egl_settings_p))) {
    return CVPI_TRUE;
  } else {
    egl_settings_p->renderable_type = old_type;
    return CVPI_FALSE;
  } 
}

CVPI_BOOL cvpi_egl_settings_stencil_size_check(EGLint size) {
  if(size == EGL_DONT_CARE || size >= 0) {
    return CVPI_TRUE;
  } else {
    fprintf(cvpi_log_file, "%s: Invalid EGL_STENCIL_SIZE.\n", __func__);
    return CVPI_FALSE;
  }
}
CVPI_BOOL cvpi_egl_settings_stencil_size(cvpi_egl_settings cvpi_egl_settings_p, EGLint size) {
  if(CVPI_TRUE_TEST(cvpi_egl_settings_stencil_size_check(size))) {
    cvpi_egl_settings_p->stencil_size = size;
    return CVPI_TRUE;
  } else {
    return CVPI_FALSE;
  }
}
CVPI_BOOL cvpi_egl_settings_surface_type_check(cvpi_egl_settings egl_settings_p) {
  int all_three = (EGL_WINDOW_BIT|EGL_PIXMAP_BIT|EGL_PBUFFER_BIT) & egl_settings_p->surface_type;
  if(!all_three) {
    fprintf(cvpi_log_file, "%s: Warning: No surface (window, pixmap, or pbuffer) selected.\n", __func__);
  }

  int valid_bits = EGL_WINDOW_BIT|EGL_PIXMAP_BIT|EGL_PBUFFER_BIT|
    EGL_MULTISAMPLE_RESOLVE_BOX_BIT|EGL_SWAP_BEHAVIOR_PRESERVED_BIT|
    EGL_VG_COLORSPACE_LINEAR_BIT|EGL_VG_ALPHA_FORMAT_PRE_BIT;

  if (egl_settings_p->surface_type == EGL_DONT_CARE || !(egl_settings_p->surface_type & ~valid_bits)) {
    return CVPI_TRUE;
  } else {
    fprintf(cvpi_log_file, "%s: EGL_SURFACE_TYPE invalid.\n", __func__);
    return CVPI_FALSE;
  }

}
CVPI_BOOL cvpi_egl_settings_surface_type(cvpi_egl_settings egl_settings_p, 
					 enum cvpi_egl_surface_bits bit,
					 enum cvpi_egl_settings_change change) {
  EGLint old_value = egl_settings_p->surface_type;

  if(bit == EGL_DONT_CARE) {
    egl_settings_p->surface_type = EGL_DONT_CARE;
  } else {
    CVPI_CHANGE_TYPE(change, egl_settings_p->surface_type, bit);
  }

  if (CVPI_TRUE_TEST(cvpi_egl_settings_surface_type_check(egl_settings_p))) {
    return CVPI_TRUE;
  } else {
    egl_settings_p->surface_type = old_value;
    return CVPI_FALSE;
  }
}

CVPI_BOOL cvpi_egl_settings_transparent_red_value_check(EGLint value) {
  if(value == EGL_DONT_CARE || value >= 0) {
    return CVPI_TRUE;
  } else {
    fprintf(cvpi_log_file, "%s: Invalid EGL_TRANSPARENT_RED_VALUE", __func__);
    return CVPI_FALSE;
  }
}
CVPI_BOOL cvpi_egl_settings_transparent_red_value(cvpi_egl_settings cvpi_egl_settings_p, EGLint value) {
  if(CVPI_TRUE_TEST(cvpi_egl_settings_transparent_red_value_check(value))) {
    cvpi_egl_settings_p->transparent_red_value = value;
    return CVPI_TRUE;
  } else {
    return CVPI_FALSE;
  }
}
CVPI_BOOL cvpi_egl_settings_transparent_green_value_check(EGLint value) {
  if(value == EGL_DONT_CARE || value >= 0) {
    return CVPI_TRUE;
  } else {
    fprintf(cvpi_log_file, "%s: Invalid EGL_TRANSPARENT_GREEN_VALUE", __func__);
    return CVPI_FALSE;
  }
}
CVPI_BOOL cvpi_egl_settings_transparent_green_value(cvpi_egl_settings cvpi_egl_settings_p, EGLint value) {
  if(CVPI_TRUE_TEST(cvpi_egl_settings_transparent_green_value_check(value))) {
    cvpi_egl_settings_p->transparent_green_value = value;
    return CVPI_TRUE;
  } else {
    return CVPI_FALSE;
  }
}
CVPI_BOOL cvpi_egl_settings_transparent_blue_value_check(EGLint value) {
  if(value == EGL_DONT_CARE || value >= 0) {
    return CVPI_TRUE;
  } else {
    fprintf(cvpi_log_file, "%s: Invalid EGL_TRANSPARENT_BLUE_VALUE", __func__);
    return CVPI_FALSE;
  }
}
CVPI_BOOL cvpi_egl_settings_transparent_blue_value(cvpi_egl_settings cvpi_egl_settings_p, EGLint value) {
  if(CVPI_TRUE_TEST(cvpi_egl_settings_transparent_blue_value_check(value))) {
    cvpi_egl_settings_p->transparent_blue_value = value;
    return CVPI_TRUE;
  } else {
    return CVPI_FALSE;
  }
}
