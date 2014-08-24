#ifndef transConfig_h
#define transConfig_h 1

/* cross cutting settings between egl, openvg, bitmap, etc  */

/* color format used */
#define TRANS_RGB_565 1
//#define TRANS_BGR_565 __LINE__
#define TRANS_RGB_888 2
//#define TRANS_BGR_888 __LINE__
#define TRANS_ARGB_8888 3
//#define TRANS_BGRA_8888 __LINE__

#define TRANS_COLOR_FMT TRANS_ARGB_8888

#define TRANS_WIDTH 1280
#define TRANS_HEIGHT 960

/* make sure egl, openvg, bmp, v4l2, etc. are all using the same pixel format */
enum trans_part {TRANS_EGL, TRANS_VG, TRANS_V4L2, TRANS_BMP, TRANS_BYTES};

unsigned long pixel_format_name(enum trans_part part);
#endif
