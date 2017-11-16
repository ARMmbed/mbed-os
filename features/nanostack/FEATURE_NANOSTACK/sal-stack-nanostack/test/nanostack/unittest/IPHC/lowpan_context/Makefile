include ../../makefile_defines.txt

COMPONENT_NAME = lowpan_context_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/6LoWPAN/IPHC_Decode/lowpan_context.c

TEST_SRC_FILES = \
	main.cpp \
        lowpan_contexttest.cpp \
        test_lowpan_context.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/buffer_dyn_stub.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/ns_list_stub.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/common_functions_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

