include ../../makefile_defines.txt

COMPONENT_NAME = AT_CellularSMS_unit

#This must be changed manually
SRC_FILES = \
        ../../../framework/AT/AT_CellularSMS.cpp

TEST_SRC_FILES = \
	main.cpp \
        at_cellularsmstest.cpp \
        test_at_cellularsms.cpp \
        ../../stubs/ATHandler_stub.cpp \
        ../../stubs/AT_CellularBase_stub.cpp \
        ../../stubs/EventQueue_stub.cpp \
        ../../stubs/FileHandle_stub.cpp \
        ../../stubs/CellularUtil_stub.cpp \
        ../../stubs/us_ticker_stub.cpp \
        ../../stubs/mbed_assert_stub.cpp \
        ../../stubs/mbed_wait_api_stub.cpp \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

