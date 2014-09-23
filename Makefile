local=/home/pi/userland/build/raspberry/release/opt/vc

# system libraries
BCM=-L /opt/vc/lib -I /opt/vc/include/ -I /opt/vc/include/EGL/ -I /opt/vc/include/interface/vmcs_host/linux/ -lbcm_host
EGL=-L /opt/vc/lib -I /opt/vc/include/ -I /opt/vc/include/interface/vcos/pthreads/ -lEGL -lGLESv2

# local libraries
#BCM=-I $(local)/lib/opt/vc/lib/ -I $(local)/lib/opt/vc/include/ -I $(local)/lib/opt/vc/include/EGL/ -I $(local)/lib/opt/vc/include/interface/vmcs_host/linux/ -L $(local)/lib/opt/vc/lib/
#EGL=-I $(local)/lib/opt/vc/lib/ -I $(local)/lib/opt/vc/include/ -I $(local)/lib/opt/vc/include/interface/vcos/pthreads/ -L $(local)/lib/opt/vc/lib/ -L $(local)/include/

# remove unused code
size_C=-fdata-sections -ffunction-sections
SIZE_L=-Wl,--gc-sections
CC=gcc -Wall
# -std=gnu99 


# libEGL.so => /opt/vc/lib/libEGL.so (0xb6f67000) /home/pi/userland/build/raspberry/release/opt/vc/lib/libEGL.so
# libGLESv2.so => /opt/vc/lib/libGLESv2.so (0xb6f4a000) /home/pi/userland/build/raspberry/release/opt/vc/lib/libGLESv2.so
# libbcm_host.so => /opt/vc/lib/libbcm_host.so (0xb6f30000) /home/pi/userland/build/raspberry/release/opt/vc/lib/libbcm_host.so
# libc.so.6 => /lib/arm-linux-gnueabihf/libc.so.6 (0xb6e01000)
# libvchiq_arm.so => /opt/vc/lib/libvchiq_arm.so (0xb6df3000) /home/pi/userland/build/raspberry/release/opt/vc/lib/libvchiq_arm.so
# libvcos.so => /opt/vc/lib/libvcos.so (0xb6de0000) /home/pi/userland/build/raspberry/release/opt/vc/lib/libvcos.so
# libm.so.6 => /lib/arm-linux-gnueabihf/libm.so.6 (0xb6d6f000)
# libpthread.so.0 => /lib/arm-linux-gnueabihf/libpthread.so.0 (0xb6d50000)
# libdl.so.2 => /lib/arm-linux-gnueabihf/libdl.so.2 (0xb6d45000)
# librt.so.1 => /lib/arm-linux-gnueabihf/librt.so.1 (0xb6d36000)
# /lib/ld-linux-armhf.so.3 (0xb6fa8000)

# modified library code for debugging purposes
#custom=$(local)/lib/libEGL.so $(local)/lib/libGLESv2.so $(local)/lib/libbcm_host.so $(local)/lib/libvchiq_arm.so $(local)/lib/libvcos.so /lib/arm-linux-gnueabihf/libc.so.6 /lib/arm-linux-gnueabihf/libm.so.6 /lib/arm-linux-gnueabihf/libpthread.so.0 /lib/arm-linux-gnueabihf/libdl.so.2 /lib/arm-linux-gnueabihf/librt.so.1 /lib/ld-linux-armhf.so.3

custom= 


# main program suite
cvpi_egl_config.o: cvpi_egl_config.c
	$(CC) $(custom) $(BCM) $(EGL) -c $^
cvpi_vg_ext.o: cvpi_vg_ext.c
	$(CC) $(custom) $(BCM) $(EGL) -c $^
cvpi_camera_setup.o: cvpi_camera_setup.c
	$(CC) -c $^
cvpi_image_headers.o: cvpi_image_headers.c
	$(CC) $(custom) $(BCM) $(EGL) -c $^
cvpi_egl_surface_functions.o: cvpi_egl_surface_functions.c
	$(CC) $(custom) $(BCM) $(EGL) -c $^
cvpi_egl_surface_functions_expand: cvpi_egl_surface_functions.c
	$(CC) -E $(custom) $(BCM) $(EGL) -o cvpi_egl_surface_functions_expand.c $^
cvpi_image_functions.o: cvpi_image_functions.c
	$(CC) $(custom) $(BCM) $(EGL) -c $^
cvpi: cvpi_egl_config.o cvpi_vg_ext.o cvpi_camera_setup.o\
	 cvpi_image_headers.o cvpi_egl_surface_functions.o cvpi_image_functions.o
clean:
	rm cvpi_egl_config.o cvpi_vg_ext.o cvpi_camera_setup.o cvpi_image_headers.o\
	 cvpi_egl_surface_functions.o cvpi_image_functions.o

# testing programs
cvpi_egl_config_tests.o: cvpi_egl_config_tests.c
	$(CC) $(custom) $(BCM) $(EGL) -c $^
cvpi_image_tests.o: cvpi_image_tests.c
	$(CC) $(custom) $(BCM) $(EGL) -c $^
cvpi_tests.o: cvpi_tests.c
	$(CC) $(custom) $(BCM) $(EGL) -c $^

cvpi_test: cvpi cvpi_egl_config_tests.o cvpi_image_tests.o cvpi_tests.o
	$(CC) $(custom) $(BCM) $(EGL) -o cvpi_test cvpi_egl_config_tests.o cvpi_image_tests.o cvpi_tests.o cvpi_egl_config.o cvpi_vg_ext.o cvpi_camera_setup.o cvpi_image_headers.o cvpi_egl_surface_functions.o cvpi_image_functions.o

camera_setup_test: camera_setup.c camera_setup_test.c bitmap.c
	$(CC) $(custom) $(BCM) $(EGL) -Wall -o camera_setup_test $^

yuv_convert: image_functions.c yuv_convert.c cvpi_egl_config.c transConfig.c vgConfig.c bitmap.c
	$(CC) $(custom) $(BCM) $(EGL) -o yuv_convert $^

# test the eglConfig functions
#-Wl,-rpath,$(local)
eglconfigtest: eglInterface cvpi_egl_config.c eglConfigTest.c vgConfig.c
	$(CC) $(custom) -Wall -o eglGetConfigTest cvpi_egl_config.c vgConfig.c eglConfigTest.c $(BCM) $(EGL)

# returns a CSV string to stdout of EGL configurations
eglGetConfigs: 
	$(CC) -Wall $(EGL) -o eglGetConfigs eglGetConfigs.c