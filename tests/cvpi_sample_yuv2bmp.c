#include <stdio.h>
#include <stdlib.h>
#include "cvpi.h"

#define seek_error_check(seeker)\
  do {\
  int val = seeker;\
  if(val == -1) {\
  cvpi_log_3("fseek error, errno = ", __func__, __LINE__, errno);\
  goto TAKEDOWN;\
  }} while(0)

int main(int argc, char** argv) {
  if(argc < 5) {
    fprintf(stderr, "Too few arguments.\n");
    return 1;
  }

  int width = atoi(argv[1]);
  int height = atoi(argv[2]);

  if(width < 1 || height < 1) {
    fprintf(stderr, "%s: Invalid size\n", __FILE__);
    return 1;
  }

  FILE* input_file = NULL;
  FILE* output_file = NULL;

  cvpi_pixel* image_data = NULL;

  int not_flushed;
  size_t size_check;

  input_file = fopen(argv[3], "r");
  if(input_file == NULL) {
    fprintf(stderr, "unable to open input file\n");
    goto TAKEDOWN;
  }

  output_file = fopen(argv[4], "w");
  if(output_file == NULL) {
    fprintf(stderr, "unable to open output file\n");
    goto TAKEDOWN;
  }

  CVPI_BOOL return_value = cvpi_bmp_header_alloc_write(output_file, width, height, 100, CVPI_BMP_DEFAULT);
  if(CVPI_FALSE_TEST(return_value)) {
    fprintf(stderr, "Failed to create BMP header\n");
    goto TAKEDOWN;
  }

  seek_error_check(fseek(input_file, 0L, SEEK_END));
  size_t file_size = ftell(input_file);		/* input file size */
  seek_error_check(file_size);
  seek_error_check(fseek(input_file, 0L, SEEK_SET));

  image_data = malloc(file_size);
  if(image_data == NULL) {
    fprintf(stderr, "image_data malloc failed\n");
    goto TAKEDOWN;
  }

  size_check = fread(image_data, 1, file_size, input_file);
  not_flushed = fflush(input_file);
  if(size_check != file_size || not_flushed) {
    fprintf(stderr, "Failed to read input file\n");
    goto TAKEDOWN;
  }

  CVPI_BOOL avuy2argb_ret = cvpi_avuy2argb(image_data, image_data, width, height);
  if(CVPI_FALSE_TEST(avuy2argb_ret)) {
    fprintf(stderr, "avuy2argb failed\n");
    goto TAKEDOWN;
  }

  size_check = fwrite(image_data, 1, file_size, output_file);
  not_flushed = fflush(output_file);
  if(size_check != file_size || not_flushed) {
    fprintf(stderr, "Failed to write to output file\n");
    goto TAKEDOWN;
  }

 TAKEDOWN:
  if(output_file != NULL) {
    fclose(output_file);  
  }

  if(input_file != NULL) {
    fclose(input_file);
  }

  if(image_data != NULL) {
    free(image_data);
  }
}
