#ifndef EGL_GENERATED_CONFIG_HEADER_H
#include "eglGeneratedConfigHeader.h"
#endif


#ifndef EGL_CONFIG
#include "eglConfig.h"
#endif

#ifndef EGLEXT_BRCM_H
#include "eglext_brcm.h"
#endif

#ifndef transConfig_h
#include "transConfig.h"
#endif

/* used by setupEGLFull subroutine eglChooseConfig. Ordering must
   correspond to the EGL_CONFIG struct in
   eglGeneratedConfigHeader.h */
const EGLint egl_settings_names[] = {EGL_BUFFER_SIZE, EGL_RED_SIZE, EGL_GREEN_SIZE, EGL_BLUE_SIZE, EGL_LUMINANCE_SIZE, EGL_ALPHA_SIZE, EGL_ALPHA_MASK_SIZE, EGL_CONFIG_ID, EGL_DEPTH_SIZE, EGL_LEVEL, EGL_MAX_PBUFFER_WIDTH, EGL_MAX_PBUFFER_HEIGHT, EGL_MAX_PBUFFER_PIXELS, EGL_MAX_SWAP_INTERVAL, EGL_MIN_SWAP_INTERVAL, EGL_NATIVE_VISUAL_ID, EGL_NATIVE_VISUAL_TYPE, EGL_SAMPLE_BUFFERS, EGL_SAMPLES, EGL_STENCIL_SIZE, EGL_TRANSPARENT_RED_VALUE, EGL_TRANSPARENT_GREEN_VALUE, EGL_TRANSPARENT_BLUE_VALUE, EGL_BIND_TO_TEXTURE_RGB, EGL_BIND_TO_TEXTURE_RGBA, EGL_NATIVE_RENDERABLE, EGL_SURFACE_TYPE, EGL_RENDERABLE_TYPE, EGL_TRANSPARENT_TYPE, EGL_COLOR_BUFFER_TYPE, EGL_CONFIG_CAVEAT};

/* used for printing error conditions */
char* eglErrorString(EGLint error) {
  switch(error) {
  case EGL_SUCCESS:
    return " succeeded ";
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
    return" EGL_CONTEXT_LOST ";
  default:
    return " undefined error ";
  }
}

/* setup EGL

   parameters
   width, height :: egl canvas width and height in pixels
   display_id :: parameter for eglGetDisplay, if 0 then EGL_DEFAULT_DISPLAY is used

   parameters set in headers
   pixel format set in transConfig.h
   configuration settings struct EGL_CONFIG set in eglConfig.h

   on success the function returns a pointer to an egl_deletable
   struct, which should be passed to takedownEGL to take down the EGL session.

   if unsuccessful, what has been setup by the function will be taken
   down and a NULL pointer will be returned
 */
