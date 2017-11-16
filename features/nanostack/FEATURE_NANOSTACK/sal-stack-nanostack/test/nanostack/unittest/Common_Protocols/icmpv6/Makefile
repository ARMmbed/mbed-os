include ../../makefile_defines.txt

COMPONENT_NAME = icmpv6_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/Common_Protocols/icmpv6.c

TEST_SRC_FILES = \
	main.cpp \
        icmpv6test.cpp \
        test_icmpv6.c \
        ../../stub/ns_list_stub.c \
        ../../stub/buffer_dyn_stub.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/common_functions_stub.c \
        ../../stub/address_stub.c \
        ../../stub/protocol_core_stub.c \
        ../../stub/protocol_stats_stub.c \
        ../../stub/ipv6_stub.c \
        ../../stub/ipv6_routing_table_stub.c \
        ../../stub/nd_router_object_stub.c \
        ../../stub/randLIB_stub.c \
        ../../stub/rpl_control_stub.c \
        ../../stub/mpl_stub.c \
        ../../stub/mld_stub.c \
        ../../stub/nd_proxy_stub.c \
        ../../stub/rpl_data_stub.c \
        ../../stub/icmpv6_radv_stub.c \
        ../../stub/protocol_6lowpan_stub.c \
        ../../stub/protocol_ipv6_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

