include ../../makefile_defines.txt

COMPONENT_NAME = protocol_6lowpan_bootstrap_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/6LoWPAN/Bootstraps/Generic/protocol_6lowpan_bootstrap.c

TEST_SRC_FILES = \
	main.cpp \
        protocol_6lowpan_bootstraptest.cpp \
        test_protocol_6lowpan_bootstrap.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/buffer_dyn_stub.c \
        ../../stub/address_stub.c \
        ../../stub/mle_stub.c \
        ../../stub/mle_tlv_stub.c \
        ../../stub/mle_service_stub.c \
        ../../stub/mle_service_buffer_stub.c \
        ../../stub/protocol_core_stub.c \
        ../../stub/protocol_6lowpan_stub.c \
        ../../stub/protocol_6lowpan_interface_stub.c \
        ../../stub/common_functions_stub.c \
        ../../stub/mac_helper_stub.c \
        ../../stub/rpl_control_stub.c \
        ../../stub/beacon_handler_stub.c \
        ../../stub/border_router_stub.c \
        ../../stub/nd_router_object_stub.c \
        ../../stub/net_stub.c \
        ../../stub/net_mle_stub.c \
        ../../stub/blacklist_stub.c \
        ../../stub/mac_data_poll_stub.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/randLIB_stub.c \
        ../../stub/icmpv6_radv_stub.c \
        ../../stub/icmpv6_stub.c \
        ../../stub/thread_common_stub.c \
        ../../stub/ns_list_stub.c \
        ../../stub/etx_stub.c \
        ../../stub/event_stub.c \
        ../../stub/pana_stub.c \
        ../../stub/pana_client_stub.c \
        ../../stub/net_load_balance_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT -DHAVE_6LOWPAN_ND

