project(CVPI)

# required for linking
CMAKE_POLICY( SET CMP0003 OLD )

#TEST_BIG_ENDIAN(ENDIANESS)
#message("Endianess = ${ENDIANESS}")

set(BCM_LIBS bcm_host EGL GLESv2)
set(BCM_DIR /opt/vc/lib/)

# cmake version probably higher than necessary
cmake_minimum_required(VERSION 2.8.9 FATAL_ERROR)
# Options; CMake and CPP options are separate, pass options given to CMake to CPP
# CVPI_ASSEMBLY :: T/F; use inline assembly
if(CVPI_ASSEMBLY)
  add_definitions("-DCVPI_ASSEMBLY=1")
else()
  add_definitions("-DCVPI_ASSEMBLY=0")
endif()

# HAVE_BCM_HOST :: T/F; using Broadcom GPU, such as on the Raspberry Pi
if(HAVE_BCM_HOST)
  add_definitions("-DHAVE_BCM_HOST=1")
else()
  add_definitions("-DHAVE_BCM_HOST=0")
endif()
# CVPI_CAREFUL  :: T/F; check for all possible errors, or just for memory errors
if(CVPI_CAREFUL)
  add_definitions("-DCVPI_CAREFUL=1")
else()
  add_definitions("-DCVPI_CAREFUL=0")
endif()

# CVPI_LOGGING  :: 0,1,2; no logging, synchronous logging, asynchronous logging (requires pthreads)
if((CVPI_LOGGING EQUAL 1) OR (CVPI_LOGGING EQUAL 2))
  add_definitions("-DCVPI_LOGGING=${CVPI_LOGGING}")
else()
  add_definitions("-DCVPI_LOGGING=0")
endif()

include_directories(include include-aux)

#add_subdirectory(include)
#add_subdirectory(include-aux)

set(CVPI_LIBS cvpi -lm)
if(CMAKE_USE_PTHREADS_INIT)
  set(CVPI_LIBS ${CVPI_LIBS} pthread)
endif(CMAKE_USE_PTHREADS_INIT)

# LDFLAGS
# Raspberry Pi uses libraspberrypi, not found with pkg-config, look in /opt/vc/lib for files
if(HAVE_BCM_HOST AND (EXISTS "${BCM_DIR}"))
  set(PI_DIRS /opt/vc/include/ /opt/vc/include/EGL/ /opt/vc/include/interface/vmcs_host/linux/ /opt/vc/include/interface/vcos/pthreads/)
  set(CVPI_LIBS ${CVPI_LIBS} ${BCM_LIBS})
  link_directories(${BCM_DIR})
  include_directories(${PI_DIRS})
else()
  # other systems, such as Odroid, use Mesa, found with pkg-config as EGL and VG
  MESSAGE(FATAL_ERROR "Userland library required.")
endif()

add_subdirectory(src lib)
add_subdirectory(src-aux lib-aux)
add_subdirectory(tests)

#add_library(chicken-ffi openvg-chicken.scd cvpi-chicken.scd)
#target_link_libraries(chicken-ffi EGL GLESv2 bcm_host)
#add_executable(cvpi_test cvpi_test.c cvpi_egl_config_tests.c cvpi_egl_surface_functions.c cvpi_image_tests.c)
#add_executable(chicken-test cvpi_image_tests-chicken.scm)
#target_link_libraries(chicken-test EGL GLESv2 bcm_host)