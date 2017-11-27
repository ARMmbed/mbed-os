include ../../makefile_defines.txt

COMPONENT_NAME = mac_filter_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/MAC/IEEE802_15_4/mac_filter.c

TEST_SRC_FILES = \
	main.cpp \
        mac_filtertest.cpp \
        test_mac_filter.c \
        ../../stub/common_functions_stub.c \
        ../../stub/ns_list_stub.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/mac_header_helper_functions_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

