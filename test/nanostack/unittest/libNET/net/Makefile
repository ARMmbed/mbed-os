include ../../makefile_defines.txt

COMPONENT_NAME = net_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/libNET/src/ns_net.c

TEST_SRC_FILES = \
	main.cpp \
        nettest.cpp \
        test_net.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/ns_list_stub.c \
        ../../stub/protocol_core_stub.c \
        ../../stub/rpl_control_stub.c \
        ../../stub/address_stub.c \
        ../../stub/protocol_stats_stub.c \
        ../../stub/common_functions_stub.c \
        ../../stub/mac_helper_stub.c \
        ../../stub/mac_data_poll_stub.c \
        ../../stub/protocol_6lowpan_stub.c \
        ../../stub/protocol_6lowpan_bootstrap_stub.c \
        ../../stub/ipv6_stub.c \
        ../../stub/ipv6_routing_table_stub.c \
        ../../stub/rpl_data_stub.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/beacon_handler_stub.c \
        ../../stub/mle_service_stub.c \
        ../../stub/event_stub.c \
        ../../stub/thread_management_if_stub.c \
        ../../stub/thread_bootstrap_stub.c \
        ../../stub/protocol_ipv6_stub.c \
        ../../stub/rpl_mrhof_stub.c \
        ../../stub/rpl_of0_stub.c \
        ../../stub/network_lib_stub.c \
        ../../stub/pana_stub.c \
        ../../stub/pana_server_stub.c \
        ../../stub/pana_client_stub.c \
        ../../stub/socket_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

