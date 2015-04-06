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

#ifndef CVPI_PRIVATE_HEADER
#include "cvpi_private_header.h"
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
  if(cvpi_log_file_private == NULL) {
    cvpi_log_file_private = cvpi_log_file_default;
  }
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

#if CVPI_LOGGING == 2
typedef struct {
  int type;			/* printf format type */
  char* format;			/* printf format string */
  char* func;			/* __func__ or __FILE__ */
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

  if(data != NULL) {
    switch(data->type) {
    case 1:
      if(data->format != NULL && data->func != NULL && data->line != NULL) {
	fprintf(cvpi_log_file, data->format, data->func, data->line);
	freeSafe(data->format);
	freeSafe(data->func);
	freeSafe(data->line);
      }
      break;
    case 2:
      if(data->format != NULL && data->func != NULL && data->line != NULL && data->text != NULL) {
	fprintf(cvpi_log_file, data->format, data->func, data->line, data->text);
	freeSafe(data->format);
	freeSafe(data->text);
	freeSafe(data->func);
      }
      break;
    case 3:
      if(data->format != NULL && data->func != NULL && data->line != NULL && data->num1 != NULL) {
	fprintf(cvpi_log_file, data->format, data->func, data->line, data->num1);
	freeSafe(data->format);
	freeSafe(data->func);
	freeSafe(data->line);
	freeSafe(data->num1);
      }
      break;
    case 4:
      if(data->format != NULL && data->func != NULL && data->line != NULL && data->num1 != NULL && data->num2 != NULL) {
	fprintf(cvpi_log_file, data->format, data->func, data->line, data->num1, data->num2);
	freeSafe(data->format);
	freeSafe(data->func);
	freeSafe(data->line);
	freeSafe(data->num1);
	freeSafe(data->num2);
      }
      break;
    case 5:
      if(data->format != NULL && data->func != NULL && data->line != NULL && data->num1  != NULL && 
	 data->num2  != NULL &&  data->num3  != NULL) {
	fprintf(cvpi_log_file, data->format, data->func, data->line, data->num1, 
		data->num2, data->num3);
	freeSafe(data->format);
	freeSafe(data->func);
	freeSafe(data->line);
	freeSafe(data->num1);
	freeSafe(data->num2);
	freeSafe(data->num3);
      }
      break;
    case 6:
      if(data->format != NULL && data->func != NULL && data->line != NULL && data->num1 != NULL && 
		data->num2 != NULL && data->num3 != NULL && data->num4) {
	fprintf(cvpi_log_file, data->format, data->func, data->line, data->num1, 
		data->num2, data->num3, data->num4);
	freeSafe(data->format);
	freeSafe(data->func);
	freeSafe(data->line);
	freeSafe(data->num1);
	freeSafe(data->num2);
	freeSafe(data->num3);
	freeSafe(data->num4);
      }
      break;
    default:
      fprintf(cvpi_log_file, "Bad cvpi_log_format\n");
      break;
    }
    freeSafe(data);
  }

  if(status != 0) {
    fprintf(stderr, "print_threaded failed to detach with status = %d\n", status);
    abort();
  }
}

void cvpi_log_print_threaded(const int type, const int format_size, 
			     const char * format, ...) {
#define ERROR_CLEANUP cvpi_log_print_threaded_cleanup

  if(format == NULL) {
    fprintf(stderr, "%s:%d: Format is NULL\n", __func__, __LINE__);
    return;
  }
    
  log_data* data = malloc(sizeof(*data));
  if(data == NULL) {
    fprintf(stderr, "%s:%d: Malloc failed\n", __func__, __LINE__);
   return;
  }

  data->type = type;
  data->format = NULL;
  data->func = NULL;
  data->text = NULL;

  va_list argptr;
  va_start(argptr, format);
  char * func;			/* data->func */
  char* text;			/* data->text */

  switch(type) {
  case 1:
    data->type = 1;
    data->format = malloc(format_size);
    if(data->format == NULL) {
      fprintf(stderr, "%s:%d: Malloc failed\n", __func__, __LINE__);
      goto ERROR_CLEANUP;
    }
    strcpy(data->format, format);
    func = va_arg(argptr, char *);
    data->func = malloc(1+strlen(func));
    if(data->func == NULL) {
      fprintf(stderr, "%s:%d: Malloc failed\n", __func__, __LINE__);
      goto ERROR_CLEANUP;
    }
    strcpy(data->func, func);
    data->line = va_arg(argptr, int);
    break;
  case 2:
    data->type = 2;
    data->format = malloc(format_size);
    if(data->format == NULL) {
      fprintf(stderr, "%s:%d: Malloc failed\n", __func__, __LINE__);
      goto ERROR_CLEANUP;
    }
    strcpy(data->format, format);
    func = va_arg(argptr, char *);
    data->func = malloc(1+strlen(func));
    if(data->func == NULL) {
      fprintf(stderr, "%s:%d: Malloc failed\n", __func__, __LINE__);
      goto ERROR_CLEANUP;
    }
    strcpy(data->func, func);
    data->line = va_arg(argptr, int);
    text = va_arg(argptr, char *);
    data->text = malloc(1+strlen(text));
    if(data->text == NULL) {
      fprintf(stderr, "%s:%d: Malloc failed\n", __func__, __LINE__);
      goto ERROR_CLEANUP;
    }
    strcpy(data->text, text);
    break;
  case 3:
    data->type = 3;
    data->format = malloc(format_size);
    if(data->format == NULL) {
      fprintf(stderr, "%s:%d: Malloc failed\n", __func__, __LINE__);
      goto ERROR_CLEANUP;
    }
    strcpy(data->format, format);
    func = va_arg(argptr, char *);
    data->func = malloc(1+strlen(func));
    if(data->func == NULL) {
      fprintf(stderr, "%s:%d: Malloc failed\n", __func__, __LINE__);
      goto ERROR_CLEANUP;
    }
    strcpy(data->func, func);
    data->line = va_arg(argptr, int);
    data->num1 = va_arg(argptr, int);
    break;
  case 4:
    data->type = 4;
    data->format = malloc(format_size);
    if(data->format == NULL) {
      fprintf(stderr, "%s:%d: Malloc failed\n", __func__, __LINE__);
      goto ERROR_CLEANUP;
    }
    strcpy(data->format, format);
    func = va_arg(argptr, char *);
    data->func = malloc(1+strlen(func));
    if(data->func == NULL) {
      fprintf(stderr, "%s:%d: Malloc failed\n", __func__, __LINE__);
      goto ERROR_CLEANUP;
    }
    strcpy(data->func, func);
    data->line = va_arg(argptr, int);
    data->num1 = va_arg(argptr, int);
    data->num2 = va_arg(argptr, int);
    break;
  case 5:
    data->type = 5;
    data->format = malloc(format_size);
    if(data->format == NULL) {
      fprintf(stderr, "%s:%d: Malloc failed\n", __func__, __LINE__);
      goto ERROR_CLEANUP;
    }
    strcpy(data->format, format);
    func = va_arg(argptr, char *);
    data->func = malloc(1+strlen(func));
    if(data->func == NULL) {
      fprintf(stderr, "%s:%d: Malloc failed\n", __func__, __LINE__);
      goto ERROR_CLEANUP;
    }
    strcpy(data->func, func);
    data->line = va_arg(argptr, int);
    data->num1 = va_arg(argptr, int);
    data->num2 = va_arg(argptr, int);
    data->num3 = va_arg(argptr, int);
    break;
  case 6:
    data->type = 6;
    data->format = malloc(format_size);
    if(data->format == NULL) {
      fprintf(stderr, "%s:%d: Malloc failed\n", __func__, __LINE__);
      goto ERROR_CLEANUP;
    }
    strcpy(data->format, format);
    func = va_arg(argptr, char *);
    data->func = malloc(1+strlen(func));
    if(data->func == NULL) {
      fprintf(stderr, "%s:%d: Malloc failed\n", __func__, __LINE__);
      goto ERROR_CLEANUP;
    }
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

  return;

 ERROR_CLEANUP:
  freeSafe(data->format);
  freeSafe(data->func);
  freeSafe(data->text);  
  freeSafe(data);
#undef ERROR_CLEANUP
}
#endif	/* CVPI_LOGGING == 2 */
