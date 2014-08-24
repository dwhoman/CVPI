/* 15th bit of AIRCR indicates the endianness: 0 = little, 1 = big */
#include <stdio.h>
#include <endian.h>

int main() {
#ifdef __BYTE_ORDER
  #if __BYTE_ORDER == __BIG_ENDIAN
  printf("Big Endian\n");
  #elif __BYTE_ORDER == __LITTLE_ENDIAN
  printf("Little Endian\n");
  #else 
  printf("Endian determination failed\n");
  #endif
#endif

  return 0;
}
