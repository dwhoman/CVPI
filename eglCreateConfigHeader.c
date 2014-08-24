#include <stdio.h>
#include <stdlib.h>
#include <EGL/eglplatform.h>
// EGLint
#include <EGL/egl.h>

int main(void) {
  EGLint* num_configs = malloc(sizeof(EGLint));

  EGLDisplay dpy = eglGetDisplay(EGL_DEFAULT_DISPLAY);
  if(dpy == EGL_NO_DISPLAY) {
    fprintf(stderr,"Error no display\n");
  }
  EGLint majorValue = 0;
  EGLint minorValue = 0;

  EGLBoolean initialized = eglInitialize(dpy, &majorValue, &minorValue);

  if(initialized != EGL_TRUE) {
    fprintf(stderr,"initialized false\n");
  }

  EGLBoolean eGCN = eglGetConfigs(dpy, NULL, 0xffffFFFF, num_configs);
  if(eGCN != EGL_TRUE) {
    fprintf(stderr,"error getting number of configs\n");
  }

  int i = 0;
  int max = *num_configs;

  //  printf("max: %d\n", max);
  EGLConfig* configs = malloc(sizeof(EGLConfig) * max);
  EGLBoolean eGC = eglGetConfigs(dpy, configs, max, num_configs);
  if(eGC != EGL_TRUE) {
    fprintf(stderr,"error setting configs\n");
  }

  EGLint red, green, blue, egl_buffer_size, egl_luminance_size, egl_alpha_size, egl_alpha_mask_size, egl_config_id, egl_depth_size, egl_level, egl_max_pbuffer_width, egl_max_pbuffer_height, egl_max_pbuffer_pixels, egl_max_swap_interval, egl_min_swap_interval, egl_native_visual_id, egl_native_visual_type, egl_sample_buffers, egl_samples, egl_stencil_size, egl_transparent_red_value, egl_transparent_green_value, egl_transparent_blue_value;

  EGLBoolean egl_bind_to_texture_rgb, egl_bind_to_texture_rgba, egl_native_renderable;
  
  unsigned int egl_surface_type, egl_renderable_type, egl_transparent_type, egl_color_buffer_type, egl_config_caveat;

  printf("#ifndef __eglplatform_h_\n#include <EGL/eglplatform.h>\n#endif\n");
  printf("#ifndef __egl_h_\n#include <EGL/egl.h>\n#endif\n");
  printf("#ifndef EGL_GENERATED_CONFIG_HEADER_H\n#define EGL_GENERATED_CONFIG_HEADER_H\n");

  printf("#define EGL_OPTION_COUNT 31\n"); /* number of possible EGL options */

  printf("struct egl_settings {const EGLint buffer_size; const EGLint red_size; const EGLint green_size; const EGLint blue_size; const EGLint luminance_size; const EGLint alpha_size; const EGLint alpha_mask_size; const EGLint config_id; const EGLint depth_size; const EGLint level; const EGLint max_pbuffer_width; const EGLint max_pbuffer_height; const EGLint max_pbuffer_pixels; const EGLint max_swap_interval; const EGLint min_swap_interval; const EGLint native_visual_id; const EGLint native_visual_type; const EGLint sample_buffers; const EGLint samples; const EGLint stencil_size; const EGLint transparent_red_value; const EGLint transparent_green_value; const EGLint transparent_blue_value; const EGLBoolean bind_to_texture_rgb; const EGLBoolean bind_to_texture_rgba; const EGLBoolean native_renderable; EGLint surface_type; EGLint renderable_type; EGLenum transparent_type; EGLenum color_buffer_type; EGLenum config_caveat; EGLint renderable_api;};\n\n");
  
  for(; i < max; i++) {

    eglGetConfigAttrib(dpy, configs[i], EGL_RED_SIZE, &red);
    eglGetConfigAttrib(dpy, configs[i], EGL_GREEN_SIZE, &green);
    eglGetConfigAttrib(dpy, configs[i], EGL_BLUE_SIZE, &blue);
    eglGetConfigAttrib(dpy, configs[i], EGL_BUFFER_SIZE, &egl_buffer_size);
    eglGetConfigAttrib(dpy, configs[i], EGL_LUMINANCE_SIZE, &egl_luminance_size);
    eglGetConfigAttrib(dpy, configs[i], EGL_ALPHA_SIZE, &egl_alpha_size);
    eglGetConfigAttrib(dpy, configs[i], EGL_ALPHA_MASK_SIZE, &egl_alpha_mask_size);
    eglGetConfigAttrib(dpy, configs[i], EGL_CONFIG_ID, &egl_config_id);
    eglGetConfigAttrib(dpy, configs[i], EGL_DEPTH_SIZE, &egl_depth_size);
    eglGetConfigAttrib(dpy, configs[i], EGL_LEVEL, &egl_level);
    eglGetConfigAttrib(dpy, configs[i], EGL_MAX_PBUFFER_WIDTH, &egl_max_pbuffer_width);
    eglGetConfigAttrib(dpy, configs[i], EGL_MAX_PBUFFER_HEIGHT, &egl_max_pbuffer_height);
    eglGetConfigAttrib(dpy, configs[i], EGL_MAX_PBUFFER_PIXELS, &egl_max_pbuffer_pixels);
    eglGetConfigAttrib(dpy, configs[i], EGL_MAX_SWAP_INTERVAL, &egl_max_swap_interval);
    eglGetConfigAttrib(dpy, configs[i], EGL_MIN_SWAP_INTERVAL, &egl_min_swap_interval);
    eglGetConfigAttrib(dpy, configs[i], EGL_NATIVE_VISUAL_ID, &egl_native_visual_id);
    eglGetConfigAttrib(dpy, configs[i], EGL_NATIVE_VISUAL_TYPE, &egl_native_visual_type);
    eglGetConfigAttrib(dpy, configs[i], EGL_SAMPLE_BUFFERS, &egl_sample_buffers);
    eglGetConfigAttrib(dpy, configs[i], EGL_SAMPLES, &egl_samples);
    eglGetConfigAttrib(dpy, configs[i], EGL_STENCIL_SIZE, &egl_stencil_size);
    eglGetConfigAttrib(dpy, configs[i], EGL_TRANSPARENT_RED_VALUE, &egl_transparent_red_value);
    eglGetConfigAttrib(dpy, configs[i], EGL_TRANSPARENT_GREEN_VALUE, &egl_transparent_green_value);
    eglGetConfigAttrib(dpy, configs[i], EGL_TRANSPARENT_BLUE_VALUE, &egl_transparent_blue_value);
    /* boolean */
    /* ignore compiler warning: EGL spec p. 2

       "All legal attribute names and values, whether their type is
       boolean, bitmask, enumerant (symbolic constant), integer, handle,
       or other, may be converted to and from EGLint without loss of
       information." */
    eglGetConfigAttrib(dpy, configs[i], EGL_BIND_TO_TEXTURE_RGB, &egl_bind_to_texture_rgb);
    eglGetConfigAttrib(dpy, configs[i], EGL_BIND_TO_TEXTURE_RGBA, &egl_bind_to_texture_rgba);
    eglGetConfigAttrib(dpy, configs[i], EGL_NATIVE_RENDERABLE, &egl_native_renderable);

    /* bitmask  */
    eglGetConfigAttrib(dpy, configs[i], EGL_SURFACE_TYPE, &egl_surface_type);
    eglGetConfigAttrib(dpy, configs[i], EGL_RENDERABLE_TYPE, &egl_renderable_type);

    /* enum? */
    eglGetConfigAttrib(dpy, configs[i], EGL_TRANSPARENT_TYPE, &egl_transparent_type);
    eglGetConfigAttrib(dpy, configs[i], EGL_COLOR_BUFFER_TYPE, &egl_color_buffer_type);
    eglGetConfigAttrib(dpy, configs[i], EGL_CONFIG_CAVEAT, &egl_config_caveat);

    //printf("const EGLint egl_surface_type_%d = %d;\n", i, egl_surface_type);
    //printf("const EGLint egl_renderable_type_%d = %d;\n", i, egl_renderable_type);

    printf("#define egl_settings_%d() static struct egl_settings egl_settings_%d = {%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d};\n", i, i, egl_buffer_size, red, green, blue, egl_luminance_size, egl_alpha_size, egl_alpha_mask_size, egl_config_id, egl_depth_size, egl_level, egl_max_pbuffer_width, egl_max_pbuffer_height, egl_max_pbuffer_pixels, egl_max_swap_interval, egl_min_swap_interval, egl_native_visual_id, egl_native_visual_type, egl_sample_buffers, egl_samples, egl_stencil_size, egl_transparent_red_value, egl_transparent_green_value, egl_transparent_blue_value, egl_bind_to_texture_rgb, egl_bind_to_texture_rgba, egl_native_renderable, egl_surface_type, egl_renderable_type, egl_transparent_type, egl_color_buffer_type, egl_config_caveat, EGL_OPENVG_API);
  }

  free(num_configs);
  EGLBoolean terminated =  eglTerminate(dpy);
  if(terminated == EGL_FALSE) {
    fprintf(stderr, "terminated false\n");
  }

/* EGL_CONFIG.buffer_size = EGL_DONT_CARE; */
/* EGL_CONFIG.red_size = EGL_DONT_CARE; */
/* EGL_CONFIG.green_size = EGL_DONT_CARE; */
/* EGL_CONFIG.blue_size = EGL_DONT_CARE; */
/* EGL_CONFIG.luminance_size = EGL_DONT_CARE; */
/* EGL_CONFIG.alpha_size = EGL_DONT_CARE; */
/* EGL_CONFIG.alpha_mask_size = EGL_DONT_CARE; */
/* EGL_CONFIG.config_id = EGL_DONT_CARE; */
/* EGL_CONFIG.depth_size = EGL_DONT_CARE; */
/* EGL_CONFIG.level = 0; */
/* EGL_CONFIG.max_pbuffer_width = EGL_DONT_CARE; */
/* EGL_CONFIG.max_pbuffer_height = EGL_DONT_CARE; */
/* EGL_CONFIG.max_pbuffer_pixels = EGL_DONT_CARE; */
/* EGL_CONFIG.max_swap_interval = EGL_DONT_CARE; */
/* EGL_CONFIG.min_swap_interval = EGL_DONT_CARE; */
/* EGL_CONFIG.native_visual_id = EGL_DONT_CARE; */
/* EGL_CONFIG.native_visual_type = EGL_DONT_CARE; */
/* EGL_CONFIG.sample_buffers = EGL_DONT_CARE; */
/* EGL_CONFIG.samples = EGL_DONT_CARE; */
/* EGL_CONFIG.stencil_size = EGL_DONT_CARE; */
/* EGL_CONFIG.transparent_red_value = EGL_DONT_CARE; */
/* EGL_CONFIG.transparent_green_value = EGL_DONT_CARE; */
/* EGL_CONFIG.transparent_blue_value = EGL_DONT_CARE; */
/* EGL_CONFIG.bind_to_texture_rgb = EGL_DONT_CARE; */
/* EGL_CONFIG.bind_to_texture_rgba = EGL_DONT_CARE; */
/* EGL_CONFIG.native_renderable = EGL_DONT_CARE; */
/* EGL_CONFIG.surface_type = EGL_PIXMAP_BIT; */
/* EGL_CONFIG.renderable_type = EGL_OPENVG_BIT; */
/* EGL_CONFIG.transparent_type = EGL_DONT_CARE; */
/* EGL_CONFIG.color_buffer_type = EGL_DONT_CARE; */
/* EGL_CONFIG.config_caveat = EGL_DONT_CARE; */

  printf("#define egl_settings_custom() static struct egl_settings egl_settings_custom = {EGL_DONT_CARE,EGL_DONT_CARE,EGL_DONT_CARE,EGL_DONT_CARE,EGL_DONT_CARE,EGL_DONT_CARE,EGL_DONT_CARE,EGL_DONT_CARE,EGL_DONT_CARE,0,EGL_DONT_CARE,EGL_DONT_CARE,EGL_DONT_CARE,EGL_DONT_CARE,EGL_DONT_CARE,EGL_DONT_CARE,EGL_DONT_CARE,EGL_DONT_CARE,EGL_DONT_CARE,EGL_DONT_CARE,EGL_DONT_CARE,EGL_DONT_CARE,EGL_DONT_CARE,EGL_DONT_CARE,EGL_DONT_CARE,EGL_DONT_CARE,EGL_PIXMAP_BIT|EGL_VG_ALPHA_FORMAT_PRE_BIT|EGL_SWAP_BEHAVIOR_PRESERVED_BIT|EGL_VG_COLORSPACE_LINEAR_BIT,EGL_OPENVG_BIT,EGL_DONT_CARE,EGL_DONT_CARE,EGL_DONT_CARE,EGL_OPENVG_API};\n");
  printf("#endif\n");
  return 0;
}


