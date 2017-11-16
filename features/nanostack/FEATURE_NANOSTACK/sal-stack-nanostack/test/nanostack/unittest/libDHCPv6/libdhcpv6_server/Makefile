include ../../makefile_defines.txt

COMPONENT_NAME = libDHCPv6_server_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/libDHCPv6/libDHCPv6_server.c

TEST_SRC_FILES = \
	main.cpp \
        libdhcpv6_servertest.cpp \
        test_libdhcpv6_server.c \
        ../../stub/common_functions_stub.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/ns_list_stub.c \
        ../../stub/randLIB_stub.c \
        ../../stub/nsdynmemLIB_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT -DHAVE_DHCPV6

