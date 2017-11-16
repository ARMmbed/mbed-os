include ../../makefile_defines.txt

COMPONENT_NAME = nd_router_object_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/6LoWPAN/ND/nd_router_object.c

TEST_SRC_FILES = \
	main.cpp \
        nd_router_objecttest.cpp \
        test_nd_router_object.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/buffer_dyn_stub.c \
        ../../stub/ns_list_stub.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/protocol_6lowpan_bootstrap_stub.c \
        ../../stub/protocol_6lowpan_stub.c \
        ../../stub/icmpv6_stub.c \
        ../../stub/icmpv6_radv_stub.c \
        ../../stub/icmpv6_prefix_stub.c \
        ../../stub/ipv6_routing_table_stub.c \
        ../../stub/randLIB_stub.c \
        ../../stub/common_functions_stub.c \
        ../../stub/address_stub.c \
        ../../stub/mac_data_poll_stub.c \
        ../../stub/mac_helper_stub.c \
        ../../stub/mle_stub.c \
        ../../stub/protocol_core_stub.c \
        ../../stub/protocol_6lowpan_stub.c \
        ../../stub/whiteboard_stub.c \
        ../../stub/lowpan_context_stub.c \
        ../../stub/rpl_control_stub.c \
        ../../stub/rpl_data_stub.c \
        ../../stub/border_router_stub.c \
        ../../stub/pan_blacklist_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT -DHAVE_6LOWPAN_ND

