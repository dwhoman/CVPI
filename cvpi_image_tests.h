#ifndef CVPI_IMAGE_TESTS
#define CVPI_IMAGE_TESTS 1

#ifndef CVPI
#include "cvpi.h"
#endif

CVPI_BOOL test_cvpi_yuyv2yuva(void);

#else
#pragma message __FILE__ ": CVPI_IMAGE_TESTS already defined."
#endif
