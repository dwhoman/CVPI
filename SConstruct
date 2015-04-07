import sys

SConscript(['tests/SConscript'])

byteorder = sys.byteorder;
if 'little' != sys.byteorder:
    print "Byteorder is not little endian. The byteorder is " + byteorder

env = Environment(CCFLAGS='-O3 -std=gnu99 -DCVPI_ASSEMBLY=1 -DCVPI_CAREFUL=1 -DHAVE_BCM_HOST=1 -DCVPI_LOGGING=0',
                  CPPPATH= ['include', '/opt/vc/include/', '/opt/vc/include/EGL/', 
                            '/opt/vc/include/interface/vmcs_host/linux/', '/opt/vc/include/interface/vcos/pthreads/'])

env.Library('libcvpi', [Glob('src/*.c')], LIBS=['m', 'EGL', 'GLESv2', 'bcm_host', 'pthread'], LIBPATH=['/opt/vc/lib'])
