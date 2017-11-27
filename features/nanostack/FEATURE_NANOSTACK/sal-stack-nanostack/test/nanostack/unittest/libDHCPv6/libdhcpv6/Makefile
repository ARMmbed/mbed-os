include ../../makefile_defines.txt

COMPONENT_NAME = libDHCPv6_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/libDHCPv6/libDHCPv6.c

TEST_SRC_FILES = \
	main.cpp \
        libdhcpv6test.cpp \
        test_libdhcpv6.c \
        ../../stub/common_functions_stub.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/ns_list_stub.c \
        ../../stub/randLIB_stub.c \
        ../../stub/nsdynmemLIB_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT -DHAVE_DHCPV6

