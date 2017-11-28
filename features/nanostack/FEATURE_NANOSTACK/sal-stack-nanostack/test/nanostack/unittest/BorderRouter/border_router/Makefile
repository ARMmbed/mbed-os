include ../../makefile_defines.txt

COMPONENT_NAME = border_router_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/BorderRouter/border_router.c

TEST_SRC_FILES = \
	main.cpp \
        border_routertest.cpp \
        test_border_router.c \
        ../../stub/ns_list_stub.c \
        ../../stub/buffer_dyn_stub.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/common_functions_stub.c \
        ../../stub/whiteboard_stub.c \
        ../../stub/mac_helper_stub.c \
        ../../stub/ipv6_stub.c \
        ../../stub/icmpv6_stub.c \
        ../../stub/icmpv6_radv_stub.c \
        ../../stub/icmpv6_prefix_stub.c \
        ../../stub/neighbor_cache_stub.c \
        ../../stub/thread_common_stub.c \
        ../../stub/blacklist_stub.c \
        ../../stub/thread_bootstrap_stub.c \
        ../../stub/protocol_6lowpan_bootstrap_stub.c \
        ../../stub/adaptation_interface_stub.c \
        ../../stub/cipv6_fragmenter_stub.c \
        ../../stub/protocol_6lowpan_stub.c \
        ../../stub/protocol_ipv6_stub.c \
        ../../stub/protocol_core_stub.c \
        ../../stub/protocol_timer_stub.c \
        ../../stub/nd_router_object_stub.c \
        ../../stub/address_stub.c \
        ../../stub/lowpan_context_stub.c \
        ../../stub/mle_stub.c \
        ../../stub/mle_service_stub.c \
        ../../stub/rpl_control_stub.c \
        ../../stub/nd_proxy_stub.c \
        ../../stub/ipv6_routing_table_stub.c \
        ../../stub/beacon_handler_stub.c \
        ../../stub/pana_stub.c \
        ../../stub/pana_server_stub.c \
        ../../stub/net_load_balance_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

