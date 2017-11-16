include ../../makefile_defines.txt

COMPONENT_NAME = tls_lib_unit

#This must be changed manually
SRC_FILES = \
        ../../../../../source/Security/TLS/tls_lib.c

TEST_SRC_FILES = \
	main.cpp \
        tls_libtest.cpp \
        test_tls_lib.c \
        ../../stub/ccm_security_stub.c \
        ../../stub/nsdynmemLIB_stub.c \
        ../../stub/mbed_trace_stub.c \
        ../../stub/buffer_dyn_stub.c \
        ../../stub/common_functions_stub.c \
        ../../stub/ns_sha256_stub.c \
        ../../stub/shalib_stub.c \
        ../../stub/randLIB_stub.c \
        ../../stub/security_lib_stub.c \
        ../../stub/tls_ccm_crypt_stub.c \
        ../../stub/ns_list_stub.c \
        ../../stub/pana_stub.c \
        ../../stub/x509_stub.c \
        ../../stub/ecc_stub.c \
        ../../stub/ecdsa_stub.c \

include ../../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT -DPANA

