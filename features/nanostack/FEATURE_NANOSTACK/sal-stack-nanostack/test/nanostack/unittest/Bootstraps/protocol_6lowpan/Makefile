include ../../makefile_defines.txt

COMPONENT_NAME = protocol_6lowpan_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/6LoWPAN/Bootstraps/Generic/protocol_6lowpan.c

TEST_SRC_FILES = \
	main.cpp \
        protocol_6lowpantest.cpp \
        test_protocol_6lowpan.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/buffer_dyn_stub.c \
        ../../stub/ns_list_stub.c \
        ../../stub/address_stub.c \
        ../../stub/mac_helper_stub.c \
        ../../stub/randLIB_stub.c \
        ../../stub/mle_stub.c \
        ../../stub/rpl_control_stub.c \
        ../../stub/net_rpl_stub.c \
        ../../stub/protocol_stats_stub.c \
        ../../stub/protocol_core_stub.c \
        ../../stub/common_functions_stub.c \
        ../../stub/thread_common_stub.c \
        ../../stub/thread_nd_stub.c \
        ../../stub/protocol_6lowpan_bootstrap_stub.c \
        ../../stub/ipv6_routing_table_stub.c \
        ../../stub/nd_router_object_stub.c \
        ../../stub/mac_data_poll_stub.c \
        ../../stub/border_router_stub.c \
        ../../stub/cipv6_fragmenter_stub.c \
        ../../stub/icmpv6_stub.c \
        ../../stub/ipv6_stub.c \
        ../../stub/tcp_stub.c \
        ../../stub/udp_stub.c \
        ../../stub/mesh_stub.c \
        ../../stub/socket_stub.c \
        ../../stub/6lowpan_iphc_stub.c \
        ../../stub/adaptation_interface_stub.c \
        ../../stub/security_lib_stub.c \
        ../../stub/net_load_balance_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

