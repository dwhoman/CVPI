/*
    This file is part of CVPI.

    Copyright (C) 2015 Devin Homan

    This program is free software: you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public License
    as published by the Free Software Foundation, either version 3 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/** \file cvpi_base.h

    \brief Contains some conventions used throughout the project, as
well as logging functions. */
#ifndef CVPI_BASE
#define CVPI_BASE 1

#ifndef _STDIO_H
#include <stdio.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/** \brief if a function is returning a number representing a Boolean,
   then the function should follow this convention. */
#define CVPI_TRUE_TEST(x) ((x) != 0)
#define CVPI_FALSE_TEST(x) ((x) == 0)
#define CVPI_TRUE 1
#define CVPI_FALSE 0
#define CVPI_NOT(x) !(x)

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
     CVPI_LOG_FILE_DEFAULT, else 0 is returned. */
  int cvpi_log_file_set(cvpi_log_file_location location,
			const char* file_path);
  /* Close the file currently being used for logging. Returns errno if
     there is an error while closing, else points the log location to
     NULL and returns 0. */
  int cvpi_log_file_unset(void);

  /* cvpi_log_file_get is used exclusively as the first argument to
     fprintf for all logging. cvpi_log_file_get simply returns the
     value of a private variable. Using CVPI_LOG_FILE in its place is
     preferred. */
  FILE* cvpi_log_file_get(void);
#define CVPI_LOG_FILE cvpi_log_file_get()

#define CVPI_LOG_FILE_DEFAULT stderr

  /* TODO change logging to Strategy pattern to allow new text patterns to be
     passed in or created by the client, see Patterns in C p. 22 */

  /* Predefined formatting functions for use with cvpi_log.
  */
#define cvpi_log_format_1(text) "%s:%d:" #text "\n"
#define cvpi_log_format_2(text) "%s:%d:" #text ":%s\n"
#define cvpi_log_format_3(text) "%s:%d:" #text ":%d\n"
#define cvpi_log_format_4(text) "%s:%d:" #text ":%d:%d\n"
#define cvpi_log_format_5(text) "%s:%d:" #text ":%d:%d:%d\n"
#define cvpi_log_format_6(text) "%s:%d:" #text ":%d:%d:%d:%d\n"

  /* logging functions, prefer cvpi_log_# to cvpi_log */

  /* logging done using a detached POSIX threaded function */
#if CVPI_LOGGING == 2
#pragma message __FILE__ ": Asynchronous logging enabled."
#define cvpi_log(n, format, ...)  (cvpi_log_print_threaded(n, sizeof(#format), #format, ## __VA_ARGS__))
#define cvpi_log_1(text, ...) (cvpi_log(1, cvpi_log_format_1(text), ## __VA_ARGS__))
#define cvpi_log_2(text, ...) (cvpi_log(2, cvpi_log_format_2(text), ## __VA_ARGS__))
#define cvpi_log_3(text, ...) (cvpi_log(3, cvpi_log_format_3(text), ## __VA_ARGS__))
#define cvpi_log_4(text, ...) (cvpi_log(4, cvpi_log_format_4(text), ## __VA_ARGS__))
#define cvpi_log_5(text, ...) (cvpi_log(5, cvpi_log_format_5(text), ## __VA_ARGS__))
#define cvpi_log_6(text, ...) (cvpi_log(6, cvpi_log_format_6(text), ## __VA_ARGS__))

#elif CVPI_LOGGING == 1  /* synchronous logging */
#pragma message __FILE__ ": Synchronous logging enabled."
#define cvpi_log(n, format, ...) (fprintf(CVPI_LOG_FILE, (format), ## __VA_ARGS__))
#define cvpi_log_1(text, ...) (cvpi_log(1, cvpi_log_format_1(text), ## __VA_ARGS__))
#define cvpi_log_2(text, ...) (cvpi_log(2, cvpi_log_format_2(text), ## __VA_ARGS__))
#define cvpi_log_3(text, ...) (cvpi_log(3, cvpi_log_format_3(text), ## __VA_ARGS__))
#define cvpi_log_4(text, ...) (cvpi_log(4, cvpi_log_format_4(text), ## __VA_ARGS__))
#define cvpi_log_5(text, ...) (cvpi_log(5, cvpi_log_format_5(text), ## __VA_ARGS__))
#define cvpi_log_6(text, ...) (cvpi_log(6, cvpi_log_format_6(text), ## __VA_ARGS__))

#else  /* no logging, but checks can still be made with CVPI_CAREFUL == 1 */
#pragma message __FILE__ ": Logging not enabled."
#define cvpi_log(...)
#define cvpi_log_1(...)
#define cvpi_log_2(...)
#define cvpi_log_3(...)
#define cvpi_log_4(...)
#define cvpi_log_5(...)
#define cvpi_log_6(...)
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#else
#pragma message __FILE__ ": CVPI_BASE already defined."
#endif
