include ../../makefile_defines.txt

COMPONENT_NAME = thread_management_server_unit
SRC_FILES = \
        ../../../../../source/6LoWPAN/Thread/thread_management_server.c


TEST_SRC_FILES = main.cpp \
        thread_management_server_test.cpp \
        test_thread_management_server.cpp \
        ../../stub/timeout_stub.c \
        ../../stub/coap_service_api_stub.c \
        ../../stub/ns_list_stub.c \
        ../../stub/socket_api_stub.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/mac_mlme_stub.c \
        ../../stub/common_functions_stub.c \
        ../../stub/address_stub.c \
        ../../stub/thread_border_router_api_stub.c \
        ../../stub/thread_joiner_application_stub.c \
        ../../stub/thread_leader_service_stub.c \
        ../../stub/thread_management_if_stub.c \
        ../../stub/thread_bootstrap_stub.c \
        ../../stub/thread_commissioning_if_stub.c \
        ../../stub/thread_bbr_api_stub.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/thread_meshcop_lib_stub.c \
        ../../stub/thread_beacon_stub.c \
        ../../stub/thread_common_stub.c \
        ../../stub/thread_routing_stub.c \
        ../../stub/thread_network_data_lib_stub.c \
        ../../stub/thread_discovery_stub.c \
        ../../stub/protocol_core_stub.c \
        ../../stub/mac_helper_stub.c \
        ../../stub/mac_data_poll_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT -DHAVE_THREAD

