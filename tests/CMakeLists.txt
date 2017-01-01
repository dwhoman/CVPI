add_executable(cvpi_tests cvpi_tests.c cvpi_egl_config_tests.c cvpi_image_tests.c)
include_directories(${PI_DIRS})
target_link_libraries(cvpi_tests ${CVPI_LIBS} cvpi-aux)
