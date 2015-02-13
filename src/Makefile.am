#ACLOCAL_AMFLAGS = -I include $(CVPI_RASPBERRY_FLAGS)

include_HEADERS = \
	$(top_srcdir)/include/cvpi.h \
	$(top_srcdir)/include/cvpi_base.h \
	$(top_srcdir)/include/cvpi_egl_config.h \
	$(top_srcdir)/include/cvpi_image_functions.h \
	$(top_srcdir)/include/cvpi_image_headers.h \
	$(top_srcdir)/include/cvpi_vg_ext.h

noinst_LIBRARIES = libcvpi.a

#libcvpi_a_CPPFLAGS = -I include $(CVPI_RASPBERRY_CPPFLAGS)
#libcvpi_a_LDFLAGS = $(CVPI_RASPBERRY_LDFLAGS)

AM_CPPFLAGS = -I$(top_srcdir) -I$(top_srcdir)/include/ $(CVPI_RASPBERRY_CPPFLAGS)
AM_LDFLAGS = $(CVPI_RASPBERRY_LDFLAGS)

libcvpi_a_SOURCES = \
	cvpi_base.c \
	cvpi_egl_config.c \
	cvpi_image_functions.c \
	cvpi_image_headers.c \
	cvpi_vg_ext.c \
	$(top_srcdir)/include/cvpi.h \
	$(top_srcdir)/include/cvpi_base.h \
	$(top_srcdir)/include/cvpi_egl_config.h \
	$(top_srcdir)/include/cvpi_image_functions.h \
	$(top_srcdir)/include/cvpi_image_headers.h \
	$(top_srcdir)/include/cvpi_vg_ext.h

# LDADD = libcvpi.a

libcvpi_a_LIBADD = $(LIBOBJS) $(ALLOCA)
