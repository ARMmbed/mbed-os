include ../../makefile_defines.txt

COMPONENT_NAME = protocol_core_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/NWK_INTERFACE/protocol_core.c

TEST_SRC_FILES = \
	main.cpp \
        protocol_coretest.cpp \
        test_protocol_core.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/common_functions_stub.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/event_stub.c \
        ../../stub/ns_list_stub.c \
        ../../stub/address_stub.c \
        ../../stub/neighbor_cache_stub.c \
        ../../stub/mac_helper_stub.c \
        ../../stub/mac_data_poll_stub.c \
        ../../stub/ipv6_routing_table_stub.c \
        ../../stub/icmpv6_stub.c \
        ../../stub/icmpv6_radv_stub.c \
        ../../stub/protocol_timer_stub.c \
        ../../stub/protocol_ipv6_stub.c \
        ../../stub/protocol_6lowpan_bootstrap_stub.c \
        ../../stub/mac_mcps_sap_stub.c \
        ../../stub/mac_mlme_stub.c \
        ../../stub/socket_stub.c \
        ../../stub/buffer_dyn_stub.c \
        ../../stub/randLIB_stub.c \
        ../../stub/mac_response_handler_stub.c \
        ../../stub/beacon_handler_stub.c \
        ../../stub/cipv6_fragmenter_stub.c \
        ../../stub/thread_common_stub.c \
        ../../stub/thread_bootstrap_stub.c \
        ../../stub/mld_stub.c \
        ../../stub/mpl_stub.c \
        ../../stub/rpl_control_stub.c \
        ../../stub/platform_stub.c \
        ../../stub/lowpan_context_stub.c \
        ../../stub/nd_router_object_stub.c \
        ../../stub/ipv6_fragmentation_stub.c \
        ../../stub/adaptation_interface_stub.c \
        ../../stub/security_lib_stub.c \
        ../../stub/pan_blacklist_stub.c \
        ../../stub/ipv6_routing_table_stub.c \
        ../../stub/ipv6_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

