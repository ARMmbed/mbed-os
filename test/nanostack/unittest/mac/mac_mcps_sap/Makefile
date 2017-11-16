include ../../makefile_defines.txt

COMPONENT_NAME = mac_mcps_sap_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/MAC/IEEE802_15_4/mac_mcps_sap.c

TEST_SRC_FILES = \
	main.cpp \
        mac_mcps_saptest.cpp \
        test_mac_mcps_sap.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/common_functions_stub.c \
        ../../stub/mac_header_helper_functions_stub.c \
        ../../stub/event_stub.c \
        ../../stub/ns_timer_stub.c \
        ../../stub/sw_mac_stub.c \
        ../../stub/mac_mlme_stub.c \
        ../../stub/mac_timer_stub.c \
        ../../stub/mac_filter_stub.c \
        ../../stub/mac_indirect_data_stub.c \
        ../../stub/mac_security_mib_stub.c \
        ../../stub/mac_pd_sap_stub.c \
        ../../stub/mac_pairwise_key_stub.c \
        ../../stub/protocol_stats_stub.c \
        ../../stub/ccm_security_stub.c \
        ../../stub/fhss_config_stub.c \
        ../../stub/fhss_mac_interface_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

