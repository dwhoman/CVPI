#ifndef CVPI_BASE
#include "cvpi_base.h"
#endif

#ifndef _STDLIB_H
#include <stdlib.h>
#endif

#ifndef _STDIO_H
#include <stdio.h>
#endif

#ifndef _ERRNO_H
#include <errno.h>
#endif

/* for variadic macros */
#ifndef __STDARG_H
#include <stdarg.h>
#endif

FILE* cvpi_log_file_private = NULL;

int cvpi_log_file_set(cvpi_log_file_location location, 
		       const char* file_path) {
  if(location == cvpi_log_stdout) {
    cvpi_log_file_private = stdout;
    return 0;
  } else if(location == cvpi_log_stderr) {
    cvpi_log_file_private = stderr;
    return 0;
  } else {
    cvpi_log_file_private = fopen(file_path, "w");
    if(cvpi_log_file_private == NULL) {
      cvpi_log_file_private = cvpi_log_file_default;
      return errno;
    } else {
      return 0;
    }
  }
}
FILE* cvpi_log_file_get(void) {
  return cvpi_log_file_private;
}

int cvpi_log_file_unset(void) {
  if(cvpi_log_file_private != stdout && cvpi_log_file_private != stderr && fclose(cvpi_log_file_private)) {
    return errno;
  } else {
    cvpi_log_file_private = cvpi_log_file_default;
    return 0;
  }
}

#if CVPI_ASYNC_LOGGING == 1
typedef struct {
  int type;			/* printf format type */
  char* format;			/* printf format string */
  char* func;			/* __func__ */
  int line;			/* __LINE__ */
  char* text;			/* %s argument */
  int num1;			/* %d arguments */
  int num2;
  int num3;
  int num4;
} log_data;

static void print_threaded(void* data_) {
  log_data* data = (log_data*)data_;

  int status = pthread_detach(pthread_self());

  switch(data->type) {
  case 1:
    fprintf(cvpi_log_file, data->format, data->func, data->line);
    free(data->format);
    free(data->func);
    data->func = NULL;
    data->format = NULL;
    break;
  case 2:
    fprintf(cvpi_log_file, data->format, data->func, data->line, data->text);
    free(data->format);
    free(data->text);
    free(data->func);
    data->func = NULL;
    data->format = NULL;
    data->text = NULL;
    break;
  case 3:
    fprintf(cvpi_log_file, data->format, data->func, data->line, data->num1);
    free(data->format);
    free(data->func);
    data->func = NULL;
    data->format = NULL;
    break;
  case 4:
    fprintf(cvpi_log_file, data->format, data->func, data->line, data->num1, data->num2);
    free(data->format);
    free(data->func);
    data->func = NULL;
    data->format = NULL;
    break;
  case 5:
    fprintf(cvpi_log_file, data->format, data->func, data->line, data->num1, 
	    data->num2, data->num3);
    free(data->format);
    free(data->func);
    data->func = NULL;
    data->format = NULL;
    break;
  case 6:
    fprintf(cvpi_log_file, data->format, data->func, data->line, data->num1, 
	    data->num2, data->num3, data->num4);
    free(data->format);
    free(data->func);
    data->func = NULL;
    data->format = NULL;
    break;
  default:
    fprintf(cvpi_log_file, "Bad cvpi_log_format\n");
    break;
  }

  free(data);

  if(status != 0) {
    fprintf(stderr, "print_threaded failed to detach with status = %d\n", status);
    abort();
  }
}

void cvpi_log_print_threaded(const int type, const int format_size, 
			     const char * format, ...) {
 log_data* data = malloc(sizeof(*data));
  
  data->type = type;

  va_list argptr;
  va_start(argptr, format);
  char * func;			/* data->func */
  char* text;			/* data->text */

  switch(type) {
  case 1:
    data->type = 1;
    data->format = malloc(format_size);
    strcpy(data->format, format);
    func = va_arg(argptr, char *);
    data->func = malloc(1+strlen(func));
    strcpy(data->func, func);
    data->line = va_arg(argptr, int);
    break;
  case 2:
    data->type = 2;
    data->format = malloc(format_size);
    strcpy(data->format, format);
    func = va_arg(argptr, char *);
    data->func = malloc(1+strlen(func));
    strcpy(data->func, func);
    data->line = va_arg(argptr, int);
    text = va_arg(argptr, char *);
    data->text = malloc(1+strlen(text));
    strcpy(data->text, text);
    break;
  case 3:
    data->type = 3;
    data->format = malloc(format_size);
    strcpy(data->format, format);
    func = va_arg(argptr, char *);
    data->func = malloc(1+strlen(func));
    strcpy(data->func, func);
    data->line = va_arg(argptr, int);
    data->num1 = va_arg(argptr, int);
    break;
  case 4:
    data->type = 4;
    data->format = malloc(format_size);
    strcpy(data->format, format);
    func = va_arg(argptr, char *);
    data->func = malloc(1+strlen(func));
    strcpy(data->func, func);
    data->line = va_arg(argptr, int);
    data->num1 = va_arg(argptr, int);
    data->num2 = va_arg(argptr, int);
    break;
  case 5:
    data->type = 5;
    data->format = malloc(format_size);
    strcpy(data->format, format);
    func = va_arg(argptr, char *);
    data->func = malloc(1+strlen(func));
    strcpy(data->func, func);
    data->line = va_arg(argptr, int);
    data->num1 = va_arg(argptr, int);
    data->num2 = va_arg(argptr, int);
    data->num3 = va_arg(argptr, int);
    break;
  case 6:
    data->type = 6;
    data->format = malloc(format_size);
    strcpy(data->format, format);
    func = va_arg(argptr, char *);
    data->func = malloc(1+strlen(func));
    strcpy(data->func, func);
    data->line = va_arg(argptr, int);
    data->num1 = va_arg(argptr, int);
    data->num2 = va_arg(argptr, int);
    data->num3 = va_arg(argptr, int);
    data->num4 = va_arg(argptr, int);
    break;
  default:
    data->type = 0;
    break;
  }

  va_end(argptr);

  pthread_t id;

  pthread_create(&id, 0, print_threaded, (void*)data);
  pthread_join(id, 0);
}
#endif	/* CVPI_ASYNC_LOGGING == 1 */
