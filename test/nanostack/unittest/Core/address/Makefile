include ../../makefile_defines.txt

COMPONENT_NAME = address_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/Core/address.c

TEST_SRC_FILES = \
	main.cpp \
        addresstest.cpp \
        test_address.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/common_functions_stub.c \
        ../../stub/multicast_api_stub.c \
        ../../stub/ns_sha256_stub.c \
        ../../stub/ns_list_stub.c \
        ../../stub/thread_common_stub.c \
        ../../stub/icmpv6_stub.c \
        ../../stub/mld_stub.c \
        ../../stub/randLIB_stub.c \
        ../../stub/protocol_core_stub.c \
        ../../stub/ip6tos_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

