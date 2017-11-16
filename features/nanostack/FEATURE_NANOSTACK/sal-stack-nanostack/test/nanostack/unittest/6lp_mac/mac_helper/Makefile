include ../../makefile_defines.txt

COMPONENT_NAME = mac_helper_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/6LoWPAN/MAC/mac_helper.c

TEST_SRC_FILES = \
	main.cpp \
        mac_helpertest.cpp \
        test_mac_helper.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/protocol_core_stub.c \
        ../../stub/common_functions_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

