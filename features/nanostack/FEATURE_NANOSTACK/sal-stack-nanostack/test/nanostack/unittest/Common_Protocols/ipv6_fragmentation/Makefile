include ../../makefile_defines.txt

COMPONENT_NAME = ipv6_fragmentation_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/Common_Protocols/ipv6_fragmentation.c

TEST_SRC_FILES = \
	main.cpp \
        ipv6_fragmentationtest.cpp \
        test_ipv6_fragmentation.c \
        ../../stub/ns_list_stub.c \
        ../../stub/buffer_dyn_stub.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/common_functions_stub.c \
        ../../stub/address_stub.c \
        ../../stub/protocol_core_stub.c \
        ../../stub/protocol_stats_stub.c \
        ../../stub/icmpv6_stub.c \
        ../../stub/ipv6_stub.c \
        ../../stub/ipv6_routing_table_stub.c \
        ../../stub/socket_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

