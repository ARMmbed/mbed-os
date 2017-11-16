include ../../makefile_defines.txt

COMPONENT_NAME = protocol_6lowpan_interface_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/6LoWPAN/Bootstraps/Generic/protocol_6lowpan_interface.c

TEST_SRC_FILES = \
	main.cpp \
        protocol_6lowpan_interfacetest.cpp \
        test_protocol_6lowpan_interface.c \
        ../../stub/buffer_dyn_stub.c \
        ../../stub/mac_helper_stub.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/event_stub.c \
        ../../stub/mle_stub.c \
        ../../stub/blacklist_stub.c \
        ../../stub/nd_router_object_stub.c \
        ../../stub/neighbor_cache_stub.c \
        ../../stub/protocol_6lowpan_stub.c \
        ../../stub/protocol_6lowpan_bootstrap_stub.c \
        ../../stub/protocol_core_stub.c \
        ../../stub/adaptation_interface_stub.c \
        ../../stub/cipv6_fragmenter_stub.c \
        ../../stub/pana_stub.c \
        ../../stub/net_load_balance_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

