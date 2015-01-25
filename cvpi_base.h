/** \file cvpi_base.h

    \brief Contains some conventions used throughout the project.
*/
#ifndef CVPI_BASE
#define CVPI_BASE 1

#ifndef _STDIO_H
#include <stdio.h>
#endif

/** \brief if a function is returning a number representing a boolean,
   then the function should follow this convention. */
#define CVPI_TRUE_TEST(x) ((x) > 0)
#define CVPI_FALSE_TEST(x) ((x) <= 0)
#define CVPI_TRUE 1
#define CVPI_FALSE 0
#define CVPI_NOT(x) (CVPI_TRUE_TEST(x) ? CVPI_FALSE : CVPI_TRUE)

typedef int CVPI_BOOL;

/* static FILE* cvpi_log_file = NULL; */

#define cvpi_log_file stderr

#else 
#pragma message __FILE__ ": CVPI_BASE already defined."
#endif
