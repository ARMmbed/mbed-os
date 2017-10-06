include ../makefile_defines.txt

COMPONENT_NAME = coap_service_api_unit

#This must be changed manually
SRC_FILES = \
        ../../../../source/coap_service_api.c

TEST_SRC_FILES = \
	main.cpp \
	coap_service_apitest.cpp \
	test_coap_service_api.c \
	../stub/mbed_trace_stub.c \
	../stub/ns_list_stub.c \
	../stub/system_timer_stub.c \
	../stub/nsdynmemLIB_stub.c \
	../stub/eventOS_event_stub.c \
	../stub/coap_connection_handler_stub.c \
	../stub/coap_message_handler_stub.c \
	../stub/common_functions_stub.c \
	../stub/sn_coap_protocol_stub.c

include ../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT

