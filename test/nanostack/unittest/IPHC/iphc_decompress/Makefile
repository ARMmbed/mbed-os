include ../../makefile_defines.txt

COMPONENT_NAME = iphc_decompress_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/6LoWPAN/IPHC_Decode/iphc_decompress.c

TEST_SRC_FILES = \
	main.cpp \
        iphc_decompresstest.cpp \
        test_iphc_decompress.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/buffer_dyn_stub.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/address_stub.c \
        ../../stub/ns_list_stub.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/common_functions_stub.c \
        ../../stub/lowpan_context_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

