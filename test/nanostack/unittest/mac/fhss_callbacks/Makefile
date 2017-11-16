include ../../makefile_defines.txt

COMPONENT_NAME = fhss_callbacks_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/MAC/IEEE802_15_4/mac_fhss_callbacks.c

TEST_SRC_FILES = \
	main.cpp \
        fhss_callbackstest.cpp \
        test_fhss_callbacks.c \
        ../../stub/sw_mac_stub.c \
        ../../stub/rf_driver_storage_stub.c \
        ../../stub/mac_mlme_stub.c \
        ../../stub/mac_mcps_sap_stub.c \
        ../../stub/common_functions_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

