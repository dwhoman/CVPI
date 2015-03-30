/** \file cvpi_base.h

    \brief Contains some conventions used throughout the project.
*/
#ifndef CVPI_BASE
#define CVPI_BASE 1

#ifndef _STDIO_H
#include <stdio.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/** \brief if a function is returning a number representing a boolean,
   then the function should follow this convention. */
#define CVPI_TRUE_TEST(x) ((x) > 0)
#define CVPI_FALSE_TEST(x) ((x) <= 0)
#define CVPI_TRUE 1
#define CVPI_FALSE 0
#define CVPI_NOT(x) (CVPI_TRUE_TEST(x) ? CVPI_FALSE : CVPI_TRUE)

  typedef int CVPI_BOOL;

  typedef enum {
    cvpi_log_stdout,
    cvpi_log_stderr,
    cvpi_log_path,
  } cvpi_log_file_location;

  /* Set the location where cvpi logging is printed. If LOCATION is
     set to `cvpi_log_path', then logging is sent to the file located
     at FILE_PATH. If there is an error while opening with fopen,
     errno is returned and cvpi_log_file is set to
     cvpi_log_file_default, else 0 is returned. */
  int cvpi_log_file_set(cvpi_log_file_location location,
			const char* file_path);
  /* Close the file currently being used for logging. Returns errno if
     there is an error while closing, else points the log location to
     NULL and returns 0. */
  int cvpi_log_file_unset(void);

  /* cvpi_log_file_get is used exclusively as the first argument to
     fprintf. It simply returns the value of a private variable. Using
     cvpi_log_file in its place is preferred. */
  FILE* cvpi_log_file_get(void);
#define cvpi_log_file cvpi_log_file_get()

#define cvpi_log_file_default stderr

  /* Predefined formatting functions that can be used with cvpi_log_#.
     e.g. cvpi_log_1(cvpi_log_format_1("Error message"), __func__, __LINE__);
  */
#define cvpi_log_format_1(text) "%s:%d:" #text "\n"
#define cvpi_log_format_2(text) "%s:%d:" #text ":%s\n"
#define cvpi_log_format_3(text) "%s:%d:" #text ":%d\n"
#define cvpi_log_format_4(text) "%s:%d:" #text ":%d:%d\n"
#define cvpi_log_format_5(text) "%s:%d:" #text ":%d:%d:%d\n"
#define cvpi_log_format_6(text) "%s:%d:" #text ":%d:%d:%d:%d\n"

#if CVPI_ASYNC_LOGGING == 1
#define cvpi_log(n, format, ...)  (cvpi_log_print_threaded(n, sizeof(#format), #format, ## __VA_ARGS__))
#define cvpi_log_1(text, ...) (cvpi_log(1, cvpi_log_format_1(text), ## __VA_ARGS__))
#define cvpi_log_2(text, ...) (cvpi_log(2, cvpi_log_format_2(text), ## __VA_ARGS__))
#define cvpi_log_3(text, ...) (cvpi_log(3, cvpi_log_format_3(text), ## __VA_ARGS__))
#define cvpi_log_4(text, ...) (cvpi_log(4, cvpi_log_format_4(text), ## __VA_ARGS__))
#define cvpi_log_5(text, ...) (cvpi_log(5, cvpi_log_format_5(text), ## __VA_ARGS__))
#define cvpi_log_6(text, ...) (cvpi_log(6, cvpi_log_format_6(text), ## __VA_ARGS__))

#elif CVPI_NO_LOGGING == 1
#define cvpi_log(...)
#define cvpi_log_1(...)
#define cvpi_log_2(...)
#define cvpi_log_3(...)
#define cvpi_log_4(...)
#define cvpi_log_5(...)
#define cvpi_log_6(...)

#else  /* synchronous logging */
#define cvpi_log(n, format, ...) (fprintf(cvpi_log_file, (format), ## __VA_ARGS__))
#define cvpi_log_1(text, ...) (cvpi_log(cvpi_log_format_1(text), ## __VA_ARGS__))
#define cvpi_log_2(text, ...) (cvpi_log(cvpi_log_format_2(text), ## __VA_ARGS__))
#define cvpi_log_3(text, ...) (cvpi_log(cvpi_log_format_3(text), ## __VA_ARGS__))
#define cvpi_log_4(text, ...) (cvpi_log(cvpi_log_format_4(text), ## __VA_ARGS__))
#define cvpi_log_5(text, ...) (cvpi_log(cvpi_log_format_5(text), ## __VA_ARGS__))
#define cvpi_log_6(text, ...) (cvpi_log(cvpi_log_format_6(text), ## __VA_ARGS__))
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#else
#pragma message __FILE__ ": CVPI_BASE already defined."
#endif