void* setupEGL(EGLint width, EGLint height, NativeDisplayType display_id) {
  egl_deletable* deletable = malloc(sizeof(egl_deletable));

  if(!display_id) {
    deletable->display_id = EGL_DEFAULT_DISPLAY;
  } else {
    memcpy(&(deletable->display_id), &(display_id), sizeof(display_id));
  }

  /* Used to capture egl succeed/fail return values */
  EGLBoolean good = EGL_TRUE;

  /* has to be called before any gpu calls are made, why is not documented */
  bcm_host_init();

  /* get the display matching display_id */
  /* possible values does not appear to be documented and is probably implementation specific */
  deletable->eglDisplay = eglGetDisplay(deletable->display_id);
  if(EGL_NO_DISPLAY == deletable->eglDisplay) {
    fprintf(stderr, "eglGetDisplay returned EGL_NO_DISPLAY\n");
    goto undoBase;
  }

  /* initialize the display */
  good = eglInitialize(deletable->eglDisplay, &(deletable->major), &(deletable->minor));
  if(EGL_TRUE != good) {
    fprintf(stderr, "eglInitialize returned %s\n", eglErrorString(good));
    goto undoBase;
  }

  /* set API to EGL_OPENVG_API or EGL_OPENGL_ES_API */
  good = eglBindAPI(EGL_CONFIG.renderable_api);
  if(EGL_TRUE != good) {
    fprintf(stderr, "eglBindAPI returned %s\n", eglErrorString(good));
    goto undoInitialize;
  }

  /* create an EGLConfig */
  (deletable->attrib_list)[0] = egl_settings_names[0];
  (deletable->attrib_list)[1] = EGL_CONFIG.buffer_size;  
  (deletable->attrib_list)[2] = egl_settings_names[1];
  (deletable->attrib_list)[3] = EGL_CONFIG.red_size;
  (deletable->attrib_list)[4] = egl_settings_names[2];
  (deletable->attrib_list)[5] = EGL_CONFIG.green_size;
  (deletable->attrib_list)[6] = egl_settings_names[3];
  (deletable->attrib_list)[7] = EGL_CONFIG.blue_size;
  (deletable->attrib_list)[8] = egl_settings_names[4];
  (deletable->attrib_list)[9] = EGL_CONFIG.luminance_size;
  (deletable->attrib_list)[10] = egl_settings_names[5];
  (deletable->attrib_list)[11] = EGL_CONFIG.alpha_size;
  (deletable->attrib_list)[12] = egl_settings_names[6];
  (deletable->attrib_list)[13] = EGL_CONFIG.alpha_mask_size;
  (deletable->attrib_list)[14] = egl_settings_names[7];
  (deletable->attrib_list)[15] = EGL_CONFIG.config_id;
  (deletable->attrib_list)[16] = egl_settings_names[8];
  (deletable->attrib_list)[17] = EGL_CONFIG.depth_size;
  (deletable->attrib_list)[18] = egl_settings_names[9];
  (deletable->attrib_list)[19] = EGL_CONFIG.level;
  (deletable->attrib_list)[20] = egl_settings_names[10];
  (deletable->attrib_list)[21] = EGL_CONFIG.max_pbuffer_width;
  (deletable->attrib_list)[22] = egl_settings_names[11];
  (deletable->attrib_list)[23] = EGL_CONFIG.max_pbuffer_height;
  (deletable->attrib_list)[24] = egl_settings_names[12];
  (deletable->attrib_list)[25] = EGL_CONFIG.max_pbuffer_pixels;
  (deletable->attrib_list)[26] = egl_settings_names[13];
  (deletable->attrib_list)[27] = EGL_CONFIG.max_swap_interval;
  (deletable->attrib_list)[28] = egl_settings_names[14];
  (deletable->attrib_list)[29] = EGL_CONFIG.min_swap_interval;
  (deletable->attrib_list)[30] = egl_settings_names[15];
  (deletable->attrib_list)[31] = EGL_CONFIG.native_visual_id;
  (deletable->attrib_list)[32] = egl_settings_names[16];
  (deletable->attrib_list)[33] = EGL_CONFIG.native_visual_type;
  (deletable->attrib_list)[34] = egl_settings_names[17];
  (deletable->attrib_list)[35] = EGL_CONFIG.sample_buffers;
  (deletable->attrib_list)[36] = egl_settings_names[18];
  (deletable->attrib_list)[37] = EGL_CONFIG.samples;
  (deletable->attrib_list)[38] = egl_settings_names[19];
  (deletable->attrib_list)[39] = EGL_CONFIG.stencil_size;
  (deletable->attrib_list)[40] = egl_settings_names[20];
  (deletable->attrib_list)[41] = EGL_CONFIG.transparent_red_value;
  (deletable->attrib_list)[42] = egl_settings_names[21];
  (deletable->attrib_list)[43] = EGL_CONFIG.transparent_green_value;
  (deletable->attrib_list)[44] = egl_settings_names[22];
  (deletable->attrib_list)[45] = EGL_CONFIG.transparent_blue_value;
  (deletable->attrib_list)[46] = egl_settings_names[23];
  (deletable->attrib_list)[47] = EGL_CONFIG.bind_to_texture_rgb;
  (deletable->attrib_list)[48] = egl_settings_names[24];
  (deletable->attrib_list)[49] = EGL_CONFIG.bind_to_texture_rgba;
  (deletable->attrib_list)[50] = egl_settings_names[25];
  (deletable->attrib_list)[51] = EGL_CONFIG.native_renderable;
  (deletable->attrib_list)[52] = egl_settings_names[26];
  (deletable->attrib_list)[53] = EGL_CONFIG.surface_type;
  (deletable->attrib_list)[54] = egl_settings_names[27];
  (deletable->attrib_list)[55] = EGL_CONFIG.renderable_type;
  (deletable->attrib_list)[56] = egl_settings_names[28];
  (deletable->attrib_list)[57] = EGL_CONFIG.transparent_type;
  (deletable->attrib_list)[58] = egl_settings_names[29];
  (deletable->attrib_list)[59] = EGL_CONFIG.color_buffer_type;
  (deletable->attrib_list)[60] = egl_settings_names[30];
  (deletable->attrib_list)[61] = EGL_CONFIG.config_caveat;
  (deletable->attrib_list)[62] = EGL_NONE;

  /* get the total number of configs matching attrib_list */
  deletable->numberConfigs = 0;
  good = eglChooseConfig(deletable->eglDisplay, deletable->attrib_list, NULL, 0, &(deletable->numberConfigs));
  if(1 < deletable->numberConfigs) {
    fprintf(stderr, "eglChooseConfig number of configurations is %d\n", deletable->numberConfigs);
  }
  if(0 == deletable->numberConfigs || EGL_TRUE != good) {
    if(!deletable->numberConfigs) {
      fprintf(stderr, "first call to eglChooseConfig: number of configurations is zero\n");
    }
    fprintf(stderr, "first call to eglChooseConfig returned %s\n", eglErrorString(good));
    goto undoInitialize;
  }
  /* get a list of matching configs */
  deletable->matchingConfigs = (EGLConfig*)malloc(deletable->numberConfigs * sizeof(deletable->matchingConfigs));
  good = eglChooseConfig(deletable->eglDisplay, deletable->attrib_list, deletable->matchingConfigs, deletable->numberConfigs, &(deletable->numberConfigs));
  //good = eglChooseConfig(deletable->eglDisplay, deletable->attrib_list, deletable->matchingConfigs, 1, &(deletable->numberConfigs));
  if(EGL_TRUE != good) {
    fprintf(stderr, "second call to eglChooseConfig returned %s\n", eglErrorString(good));
    goto undoChooseConfig;
  }

  const EGLint * attrib_list = NULL; /* in the future change to allow for user modification */


  deletable->id = NULL;		/* is null for non-pixmap types */

  if(EGL_CONFIG.surface_type & EGL_PIXMAP_BIT) {

    EGLint pixel_format = pixel_format_name(TRANS_EGL) | EGL_PIXEL_FORMAT_RENDER_VG_BRCM | EGL_PIXEL_FORMAT_VG_IMAGE_BRCM | EGL_PIXEL_FORMAT_RENDER_VG_BRCM|EGL_PIXEL_FORMAT_RENDER_GL_BRCM|EGL_PIXEL_FORMAT_RENDER_GLES_BRCM|EGL_PIXEL_FORMAT_RENDER_GLES2_BRCM;
    deletable->id = malloc(sizeof(EGLint) * 5);
    (deletable->id)[0] = 0;
    (deletable->id)[1] = 0;
    (deletable->id)[2] = width;
    (deletable->id)[3] = height;
    (deletable->id)[4] = pixel_format;
    /* this function is undocumented, necessary to set the id */
    /* going off of http://www.raspberrypi.org/forums/viewtopic.php?f=63&t=6488 */
    eglCreateGlobalImageBRCM(width, height, pixel_format, NULL, pixel_format_name(TRANS_BYTES) * width, deletable->id);
    if(!(deletable->id)[0] && !(deletable->id)[1]) {
      good = eglGetError();
      fprintf(stderr, "eglCreateGlobalImageBRCM returned %s\n", eglErrorString(good));
      goto undoChooseConfig;
    }
    /* second last parameter is a void* for Broadcom, beyond that it is undocumented */
    deletable->eglSurface = EGL_TRUE;
    /* eglCreatePixmapSurface(1, 20, 0x1ecd070, 0) */
    deletable->eglSurface = eglCreatePixmapSurface(deletable->eglDisplay, *(deletable->matchingConfigs), deletable->id, attrib_list);
    //    EGLint val = eglCreatePixmapSurface(deletable->eglDisplay, *(deletable->matchingConfigs), deletable->id, attrib_list);
    //    fprintf(stderr, "pixmap surface creation return %x %s.\n", deletable->eglSurface, eglErrorString(eglGetError()));
    //    goto undoCreateGlobalImageBRCM;
    if(deletable->eglSurface == EGL_NO_SURFACE) {
      good = eglGetError();
      fprintf(stderr, "pixmap surface creation failed %s %x.\n", eglErrorString(good), good);
      goto undoCreateGlobalImageBRCM;
    }
  } else if(EGL_CONFIG.surface_type & EGL_PBUFFER_BIT) {
    deletable->eglSurface = eglCreatePbufferSurface(deletable->eglDisplay, (deletable->matchingConfigs)[0], attrib_list);

    if(deletable->eglSurface == EGL_NO_SURFACE) {
      fprintf(stderr, "pbuffer surface creation failed %s.\n", eglErrorString(eglGetError()));
      goto undoChooseConfig;
    }
  } else if(EGL_CONFIG.surface_type & EGL_WINDOW_BIT) {
    /* TODO */
    fprintf(stderr, "window surface currently unhandled\n");
    goto undoChooseConfig;
    //    deletable->eglSurface = eglCreateWindowSurface(deletable->eglDisplay, (deletable->matchingConfigs)[0], NativeWindowType win, attrib_list);
  } else {
    good = EGL_FALSE;
    fprintf(stderr, "Unhandled surface type: %u\n", EGL_CONFIG.surface_type);
    goto undoChooseConfig;
  }
  //  printf("here\n");
  deletable->eglContext = eglCreateContext(deletable->eglDisplay, *(deletable->matchingConfigs), eglGetCurrentContext(), NULL);
  if(EGL_NO_CONTEXT == deletable->eglContext) {
    fprintf(stderr, "eglCreateContext returned %s\n", eglErrorString(eglGetError()));
    goto undoCreateSurface;
  }


  good = eglMakeCurrent(deletable->eglDisplay,
			deletable->eglSurface, deletable->eglSurface,
			deletable->eglContext);
  //  printf("here1\n");
  if(EGL_FALSE == good) {
    fprintf(stderr, "eglMakeCurrent returned false\n");
    goto undoCreateContext;
  }

  printf("deletable ptr: %p\n", deletable);
  return deletable;

  /* If there is an error in setup, then take down */

 undoCreateContext:
  good = eglDestroyContext(deletable->eglDisplay, deletable->eglContext);
  if(EGL_TRUE != good) {
    fprintf(stderr, "eglDestroyContext returned %s\n", eglErrorString(good));
  }

  undoCreateSurface:
  good = eglDestroySurface(deletable->eglDisplay, deletable->eglSurface);
  if(EGL_FALSE == good) {
    fprintf(stderr, "eglDestroySurface returned false\n");
  }

  undoCreateGlobalImageBRCM:
  if(deletable->id) {		/* id only set to non-zerowhen pixmap is used*/
    good = eglDestroyGlobalImageBRCM(deletable->id);
    if(EGL_FALSE == good) {
      fprintf(stderr, "eglDestroyGlobalImageBRCM returned false\n");
    }
    free(deletable->id);
  }

  undoChooseConfig:
  free(deletable->matchingConfigs);

  undoInitialize:
  good = eglTerminate(deletable->eglDisplay);
  if(EGL_TRUE != good) {
    fprintf(stderr, "eglTerminate returned %s\n", eglErrorString(good));
  }
  undoBase:

  free(deletable);
  bcm_host_deinit();

  return NULL;
}

