include ../../makefile_defines.txt

COMPONENT_NAME = security_lib_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/Security/Common/security_lib.c

TEST_SRC_FILES = \
	main.cpp \
        security_libtest.cpp \
        test_security_lib.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/pana_stub.c \
        ../../stub/pana_server_stub.c \
        ../../stub/buffer_dyn_stub.c \
        ../../stub/tls_lib_stub.c \
        ../../stub/tls_ccm_crypt_stub.c \
        ../../stub/ns_list_stub.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/randLIB_stub.c \
        ../../stub/shalib_stub.c \
        ../../stub/ccm_security_stub.c \
        ../../stub/common_functions_stub.c \
        ../../stub/x509_stub.c \
        ../../stub/ecc_stub.c \
        ../../stub/protocol_core_stub.c \
        ../../stub/event_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT -DPANA

