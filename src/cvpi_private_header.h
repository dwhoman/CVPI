#ifndef CVPI_PRIVATE_HEADER
#define CVPI_PRIVATE_HEADER 1

#define freeSafe(pointer)\
      do {\
      if(pointer != NULL) {\
      free(pointer);\
      pointer = NULL;\
      }\
      } while(0)

#else
#pragma message __FILE__ ": CVPI_PRIVATE_HEADER already defined."
#endif
