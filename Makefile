local=/home/pi/userland/build/raspberry/release/opt/vc/lib
BCM=-L /opt/vc/lib -I /opt/vc/include/ -I /opt/vc/include/EGL/ -I /opt/vc/include/interface/vmcs_host/linux/ -lbcm_host
#BCM=-I $(local)/opt/vc/lib -I $(local)/opt/vc/include/ -I $(local)/opt/vc/include/EGL/ -I $(local)/opt/vc/include/interface/vmcs_host/linux/ -L $(local)/opt/vc/lib
EGL=-L /opt/vc/lib -I /opt/vc/include/ -I /opt/vc/include/interface/vcos/pthreads/ -lEGL -lGLESv2
#EGL=-I $(local)/opt/vc/lib -I $(local)/opt/vc/include/ -I $(local)/opt/vc/include/interface/vcos/pthreads/ -L $(local)/opt/vc/lib
# remove unused code
size_C=-fdata-sections -ffunction-sections
SIZE_L=-Wl,--gc-sections
CC=gcc -g 
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

custom=$(local)/libEGL.so $(local)/libGLESv2.so $(local)/libbcm_host.so $(local)/libvchiq_arm.so $(local)/libvcos.so /lib/arm-linux-gnueabihf/libc.so.6 /lib/arm-linux-gnueabihf/libm.so.6 /lib/arm-linux-gnueabihf/libpthread.so.0 /lib/arm-linux-gnueabihf/libdl.so.2 /lib/arm-linux-gnueabihf/librt.so.1 /lib/ld-linux-armhf.so.3

# returns a CSV string to stdout of EGL configurations
eglGetConfigs: 
	$(CC) -Wall $(EGL) -o eglGetConfigs eglGetConfigs.c

# test the eglConfig functions
#-Wl,-rpath,$(local)
eglconfigtest: eglInterface eglConfig.c eglConfigTest.c vgConfig.c
	$(CC) $(custom) -Wall -o eglGetConfigTest eglConfig.c vgConfig.c eglConfigTest.c $(BCM) $(EGL)
eglconfigtestlinkc: eglInterface eglConfig.c eglConfigTest.c
	$(CC) -v -I /home/pi/userland/build/raspberry/release/opt/vc/include/ -I /home/pi/userland/build/raspberry/release/opt/vc/include/interface/vcos/pthreads/ -I /home/pi/userland/build/raspberry/release/opt/vc/include/interface/vmcs_host/linux/ -I /home/pi/userland/build/raspberry/release/opt/vc/include/EGL/ -o eglConfigTest -Wall $(custom) eglConfig.c eglConfigTest.c
#	ld $(custom) eglConfig.o eglConfigTest.o -o eglConfigTest --entry main
eglInterface: eglCreateConfigHeader
	./eglCreateConfigHeader > eglGeneratedConfigHeader.h;
#	cat eglConfig.h >> eglGeneratedConfigHeader.h;
eglCreateConfigHeader: eglCreateConfigHeader.c
	$(CC) $(EGL) -o eglCreateConfigHeader eglCreateConfigHeader.c
eglconfigexpand: eglConfig.c
	$(CC) $(BCM) $(EGL) -E eglConfig.c -o eglConfig_expanded.c
eglconfigtestexpand: eglConfigTest.c
	$(CC) $(BCM) $(EGL) -E eglConfigTest.c -o eglConfigTest_expanded.c
simple_gl_example: simple_gl_example.cpp
	g++ -lX11 -lEGL -lGLESv2 simple_gl_example.cpp $(EGL) $(BCM) -g -o egl_example -O3

eglConfig.o: eglConfig.c
	$(CC) $(BCM) $(EGL) -c $^
vgConfig.o: vgConfig.c
	$(CC) $(BCM) $(EGL) -c $^
camera_setup.o: camera_setup.c
	$(CC) $(BCM) $(EGL) -c $^
transConfig.o: transConfig.c
	$(CC) $(BCM) $(EGL) -c $^
main_loop.o: main_loop.c
	$(CC) $(BCM) $(EGL) -c $^
bitmap.o: bitmap.c
	$(CC) $(BCM) $(EGL) -c $^
main_loop_expand: main_loop.c
	$(CC) $(BCM) $(EGL) -o main_loop_expanded.c -E $^

capture_frame: eglInterface main_loop.o eglConfig.o vgConfig.o camera_setup.o transConfig.o bitmap.o
	$(CC) $(BCM) $(EGL) -lv4l2 -o capture_frame eglConfig.o vgConfig.o camera_setup.o transConfig.o bitmap.o main_loop.o
clean:
	rm main_loop.o eglConfig.o vgConfig.o camera_setup.o transConfig.o bitmap.o

camera_setup_expand: camera_setup.c
	$(CC) -o camera_setup_expand.c -E $^

yuyv2bmp: yuyv2bmp.c
	$(CC) $(BCM) $(EGL) -o yuyv2bmp $^