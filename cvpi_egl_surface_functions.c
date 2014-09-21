#ifndef CVPI_EGL_CONFIG
#include "cvpi_egl_config.h"
#endif

#ifndef _STDIO_H
#include <stdio.h>
#endif

/* EGL Pixmap */

/* the surface_pixmap_create_function function pointer type */
typedef EGLNativePixmapType (*pixmap_function_pointer)(cvpi_egl_instance);

/* Store the last argument to eglCreateGlobalImageBRCM in an
   association list, alist, using function_pointer as the key. The number of
   EGL instances is likely small, probably one or two. The insert
   function checks for duplicate keys.*/
struct pixmap_image {
  EGLint* pixmap_id;		/* eglCreateGlobalImageBRCM argument */
  pixmap_function_pointer pointer;	/* a cvpi_egl_surface_pixmap_native_creator pointer */
  struct pixmap_image* next;	/* next node in list */
};

/* start of the association list */
static struct pixmap_image* pixmap_image_head = NULL;

/* the previous node to the found node when searching */
static struct pixmap_image* pixmap_image_previous = NULL;

/* Find the cons cell where key = pointer. Return NULL if not found. */
struct pixmap_image* pixmap_image_find(pixmap_function_pointer pointer) {
  pixmap_image_previous = NULL;
  struct pixmap_image* current = pixmap_image_head;
  if(current == NULL) {
    return NULL;
  } else {
    do {
      if(current->pointer == pointer) {
	return current;
      }
      pixmap_image_previous = current;
    } while((current = current->next) != NULL);
    
    return NULL;
  }
}

/* Insert into the alist. If already in the alist, update the info. */
static struct pixmap_image* pixmap_image_insert(cvpi_egl_settings egl_settings_p) {
  /* no duplicate keys */
  struct pixmap_image* found = pixmap_image_find(egl_settings_p->pointer);
  if(found == NULL) {
    /* insert at beginning of list, making a new head */
    struct pixmap_image* found = pixmap_image_head;
    pixmap_image_head = malloc(sizeof(*pixmap_image_head));
    pixmap_image_head->next = found; /* found = old head */
    pixmap_image_head->pointer = egl_settings_p->surface_pixmap_create_function;
    pixmap_image_head->pixmap_id = malloc(sizeof(pixmap_image_head->pixmap_id) * 5);
    found = pixmap_image_head;	/* found = new head */
  }
  /* not sure why eglCreateGlobalImageBRCM requires this array format */
  found->pixmap_id[0] = 0;
  found->pixmap_id[1] = 0;
  found->pixmap_id[2] = egl_settings_p->width;
  found->pixmap_id[3] = egl_settings_p->height;
  found->pixmap_id[4] = egl_settings_p->pixel_format;

  return found;
}

/* Delete the node in the alist that pointer points to.  Used as a
utility function for pixmap_image_delete_find. Requires that
pixmap_image_find() be ran first. */
static void pixmap_image_delete(struct pixmap_image* pointer) {
  if(pointer == NULL) {
    return;
  } else if(pixmap_image_head == pointer) { /* first in the list */
    pixmap_image_head = pointer->next;
  } else {			/* middle or end of list */
    pixmap_image_previous->next = pointer->next;
  }
  free(pointer->pixmap_id);
  pointer->pixmap_id = NULL;
  free(pointer);
}

/* Find an element in the alist with the given key. */
static void pixmap_image_delete_find(pixmap_function_pointer key) {
  struct pixmap_image* found = pixmap_image_find(key);
  pixmap_image_delete(found);
}

/* Destroy the entire alist. */
void cvpi_egl_surface_pixmap_list_destroy() {
  struct pixmap_image* current;
  while(pixmap_image_head != NULL) {
    current = pixmap_image_head;
    pixmap_image_head = pixmap_image_head->next;
    free(current->pixmap_id);
    current->pixmap_id = NULL;
    free(current);
  }
}

/* function for surface_pixmap_create_function function pointer */
EGLNativePixmapType cvpi_egl_surface_pixmap_native_creator(cvpi_egl_instance egl_instance) {
  cvpi_egl_settings egl_settings_p = egl_instance->egl_settings;
  struct pixmap_image* pointer = pixmap_image_insert(egl_settings_p);

  /* user defined formats plus Broadcom, seemingly required, settings */
  EGLint format = egl_settings_p->pixel_format | egl_settings_p->pixel_format_brcm;

  /* eglCreateGlobalImageBRCM is undocumented, necessary to set the id */
  /* going off of http://www.raspberrypi.org/forums/viewtopic.php?f=63&t=6488 */
  EGLint stride = cvpi_egl_bytes_per_pixel(egl_settings_p->pixel_format) * egl_settings_p->width;
    
  eglCreateGlobalImageBRCM(egl_settings_p->width, egl_settings_p->height, 
			   format, NULL, stride, pointer->pixmap_id);
  if(!(pointer->pixmap_id[0]) && !(pointer->pixmap_id[1])) {
    fprintf(stderr, "%s: eglCreateGlobalImageBRCM returned %s\n", __func__, cvpi_egl_error_string(eglGetError()));
    pixmap_image_delete(pointer);
    return NULL
  } 
  return pointer->pixmap_id;
}

/* function for surface_pixmap_destroy_function function pointer */
EGLBoolean cvpi_egl_surface_pixmap_native_destroyer(cvpi_egl_instance egl_instance) {
  cvpi_egl_settings egl_settings_p = egl_instance->egl_settings;
  struct pixmap_image* found = pixmap_image_find(egl_settings_p->surface_pixmap_create_function);
  EGLint good = EGL_TRUE;
  if(found != NULL) {
    good = eglDestroyGlobalImageBRCM(found->pixmap_id);
    if(EGL_FALSE == good) {
      fprintf(stderr, "%s: eglDestroyGlobalImageBRCM returned false\n", __func__);
    }

    pixmap_image_delete(found);
  }

  return good;
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
