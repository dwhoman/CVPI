# add_executable(cvpi_tests cvpi_tests.c cvpi_egl_config_tests.c cvpi_image_tests.c)
#add_compile_options("-std=gnu++11" "-fpermissive") #  "-ggdb" "-O0"
#add_definitions("-DCMAKE_BUILD_TYPE=Debug")
add_executable(cvpi_tests_hyp cvpi_property_tests_remote.c)
include_directories(${PI_DIRS}) # ../googletest/googletest/include/ ../rapidcheck/extras/gtest/include  ../rapidcheck/include/
target_link_libraries(cvpi_tests_hyp ${CVPI_LIBS} cvpi-aux) # gtest  ${OpenCV_LIBS} rapidcheck
