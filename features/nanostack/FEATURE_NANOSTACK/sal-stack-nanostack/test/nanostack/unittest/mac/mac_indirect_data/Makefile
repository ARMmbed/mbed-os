include ../../makefile_defines.txt

COMPONENT_NAME = mac_indirect_data_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/MAC/IEEE802_15_4/mac_indirect_data.c

TEST_SRC_FILES = \
	main.cpp \
        mac_indirect_datatest.cpp \
        test_mac_indirect_data.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/common_functions_stub.c \
        ../../stub/mac_header_helper_functions_stub.c \
        ../../stub/sw_mac_stub.c \
        ../../stub/mac_mcps_sap_stub.c \
        ../../stub/ns_timer_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

