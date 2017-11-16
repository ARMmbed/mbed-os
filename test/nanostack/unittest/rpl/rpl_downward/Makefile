include ../../makefile_defines.txt

COMPONENT_NAME = rpl_downward_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/RPL/rpl_downward.c

TEST_SRC_FILES = \
	main.cpp \
        rpl_downwardtest.cpp \
        test_rpl_downward.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/common_functions_stub.c \
        ../../stub/ns_list_stub.c \
        ../../stub/rpl_policy_stub.c \
        ../../stub/rpl_control_stub.c \
        ../../stub/buffer_dyn_stub.c \
        ../../stub/protocol_core_stub.c \
        ../../stub/rpl_upward_stub.c \
        ../../stub/rpl_data_stub.c \
        ../../stub/address_stub.c \
        ../../stub/icmpv6_stub.c \
        ../../stub/ipv6_stub.c \
        ../../stub/randLIB_stub.c \
        ../../stub/ipv6_routing_table_stub.c \
        ../../stub/ip6tos_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT -DHAVE_RPL

