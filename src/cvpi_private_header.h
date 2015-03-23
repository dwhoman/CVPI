#ifndef CVPI_PRIVATE_HEADER
#define CVPI_PRIVATE_HEADER 1

#define free_safe(ptr) do{if(ptr != NULL){free(ptr); ptr=NULL;}}while(0)

#else
#pragma message __FILE__ ": CVPI_PRIVATE_HEADER already defined."
#endif
