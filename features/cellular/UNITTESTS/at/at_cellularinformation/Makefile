include ../../makefile_defines.txt

COMPONENT_NAME = AT_CellularInformation_unit

#This must be changed manually
SRC_FILES = \
        ../../../framework/AT/AT_CellularInformation.cpp

TEST_SRC_FILES = \
	main.cpp \
        at_cellularinformationtest.cpp \
        test_at_cellularinformation.cpp \
        ../../stubs/ATHandler_stub.cpp \
        ../../stubs/AT_CellularBase_stub.cpp \
        ../../stubs/EventQueue_stub.cpp \
        ../../stubs/FileHandle_stub.cpp \
        ../../stubs/mbed_assert_stub.cpp \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

