include ../makefile_defines.txt

COMPONENT_NAME = coap_connection_handler_unit

#This must be changed manually
SRC_FILES = \
	../../../../source/coap_connection_handler.c

TEST_SRC_FILES = \
	main.cpp \
	coap_connection_handlertest.cpp \
	test_coap_connection_handler.c \
	../stub/mbed_trace_stub.c \
	../stub/ns_list_stub.c \
	../stub/ns_timer_stub.c \
	../stub/timeout_stub.c \
	../stub/nsdynmemLIB_stub.c \
	../stub/socket_api_stub.c \
	../stub/coap_security_handler_stub.c \
	../stub/coap_service_api_stub.c \
	../stub/coap_message_handler_stub.c \

include ../MakefileWorker.mk

CPPUTESTFLAGS += -DFEA_TRACE_SUPPORT -DHAVE_THREAD

