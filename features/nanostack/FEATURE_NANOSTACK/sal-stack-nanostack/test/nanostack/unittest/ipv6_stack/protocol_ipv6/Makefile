include ../../makefile_defines.txt

COMPONENT_NAME = protocol_ipv6_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/ipv6_stack/protocol_ipv6.c

TEST_SRC_FILES = \
	main.cpp \
        protocol_ipv6test.cpp \
        test_protocol_ipv6.c \
        ../../stub/ns_list_stub.c \
        ../../stub/buffer_dyn_stub.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/common_functions_stub.c \
        ../../stub/tcp_stub.c \
        ../../stub/udp_stub.c \
        ../../stub/socket_stub.c \
        ../../stub/address_stub.c \
        ../../stub/cudp_stub.c \
        ../../stub/nd_router_object_stub.c \
        ../../stub/icmpv6_stub.c \
        ../../stub/icmpv6_radv_stub.c \
        ../../stub/icmpv6_prefix_stub.c \
        ../../stub/ipv6_stub.c \
        ../../stub/ipv6_routing_table_stub.c \
        ../../stub/protocol_6lowpan_bootstrap_stub.c \
        ../../stub/protocol_6lowpan_stub.c \
        ../../stub/protocol_core_stub.c \
        ../../stub/whiteboard_stub.c \
        ../../stub/nd_proxy_stub.c \
        ../../stub/ipv6_resolution_stub.c \
        ../../stub/neighbor_cache_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT -DHAVE_ETHERNET

