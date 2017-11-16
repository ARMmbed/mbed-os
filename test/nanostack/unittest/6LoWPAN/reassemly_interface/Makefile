include ../../makefile_defines.txt

COMPONENT_NAME = reassembly_interface_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/6LoWPAN/Fragmentation/cipv6_fragmenter.c

TEST_SRC_FILES = \
	main.cpp \
        reassembly_interfacetest.cpp \
        test_cipv6_reassembly_interface.c \
        ../../stub/common_functions_stub.c \
        ../../stub/ns_list_stub.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/buffer_dyn_stub.c \
        ../../stub/protocol_core_stub.c \
        ../../stub/socket_stub.c \
        ../../stub/mac_helper_stub.c \
        ../../stub/mle_stub.c \
        ../../stub/platform_stub.c \
        ../../stub/address_stub.c \
        ../../stub/etx_stub.c \
        ../../stub/protocol_stats_stub.c \
        ../../stub/randLIB_stub.c \
        ../../stub/mesh_stub.c \
        ../../stub/iphc_decompress_stub.c \
        

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

