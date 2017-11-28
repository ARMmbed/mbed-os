include ../../makefile_defines.txt

COMPONENT_NAME = mle_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/MLE/mle.c

TEST_SRC_FILES = \
	main.cpp \
        mle_test.cpp \
        test_mle.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/protocol_core_stub.c \
        ../../stub/adaptation_interface_stub.c \
        ../../stub/mac_helper_stub.c \
        ../../stub/protocol_6lowpan_stub.c \
        ../../stub/ns_list_stub.c \
        ../../stub/etx_stub.c \
        ../../stub/address_stub.c \
        ../../stub/common_functions_stub.c \
        ../../stub/mle_tlv_stub.c \
        ../../stub/event_stub.c \
        ../../stub/system_timer_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

