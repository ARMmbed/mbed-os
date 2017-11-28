include ../../makefile_defines.txt

COMPONENT_NAME = socket_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/Core/ns_socket.c

TEST_SRC_FILES = \
	main.cpp \
        sockettest.cpp \
        test_socket.c \
        ../../stub/ns_list_stub.c \
        ../../stub/buffer_dyn_stub.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/randLIB_stub.c \
        ../../stub/event_stub.c \
        ../../stub/address_stub.c \
        ../../stub/tcp_stub.c \
        ../../stub/socket_api_stub.c \
        ../../stub/protocol_core_stub.c \
        ../../stub/ipv6_stub.c \
        ../../stub/ipv6_routing_table_stub.c \
        ../../stub/security_lib_stub.c \
        ../../stub/ip6tos_stub.c \
        ../../stub/sockbuf_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

