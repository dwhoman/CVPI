#include <stdio.h>
#include <stdlib.h>

#define IMAGES_CORRECT_DIR "./test_images_correct/"
#define IMAGES_TMP_DIR "./test_images_tmp/"

int test_yuyv2yuva() {
  char* yuyv2yuva_input = IMAGES_CORRECT_DIR "mark1_black.yuv";
  char* yuyv2yuva_correct_result = IMAGES_CORRECT_DIR "mark1_black_yuva.yuv";
  char* yuyv2yuva_tmp_result = IMAGES_TMP_DIR "mark1_black_yuva.yuv";

  FILE* input = fopen(yuyv2yuva_input, "r");
  FILE* correct = fopen(yuyv2yuva_correct_result, "r");
  FILE* ouput = fopen(yuyv2yuva_tmp_result, "w");

  void* input_data;


  size_t input_size = fread();
  
}

int main() {

  return 0;
}
