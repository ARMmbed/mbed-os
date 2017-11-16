include ../../makefile_defines.txt

COMPONENT_NAME = mac_mlme_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/MAC/IEEE802_15_4/mac_mlme.c

TEST_SRC_FILES = \
	main.cpp \
        mac_mlmetest.cpp \
        test_mac_mlme.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/common_functions_stub.c \
        ../../stub/sw_mac_stub.c \
        ../../stub/mac_header_helper_functions_stub.c \
        ../../stub/mac_timer_stub.c \
        ../../stub/mac_filter_stub.c \
        ../../stub/mac_indirect_data_stub.c \
        ../../stub/mac_security_mib_stub.c \
        ../../stub/mac_pd_sap_stub.c \
        ../../stub/mac_mcps_sap_stub.c \
        ../../stub/event_stub.c \
        ../../stub/ns_timer_stub.c \
        ../../stub/platform_stub.c \
        ../../stub/rf_driver_storage_stub.c \
        ../../stub/randLIB_stub.c \
        ../../stub/protocol_core_stub.c \
        ../../stub/fhss_config_stub.c \
        ../../stub/fhss_mac_interface_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

