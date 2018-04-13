include ../../makefile_defines.txt

COMPONENT_NAME = AT_CellularBase_unit

#This must be changed manually
SRC_FILES = \
        ../../../framework/AT/AT_CellularBase.cpp

TEST_SRC_FILES = \
	main.cpp \
        at_cellularbasetest.cpp \
        test_at_cellularbase.cpp \
        ../../stubs/ATHandler_stub.cpp \
        ../../stubs/EventQueue_stub.cpp \
        ../../stubs/FileHandle_stub.cpp \
        ../../stubs/mbed_assert_stub.cpp \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

