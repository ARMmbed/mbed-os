include ../../makefile_defines.txt

COMPONENT_NAME = AT_CellularStack_unit

#This must be changed manually
SRC_FILES = \
        ../../../framework/AT/AT_CellularStack.cpp

TEST_SRC_FILES = \
	main.cpp \
        at_cellularstacktest.cpp \
        test_at_cellularstack.cpp \
        ../../stubs/ATHandler_stub.cpp \
        ../../stubs/AT_CellularBase_stub.cpp \
        ../../stubs/EventQueue_stub.cpp \
        ../../stubs/FileHandle_stub.cpp \
        ../../stubs/CellularUtil_stub.cpp \
        ../../stubs/us_ticker_stub.cpp \
        ../../stubs/NetworkStack_stub.cpp \
        ../../stubs/SocketAddress_stub.cpp \
        ../../stubs/mbed_assert_stub.cpp \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

