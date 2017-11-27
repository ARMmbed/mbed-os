include ../../makefile_defines.txt

COMPONENT_NAME = DHCPv6_Server_service_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/DHCPv6_Server/DHCPv6_Server_service.c

TEST_SRC_FILES = \
	main.cpp \
        dhcpv6_server_servicetest.cpp \
        test_dhcpv6_server_service.c \
        ../../stub/ns_list_stub.c \
        ../../stub/buffer_dyn_stub.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/common_functions_stub.c \
        ../../stub/tcp_stub.c \
        ../../stub/udp_stub.c \
        ../../stub/socket_stub.c \
        ../../stub/address_stub.c \
        ../../stub/cudp_stub.c \
        ../../stub/event_stub.c \
        ../../stub/ipv6_stub.c \
        ../../stub/ipv6_routing_table_stub.c \
        ../../stub/libDHCPv6_server_stub.c \
        ../../stub/libDHCPv6_stub.c \
        ../../stub/protocol_core_stub.c \
        ../../stub/protocol_6lowpan_bootstrap_stub.c \
        ../../stub/protocol_6lowpan_stub.c \
        ../../stub/system_timer_stub.c \
        ../../stub/dhcp_service_api_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT -DHAVE_DHCPV6_SERVER

