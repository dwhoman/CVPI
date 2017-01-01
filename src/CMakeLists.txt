set(CVPI_SOURCE 
  cvpi_base.c
  cvpi_egl_config.c
  cvpi_image_functions.c
  cvpi_image_headers.c
  cvpi_vg_ext.c)

add_library(cvpi SHARED ${CVPI_SOURCE})
include_directories(${PI_DIRS})
target_link_libraries(cvpi ${CVPI_LIBS})
