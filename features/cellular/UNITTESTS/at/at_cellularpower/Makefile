include ../../makefile_defines.txt

COMPONENT_NAME = AT_CellularPower_unit

#This must be changed manually
SRC_FILES = \
        ../../../framework/AT/AT_CellularPower.cpp

TEST_SRC_FILES = \
	main.cpp \
        at_cellularpowertest.cpp \
        test_at_cellularpower.cpp \
        ../../stubs/ATHandler_stub.cpp \
        ../../stubs/AT_CellularBase_stub.cpp \
        ../../stubs/EventQueue_stub.cpp \
        ../../stubs/FileHandle_stub.cpp \
        ../../stubs/CellularUtil_stub.cpp \
        ../../stubs/mbed_assert_stub.cpp \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

