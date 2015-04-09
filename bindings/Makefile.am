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
