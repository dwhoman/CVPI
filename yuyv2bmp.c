#ifndef transConfig_h
#include "transConfig.h"
#endif

#ifndef EGL_CONFIG
#include "eglConfig.h"
#endif

#ifndef VG_CONFIG
#include "vgConfig.h"
#endif

#ifndef _OPENVG_H
#include <VG/openvg.h>
#endif

#ifndef BITMAP_H
#include "bitmap.h"
#endif

#ifndef _STDIO_H
#include <stdio.h>
#endif

#ifndef _STDLIB_H
#include <stdlib.h>
#endif

int main(int argc, char** argv) {
  if(argc >= 3) {
    FILE * input = fopen(argv[1],"r");

    FILE * output = fopen(argv[2], "w");

    fseek(input, 0L, SEEK_END);
    int f_size = ftell(input);		/* file size */
    fseek(input, 0L, SEEK_SET);

    void* image_in = malloc(f_size);
    void* image_out = malloc(f_size);

    fread(image_in, 1, f_size, input);


    egl_deletable* takedown;
    takedown = setupEGL(1280, 960, 0);

    vgWritePixels(image_in, 2, VG_sRGBA_8888_PRE, 0, 0, 1280, 960);

    vgReadPixels(image_out, 2, VG_sRGBA_8888_PRE, 0, 0, 1280, 960);

    fwrite(image_out, 1, f_size, output);
    fflush(output);

    takedownEGL(takedown);
    free(image_in);
    free(image_out);

    fclose(output);

    fclose(input);
  }

  return 0;
}
