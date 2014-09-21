#ifndef
#define CVPI_EGL_SURFACE_FUNCTIONS 1
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
