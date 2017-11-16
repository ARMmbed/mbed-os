include ../../makefile_defines.txt

COMPONENT_NAME = beacon_handler_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/6LoWPAN/MAC/beacon_handler.c

TEST_SRC_FILES = \
	main.cpp \
        beacon_handlertest.cpp \
        test_beacon_handler.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/protocol_core_stub.c \
        ../../stub/mac_helper_stub.c \
        ../../stub/protocol_6lowpan_stub.c \
        ../../stub/pan_blacklist_stub.c \
        ../../stub/common_functions_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

