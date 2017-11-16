include ../../makefile_defines.txt

COMPONENT_NAME = net_ipv6_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/libNET/src/net_ipv6.c

TEST_SRC_FILES = \
	main.cpp \
        net_ipv6test.cpp \
        test_net_ipv6.c \
        ../../stub/protocol_core_stub.c \
        ../../stub/ipv6_fragmentation_stub.c \
        ../../stub/ipv6_flow_stub.c \
        ../../stub/ipv6_routing_table_stub.c \
        ../../stub/address_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

