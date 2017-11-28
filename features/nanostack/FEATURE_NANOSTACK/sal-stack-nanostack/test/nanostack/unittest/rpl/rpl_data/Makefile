include ../../makefile_defines.txt

COMPONENT_NAME = rpl_data_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/RPL/rpl_data.c

TEST_SRC_FILES = \
	main.cpp \
        rpl_datatest.cpp \
        test_rpl_data.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/common_functions_stub.c \
        ../../stub/buffer_dyn_stub.c \
        ../../stub/icmpv6_stub.c \
        ../../stub/address_stub.c \
        ../../stub/protocol_core_stub.c \
        ../../stub/protocol_stats_stub.c \
        ../../stub/rpl_policy_stub.c \
        ../../stub/rpl_control_stub.c \
        ../../stub/ipv6_resolution_stub.c \
        ../../stub/ipv6_stub.c \
        ../../stub/ipv6_routing_table_stub.c \
        ../../stub/rpl_downward_stub.c \
        ../../stub/rpl_upward_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

