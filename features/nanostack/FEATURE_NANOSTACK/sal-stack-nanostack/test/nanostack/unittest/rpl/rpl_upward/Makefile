include ../../makefile_defines.txt

COMPONENT_NAME = rpl_upward_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/RPL/rpl_upward.c

TEST_SRC_FILES = \
	main.cpp \
        rpl_upwardtest.cpp \
        test_rpl_upward.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/common_functions_stub.c \
        ../../stub/ns_list_stub.c \
        ../../stub/buffer_dyn_stub.c \
        ../../stub/rpl_downward_stub.c \
        ../../stub/rpl_policy_stub.c \
        ../../stub/rpl_control_stub.c \
        ../../stub/rpl_objective_stub.c \
        ../../stub/protocol_stats_stub.c \
        ../../stub/ipv6_resolution_stub.c \
        ../../stub/ipv6_stub.c \
        ../../stub/ipv6_routing_table_stub.c \
        ../../stub/randLIB_stub.c \
        ../../stub/icmpv6_stub.c \
        ../../stub/icmpv6_prefix_stub.c \
        ../../stub/address_stub.c \
        ../../stub/trickle_stub.c \
        ../../stub/protocol_6lowpan_stub.c \
        ../../stub/protocol_core_stub.c \
        ../../stub/ip6tos_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT -DHAVE_RPL

