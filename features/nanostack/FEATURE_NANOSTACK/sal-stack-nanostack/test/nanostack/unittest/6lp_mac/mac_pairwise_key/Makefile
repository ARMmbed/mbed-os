include ../../makefile_defines.txt

COMPONENT_NAME = mac_pairwise_key_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/6LoWPAN/MAC/mac_pairwise_key.c

TEST_SRC_FILES = \
	main.cpp \
        mac_pairwise_keytest.cpp \
        test_mac_pairwise_key.c \
        ../../stub/protocol_core_stub.c \
        ../../stub/mac_helper_stub.c \
        ../../stub/mle_stub.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/ns_list_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

