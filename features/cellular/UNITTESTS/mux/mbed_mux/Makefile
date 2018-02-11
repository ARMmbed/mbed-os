include ../../makefile_defines.txt

COMPONENT_NAME = mbed_mux_unit

#This must be changed manually
SRC_FILES = \
        ../../../framework/mux/mbed_mux.c

TEST_SRC_FILES = \
	main.cpp \
        mbed_muxtest.cpp \
        test_mbed_mux.cpp \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

