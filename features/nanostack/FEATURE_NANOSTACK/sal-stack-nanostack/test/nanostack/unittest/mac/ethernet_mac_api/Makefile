include ../../makefile_defines.txt

COMPONENT_NAME = ethernet_mac_api_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/MAC/ethernet/ethernet_mac_api.c

TEST_SRC_FILES = \
	main.cpp \
        ethernet_mac_apitest.cpp \
        test_ethernet_mac_api.c \
        ../../stub/common_functions_stub.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/event_stub.c \
        ../../stub/rf_driver_storage_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

