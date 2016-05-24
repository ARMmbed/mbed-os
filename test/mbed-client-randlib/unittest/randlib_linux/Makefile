include ../makefile_defines.txt

COMPONENT_NAME = randLIB_linux_unit

#This must be changed manually
SRC_FILES = \
        ../../../../linux/randLIB.c

TEST_SRC_FILES = \
	main.cpp \
	randlibtest.cpp \
	../randlib/test_randlib.c \
	../stubs/random_stub.c \
	../stubs/open_stub.c \

include ../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

CPPUTEST_LDFLAGS = -Wl,--wrap,open
