include ../../makefile_defines.txt

COMPONENT_NAME = ipv6_routing_table_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/ipv6_stack/ipv6_routing_table.c

TEST_SRC_FILES = \
	main.cpp \
        ipv6_routing_tabletest.cpp \
        test_ipv6_routing_table.c \
        ../../stub/ns_list_stub.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/common_functions_stub.c \
        ../../stub/ipv6_resolution_stub.c \
        ../../stub/randLIB_stub.c \
        ../../stub/address_stub.c \
        ../../stub/etx_stub.c \
        ../../stub/ip6tos_stub.c \
        ../../stub/protocol_core_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

