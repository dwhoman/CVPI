set(CVPI_SOURCE_AUX cvpi_camera_setup.c  cvpi_egl_surface_functions.c)
add_library(cvpi-aux SHARED ${CVPI_SOURCE_AUX})
include_directories(${PI_DIRS})
target_link_libraries(cvpi-aux ${CVPI_LIBS})