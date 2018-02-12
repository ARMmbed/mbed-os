include ../../makefile_defines.txt

COMPONENT_NAME = cellular_mux_unit

#This must be changed manually
SRC_FILES = \
        ../../../framework/mux/cellular_mux.c

TEST_SRC_FILES = \
	main.cpp \
        cellular_muxtest.cpp \
        test_cellular_mux.cpp \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

