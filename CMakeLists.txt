project(cvpi)
add_library(cvpi cvpi_egl_config.c cvpi_vg_ext.c cvpi_image_headers.c cvpi_image_functions.c)
target_link_libraries(cvpi EGL GLESv2 bcm_host)

add_library(chicken-ffi openvg-chicken.scd cvpi-chicken.scd)
target_link_libraries(chicken-ffi EGL GLESv2 bcm_host)
#add_executable(cvpi_test cvpi_egl_config_tests.c cvpi_egl_surface_functions.c cvpi_image_tests.c)
add_executable(chicken-test cvpi_image_tests-chicken.scm)
target_link_libraries(chicken-test EGL GLESv2 bcm_host)