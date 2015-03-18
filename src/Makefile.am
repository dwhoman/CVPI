#ACLOCAL_AMFLAGS = -I include $(CVPI_RASPBERRY_FLAGS)

lib_LTLIBRARIES = libcvpi.la

libcvpi_la_SOURCES = \
	cvpi_base.c \
	cvpi_egl_config.c \
	cvpi_image_functions.c \
	cvpi_image_headers.c \
	cvpi_vg_ext.c

libcvpi_la_CPPFLAGS = -I$(top_srcdir) -I$(top_srcdir)/include/ $(CVPI_RASPBERRY_CPPFLAGS)
libcvpi_la_LDFLAGS = $(CVPI_RASPBERRY_LDFLAGS)

# LDADD = libcvpi.a

# libcvpi_la_LIBADD = ../libcvpi.la
