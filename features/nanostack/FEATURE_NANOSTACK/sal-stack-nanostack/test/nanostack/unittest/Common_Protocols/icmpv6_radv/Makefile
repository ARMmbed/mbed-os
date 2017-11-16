include ../../makefile_defines.txt

COMPONENT_NAME = icmpv6_radv_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/Common_Protocols/icmpv6_radv.c

TEST_SRC_FILES = \
	main.cpp \
        icmpv6_radvtest.cpp \
        test_icmpv6_radv.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/ns_list_stub.c \
        ../../stub/buffer_dyn_stub.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/common_functions_stub.c \
        ../../stub/address_stub.c \
        ../../stub/protocol_core_stub.c \
        ../../stub/randLIB_stub.c \
        ../../stub/nd_router_object_stub.c \
        ../../stub/icmpv6_stub.c \
        ../../stub/ipv6_routing_table_stub.c \
        ../../stub/protocol_ipv6_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

