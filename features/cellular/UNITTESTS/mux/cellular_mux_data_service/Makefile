include ../../makefile_defines.txt

COMPONENT_NAME = cellular_mux_data_service_unit

#This must be changed manually
SRC_FILES = \
        ../../../framework/mux/cellular_mux_data_service.c

TEST_SRC_FILES = \
	main.cpp \
        cellular_mux_data_servicetest.cpp \
        test_cellular_mux_data_service.cpp \
        ../../stubs/FileHandle_stub.cpp \
        ../../stubs/cellular_mux_stub.cpp \
        ../../stubs/mbed_assert_stub.cpp \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

