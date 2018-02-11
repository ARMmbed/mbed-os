include ../../makefile_defines.txt

COMPONENT_NAME = mbed_mux_data_service_unit

#This must be changed manually
SRC_FILES = \
        ../../../framework/mux/mbed_mux_data_service.c

TEST_SRC_FILES = \
	main.cpp \
        mbed_mux_data_servicetest.cpp \
        test_mbed_mux_data_service.cpp \
        ../../stubs/FileHandle_stub.cpp \
        ../../stubs/mbed_mux_stub.cpp \
        ../../stubs/mbed_assert_stub.cpp \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

