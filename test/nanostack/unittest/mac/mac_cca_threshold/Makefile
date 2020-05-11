include ../../makefile_defines.txt

COMPONENT_NAME = mac_cca_threshold_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/MAC/IEEE802_15_4/mac_cca_threshold.c

TEST_SRC_FILES = \
	main.cpp \
        mac_cca_threshold_test.cpp \
        test_mac_cca_threshold.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/nsdynmemLIB_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

