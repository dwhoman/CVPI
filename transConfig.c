#ifndef transConfig_h
#include "transConfig.h"
#endif

#ifndef BITMAP_H
#include "bitmap.h"
#endif

#ifndef __eglplatform_h_
#include <EGL/eglplatform.h>
#endif

#ifndef __egl_h_
#include <EGL/egl.h>
#endif

#ifndef EGLEXT_BRCM_H
#include "eglext_brcm.h"
#endif

#ifndef _OPENVG_H
#include <VG/openvg.h>
#endif

#ifndef __LINUX_VIDEODEV2_H
#include <linux/videodev2.h>
#endif

/* returns the pixel format information for the given trans_part */
/* the pixel format is set in transConfig.h */
unsigned long pixel_format_name(enum trans_part part) {
  unsigned long a[] = 
#if TRANS_COLOR_FMT == TRANS_RGB_565
    {EGL_PIXEL_FORMAT_RGB_565_BRCM, VG_sRGB_565, V4L2_PIX_FMT_RGB565, &pf_VG_sRGB_565, 2};
#elif TRANS_COLOR_FMT == TRANS_RGB_888
  {EGL_PIXEL_FORMAT_XRGB_8888_BRCM, VG_sXRGB_8888, V4L2_PIX_FMT_BGR24, &pf_VG_sXRGB_8888, 3};
#elif TRANS_COLOR_FMT == TRANS_ARGB_8888
  {EGL_PIXEL_FORMAT_ARGB_8888_BRCM, VG_sRGBA_8888, V4L2_PIX_FMT_RGB32, &pf_VG_sARGB_8888, 4};
#endif
 return a[part];
}
