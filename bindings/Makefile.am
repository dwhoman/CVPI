# chicken bindings
# based on wiki.call-cc.org/autoconf - automake
lib_LTLIBRARIES = libvg_chicken.la
libvg_chicken_la_SOURCES = ./chicken/src/openvg-chicken.scd

lib_LTLIBRARIES += libvgu_chicken.la
libvgu_chicken_la_SOURCES = ./chicken/src/vgu-chicken.scd

lib_LTLIBRARIES += libcvpi_chicken.la
libcvpi_chicken_la_SOURCES = ./chicken/src/cvpi-chicken.scd

libvg_chicken_la_CPPFLAGS = -I$(top_srcdir) -I$(top_srcdir)/include/ $(CVPI_RASPBERRY_CPPFLAGS)
libvg_chicken_la_LDFLAGS = $(CVPI_RASPBERRY_LDFLAGS)

libvgu_chicken_la_CPPFLAGS = -I$(top_srcdir) -I$(top_srcdir)/include/ $(CVPI_RASPBERRY_CPPFLAGS)
libvgu_chicken_la_LDFLAGS = $(CVPI_RASPBERRY_LDFLAGS)

libcvpi_chicken_la_CPPFLAGS = -I$(top_srcdir) -I$(top_srcdir)/include/ $(CVPI_RASPBERRY_CPPFLAGS)
libcvpi_chicken_la_LDFLAGS = $(CVPI_RASPBERRY_LDFLAGS)

.scm.o:

.scd.o:

OBJEXT=o

LINK=csc -o $@

%.o: %.scm
	csc -c $< -o $@ $(CHICKENFLAGS)

%.o: %.scd
	csc -c $< -o $@ $(CHICKENFLAGS)
