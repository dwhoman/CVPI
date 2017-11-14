# add_executable(cvpi_tests cvpi_tests.c cvpi_egl_config_tests.c cvpi_image_tests.c)
add_compile_options("-std=gnu++11") #  "-ggdb" "-O0"  "-fpermissive"
#add_definitions("-DCMAKE_BUILD_TYPE=Debug")
#add_subdirectory(googletest gtest_lib)
add_subdirectory(rapidcheck rapidcheck_lib)
find_package( OpenCV REQUIRED )
add_executable(cvpi_tests_rapid cvpi_property_tests_rc.cpp)
include_directories(${PI_DIRS} rapidcheck/include/) # rapidcheck/extras/gtest/include  googletest/googletest/include/
target_link_libraries(cvpi_tests_rapid ${CVPI_LIBS} cvpi-aux ${OpenCV_LIBS} rapidcheck) # gtest
