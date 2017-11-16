include ../../makefile_defines.txt

COMPONENT_NAME = 6lowpan_iphc_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/6LoWPAN/IPHC_Decode/6lowpan_iphc.c

TEST_SRC_FILES = \
	main.cpp \
        6lowpan_iphctest.cpp \
        test_6lowpan_iphc.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/ns_list_stub.c \
        ../../stub/buffer_dyn_stub.c \
        ../../stub/address_stub.c \
        ../../stub/mac_helper_stub.c \
        ../../stub/protocol_core_stub.c \
        ../../stub/common_functions_stub.c \
        ../../stub/socket_stub.c \
        ../../stub/protocol_stats_stub.c \
        ../../stub/ipv6_stub.c \
        ../../stub/ipv6_resolution_stub.c \
        ../../stub/protocol_ipv6_stub.c \
        ../../stub/cudp_stub.c \
        ../../stub/iphc_compress_stub.c \
        ../../stub/iphc_decompress_stub.c \
        ../../stub/mesh_stub.c \
        ../../stub/mle_stub.c \
        ../../stub/thread_common_stub.c \
        ../../stub/thread_routing_stub.c \
        ../../stub/lowpan_context_stub.c \
        ../../stub/protocol_6lowpan_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

