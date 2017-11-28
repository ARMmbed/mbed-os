include ../../makefile_defines.txt

COMPONENT_NAME = sw_mac_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/MAC/IEEE802_15_4/sw_mac.c

TEST_SRC_FILES = \
	main.cpp \
        sw_mactest.cpp \
        test_sw_mac.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/rf_driver_storage_stub.c \
        ../../stub/mac_security_mib_stub.c \
        ../../stub/mac_mlme_stub.c \
        ../../stub/mac_pd_sap_stub.c \
        ../../stub/mac_mcps_sap_stub.c \
        ../../stub/fhss_callbacks_stub.c \
        ../../stub/fhss_mac_interface_stub.c \
        ../../stub/ns_timer_stub.c \
        ../../stub/common_functions_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

