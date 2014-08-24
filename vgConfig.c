#ifndef VG_CONFIG
#include "vgConfig.h"
#endif

char* vgErrorString(unsigned long error) {
  switch(error) {
  case VG_NO_ERROR:
    return " VG_NO_ERROR ";
  case VG_BAD_HANDLE_ERROR:
    return " VG_BAD_HANDLE_ERROR ";
  case VG_ILLEGAL_ARGUMENT_ERROR:
    return " VG_ILLEGAL_ARGUMENT_ERROR ";
  case VG_OUT_OF_MEMORY_ERROR:
    return " VG_OUT_OF_MEMORY_ERROR ";
  case VG_PATH_CAPABILITY_ERROR:
    return " VG_PATH_CAPABILITY_ERROR ";
  case VG_UNSUPPORTED_IMAGE_FORMAT_ERROR:
    return " VG_UNSUPPORTED_IMAGE_FORMAT_ERROR ";
  case VG_UNSUPPORTED_PATH_FORMAT_ERROR:
    return " VG_UNSUPPORTED_PATH_FORMAT_ERROR ";
  case VG_IMAGE_IN_USE_ERROR:
    return " VG_IMAGE_IN_USE_ERROR ";
  case VG_NO_CONTEXT_ERROR:
    return " VG_NO_CONTEXT_ERROR ";
  default:
    return " undefined error ";
  }
}
