include ../../makefile_defines.txt

COMPONENT_NAME = protocol_stats_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/NWK_INTERFACE/protocol_stats.c

TEST_SRC_FILES = \
	main.cpp \
        protocol_statstest.cpp \
        test_protocol_stats.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/common_functions_stub.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/protocol_core_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

