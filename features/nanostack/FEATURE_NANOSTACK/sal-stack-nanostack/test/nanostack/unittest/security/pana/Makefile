include ../../makefile_defines.txt

COMPONENT_NAME = pana_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/Security/PANA/pana.c

TEST_SRC_FILES = \
	main.cpp \
        panatest.cpp \
        test_pana.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/shalib_stub.c \
        ../../stub/buffer_dyn_stub.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/common_functions_stub.c \
        ../../stub/security_lib_stub.c \
        ../../stub/tls_lib_stub.c \
        ../../stub/protocol_core_stub.c \
        ../../stub/mac_helper_stub.c \
        ../../stub/mac_data_poll_stub.c \
        ../../stub/event_stub.c \
        ../../stub/protocol_6lowpan_bootstrap_stub.c \
        ../../stub/protocol_6lowpan_stub.c \
        ../../stub/mle_service_stub.c \
        ../../stub/randLIB_stub.c \
        ../../stub/socket_stub.c \
        ../../stub/ccm_security_stub.c \
        ../../stub/address_stub.c \
        ../../stub/nd_router_object_stub.c \
        ../../stub/x509_stub.c \
        ../../stub/ecc_stub.c \
        ../../stub/pana_avp_stub.c \
        ../../stub/pana_header_stub.c \
        ../../stub/eap_protocol_stub.c \
        ../../stub/socket_api_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT -DPANA

