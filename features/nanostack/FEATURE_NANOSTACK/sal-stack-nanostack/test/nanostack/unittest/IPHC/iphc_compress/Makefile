include ../../makefile_defines.txt

COMPONENT_NAME = iphc_compress_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/6LoWPAN/IPHC_Decode/iphc_compress.c

TEST_SRC_FILES = \
	main.cpp \
        iphc_compresstest.cpp \
        test_iphc_compress.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/buffer_dyn_stub.c \
        ../../stub/address_stub.c \
        ../../stub/ns_list_stub.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/common_functions_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

