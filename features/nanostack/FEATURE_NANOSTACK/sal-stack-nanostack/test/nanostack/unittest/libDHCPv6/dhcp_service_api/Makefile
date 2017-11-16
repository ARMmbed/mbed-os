include ../../makefile_defines.txt

COMPONENT_NAME = dhcp_service_api_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/libDHCPv6/dhcp_service_api.c

TEST_SRC_FILES = \
	main.cpp \
        dhcp_service_apitest.cpp \
        test_dhcp_service_api.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/buffer_dyn_stub.c \
        ../../stub/ns_list_stub.c \
        ../../stub/randLIB_stub.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/common_functions_stub.c \
        ../../stub/socket_api_stub.c \
        ../../stub/event_stub.c \
        ../../stub/system_timer_stub.c \
        ../../stub/libDHCPv6_stub.c \
        ../../stub/protocol_core_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT -DHAVE_DHCPV6

