include ../../makefile_defines.txt

COMPONENT_NAME = thread_unit

SRC_FILES = ../../../../../source/6LoWPAN/Thread/thread_common.c

TEST_SRC_FILES = main.cpp \
	threadtest.cpp \
	test_thread.c \
	../../stub/mbed_trace_stub.c \
	../../stub/thread_nvm_store_stub.c \
	../../stub/randLIB_stub.c \
	../../stub/thread_joiner_application_stub.c \
	../../stub/protocol_6lowpan_stub.c \
	../../stub/mle_tlv_stub.c \
	../../stub/address_stub.c \
	../../stub/thread_routing_stub.c \
	../../stub/thread_nd_stub.c \
	../../stub/nsdynmemLIB_stub.c \
	../../stub/common_functions_stub.c \
	../../stub/protocol_6lowpan_interface_stub.c \
	../../stub/protocol_core_stub.c \
	../../stub/ns_list_stub.c \
	../../stub/lowpan_context_stub.c \
	../../stub/ipv6_routing_table_stub.c \
	../../stub/thread_bootstrap_stub.c \
	../../stub/thread_border_router_api_internal_stub.c \
	../../stub/thread_network_data_storage_stub.c \
	../../stub/thread_test_api_stub.c \
	../../stub/thread_network_data_lib_stub.c \
	../../stub/thread_border_router_api_stub.c \
	../../stub/mle_stub.c \
	../../stub/mle_service_stub.c \
	../../stub/ipv6_stub.c \
	../../stub/icmpv6_stub.c \
	../../stub/blacklist_stub.c \
	../../stub/protocol_ipv6_stub.c \
	../../stub/thread_dhcpv6_client_stub.c \
	../../stub/thread_management_client_stub.c \
	../../stub/thread_discovery_stub.c \
	../../stub/thread_management_server_stub.c \
	../../stub/thread_management_if_stub.c \
	../../stub/thread_network_synch_stub.c \
	../../stub/thread_resolution_client_stub.c \
	../../stub/thread_address_registration_client_stub.c \
	../../stub/thread_router_bootstrap_stub.c \
	../../stub/thread_leader_service_stub.c \
	../../stub/thread_beacon_stub.c \
    ../../stub/thread_bbr_api_stub.c \
	../../stub/nd_proxy_stub.c \
	../../stub/thread_meshcop_lib_stub.c \
	../../stub/mac_helper_stub.c \
	../../stub/mac_pairwise_key_stub.c \
	../../stub/sw_mac_stub.c \
	../../stub/mac_data_poll_stub.c \
	../../stub/etx_stub.c \
	../../stub/timeout_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT -DHAVE_THREAD -DHAVE_ND_PROXY