/* takedown EGL */
void takedownEGL(void* delete) {
  egl_deletable* deletable = delete;
  EGLBoolean good = EGL_TRUE;
  /* undo eglInitialize */
  good = eglDestroyContext(deletable->eglDisplay, deletable->eglContext);
  if(EGL_TRUE != good) {
    fprintf(stderr, "eglDestroyContext returned %s\n", eglErrorString(good));
  }

  good = eglDestroySurface(deletable->eglDisplay, deletable->eglSurface);
  if(EGL_FALSE == good) {
    fprintf(stderr, "eglDestroySurface returned false\n");
  }

  if(deletable->id) {
    good = eglDestroyGlobalImageBRCM(deletable->id);
    if(EGL_FALSE == good) {
      fprintf(stderr, "eglDestroyGlobalImageBRCM returned false\n");
    }
    free(deletable->id);
  }

  free(deletable->matchingConfigs);

  good = eglTerminate(deletable->eglDisplay);
  if(EGL_TRUE != good) {
    fprintf(stderr, "eglTerminate returned %s\n", eglErrorString(good));
  }
  free(deletable);
  delete = NULL;
  bcm_host_deinit();
}

void change_surface_type(enum egl_surface_bits bit) {
  EGL_CONFIG.surface_type = bit;
}
void change_renderable_type(enum egl_renderable_bits bit) {
  EGL_CONFIG.renderable_type = bit;
}
void change_transparent_type(enum egl_transparent_type trans) {
  EGL_CONFIG.transparent_type = trans;
}
void change_color_buffer_type(enum egl_color_buffer_type color) {
  EGL_CONFIG.color_buffer_type = color;
}
void change_config_caveat(enum egl_config_caveat caveat) {
  EGL_CONFIG.config_caveat = caveat;
}
void change_renderable_api(enum egl_renderable_api api) {
  EGL_CONFIG.renderable_api = api;
}
