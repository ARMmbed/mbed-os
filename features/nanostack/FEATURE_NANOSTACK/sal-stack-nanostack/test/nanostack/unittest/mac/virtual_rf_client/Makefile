include ../../makefile_defines.txt

COMPONENT_NAME = virtual_rf_client_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/MAC/virtual_rf/virtual_rf_client.c

TEST_SRC_FILES = \
	main.cpp \
        virtual_rf_clienttest.cpp \
        test_virtual_rf_client.c \
        ../../stub/common_functions_stub.c \
        ../../stub/ns_list_stub.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/rf_driver_storage_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

