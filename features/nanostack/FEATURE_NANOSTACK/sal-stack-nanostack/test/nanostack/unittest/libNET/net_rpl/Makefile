include ../../makefile_defines.txt

COMPONENT_NAME = net_rpl_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/libNET/src/net_rpl.c

TEST_SRC_FILES = \
	main.cpp \
        net_rpltest.cpp \
        test_net_rpl.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/protocol_core_stub.c \
        ../../stub/protocol_6lowpan_stub.c \
        ../../stub/protocol_6lowpan_bootstrap_stub.c \
        ../../stub/mle_service_stub.c \
        ../../stub/rpl_control_stub.c \
        ../../stub/rpl_data_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT -DHAVE_RPL

