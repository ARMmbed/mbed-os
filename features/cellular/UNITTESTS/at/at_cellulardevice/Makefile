include ../../makefile_defines.txt

COMPONENT_NAME = AT_CellularDevice_unit

#This must be changed manually
SRC_FILES = \
        ../../../framework/AT/AT_CellularDevice.cpp

TEST_SRC_FILES = \
	main.cpp \
        at_cellulardevicetest.cpp \
        test_at_cellulardevice.cpp \
        ../../stubs/AT_CellularNetwork_stub.cpp \
        ../../stubs/ATHandler_stub.cpp \
        ../../stubs/AT_CellularSMS_stub.cpp \
        ../../stubs/AT_CellularSIM_stub.cpp \
        ../../stubs/AT_CellularPower_stub.cpp \
        ../../stubs/AT_CellularInformation_stub.cpp \
        ../../stubs/CellularUtil_stub.cpp \
        ../../stubs/AT_CellularBase_stub.cpp \
        ../../stubs/NetworkInterface_stub.cpp \
        ../../stubs/EventQueue_stub.cpp \
        ../../stubs/FileHandle_stub.cpp \
        ../../stubs/mbed_assert_stub.cpp \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

