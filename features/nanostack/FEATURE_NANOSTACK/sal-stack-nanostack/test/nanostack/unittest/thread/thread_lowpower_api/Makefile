include ../../makefile_defines.txt

COMPONENT_NAME = thread_lowpower_api_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/6LoWPAN/Thread/thread_lowpower_private_api.c

TEST_SRC_FILES = \
        main.cpp \
        thread_lowpower_apitest.cpp \
        test_thread_lowpower_api.c \
        ../../stub/ns_list_stub.c \
        ../../stub/socket_api_stub.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/common_functions_stub.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/ns_sha256_stub.c \
        ../../stub/mac_data_poll_stub.c \
        ../../stub/mac_security_interface_stub.c \
        ../../stub/mac_mlme_stub.c \
        ../../stub/thread_management_if_stub.c \
        ../../stub/thread_router_bootstrap_stub.c \
        ../../stub/thread_commissioning_if_stub.c \
        ../../stub/thread_meshcop_lib_stub.c \
        ../../stub/thread_management_api_stub.c \
        ../../stub/event_stub.c \
        ../../stub/thread_dhcpv6_client_stub.c \
        ../../stub/thread_border_router_api_stub.c \
        ../../stub/thread_border_router_api_internal_stub.c \
        ../../stub/thread_common_stub.c \
        ../../stub/thread_joiner_application_stub.c \
        ../../stub/thread_leader_service_stub.c \
        ../../stub/thread_management_client_stub.c \
        ../../stub/thread_management_server_stub.c \
        ../../stub/thread_discovery_stub.c \
        ../../stub/thread_nd_stub.c \
        ../../stub/thread_routing_stub.c \
        ../../stub/address_stub.c \
        ../../stub/mpl_stub.c \
        ../../stub/blacklist_stub.c \
        ../../stub/routing_table_stub.c \
        ../../stub/protocol_ipv6_stub.c \
        ../../stub/ipv6_routing_table_stub.c \
        ../../stub/libDHCPv6_server_stub.c \
        ../../stub/lowpan_context_stub.c \
        ../../stub/system_timer_stub.c \
        ../../stub/timeout_stub.c \
        ../../stub/mle_stub.c \
        ../../stub/mle_service_stub.c \
        ../../stub/mle_tlv_stub.c \
        ../../stub/protocol_core_stub.c \
        ../../stub/randLIB_stub.c \
        ../../stub/neighbor_cache_stub.c \
        ../../stub/protocol_6lowpan_stub.c \
        ../../stub/thread_test_api_stub.c \
        ../../stub/thread_beacon_stub.c \
        ../../stub/thread_network_synch_stub.c \
        ../../stub/thread_network_data_storage_stub.c \
        ../../stub/thread_address_registration_client_stub.c \
        ../../stub/thread_common_stub.c \
        ../../stub/net_stub.c \
        ../../stub/nd_proxy_stub.c \
        ../../stub/thread_router_bootstrap_stub.c \
        ../../stub/thread_host_bootstrap_stub.c \
        ../../stub/rpl_control_stub.c \
        ../../stub/mle_stub.c \
        ../../stub/mac_helper_stub.c \
        ../../stub/icmpv6_stub.c \
        ../../stub/thread_mle_message_handler_stub.c \


include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT -DHAVE_THREAD -DHAVE_DHCPV6_SERVER

