TEST_PROGS = cvpi_tests
noinst_PROGRAMS = $(TEST_PROGS)

cvpi_tests_CPPFLAGS = -I$(top_srcdir) -I$(top_srcdir)/include/ $(CVPI_RASPBERRY_CPPFLAGS)
cvpi_tests_LDFLAGS = $(CVPI_RASPBERRY_LDFLAGS)

cvpi_tests_LDADD = ../src/libcvpi.la
#cvpi_tests_LDADD += ../bindings/libcvpi_chicken.la
#cvpi_tests_LDADD += ../bindings/libvg_chicken.la

cvpi_tests_SOURCES = \
	cvpi_tests.c \
	cvpi_egl_config_tests.c \
	cvpi_egl_config_tests.h \
	cvpi_egl_surface_functions.c \
	cvpi_egl_surface_functions.h \
	cvpi_image_tests.h \
	cvpi_image_tests.c

#cvpi_tests_SOURCES = cvpi_image_tests-chicken.scm

#.scm.o:

#OBJEXT=o
#LINK=csc -o $@

#%.o: %.scm
#	csc -c $< -o $@

check_SCRIPTS = cvpi_tests.sh
TESTS = $(check_SCRIPTS)

cvpi_tests.sh: $(TEST_PROGS)
	echo './cvpi_tests' > cvpi_tests.sh
	chmod +x cvpi_tests.sh

CLEANFILES = cvpi_tests.sh
