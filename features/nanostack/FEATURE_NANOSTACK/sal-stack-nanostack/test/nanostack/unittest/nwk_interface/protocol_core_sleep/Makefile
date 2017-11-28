include ../../makefile_defines.txt

COMPONENT_NAME = protocol_core_sleep_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/NWK_INTERFACE/protocol_core_sleep.c

TEST_SRC_FILES = \
	main.cpp \
        protocol_core_sleeptest.cpp \
        test_protocol_core_sleep.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/common_functions_stub.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/protocol_core_stub.c \
        ../../stub/platform_stub.c \
        ../../stub/nd_router_object_stub.c \
        ../../stub/mac_data_poll_stub.c \
        ../../stub/protocol_timer_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

