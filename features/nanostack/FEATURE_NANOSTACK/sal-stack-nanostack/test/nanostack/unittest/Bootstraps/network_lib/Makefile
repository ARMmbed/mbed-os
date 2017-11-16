include ../../makefile_defines.txt

COMPONENT_NAME = network_lib_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/6LoWPAN/Bootstraps/Generic/network_lib.c

TEST_SRC_FILES = \
	main.cpp \
        network_libtest.cpp \
        test_network_lib.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/buffer_dyn_stub.c \
        ../../stub/socket_stub.c \
        ../../stub/thread_common_stub.c \
        ../../stub/protocol_stats_stub.c \
        ../../stub/protocol_core_stub.c \
        ../../stub/protocol_6lowpan_stub.c \
        ../../stub/thread_bootstrap_stub.c \
        ../../stub/address_stub.c \
        ../../stub/rpl_data_stub.c \


include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

