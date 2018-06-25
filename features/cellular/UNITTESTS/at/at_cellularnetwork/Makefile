include ../../makefile_defines.txt

COMPONENT_NAME = AT_CellularNetwork_unit

#This must be changed manually
SRC_FILES = \
        ../../../framework/AT/AT_CellularNetwork.cpp \
        ../../../framework/AT/AT_CellularStack.cpp \
        ../../../framework/common/CellularUtil.cpp \

TEST_SRC_FILES = \
	main.cpp \
        at_cellularnetworktest.cpp \
        test_at_cellularnetwork.cpp \
        ../../stubs/ATHandler_stub.cpp \
        ../../stubs/AT_CellularBase_stub.cpp \
        ../../stubs/EventQueue_stub.cpp \
        ../../stubs/FileHandle_stub.cpp \
        ../../stubs/NetworkInterface_stub.cpp \
        ../../stubs/NetworkStack_stub.cpp \
        ../../stubs/us_ticker_stub.cpp \
        ../../stubs/mbed_assert_stub.cpp \
        ../../stubs/SocketAddress_stub.cpp \
        ../../stubs/randLIB_stub.cpp \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

