include ../../makefile_defines.txt

COMPONENT_NAME = rpl_control_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/RPL/rpl_control.c

TEST_SRC_FILES = \
	main.cpp \
        rpl_controltest.cpp \
        test_rpl_control.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/common_functions_stub.c \
        ../../stub/ns_list_stub.c \
        ../../stub/buffer_dyn_stub.c \
        ../../stub/rpl_downward_stub.c \
        ../../stub/rpl_upward_stub.c \
        ../../stub/rpl_policy_stub.c \
        ../../stub/protocol_core_stub.c \
        ../../stub/protocol_stats_stub.c \
        ../../stub/address_stub.c \
        ../../stub/icmpv6_stub.c \
        ../../stub/etx_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT


