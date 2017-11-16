include ../../makefile_defines.txt

COMPONENT_NAME = mac_data_poll_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/6LoWPAN/MAC/mac_data_poll.c

TEST_SRC_FILES = \
	main.cpp \
        mac_data_polltest.cpp \
        test_mac_data_poll.c \
        ../../stub/common_functions_stub.c \
        ../../stub/ns_list_stub.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/mac_helper_stub.c \
        ../../stub/protocol_core_stub.c \
        ../../stub/system_timer_stub.c \
        ../../stub/protocol_6lowpan_bootstrap_stub.c \
        ../../stub/thread_bootstrap_stub.c \
        ../../stub/mle_stub.c \
        ../../stub/network_lib_stub.c \
        ../../stub/event_stub.c \


include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT -DHAVE_THREAD -DHAVE_6LOWPAN_ND

