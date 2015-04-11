# This file is part of CVPI.
# 
# Copyright (C) 2015 Devin Homan
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

import sys

SConscript(['tests/SConscript'])

byteorder = sys.byteorder;
if 'little' != sys.byteorder:
    print "Byteorder is not little endian. The byteorder is " + byteorder

env = Environment(CCFLAGS='-O3 -std=gnu99 -DCVPI_ASSEMBLY=1 -DCVPI_CAREFUL=1 -DHAVE_BCM_HOST=1 -DCVPI_LOGGING=0',
                  CPPPATH= ['include', '/opt/vc/include/', '/opt/vc/include/EGL/', 
                            '/opt/vc/include/interface/vmcs_host/linux/', '/opt/vc/include/interface/vcos/pthreads/'])

env.Library('libcvpi', [Glob('src/*.c')], LIBS=['m', 'EGL', 'GLESv2', 'bcm_host', 'pthread'], LIBPATH=['/opt/vc/lib'])
