include ../makefile_defines.txt

COMPONENT_NAME = coap_security_handler_unit

#This must be changed manually
SRC_FILES = \
	../../../../source/coap_security_handler.c

TEST_SRC_FILES = \
	main.cpp \
	coap_security_handlertest.cpp \
	test_coap_security_handler.c \
	../stub/mbed_trace_stub.c \
	../stub/ns_list_stub.c \
	../stub/ns_timer_stub.c \
	../stub/mbedtls_stub.c \
	../stub/randLIB_stub.c \
	../stub/nsdynmemLIB_stub.c \
	../stub/socket_api_stub.c \

include ../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT -DMBEDTLS_KEY_EXCHANGE_ECJPAKE_ENABLED -DMBEDTLS_ECJPAKE_C -DMBEDTLS_SHA256_C -DMBEDTLS_ECP_DP_SECP256R1_ENABLED

