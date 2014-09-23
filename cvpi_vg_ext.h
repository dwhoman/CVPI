#ifndef CVPI_VG_EXT
#define CVPI_VG_EXT

char* cvpi_vg_error_string(unsigned long error);
#else
#pragma message __FILE__ ": CVPI_VG_EXT already defined."
#endif
