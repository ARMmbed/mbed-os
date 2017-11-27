include ../../makefile_defines.txt

COMPONENT_NAME = mac_header_helper_functions_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/MAC/IEEE802_15_4/mac_header_helper_functions.c

TEST_SRC_FILES = \
	main.cpp \
        mac_header_helper_functionstest.cpp \
        test_mac_header_helper_functions.c \
        ../../stub/common_functions_stub.c \
        ../../stub/nsdynmemLIB_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

