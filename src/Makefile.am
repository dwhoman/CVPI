# This file is part of CVPI.
# 
# Copyright (C) 2015
# 
# This program is free software: you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public License
# as published by the Free Software Foundation, either version 3 of
# the License, or (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ACLOCAL_AMFLAGS = -I include $(CVPI_RASPBERRY_FLAGS)

lib_LTLIBRARIES = libcvpi.la

libcvpi_la_SOURCES = \
	cvpi_private_header.h \
	cvpi_base.c \
	cvpi_egl_config.c \
	cvpi_image_functions.c \
	cvpi_image_headers.c \
	cvpi_vg_ext.c

libcvpi_la_CPPFLAGS = -I$(top_srcdir) -I$(top_srcdir)/include/ $(CVPI_RASPBERRY_CPPFLAGS)
libcvpi_la_LDFLAGS = $(CVPI_RASPBERRY_LDFLAGS)

# LDADD = libcvpi.a

# libcvpi_la_LIBADD = ../libcvpi.la
