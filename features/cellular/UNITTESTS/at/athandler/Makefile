include ../../makefile_defines.txt

COMPONENT_NAME = ATHandler_unit

#This must be changed manually
SRC_FILES = \
        ../../../framework/AT/ATHandler.cpp

TEST_SRC_FILES = \
	main.cpp \
        athandlertest.cpp \
        test_athandler.cpp \
        ../../stubs/AT_CellularBase_stub.cpp \
        ../../stubs/EventQueue_stub.cpp \
        ../../stubs/FileHandle_stub.cpp \
        ../../stubs/CellularUtil_stub.cpp \
        ../../stubs/us_ticker_stub.cpp \
        ../../stubs/mbed_wait_api_stub.cpp \
        ../../stubs/mbed_assert_stub.cpp \
        ../../stubs/mbed_poll_stub.cpp \
        ../../stubs/Timer_stub.cpp \
        ../../stubs/equeue_stub.cpp \
        ../../stubs/Kernel.cpp \
        ../../stubs/Thread_stub.cpp \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT -DMBED_CONF_CELLULAR_DEBUG_AT=1

