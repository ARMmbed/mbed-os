include ../../makefile_defines.txt

COMPONENT_NAME = buffer_dyn_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/Core/buffer_dyn.c

TEST_SRC_FILES = \
	main.cpp \
        buffer_dyntest.cpp \
        test_buffer_dyn.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/protocol_stats_stub.c \
        ../../stub/platform_stub.c \
        ../../stub/socket_stub.c \
        ../../stub/ip_fsc_stub.c \
        ../../stub/ns_list_stub.c\

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

