#ifndef _STDIO_H
#include <stdio.h>
#endif

#ifndef	_STDLIB_H
#include <stdlib.h>
#endif

#ifndef CVPI
#include "cvpi.h"
#endif

#ifndef CVPI_EGL_CONFIG_TESTS
#include "cvpi_egl_config_tests.h"
#endif

#ifndef CVPI_IMAGE_TESTS
#include "cvpi_image_tests.h"
#endif

void test_runner(CVPI_BOOL(*test_function)(void), char* name) {
  if(CVPI_TRUE_TEST(test_function())) {
    printf("PASSED: %s\n", name);
  } else {
    printf("FAILED: %s\n", name);
  }
  printf("--------------------\n");
}

#define TEST(f) do {CVPI_BOOL(*p18025ldgfaln1293)(void) = (f); test_runner(p18025ldgfaln1293,(#f));} while(0)

int main() {
  TEST(test_cvpi_egl_settings_create);
  TEST(test_cvpi_egl_instance_setup_takedown);
  TEST(test_cvpi_egl_instance_openvg_pixmap);
  
  //TEST(test_cvpi_yuyv2yuva);

  return 0;
}
