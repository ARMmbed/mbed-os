include ../../makefile_defines.txt

COMPONENT_NAME = protocol_timer_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/NWK_INTERFACE/protocol_timer.c

TEST_SRC_FILES = \
	main.cpp \
        protocol_timertest.cpp \
        test_protocol_timer.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/common_functions_stub.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/protocol_core_stub.c \
        ../../stub/platform_stub.c \
        ../../stub/event_stub.c \
        ../../stub/ns_timer_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

