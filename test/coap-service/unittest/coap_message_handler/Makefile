include ../makefile_defines.txt

COMPONENT_NAME = coap_message_handler_unit

#This must be changed manually
SRC_FILES = \
        ../../../../source/coap_message_handler.c

TEST_SRC_FILES = \
	main.cpp \
	coap_message_handlertest.cpp \
	test_coap_message_handler.c \
	../stub/mbed_trace_stub.c \
	../stub/sn_coap_protocol_stub.c \
	../stub/sn_coap_parser_stub.c \
	../stub/sn_coap_builder_stub.c \
	../stub/nsdynmemLIB_stub.c \
	../stub/ns_list_stub.c \
	../stub/randLIB_stub.c \
	../stub/coap_service_api_stub.c \
	../stub/socket_api_stub.c \

include ../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

