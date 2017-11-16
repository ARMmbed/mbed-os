include ../../makefile_defines.txt

COMPONENT_NAME = mac_security_mib_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/MAC/IEEE802_15_4/mac_security_mib.c

TEST_SRC_FILES = \
	main.cpp \
        mac_security_mibtest.cpp \
        test_mac_security_mib.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/common_functions_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

