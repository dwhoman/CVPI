/* Used for printing out a csv list of EGL configurations. Might be
   useful for finding possible configurations. */
#include <stdio.h>
#include <stdlib.h>
#include <EGL/eglplatform.h>
// EGLint
#include <EGL/egl.h>

int main(void) {
  EGLint* num_configs = malloc(sizeof(EGLint));

  EGLDisplay dpy = eglGetDisplay(EGL_DEFAULT_DISPLAY);
  if(dpy == EGL_NO_DISPLAY) {
    printf("Error no display\n");
  }
  EGLint majorValue = 0;
  EGLint minorValue = 0;

  EGLBoolean initialized = eglInitialize(dpy, &majorValue, &minorValue);

  if(initialized != EGL_TRUE) {
    printf("initialized false\n");
  }

  EGLBoolean eGCN = eglGetConfigs(dpy, NULL, 0xffffFFFF, num_configs);
  if(eGCN != EGL_TRUE) {
    printf("error getting number of configs\n");
  }

  printf("%d Configurations\n", *num_configs);

  int i = 0;
  int max = *num_configs;

  //  printf("max: %d\n", max);
  EGLConfig* configs = malloc(sizeof(EGLConfig) * max);;
  EGLBoolean eGC = eglGetConfigs(dpy, configs, max, num_configs);
  if(eGC != EGL_TRUE) {
    printf("error setting configs\n");
  }

  EGLint red, green, blue, egl_buffer_size, egl_luminance_size, egl_alpha_size, egl_alpha_mask_size, egl_config_id, egl_depth_size, egl_level, egl_max_pbuffer_width, egl_max_pbuffer_height, egl_max_pbuffer_pixels, egl_max_swap_interval, egl_min_swap_interval, egl_native_visual_id, egl_native_visual_type, egl_sample_buffers, egl_samples, egl_stencil_size, egl_transparent_red_value, egl_transparent_green_value, egl_transparent_blue_value;

  EGLBoolean egl_bind_to_texture_rgb, egl_bind_to_texture_rgba, egl_native_renderable;
  
  unsigned int egl_surface_type, egl_renderable_type, egl_transparent_type, egl_color_buffer_type, egl_config_caveat;

  printf("#,buffer_size, red_size, green_size, blue_size, luminance_size, alpha_size, alpha_mask_size, config_id, depth_size, level, max_pbuffer_width, max_pbuffer_height, max_pbuffer_pixels, max_swap_interval, min_swap_interval, native_visual_id, native_visual_type, sample_buffers, samples, stencil_size, transparent_red_value, transparent_green_value, transparent_blue_value,bind_to_texture_rgb, bind_to_texture_rgba, native_renderable, surface_type, renderable_type, transparent_type, color_buffer_type, config_caveat\n");
  
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

    printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%x,%x,%x,%x,%x\n", i, egl_buffer_size, red, green, blue, egl_luminance_size, egl_alpha_size, egl_alpha_mask_size, egl_config_id, egl_depth_size, egl_level, egl_max_pbuffer_width, egl_max_pbuffer_height, egl_max_pbuffer_pixels, egl_max_swap_interval, egl_min_swap_interval, egl_native_visual_id, egl_native_visual_type, egl_sample_buffers, egl_samples, egl_stencil_size, egl_transparent_red_value, egl_transparent_green_value, egl_transparent_blue_value, egl_bind_to_texture_rgb, egl_bind_to_texture_rgba, egl_native_renderable, egl_surface_type, egl_renderable_type, egl_transparent_type, egl_color_buffer_type, egl_config_caveat);
  }

  free(num_configs);
  EGLBoolean terminated =  eglTerminate(dpy);
  if(terminated == EGL_FALSE) {
    printf("terminated false\n");
  }
  return 0;
}


