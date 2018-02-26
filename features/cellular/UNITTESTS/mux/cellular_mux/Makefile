include ../../makefile_defines.txt

COMPONENT_NAME = cellular_mux_unit

#This must be changed manually
SRC_FILES = \
        ../../../framework/mux/cellular_mux.c

TEST_SRC_FILES = \
	main.cpp \
        cellular_muxtest.cpp \
        test_cellular_mux.cpp \
        ../../stubs/mbed_assert_stub.cpp \
        ../../stubs/FileHandle_stub.cpp \
        ../../stubs/EventQueue_stub.cpp \
        ../../stubs/equeue_stub.c \
        ../../stubs/cellular_mux_data_service_stub.cpp \
        ../../stubs/Semaphore_stub.cpp \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

