include ../../makefile_defines.txt

COMPONENT_NAME = tcp_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/Common_Protocols/tcp.c

TEST_SRC_FILES = \
	main.cpp \
        tcptest.cpp \
        test_tcp.c \
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
        ../../stub/ns_sha256_stub.c\
        ../../stub/ipv6_flow_stub.c\
        ../../stub/sockbuf_stub.c\
        ../../stub/ipv6_routing_table_stub.c\
        
include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

