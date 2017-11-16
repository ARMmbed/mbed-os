include ../../makefile_defines.txt

COMPONENT_NAME = udp_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/Common_Protocols/udp.c

TEST_SRC_FILES = \
	main.cpp \
        udptest.cpp \
        test_udp.c \
        ../../stub/ns_list_stub.c \
        ../../stub/buffer_dyn_stub.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/common_functions_stub.c \
        ../../stub/socket_stub.c \
        ../../stub/protocol_core_stub.c \
        ../../stub/protocol_timer_stub.c \
        ../../stub/event_stub.c \
        ../../stub/protocol_stats_stub.c \
        ../../stub/address_stub.c \
        ../../stub/randLIB_stub.c \
        ../../stub/icmpv6_stub.c \
        ../../stub/network_lib_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

